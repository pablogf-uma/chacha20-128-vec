# include <stdio.h>
# include <stdint.h>
# include "chacha20_v_functions.h"
# include <immintrin.h>

// 128-BIT VECTORIZATION: SIMD (immintrin.h) for parallel processing contains:

/*

ROTATE_BITS:

- Performs n-bit rotations for the elements inside vector v.
    - Combining a left shift and a right shift this way mimics a circular or left rotation.

- It will be used within the whole_round function.

- Contents from immintrin.h:
    __m128i : Data type provided by immintrin.h that to represent a  128-bit vector.
    _mm_slli_epi32(x, n): This function performs a left bitwise shift on each 32-bit element in the vector x by n bits.
    _mm_srli_epi32(x, 32 - n): This function performs a right bitwise shift on each 32-bit element in x by 32 - n bits.
    _mm_or_si128(...): This performs a bitwise OR between the two shifted results.

*/

__m128i rotate_left(__m128i v, int n) {
    return _mm_or_si128(_mm_slli_epi32(v, n), _mm_srli_epi32(v, 32 - n));
}

/*

WHOLE_ROUND:

- Does the job of 4 consecutive operations in the previous chacha into 1

- It will concatenate the columns and diagonal permutation rounds one after each other
     - Double (columns + diagonals) Whole (4 parallel quarter rounds) Round

- Parameters: 4 vectors (one for each row: 4 words * 4 bytes * 8 bits = 128 bits)
      - The bit shifting in between operation sets rearranges the vectors so they contain the diagonals

- Contents from immintrin.h:
    _mm_loadu_si128((__m128i*)vector) : Loads 128 bits (4 x 32-bit integers) from an unaligned memory location into the SIMD register
    _mm_add_epi32(v1, v2) : Adds each element of v1 with the corresponding element of v2
    _mm_xor_si128(v1, v2) : XOR's each element of v1 with the corresponding element of v2
    _mm_shuffle_epi32(v1_vec, _MM_SHUFFLE(a,b,c,d)) : Shuffles the elements in a vector based on the mask provided
    _MM_SHUFFLE(d, c, b, a) specifies the new order for elements. The element at position 0 is moved to position a, element at position 2 is moved to position b...
    _mm_storeu_si128((__m128i*)v1, v1_vec): Stores the values of the register back to the original arrays

*/

void double_whole_round(uint32_t *v0, uint32_t *v1, uint32_t *v2, uint32_t *v3) 
{
    // Load vectors into SIMD register
    __m128i v0_vec = _mm_loadu_si128((__m128i*)v0);
    __m128i v1_vec = _mm_loadu_si128((__m128i*)v1);
    __m128i v2_vec = _mm_loadu_si128((__m128i*)v2);
    __m128i v3_vec = _mm_loadu_si128((__m128i*)v3);

    // First set of operations (columns):
    v0_vec = _mm_add_epi32(v0_vec, v1_vec);
    v3_vec = _mm_xor_si128(v3_vec, v0_vec);
    v3_vec = rotate_left(v3_vec, 16);

    v2_vec = _mm_add_epi32(v2_vec, v3_vec);
    v1_vec = _mm_xor_si128(v1_vec, v2_vec);
    v1_vec = rotate_left(v1_vec, 12);

    v0_vec = _mm_add_epi32(v0_vec, v1_vec);
    v3_vec = _mm_xor_si128(v3_vec, v0_vec);
    v3_vec = rotate_left(v3_vec, 8);

    v2_vec = _mm_add_epi32(v2_vec, v3_vec);
    v1_vec = _mm_xor_si128(v1_vec, v2_vec);
    v1_vec = rotate_left(v1_vec, 7);

    // Reorder elements in each vector to operate on diagonals:
    v1_vec = _mm_shuffle_epi32(v1_vec, _MM_SHUFFLE(0,3,2,1));
    v2_vec = _mm_shuffle_epi32(v2_vec, _MM_SHUFFLE(1,0,3,2));
    v3_vec = _mm_shuffle_epi32(v3_vec, _MM_SHUFFLE(2,1,0,3));

    // Second set of operations (diagonals):
    v0_vec = _mm_add_epi32(v0_vec, v1_vec);
    v3_vec = _mm_xor_si128(v3_vec, v0_vec);
    v3_vec = rotate_left(v3_vec, 16);

    v2_vec = _mm_add_epi32(v2_vec, v3_vec);
    v1_vec = _mm_xor_si128(v1_vec, v2_vec);
    v1_vec = rotate_left(v1_vec, 12);

    v0_vec = _mm_add_epi32(v0_vec, v1_vec);
    v3_vec = _mm_xor_si128(v3_vec, v0_vec);
    v3_vec = rotate_left(v3_vec, 8);

    v2_vec = _mm_add_epi32(v2_vec, v3_vec);
    v1_vec = _mm_xor_si128(v1_vec, v2_vec);
    v1_vec = rotate_left(v1_vec, 7);
  
    // Restore original order:
    v1_vec = _mm_shuffle_epi32(v1_vec, _MM_SHUFFLE(2,1,0,3));
    v2_vec = _mm_shuffle_epi32(v2_vec, _MM_SHUFFLE(1,0,3,2));
    v3_vec = _mm_shuffle_epi32(v3_vec, _MM_SHUFFLE(0,3,2,1));

    // Store result back in the input parameters:
    _mm_storeu_si128((__m128i*)v0, v0_vec);
    _mm_storeu_si128((__m128i*)v1, v1_vec);
    _mm_storeu_si128((__m128i*)v2, v2_vec);
    _mm_storeu_si128((__m128i*)v3, v3_vec);
}
