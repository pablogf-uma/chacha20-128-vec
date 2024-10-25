# include <stdio.h>
# include <stdint.h>
# include "chacha20_v_functions.h"

// 128-BIT VECTORIZATION

// rotate_bits performs 32_bit rotations for the elements inside the vectors.
// It is comprehended within the whole_round function

uint32_t rotate_left(uint32_t element, uint32_t shift) {
    return (element << shift) | (element >> (32 - shift));
}

// Whole_round will do the job of 4 consecutive operations in the previous chacha into 1
// It will concatenate the columns and diagonal permutation rounds one after each other
//      - Double (columns + diagonals) Whole (4 parallel quarter rounds) Round
//      - 
// Parameters: 4 vectors (one for each row: 4 words * 4 bytes * 8 bits = 128 bits)
//      - The bit shifting in between operation sets rearranges the vectors so they contain the diagonals

void double_whole_round(uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3) 
{
    // First set of operations
    for (int i = 0; i < 4; i++) {
        v0[i] += v1[i];
        v3[i] ^= v0[i];
        v3[i] = rotate_left(v3[i], 16);
    }

    for (int i = 0; i < 4; i++) {
        v2[i] += v3[i];
        v1[i] ^= v2[i];
        v1[i] = rotate_left(v1[i], 12);
    }

    for (int i = 0; i < 4; i++) {
        v0[i] += v1[i];
        v3[i] ^= v0[i];
        v3[i] = rotate_left(v3[i], 8);
    }

    for (int i = 0; i < 4; i++) {
        v2[i] += v3[i];
        v1[i] ^= v2[i];
        v1[i] = rotate_left(v1[i], 7);
    }

    uint32_t temp_v;
    // Rotate right v1 by 32 bits (simply swap pairs)
    temp_v = v1[0];
    v1[0] = v1[1];
    v1[1] = v1[2];
    v1[2] = v1[3];
    v1[3] = temp_v;

    // Rotate right v2 by 64 bits
    temp_v = v2[0];
    v2[0] = v2[2];
    v2[2] = temp_v;
    temp_v = v2[1];
    v2[1] = v2[3];
    v2[3] = temp_v;

    // Rotate right v3 by 96 bits
    temp_v = v3[3];
    v3[3] = v3[2];
    v3[2] = v3[1];
    v3[1] = v3[0];
    v3[0] = temp_v;

    // Second set of operations
    for (int i = 0; i < 4; i++) {
        v0[i] += v1[i];
        v3[i] ^= v0[i];
        v3[i] = rotate_left(v3[i], 16);
    }

    for (int i = 0; i < 4; i++) {
        v2[i] += v3[i];
        v1[i] ^= v2[i];
        v1[i] = rotate_left(v1[i], 12);
    }

    for (int i = 0; i < 4; i++) {
        v0[i] += v1[i];
        v3[i] ^= v0[i];
        v3[i] = rotate_left(v3[i], 8);
    }

    for (int i = 0; i < 4; i++) {
        v2[i] += v3[i];
        v1[i] ^= v2[i];
        v1[i] = rotate_left(v1[i], 7);
    }

    // Rotate left v1 by 32 bits (restore original order)
    temp_v = v1[3];
    v1[3] = v1[2];
    v1[2] = v1[1];
    v1[1] = v1[0];
    v1[0] = temp_v;

    // Rotate left v2 by 64 bits (restore original order)
    temp_v = v2[0];
    v2[0] = v2[2];
    v2[2] = temp_v;
    temp_v = v2[1];
    v2[1] = v2[3];
    v2[3] = temp_v;

    // Rotate left v3 by 96 bits (restore original order)
    temp_v = v3[0];
    v3[0] = v3[1];
    v3[1] = v3[2];
    v3[2] = v3[3];
    v3[3] = temp_v;    
}

/*

// RFC 7539 (Page 5) - CHACHA Quarter Round Test Vector

int main()
{
    uint32_t state[16] = {  0x61707865, 0x3320646e, 0x79622d32, 0x6b206574, 
                            0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
                            0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c,
                            0x00000001, 0x09000000, 0x4a000000, 0x00000000 };

    uint32_t v0[4];
    uint32_t v1[4];
    uint32_t v2[4];
    uint32_t v3[4];

    rows_to_vectors(state, v0, v1, v2, v3);

    whole_round(v0, v1, v2, v3);

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

    return 0;
}

*/
