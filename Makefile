CC = gcc
CFLAGS = -Wall -Wextra -pthread
LIBS = -lmta_crypt -lmta_rand
TARGET = password_cracker
SRC = password_cracker.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
