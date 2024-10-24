# include <stdio.h>
# include <stdint.h>
# include "chacha20_v_functions.h"

int main()
{
    uint32_t state[16];
    char constant[16] = "expand 32-byte k";
    uint8_t key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
    uint32_t blockcount = 1;
    uint8_t nonce[12] = {0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4a,
                          0x00, 0x00, 0x00, 0x00};


    state_init(state, constant, key, blockcount, nonce);

    uint32_t v0[4];
    uint32_t v1[4];
    uint32_t v2[4];
    uint32_t v3[4];

    rows_to_vectors(state, v0, v1, v2, v3);

    whole_round(v0, v1, v2,v3);

    uint32_t *vectors[4] = {v0, v1, v2, v3};


    // Output vectors
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