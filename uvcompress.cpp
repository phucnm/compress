#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

using pii = std::pair<int, int>;

// num_bits to keep the prefix zeros
// e.g. 0011 0000 -> 00001100 not 000011
// this behavior thanks to `remain`
uint16_t reverse_bits(uint16_t val, int num_bits) {
    uint16_t res = 0;
    uint16_t _val = val;
    int remain = num_bits;
    while (_val) {
        res <<= 1;
        res |= _val & 1;
        _val >>= 1;
        remain--;
    }
    res <<= remain;
    return res;
}

// output bytes as long as the buffer is larger or equal 8 bits
std::pair<int, int> output(uint16_t val, int num_bits, std::pair<int, int> left_over_pair) {
    uint16_t rev = reverse_bits(val, num_bits);
    // compute the number of remaining bits of val after being concat'ed to the left over
    int left_over_size = num_bits - 8 + left_over_pair.second;
    // byte to output, consists of left over and a prefix from rev
    uint16_t res = (left_over_pair.first << (8-left_over_pair.second)) ^ (rev >> left_over_size);
    std::cout.put(reverse_bits(res, 8));

    // Compute left-over
    int left_over = rev & ((1 << left_over_size) - 1);
    // Output left over if sufficient
    while (left_over_size >= 8) {
        std::cout.put(reverse_bits(left_over >> (left_over_size - 8), 8));
        left_over_size -= 8;
        left_over &= (1 << left_over_size) - 1;
    }

    return std::make_pair(left_over, left_over_size);
}

int main(int argc, const char * argv[]) {
    int next_symbol = 257;
    int num_bits = 9;
    int max_bits = 16;
    int two_to_max_bits = 1 << max_bits;

    std::string working;
    std::string aug;

    std::unordered_map<std::string, int> symbol_table;
    for (int i = 0; i <= 255; i++) {
        std::string s(1, char(i));
        symbol_table[s] = i;
    }

    char c;
    pii left_over_pair = std::make_pair(0, 0);

    std::cout.put(0x1f);
    std::cout.put(0x9d);
    std::cout.put(0x90);
    while(std::cin.get(c)){
        aug = working + c;
        if (symbol_table.find(aug) != symbol_table.end()) {
            working = aug;
        } else if (next_symbol >= two_to_max_bits) {
            left_over_pair = output(symbol_table[working], num_bits, left_over_pair);
            working = std::string(1, c);
        } else {
            symbol_table[aug] = next_symbol++;
            left_over_pair = output(symbol_table[working], num_bits, left_over_pair);
            working = std::string(1, c);

            if (next_symbol > (1 << num_bits)) {
                num_bits++;
            }
        }
    }
    if (!working.empty()) {
        //last char
        left_over_pair = output(symbol_table[working], num_bits, left_over_pair);
    }
    //pad the last byte and output
    if (left_over_pair.second > 0) {
        std::cout.put( reverse_bits(left_over_pair.first << (8 - left_over_pair.second), 8) );
    }

    return 0;
}
