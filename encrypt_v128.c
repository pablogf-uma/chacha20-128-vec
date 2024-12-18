#include <stdio.h>
#include <string.h>
#include <stdint.h>
# include "chacha20_functions_v128.h"
#include <emmintrin.h>

void encrypt_v128(uint32_t state[16], const char *constant, const uint8_t key[32], uint32_t blockcount, const uint8_t nonce[12], uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3, char *plaintext, char *ciphertext, unsigned long plaintext_length) {

    size_t n_blocks = (plaintext_length + 63) / 64; // Calculate the number of 64-byte blocks needed

    for (size_t i = 0; i < n_blocks; i++) {
        uint8_t keystream[64];
        state_init(state, constant, key, blockcount + i, nonce);
        permute_state_v128(state, v0, v1, v2, v3, keystream);

        // Calculate bytes to process for the current block
        size_t bytes_this_block;
        if (i == n_blocks - 1) {
            bytes_this_block = plaintext_length - i * 64;
        } else {
            bytes_this_block = 64;
        }

        // Process full 16-byte chunks
        size_t j;
        for (j = 0; j < bytes_this_block - (bytes_this_block % 16); j += 16) {
            __m128i plaintext_v = _mm_loadu_si128((__m128i *)&plaintext[i * 64 + j]);
            __m128i keystream_v = _mm_loadu_si128((__m128i *)&keystream[j]);
            __m128i ciphertext_v = _mm_xor_si128(plaintext_v, keystream_v);
            _mm_storeu_si128((__m128i*)&ciphertext[i * 64 + j], ciphertext_v);
        }

        // Handle remaining bytes byte-by-byte
        for (; j < bytes_this_block; j++) {
            ciphertext[i * 64 + j] = plaintext[i * 64 + j] ^ keystream[j];
        }
    }

    // Add null terminator
    ciphertext[plaintext_length] = '\0';

        /* TEST
    for (size_t i = 0; i < ciphertext_len; i++) {
        printf("%02x", (unsigned char)plaintext[i]);
        printf(" ");
    }
    printf("\n");
    */
}