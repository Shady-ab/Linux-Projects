#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <time.h>
#include <assert.h>
#include <mta_crypt.h>
#include <mta_rand.h>

#define MAX_PASSWORD_LEN 256

char encrypted_password[MAX_PASSWORD_LEN];
char original_password[MAX_PASSWORD_LEN];
char decrypted_password[MAX_PASSWORD_LEN] = "";
int password_length;
int num_decrypters;
int timeout;
int terminate = 0;
int new_password_ready = 0;
int password_decrypted = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t new_password_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t password_decrypted_cond = PTHREAD_COND_INITIALIZER;

void *encrypter_thread_func(void *arg) {
    MTA_CRYPT_RET_STATUS res;

    while (!terminate) {
        char plain[MAX_PASSWORD_LEN];
        char key[MAX_PASSWORD_LEN];
        char enc[MAX_PASSWORD_LEN];
        int enc_len = 0;

        for (int i = 0; i < password_length; ++i) {
            char c;
            do {
                MTA_get_rand_data(&c, 1);
            } while (!isprint((unsigned char)c));
            plain[i] = c;
        }

        MTA_get_rand_data(key, password_length / 8);

        res = MTA_encrypt(key, password_length / 8, plain, password_length, enc, &enc_len);
        assert(res == MTA_CRYPT_RET_OK);

        pthread_mutex_lock(&lock);
        memcpy(encrypted_password, enc, enc_len);
        memcpy(original_password, plain, password_length);
        original_password[password_length] = '\0';
        new_password_ready = 1;
        password_decrypted = 0;

        printf("%ld [SERVER] [INFO] New password generated: %.*s, key: %.*s, After encryption: %.*s\n",
               time(NULL), password_length, plain, password_length / 8, key, enc_len, enc);

        pthread_cond_broadcast(&new_password_cond);
        pthread_mutex_unlock(&lock);

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout;

        pthread_mutex_lock(&lock);
        int rc = pthread_cond_timedwait(&password_decrypted_cond, &lock, &ts);

        if (rc == 0) {
            printf("%ld [SERVER] [OK] Password decrypted successfully: %s\n", time(NULL), decrypted_password);
        } else {
            printf("%ld [SERVER] [TIMEOUT] No successful decryption.\n", time(NULL));
        }

        new_password_ready = 0;
        memset(decrypted_password, 0, sizeof(decrypted_password));
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *decrypter_thread_func(void *arg) {
    int id = *(int *)arg;
    MTA_CRYPT_RET_STATUS res;
    char local_encrypted[MAX_PASSWORD_LEN];
    int enc_len = password_length;
    int iterations = 0;

    while (!terminate) {
        pthread_mutex_lock(&lock);
        while (!new_password_ready) {
            pthread_cond_wait(&new_password_cond, &lock);
        }
        memcpy(local_encrypted, encrypted_password, enc_len);
        pthread_mutex_unlock(&lock);

        iterations = 0;
        while (1) {
            pthread_mutex_lock(&lock);
            if (!new_password_ready || password_decrypted) {
                pthread_mutex_unlock(&lock);
                break;
            }
            pthread_mutex_unlock(&lock);

            char guess_key[MAX_PASSWORD_LEN] = {0};
            char attempt[MAX_PASSWORD_LEN] = {0};
            int dec_len = 0;

            MTA_get_rand_data(guess_key, password_length / 8);
            res = MTA_decrypt(guess_key, password_length / 8, local_encrypted, enc_len, attempt, &dec_len);
            assert(res == MTA_CRYPT_RET_OK);
            iterations++;

            int printable = 1;
            for (int i = 0; i < dec_len; ++i) {
                if (!isprint((unsigned char)attempt[i])) {
                    printable = 0;
                    break;
                }
            }

            pthread_mutex_lock(&lock);
            if (!password_decrypted && printable && new_password_ready &&
                memcmp(attempt, original_password, password_length) == 0) {
                strncpy(decrypted_password, attempt, dec_len);
                decrypted_password[dec_len] = '\0';
                password_decrypted = 1;

                printf("%ld [CLIENT #%d] [OK] Password decrypted: %.*s, key guessed(%.*s), sending to server after %d iterations\n",
                       time(NULL), id, dec_len, attempt, password_length / 8, guess_key, iterations);

                pthread_cond_signal(&password_decrypted_cond);
                pthread_mutex_unlock(&lock);
                break;
            }
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    MTA_CRYPT_RET_STATUS res = MTA_crypt_init();
    assert(res == MTA_CRYPT_RET_OK);

    static struct option long_options[] = {
        {"num-of-decrypters", required_argument, 0, 'n'},
        {"password-length", required_argument, 0, 'l'},
        {"timeout", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    int opt;
    int n_flag = 0, l_flag = 0, t_flag = 0;
    while ((opt = getopt_long(argc, argv, "n:l:t:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'n': num_decrypters = atoi(optarg); n_flag = 1; break;
            case 'l': password_length = atoi(optarg); l_flag = 1; break;
            case 't': timeout = atoi(optarg); t_flag = 1; break;
            default:
                fprintf(stderr, "Usage: %s -n num -l length -t timeout\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!n_flag || !l_flag || !t_flag) {
        fprintf(stderr, "Error: All options -n, -l, and -t are required.\n");
        fprintf(stderr, "Usage: %s -n num -l length -t timeout\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_t encrypter;
    pthread_t decrypters[num_decrypters];
    int thread_ids[num_decrypters];

    pthread_create(&encrypter, NULL, encrypter_thread_func, NULL);

    for (int i = 0; i < num_decrypters; ++i) {
        thread_ids[i] = i;
        pthread_create(&decrypters[i], NULL, decrypter_thread_func, &thread_ids[i]);
    }

    pthread_join(encrypter, NULL);
    for (int i = 0; i < num_decrypters; ++i) {
        pthread_join(decrypters[i], NULL);
    }

    return 0;
}
