# include <stdio.h>
# include <stdint.h>
# include <immintrin.h>

// This function separates the 4 rows into 4 vectors before they are passed through the whole_round function.

void vectors_to_state(uint32_t state[16], __m128i v0, __m128i v1, __m128i v2, __m128i v3)
{
    // Assign each vector to their corresponding position at each row
    for (int i = 0; i < 4; i++)
    {
        state[i] = v0[i];
    }

    for (int i = 4; i < 8; i++)
    {
        state[i] = v1[i - 4];
    }

    for (int i = 8; i < 12; i++)
    {
        state[i] = v2[i - 8];
    }

    for (int i = 12; i < 16; i++)
    {
        state[i] = v3[i - 12];
    }
}