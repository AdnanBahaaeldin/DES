/**
 * @file sp_network.c
 * @brief Implementation of the substitution-permutation network.
 * @author Ahmed Tarek
 * @date 25 Oct 2025
 */

#include <stdint.h>
#define msb6 0x20 
#define lsb 0x01

/**
 * S-Boxes used in the substitution-permutation network.
 */

uint64_t S1[] = {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
			15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};

uint64_t S2[] = {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

uint64_t S3[] = {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

uint64_t S4[] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

uint64_t S5[] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

uint64_t S6[] = {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

uint64_t S7[] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

uint64_t S8[] = {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
			 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};

/**
 * P-box used in transposition step.
 */ 

uint64_t PBox[] =  {16,  7, 20, 21,
					29, 12, 28, 17,
					 1, 15, 23, 26,
					 5, 18, 31, 10,
					 2,  8, 24, 14,
					32, 27,  3,  9,
					19, 13, 30,  6,
					22, 11,  4, 25};
 
/**
  * @brief Performs the keyed substitution using S-Boxes.
  * @param in pointer to the 48-bit input data
  * @param out pointer to the 32-bit output data
  */

void KeyedSubstitution(uint64_t *in, uint64_t *out) {
    // Reduces each 6 bits to 4 bits (8 chunks)
    // 101010 -> (1)(0101)(0) -> divided into 10 and 0101
    // 0x21 is the mask to get the first and last bits

    uint64_t ch1 = ((*in >> 42) & 0x3F);
    uint64_t ch2 = ((*in >> 36) & 0x3F);
    uint64_t ch3 = ((*in >> 30) & 0x3F);
    uint64_t ch4 = ((*in >> 24) & 0x3F);
    uint64_t ch5 = ((*in >> 18) & 0x3F);
    uint64_t ch6 = ((*in >> 12) & 0x3F);
    uint64_t ch7 = ((*in >> 6) & 0x3F);
    uint64_t ch8 = (*in & 0x3F);

    uint64_t row , col;
    *out = 0;

    // S1
    row = ((ch1 & msb6) >> 4) | (ch1 & lsb);
    col = (ch1 >> 1) & 0x0F;
    *out |= S1[row * 16 + col] << 28;

    // S2
    row = ((ch2 & msb6) >> 4) | (ch2 & lsb);
    col = (ch2 >> 1) & 0x0F;
    *out |= S2[row * 16 + col] << 24;

    // S3
    row = ((ch3 & msb6) >> 4) | (ch3 & lsb);
    col = (ch3 >> 1) & 0x0F;
    *out |= S3[row * 16 + col] << 20;

    // S4
    row = ((ch4 & msb6) >> 4) | (ch4 & lsb);
    col = (ch4 >> 1) & 0x0F;
    *out |= S4[row * 16 + col] << 16;

    // S5
    row = ((ch5 & msb6) >> 4) | (ch5 & lsb);
    col = (ch5 >> 1) & 0x0F;
    *out |= S5[row * 16 + col] << 12;

    // S6
    row = ((ch6 & msb6) >> 4) | (ch6 & lsb);
    col = (ch6 >> 1) & 0x0F;
    *out |= S6[row * 16 + col] << 8;

    // S7
    row = ((ch7 & msb6) >> 4) | (ch7 & lsb);
    col = (ch7 >> 1) & 0x0F;
    *out |= S7[row * 16 + col] << 4;

    // S8
    row = ((ch8 & msb6) >> 4) | (ch8 & lsb);
    col = (ch8 >> 1) & 0x0F;
    *out |= S8[row * 16 + col];

}

/**
  * @brief Performs the transposition using P-Box.
  * @param in pointer to the 32-bit input data
  * @param out pointer to the 32-bit output data
  */

void Transposition(uint64_t *in, uint64_t *out){
    *out = 0;
    for (uint64_t i = 0; i < 32; i++){
        uint64_t bit = (*in >> (32 - PBox[i])) & lsb;
        *out |= bit << (31 - i);
    }
}