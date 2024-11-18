<h1 align="center">ChaCha20 Vectorized Implementation</h1>


This repository contains a C implementation of ChaCha20 vectorized.

This is an optimized version of the ChaCha20 algorithm that uses SIMD (Single Instruction, Multiple Data) instructions to process multiple pieces of data simultaneously, instead of the sequential processing provided by the regular chacha20 algorithm.

It is called 128-bit vectorization because it requires 4 128-bit vectors as input (1 for each row of the state matrix).


<h2>Contents:</h2>




<h2>Performance:</h2>




<h2>Resources:</h2>

These resources were used throughout the development of the project outlined above.

- [Paper](https://eprint.iacr.org/2013/759.pdf)

<h5 align="center">Developed by Pablo Gutiérrez - October 2024</h5>
