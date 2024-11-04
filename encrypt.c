# include <stdio.h>
# include <string.h>
# include <stdint.h>
# include <string.h>
# include "chacha20_v_functions.h"

void encrypt(uint32_t state[16], const char *constant, const uint8_t key[32], uint32_t blockcount, const uint8_t nonce[12], uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3, char *plaintext, char *output) {

    size_t plaintext_len = strlen(plaintext);
    size_t number_of_blocks = plaintext_len / 64;
    size_t remaining_bytes = plaintext_len % 64;

    // Each block of ciphertext is 64 bytes long, each xored with a state containing a different counter value
    for (int i = 0; i < number_of_blocks; i++) {

        // Generate the keystream for the current block
        uint8_t keystream[64];
        state_init(state, constant, key, blockcount + i, nonce);
        permute_v_state(state, v0, v1, v2, v3, keystream);
        
        // XOR the plaintext with the keystream
        for (int j = 0; j < 64; j++){
            output[i * 64 + j] = plaintext[i * 64 + j] ^ keystream[j];
        }

        /* TEST:
        printf("\nBlock #%d\n", i + 1);
        for (size_t j = 0; j < 64; j++) {
            printf("%02x", (unsigned char)output[i * 64 + j]);
            printf(" ");
        }
        printf("\n");
        */
    }

    // If there are remaining bytes, encrypt them
    if (remaining_bytes != 0) {
        
        uint8_t keystream[64];
        state_init(state, constant, key, blockcount + number_of_blocks, nonce);
        permute_v_state(state, v0, v1, v2, v3, keystream);

        for (size_t i = 0; i < remaining_bytes; i++) {
            output[number_of_blocks * 64 + i] = plaintext[number_of_blocks * 64 + i] ^ keystream[i];
        }
        
        /* TEST:
        printf("\nBlock #%d\n", number_of_blocks + 1);
        for (size_t j = 0; j < 64; j++) {
            printf("%02x", (unsigned char)output[number_of_blocks * 64 + j]);
            printf(" ");
        }
        */
    }

    // Add the null terminator to the output string
    output[plaintext_len] = '\0';
}