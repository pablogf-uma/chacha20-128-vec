# include <stdio.h>
# include <string.h>
# include <stdint.h>

void state_init(uint32_t state[16], const char *constant, const uint8_t key[32], uint32_t blockcount, const uint8_t nonce[12]);
void rows_to_vectors(uint32_t state[16], uint32_t v0[4], uint32_t v1[4], uint32_t v2[4], uint32_t v3[4]);
void double_whole_round(uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3);
void permute_v_state(uint32_t state[16], uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3, uint32_t output_keystream[64]);