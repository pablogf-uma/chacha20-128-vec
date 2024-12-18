#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "chacha20_functions_v128.h"
#include <x86intrin.h>

// Function to run a single test and measure clock cyles per byte for each test
int run_decrypt_test(test_vector_t *test) {
    
    uint32_t state1[16];
    unsigned long plaintext_length = test->plaintext_length;
    char output_plaintext[plaintext_length];
    
    uint32_t v0[4];
    uint32_t v1[4];
    uint32_t v2[4];
    uint32_t v3[4];

    // Start cycle counting.
    unsigned long long start_cycles = __rdtsc();

    // Note that for decrypt tests we are swapping the test vectors plaintext and ciphertext
    decrypt_v128(state1, "expand 32-byte k", test->key, test->blockcount, test->nonce, v0, v1, v2, v3, output_plaintext, test->plaintext, plaintext_length);
    
    // End cycle counting.
    unsigned long long end_cycles = __rdtsc();

    // Calculate cycles per byte
    unsigned long long cycles = end_cycles - start_cycles;   
    double cycles_per_byte = (double)cycles / plaintext_length;
    printf("Total input bytes: %lu", plaintext_length);
    printf("\nClock cycles per byte: %.2f\n", cycles_per_byte);

    // Compare output plaintext with expected original plaintext.
    if (memcmp(output_plaintext, test->expected_ciphertext, plaintext_length) == 0) {
        
        return 1;
    } else {
        return 0;
    }
}