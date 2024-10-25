# include <stdio.h>
# include <stdint.h>
# include <string.h>
# include "chacha20_v_functions.h"

void permute_v_state(uint32_t state[16], uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3, uint32_t output_keystream[64])
{
    // Make copy of the original state, for later addition to the permuted state
    uint32_t original_state[16];
    for (int i = 0; i < 16; i++) {
        original_state[i] = state[i];
    }


    // Initialize vectors
    rows_to_vectors(state, v0, v1, v2, v3);

    // Perform permutations on the state: 20 total rounds (10 column-diagonal operations)
    for (int i = 0; i < 10; i++) {
        double_whole_round(v0, v1, v2, v3);
    }

    uint32_t *vectors[4] = {v0, v1, v2, v3};

    // TESTING: Output the output vectors

    for (int i = 0; i < 4; i++)
    {
        printf("Vector %i:\n", i + 1);

        for (int b = 0; b < 4; b++)
        {
            printf("%08x", vectors[i][b]);
        }
        printf("\n\n");
    }


    /*

    // Add the permuted state to the original state for pseudo-randomness:
    for (int i = 0; i < 16; i++) {
        state[i] = (state[i] + original_state[i]);

    }


    // Serialize the permuted state into the output keystream
    for (size_t i = 0; i < 16; i++) {
        uint32_t word = state[i];
        output_keystream[i * 4] = (word >> 0)  & 0xFF;
        output_keystream[i * 4 + 1] = (word >> 8)  & 0xFF;
        output_keystream[i * 4 + 2] = (word >> 16) & 0xFF;
        output_keystream[i * 4 + 3] = (word >> 24) & 0xFF;
    }

    */
}
