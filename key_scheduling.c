
unsigned long long round_keys[16];

static const int PC1_Table[56] = {
    57,49,41,33,25,17,9,1,
    58,50,42,34,26,18,10,2,
    59,51,43,35,27,19,11,3,
    60,52,44,36,63,55,47,39,
    31,23,15,7,62,54,46,38,
    30,22,14,6,61,53,45,37,
    29,21,13,5,28,20,12,4
};

static const int PC2_Table[48] = {
    14,17,11,24,1,5,3,28,
    15,6,21,10,23,19,12,4,
    26,8,16,7,27,20,13,2,
    41,52,31,37,47,55,30,40,
    51,45,33,48,44,49,39,56,
    34,53,46,42,50,36,29,32
};

unsigned long long PC1(const unsigned long long * key64) {
    unsigned long long result=0;
    for (int i = 0; i < 56; i++) {
        result = (result << 1) | ( ( *key64 >> (64-PC1_Table[i]) ) & 1ULL );
    }
    return result;
}

void left_circular_shift(unsigned long long *key56, const int round_num) {
    unsigned long long left = (*key56 >> 28) & 0xFFFFFFF;
    unsigned long long right = *key56 & 0xFFFFFFF;
    int shift = ( round_num == 1 || round_num == 2 || round_num == 9 || round_num == 16) ? 1 : 2;
    left = ((left << shift) | (left >> (28 - shift))) & 0xFFFFFFF;
    right = ((right << shift) | (right >> (28 - shift))) & 0xFFFFFFF;
    *key56 = (left << 28) | right;

}

void PC2(const unsigned long long * key56,const int round_num) {
    unsigned long long result=0;
    for (int i = 0; i < 48; i++) {
        result = (result << 1) | ( ( *key56 >> (56-PC2_Table[i]) ) & 1ULL );
    }
    round_keys[(round_num)-1] = result;
}

void generate_round_keys(const unsigned long long * key64) {
    unsigned long long key56 = PC1(key64);
    for (int i = 1; i <= 16; i++) {
        left_circular_shift(&key56, i);
        PC2(&key56, i);
    }
}

