#include <stdint.h>

uint64_t get_bit(uint32_t R, int pos) {
    return (R >> (32-pos)) & 1;
}

//expansion permutation function
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

//f function: expansion + xor with key
uint64_t f_function(uint32_t R, uint64_t K){
    uint64_t expanded_R = EP(R);
    uint64_t result = expanded_R ^ K;
    return result;
}

