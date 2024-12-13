# include <stdio.h>
# include <stdint.h>
# include <string.h>
# include "chacha20_functions_v128.h"
# include <immintrin.h>

void permute_state_v128(uint32_t state[16], uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3, uint8_t keystream[64])
{
    // Initialize vectors
    state_to_vectors_v128(state, v0, v1, v2, v3);

    // Use of vectors for vectorized addition after the rounds
    __m128i v0_og = _mm_loadu_si128((__m128i*)v0);
    __m128i v1_og = _mm_loadu_si128((__m128i*)v1);
    __m128i v2_og = _mm_loadu_si128((__m128i*)v2);
    __m128i v3_og = _mm_loadu_si128((__m128i*)v3);

    // Perform permutations on the state: 20 total rounds (10 column-diagonal operations)
    for (int i = 0; i < 10; i++) {
        double_whole_round_v128(v0, v1, v2, v3);
    }

    __m128i v0_permuted = _mm_loadu_si128((__m128i*)v0);
    __m128i v1_permuted = _mm_loadu_si128((__m128i*)v1);
    __m128i v2_permuted = _mm_loadu_si128((__m128i*)v2);
    __m128i v3_permuted = _mm_loadu_si128((__m128i*)v3);

    // Add the permuted vectors to the original vectors for pseudo-randomness:
    v0_permuted = _mm_add_epi32(v0_permuted, v0_og);
    v1_permuted = _mm_add_epi32(v1_permuted, v1_og);
    v2_permuted = _mm_add_epi32(v2_permuted, v2_og);
    v3_permuted = _mm_add_epi32(v3_permuted, v3_og);

    // Serialize the vectors into the keystream using 128-bit vectorization
    __m128i permuted_vectors[4] = {v0_permuted, v1_permuted, v2_permuted, v3_permuted};

    for (size_t i = 0; i < 4; i++) { 
        _mm_storeu_si128((__m128i*)&keystream[i * 16], permuted_vectors[i]); // Store vectors into the keystream 128-bits at a time
    }
    
    /*
    // TESTING: Output the permuted vectors, state, and keystream

    uint32_t *vectors[4] = {v0, v1, v2, v3};
    for (int i = 0; i < 4; i++)
    {
        printf("Vector %i:\n", i + 1);
        for (int b = 0; b < 4; b++)
        {
            printf("%08x", vectors[i][b]);
        }
        printf("\n\n");
    }

    for (int i = 0; i < 4; i++)
    {
        printf("Vector %i:\n", i + 1);
        for (int b = 0; b < 4; b++)
        {
            uint32_t value;
            switch (b) {
                case 0: value = _mm_extract_epi32(permuted_vectors[i], 0); break;
                case 1: value = _mm_extract_epi32(permuted_vectors[i], 1); break;
                case 2: value = _mm_extract_epi32(permuted_vectors[i], 2); break;
                case 3: value = _mm_extract_epi32(permuted_vectors[i], 3); break;
            }
            printf("%08x", value);
        }
        printf("\n\n");
    }

    for (int i = 0; i < 64; i++) {
        printf("%02x", keystream[i]);
        printf(":");
    }
    printf("\b \b");
    printf("\n");*/
    
}