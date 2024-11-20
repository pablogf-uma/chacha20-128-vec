#include <stdio.h>
#include <string.h>
#include <stdint.h>
# include "chacha20_functions_v128.h"
#include <emmintrin.h>

void encrypt_v128(uint32_t state[16], const char *constant, const uint8_t key[32], uint32_t blockcount, const uint8_t nonce[12], uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3, char *plaintext, char *output) {

    size_t plaintext_len = strlen(plaintext);
    size_t number_of_blocks = plaintext_len / 64;
    size_t remaining_bytes = plaintext_len % 64;

    // Each block of ciphertext is 64 bytes long, each xored with a state containing a different counter value
    for (int i = 0; i < number_of_blocks; i++) {

        // Generate the keystream for the current block
        uint8_t keystream[64];
        state_init(state, constant, key, blockcount + i, nonce);
        permute_state_v128(state, v0, v1, v2, v3, keystream);

        // XOR the plaintext with the keystream (vectorized version)
        for (int j = 0; j < 64; j += 16) {
            __m128i plaintext_v = _mm_loadu_si128((__m128i *)&plaintext[i * 64 + j]);
            __m128i keystream_v = _mm_loadu_si128((__m128i *)&keystream[j]);
            __m128i output_v = _mm_xor_si128(plaintext_v, keystream_v);
            _mm_storeu_si128((__m128i*)&output[i * 64 + j], output_v);
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

    // If there are remaining bytes, encrypt them (vectorized version)
    if (remaining_bytes != 0) {

        uint8_t keystream[64];
        state_init(state, constant, key, blockcount + number_of_blocks, nonce);
        permute_state_v128(state, v0, v1, v2, v3, keystream);

        // Group the remaining block in chunks of 16 to vectorize what is possible
        size_t offset_bytes = number_of_blocks * 64;
        size_t n_blocks_of_16_byte = remaining_bytes / 16;
        size_t remaining_after_16_grouping = remaining_bytes % 16;

        for (size_t i = 0; i < n_blocks_of_16_byte; i++) {
            __m128i plaintext_v = _mm_loadu_si128((__m128i *)&plaintext[offset_bytes + i * 16]);
            __m128i keystream_v = _mm_loadu_si128((__m128i *)&keystream[i * 16]);
            __m128i output_v = _mm_xor_si128(plaintext_v, keystream_v);
            _mm_storeu_si128((__m128i*)&output[offset_bytes + i * 16], output_v);
        }

        if (remaining_after_16_grouping != 0) {
            uint8_t temp_plaintext[16] = {0};
            uint8_t temp_keystream[16] = {0};

            // Copy the remaining bytes after 16 bytes grouping to the buffer variables
            memcpy(temp_plaintext, &plaintext[offset_bytes + n_blocks_of_16_byte * 16], remaining_after_16_grouping);
            memcpy(temp_keystream, &keystream[n_blocks_of_16_byte * 16], remaining_after_16_grouping);

            // XOR the remaining bytes
            for (size_t i = 0; i < remaining_after_16_grouping; i++) {
                output[offset_bytes + n_blocks_of_16_byte * 16 + i] = temp_plaintext[i] ^ temp_keystream[i];
            }
        }
    }

    // Add the null terminator to the output string
    output[plaintext_len] = '\0';
}