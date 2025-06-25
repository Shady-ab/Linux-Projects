#!/bin/bash

# Check for input argument
if [ -z "$1" ]; then
    echo "Usage: $0 <number_of_blocks>"
    exit 1
fi

X=$1
OUTPUT_FILE="block_data/blocks.txt"

# Clear the file before writing
> "$OUTPUT_FILE"

# Get the latest block hash
BLOCK_HASH=$(curl -s https://api.blockcypher.com/v1/btc/main | sed -n 's/.*"hash": "\([^"]*\)".*/\1/p')

# Fetch and save X blocks
for ((i=0; i<X; i++)); do
    echo "Fetching block: $BLOCK_HASH"

    RESPONSE=$(curl -s https://api.blockcypher.com/v1/btc/main/blocks/$BLOCK_HASH)

    HASH=$(echo "$RESPONSE" | grep -oP '"hash":\s*"\K[^"]+')
    HEIGHT=$(echo "$RESPONSE" | grep -oP '"height":\s*\K[0-9]+')
    TOTAL=$(echo "$RESPONSE" | grep -oP '"total":\s*\K[0-9]+')
    TIME=$(echo "$RESPONSE" | grep -oP '"time":\s*"\K[^"]+')
    RECEIVED_TIME=$(echo "$RESPONSE" | grep -oP '"received_time":\s*"\K[^"]+')
    RELAYED_BY=$(echo "$RESPONSE" | grep -oP '"relayed_by":\s*"\K[^"]+')
    PREV_BLOCK=$(echo "$RESPONSE" | grep -oP '"prev_block":\s*"\K[^"]+')

    {
        echo "block $((i+1)) {"
        echo "hash: $HASH"
        echo "height: $HEIGHT"
        echo "total: $TOTAL"
        echo "time: $TIME"
        echo "received_time: $RECEIVED_TIME"
        echo "relayed_by: $RELAYED_BY"
        echo "prev_block: $PREV_BLOCK"
        echo "}"
        echo ""
    } >> "$OUTPUT_FILE"

    BLOCK_HASH=$PREV_BLOCK
done

echo "Done fetching $X blocks."

