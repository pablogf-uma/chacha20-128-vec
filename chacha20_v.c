# include <stdio.h>
# include <stdint.h>
# include "chacha20_v_functions.h"

int main()
{
    int state[16];
    char constant[16] = "expand 32-byte k";
    uint8_t key[32] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint32_t blockcount = 0;
    uint8_t nonce[12] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00 };

    state_init(state, constant, key, blockcount, nonce);

    uint32_t v0[4];
    uint32_t v1[4];
    uint32_t v2[4];
    uint32_t v3[4];

    state_to_vectors(state, v0, v1, v2, v3);

    for (int i = 0; i < 4; i++)
    {
        printf("%08x", v0[i]);
        printf("%08x", v1[i]);
        printf("%08x", v2[i]);
        printf("%08x", v3[i]);
        printf("\n");
    }


    return 0;
}

// Change it so quarter_round becomes whole_round (4 quarter rounds)
// That means it should take 4 vectors: group the words into vectors where 4 words within a row or diagonal = vector
// perform the permutations from the paper
// perform rearrangement explained in the paper: rotations and shuffles. This is supposed to alter the state of the matrix so the columns (although in our case we want diagonals) are set on the rows place and we can just perform the same again.
//          I tried understanding this but its not making sense: columns dont appear as rows
//          Maybe we have to call the function twice (one for rows and the other one diagonals)? That would mean setting the vectors again so that they point towards different elements of the state (first call to the rows and second call to the diagonals)
   