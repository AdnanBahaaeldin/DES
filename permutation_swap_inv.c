#include <stdint.h>
#include <stdio.h>

// implementation of Initial permutation, Inverse initial permutation, swap + Round integration
int IP[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

int IP_INV[64] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

// DES bits are numbered 0 1 2 3 4 5 6 7..... so bit 58 in DES corresponds to bit 6 in C's index

uint64_t  Init_Permutation (uint64_t plainTxt) {
    uint64_t permutation = 0; //The final output
    for (uint64_t i = 0; i < 64; i++) {
        uint64_t bit;
        bit = (plainTxt >> (64 - IP[i])) & 1; // shift the number to the exact position accroding the LUT
        permutation |= (bit << (63 - i));     // Place it in output
    }
    return permutation;
}

uint64_t Swap_Permutation (uint32_t left, uint32_t right ) {
    uint64_t txt = 0;
    txt |= (((uint64_t)right<<32) | (left));
    return txt;
}

uint64_t Inverse_Permutation (uint64_t cipherTxt ) {
    uint64_t invPermutation = 0; //The final output
    for (uint64_t i = 0; i < 64; i++) {
        uint64_t bit;
        bit = (cipherTxt >> (64 - IP_INV[i])) & 1; // shift the number to the exact position accroding the LUT
        invPermutation |= (bit << (63 - i));       // Place it in output
    }
    return invPermutation;

}

int main(void) {
    uint64_t plaintext = 0x0123456789ABCDEF;

    printf("Original Plaintext : %016llX\n", plaintext);

    uint64_t afterIP = Init_Permutation(plaintext);
    printf("After Initial Perm : %016llX\n", afterIP);

    uint32_t left  = (afterIP >> 32) & 0xFFFFFFFF;
    uint32_t right = afterIP & 0xFFFFFFFF;
    printf("Left  Half (L16)  : %08X\n", left);
    printf("Right Half (R16)  : %08X\n", right);

    uint64_t afterSwap = Swap_Permutation(left, right);
    printf("After Swap         : %016llX\n", afterSwap);

    uint64_t recovered = Inverse_Permutation(afterIP);
    printf("Recovered Plaintext: %016llX\n", recovered);

    return 0;
}