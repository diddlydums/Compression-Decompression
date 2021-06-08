#include "BitInputStream.hpp"

// Fill the buffer from the input stream.
void BitInputStream::fill() {
    buf = in.get();
    nbits = 0;
}

// Read the next bit from the buffer.
unsigned int BitInputStream::readBit() {
    if (nbits == 8) {
        fill();
    }
    bool i = (buf & (1 << (7 - nbits)));

    nbits++;
    if (i) {
        return 1;
    } else {
        return 0;
    }
}
