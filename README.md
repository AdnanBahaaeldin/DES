# DES Encryption and Decryption in C

This project is a from-scratch implementation of the **Data Encryption Standard (DES)** algorithm in the **C programming language**.  
It supports both **encryption** and **decryption** in **ECB (Electronic Codebook)** mode without using any external cryptographic libraries.

---

## Features
- Full implementation of the DES algorithm including:
  - Initial and Final Permutations (IP & FP)
  - Key Generation (PC-1, PC-2, Left Shifts)
  - Expansion, S-box Substitution, and P-box Permutation
  - 16 Feistel Rounds
- ECB Mode operation (processes data in 64-bit blocks)
- Supports both encryption and decryption
- Works with binary files (`.bin`)
- Clean and modular C code structure
- Tested with standard DES test vectors
