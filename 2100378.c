/**
 * @file 2100378.c
 * @brief Implementation of the DES algorithm.
 * @author Adnan Bahaaeldin Atef
 * @date 29 Oct 2025
 */

#include <stdint.h>
#include <stdio.h>


/**
 * <---------------------------------------------Global Variables--------------------------------------------->
 */
#define msb6 0x20
#define lsb 0x01

//Global LUT holding values for every round key.
unsigned long long round_keys[16];


/**
 * <---------------------------------------------LUTs--------------------------------------------->
*/


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
 * IP vector used in initial permutation.
*/
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

/**
 * IP_INV vector used in inverse initial permutation.
*/
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

/**
 * PC1 Table used in key scheduling algorithm.
*/
static const int PC1_Table[56] = {
	57,49,41,33,25,17,9,1,
	58,50,42,34,26,18,10,2,
	59,51,43,35,27,19,11,3,
	60,52,44,36,63,55,47,39,
	31,23,15,7,62,54,46,38,
	30,22,14,6,61,53,45,37,
	29,21,13,5,28,20,12,4
};

/**
 * PC2 Table used in key scheduling algorithm.
*/
static const int PC2_Table[48] = {
	14,17,11,24,1,5,3,28,
	15,6,21,10,23,19,12,4,
	26,8,16,7,27,20,13,2,
	41,52,31,37,47,55,30,40,
	51,45,33,48,44,49,39,56,
	34,53,46,42,50,36,29,32
};


/**
 * <---------------------------------------------Main Algorithm Functions--------------------------------------------->
*/


// implementation of SBox and PBox substitution

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


// implementation of Initial permutation, Inverse initial permutation, and swap.
// DES bits are numbered 0 1 2 3 4 5 6 7..... so bit 58 in DES corresponds to bit 6 in C's index

/**
  * @brief Performs the initial permutation function using IP vector.
  * @param plainTxt 64 bit input plain text.
  * @returns 64 bit after performing permutation.
*/
uint64_t  Init_Permutation (uint64_t plainTxt) {
    uint64_t permutation = 0; //The final output
    for (uint64_t i = 0; i < 64; i++) {
        uint64_t bit;
        bit = (plainTxt >> (64 - IP[i])) & 1; // shift the number to the exact position accroding the LUT
        permutation |= (bit << (63 - i));     // Place it in output
    }
    return permutation;
}

/**
  * @brief Performs the swap operation of Data.
  * @param left upper 32 bits of Data.
  * @param right lower 32 bits of Data.
  * @returns 64 bit after performing swapping.
*/
uint64_t Swap_Permutation (uint32_t left, uint32_t right) {
    uint64_t txt = 0;
    txt |= (((uint64_t)right<<32) | (left));
    return txt;
}

/**
  * @brief Performs the inverse initial permutation function using IP_INV vector.
  * @param cipherTxt 64 bit input cipher text.
  * @returns 64 bit after performing permutation.
*/
uint64_t Inverse_Permutation (uint64_t cipherTxt) {
    uint64_t invPermutation = 0; //The final output
    for (uint64_t i = 0; i < 64; i++) {
        uint64_t bit;
        bit = (cipherTxt >> (64 - IP_INV[i])) & 1; // shift the number to the exact position accroding the LUT
        invPermutation |= (bit << (63 - i));       // Place it in output
    }
    return invPermutation;

}


// implementation of PC1, left circular shift, PC2 and a function to generate all round keys.

/**
  * @brief Performs PC1 operation for key scheduling using PC1 vector.
  * @param  key64 64 bit input key.
  * @returns 64 bit after performing PC1.
*/
unsigned long long PC1(const unsigned long long * key64) {
	unsigned long long result=0;
	for (int i = 0; i < 56; i++) {
		result = (result << 1) | ( ( *key64 >> (64-PC1_Table[i]) ) & 1ULL );
	}
	return result;
}

/**
  * @brief Performs left circular shift of key for key scheduling.
  * @param  key56 56 bit input key.
  * @param  round_num number of round to determine shift amount.
*/
void left_circular_shift(unsigned long long *key56, const int round_num) {
	unsigned long long left = (*key56 >> 28) & 0xFFFFFFF;
	unsigned long long right = *key56 & 0xFFFFFFF;
	int shift = ( round_num == 1 || round_num == 2 || round_num == 9 || round_num == 16) ? 1 : 2;
	left = ((left << shift) | (left >> (28 - shift))) & 0xFFFFFFF;
	right = ((right << shift) | (right >> (28 - shift))) & 0xFFFFFFF;
	*key56 = (left << 28) | right;

}

/**
  * @brief Performs PC2 operation for key scheduling using PC2 vector.
  * @param  key56 56 bit input key.
  * @param  round_num number of round to store the round key at its respective position in global array.
*/
void PC2(const unsigned long long * key56,const int round_num) {
	unsigned long long result=0;
	for (int i = 0; i < 48; i++) {
		result = (result << 1) | ( ( *key56 >> (56-PC2_Table[i]) ) & 1ULL );
	}
	round_keys[(round_num)-1] = result;
}

/**
  * @brief Generates all the round keys and stores them in the global array defined above.
  * @param  key64 64 bit input key.
*/
void generate_round_keys(const unsigned long long * key64) {
	unsigned long long key56 = PC1(key64);
	for (int i = 1; i <= 16; i++) {
		left_circular_shift(&key56, i);
		PC2(&key56, i);
	}
}


// implementation of expansion inside mangler function along with XORing data with key.

/**
  * @brief Gets a certain bit for the 32 bit input
  * @param R 32 bit input.
  * @param pos Position of bit to be extracted.
*/
uint64_t get_bit(uint32_t R, int pos) {
    return (R >> (32-pos)) & 1;
}

/**
  * @brief Performs expansion permutation function.
  * @param R 32 bit input.
  * @returns 64 bit intermediate text.
*/
uint64_t EP(uint32_t R){
    uint64_t result = 0;
    result |= get_bit(R,32) << 47;
    result |= get_bit(R,1) << 46;
    result |= get_bit(R,2) << 45;
    result |= get_bit(R,3) << 44;
    result |= get_bit(R,4) << 43;
    result |= get_bit(R,5) << 42;
    result |= get_bit(R,4) << 41;
    result |= get_bit(R,5) << 40;
    result |= get_bit(R,6) << 39;
    result |= get_bit(R,7) << 38;
    result |= get_bit(R,8) << 37;
    result |= get_bit(R,9) << 36;
    result |= get_bit(R,8) << 35;
    result |= get_bit(R,9) << 34;
    result |= get_bit(R,10) << 33;
    result |= get_bit(R,11) << 32;
    result |= get_bit(R,12) << 31;
    result |= get_bit(R,13) << 30;
    result |= get_bit(R,12) << 29;
    result |= get_bit(R,13) << 28;
    result |= get_bit(R,14) << 27;
    result |= get_bit(R,15) << 26;
    result |= get_bit(R,16) << 25;
    result |= get_bit(R,17) << 24;
    result |= get_bit(R,16) << 23;
    result |= get_bit(R,17) << 22;
    result |= get_bit(R,18) << 21;
    result |= get_bit(R,19) << 20;
    result |= get_bit(R,20) << 19;
    result |= get_bit(R,21) << 18;
    result |= get_bit(R,20) << 17;
    result |= get_bit(R,21) << 16;
    result |= get_bit(R,22) << 15;
    result |= get_bit(R,23) << 14;
    result |= get_bit(R,24) << 13;
    result |= get_bit(R,25) << 12;
    result |= get_bit(R,24) << 11;
    result |= get_bit(R,25) << 10;
    result |= get_bit(R,26) << 9;
    result |= get_bit(R,27) << 8;
    result |= get_bit(R,28) << 7;
    result |= get_bit(R,29) << 6;
    result |= get_bit(R,28) << 5;
    result |= get_bit(R,29) << 4;
    result |= get_bit(R,30) << 3;
    result |= get_bit(R,31) << 2;
    result |= get_bit(R,32) << 1;
    result |= get_bit(R,1) << 0;
    return result;
}

/**
  * @brief Calls the expansion algorithm and XORs data with key.
  * @param R 32-bit input data (right half).
  * @param K 64-bit key.
*/
uint64_t f_function(uint32_t R, uint64_t K){
    uint64_t expanded_R = EP(R);
    uint64_t result = expanded_R ^ K;
    return result;
}


/**
 * <---------------------------------------------Helper Functions--------------------------------------------->
*/


/**
  * @brief Aligns input data to have the MSB on the left.
  * @param dataIn 64-bit input data passed by reference.
*/
void alignData (uint64_t* dataIn) {

	*dataIn = ((*dataIn & 0xFF00000000000000ULL) >> 56) |
		((*dataIn & 0x00FF000000000000ULL) >> 40) |
		((*dataIn & 0x0000FF0000000000ULL) >> 24) |
		((*dataIn & 0x000000FF00000000ULL) >> 8)  |
		((*dataIn & 0x00000000FF000000ULL) << 8)  |
		((*dataIn & 0x0000000000FF0000ULL) << 24) |
		((*dataIn & 0x000000000000FF00ULL) << 40) |
		((*dataIn & 0x00000000000000FFULL) << 56);

}

/**
  * @brief Opens the files using names extracted from the terminal.
  * @param KEY_F input string for key file name, @param KEY_FILE Output double pointer to key file.
  * @param INPUT input string for input file name, @param INPUT_FILE Output double pointer to input file.
  * @param OUTPUT input string for output file name, @param OUTPUT_FILE Output double pointer to output file.
  * @returns 0/1 incase-of failure/success respectively.
*/
int openFiles(const char* KEY_F, FILE** KEY_FILE, const char* INPUT, FILE** INPUT_FILE, const char* OUTPUT, FILE** OUTPUT_FILE) {

	*KEY_FILE = fopen(KEY_F, "rb");
	if (*KEY_FILE == NULL) {
		printf("Error opening key file\n");
		return 0;
	}

	*INPUT_FILE = fopen(INPUT, "rb");
	if (*INPUT_FILE == NULL) {
		printf("Error opening plaintext file\n");
		return 0;
	}

	*OUTPUT_FILE = fopen(OUTPUT, "wb");
	if (*OUTPUT_FILE == NULL) {
		printf("Error opening ciphertext file\n");
		return 0;
	}

	return 1;
}

/**
  * @brief Runs the DES algorithm in specified mode.
  * @param MODE Specifies the operating mode of DES (Encrypt/Decrypt)
  * @param INPUT_FILE input pointer to input file.
  * @param OUTPUT_FILE input pointer to output file
*/
void run_Algorithm(const char MODE, FILE* INPUT_FILE, FILE* OUTPUT_FILE) {
	uint64_t plaintext_Block = 0;
	uint64_t sbox_Input;
	uint64_t pbox_input;
	uint64_t pbox_output;

	//While file is not empty, read a block (64-bit) of plaintext.
	while (fread (&plaintext_Block, sizeof(plaintext_Block), 1, INPUT_FILE) > 0) {

		//Align data
		alignData(&plaintext_Block);

		//Perform initial permutation and split data into 2 halves.
		uint64_t intermediate_Text = Init_Permutation(plaintext_Block);
		uint32_t right_Half_Intermediate_Text = (uint32_t)intermediate_Text;
		uint32_t left_Half_Intermediate_Text = (uint32_t)(intermediate_Text >> 32);

		if (MODE == 'e') {
			for (int i=0; i<16; i++) {
				//Perform mangler function (Expansion, XOR with key, SBOX, PBOX)
				sbox_Input = f_function(right_Half_Intermediate_Text, round_keys[i]);

				KeyedSubstitution(&sbox_Input, &pbox_input);
				Transposition(&pbox_input, &pbox_output);

				//Perform RH(i-1)->LH(i) and LH(i-1)^(Output of mangler)->RH(i)
				intermediate_Text = right_Half_Intermediate_Text;
				right_Half_Intermediate_Text = (uint32_t) (pbox_output ^ left_Half_Intermediate_Text);
				left_Half_Intermediate_Text = (uint32_t) (intermediate_Text);
			}
		}else if (MODE == 'd') {
			//Repeat for 16 rounds
			for (int i=15; i>=0; i--) {
				//Perform mangler function (Expansion, XOR with key, SBOX, PBOX)
				sbox_Input = f_function(right_Half_Intermediate_Text, round_keys[i]);

				KeyedSubstitution(&sbox_Input, &pbox_input);
				Transposition(&pbox_input, &pbox_output);

				//Perform RH(i-1)->LH(i) and LH(i-1)^(Output of mangler)->RH(i)
				intermediate_Text = right_Half_Intermediate_Text;
				right_Half_Intermediate_Text = (uint32_t) (pbox_output ^ left_Half_Intermediate_Text);
				left_Half_Intermediate_Text = (uint32_t) (intermediate_Text);
			}
		}

		//Perform the swap and inverse initial permutation
		intermediate_Text = Swap_Permutation(left_Half_Intermediate_Text, right_Half_Intermediate_Text);
		intermediate_Text = Inverse_Permutation(intermediate_Text);

		//Align data and write the output into the file
		alignData(&intermediate_Text);
		fwrite(&intermediate_Text, sizeof(intermediate_Text), 1, OUTPUT_FILE);
	}
}

/**
  * @brief Abstract function to run the DES algorithm
  * @param MODE char specifying the operating mode of DES (Encrypt/Decrypt)
  * @param KEY_F String specifying the name for key file.
  * @param INPUT_F String specifying the name for input data file.
  * @param OUTPUT_F String specifying the name for output data file.
*/
void DES(const char MODE, const char* KEY_F ,const char* INPUT_F, const char* OUTPUT_F) {

	FILE* KEY_FILE;
	FILE* INPUT_FILE;
	FILE* OUTPUT_FILE;

	//Opens the files and handles case where an error occurs
	if (!openFiles(KEY_F,&KEY_FILE, INPUT_F, &INPUT_FILE, OUTPUT_F, &OUTPUT_FILE)) {
		printf("Couldn't open files.\n");
		return;
	}

	//Reads the key from the file, algins it, and generates the round keys
	uint64_t key_Block = 0;
	fread(&key_Block, sizeof(key_Block), 1, KEY_FILE);
	alignData(&key_Block);
	generate_round_keys(&key_Block);

	run_Algorithm(MODE,INPUT_FILE,OUTPUT_FILE);

}


/**
 * <---------------------------------------------MAIN--------------------------------------------->
*/


int main (const int argc, char** argv) {

	//Check whether correct number of arguments is entered
	if (argc != 5) {
	 	printf("Incorrect number of arguments specified\n");
	 	return 0;
	}

	//Extract the Mode and file names from the terminal.
	const char MODE = argv[1][0];
	const char* KEY_F = argv[2];
	const char* INPUT = argv[3];
	const char* OUTPUT = argv[4];

	//Run the DES algorithm
	DES(MODE, KEY_F, INPUT, OUTPUT);

}



