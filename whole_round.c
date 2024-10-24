# include <stdio.h>
# include <stdint.h>

// 128-BIT VECTORIZATION

// This function will do the job of 4 consecutive operations in the previous chacha into 1
// A rows + diagonal round (which requires eight quarter-rounds) can now be reduced to two vectorized operations (one for the rows, the other for the diagonals)
// Parameters: 4 vectors (one for each row/diagonal: 4 words * 4 bytes * 8 bits = 128 bits)

void whole_round(uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3) {

}

/* 

// RFC 7539 (Page 5) - CHACHA Quarter Round Test Vector


{
    uint32_t state[16] = { 0x879531e0,  0xc5ecf37d,  0x516461b1, 0xc9a62f8a,
                            0x44c20ef3,  0x3390af7f,  0xd9fc690b,  0x2a5f714c,
                            0x53372767,  0xb00a5631,  0x974c541a,  0x359e9963,
                            0x5c971061,  0x3d631689,  0x2098d9d6,  0x91dbd320 };

    quarter_round(state, 2, 7, 8, 13);

    // TESTING: Output the state matrix.
    printf("\nOriginal state matrix:\n");

    for (int a = 0; a < 4; a++)  // Loop over rows
    {
        for (int b = 0; b < 4; b++)  // Loop over columns
        {
            printf("%08x ", state[a * 4 + b]);  // Finding each element's index: row * 4 + column
        }
        printf("\n");
    }

    return 0;
}

*/