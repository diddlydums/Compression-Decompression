#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    bos.writeBit(1);
    bos.flush();

    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}
TEST(BitOutputStreamTests, SIMPLE_TEST2) {
    stringstream ss;
    BitOutputStream bos(ss);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.flush();

    string bitsStr = "11100000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}
TEST(BitOutputStreamTests, SIMPLE_TEST3) {
    stringstream ss;
    BitOutputStream bos(ss);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
 

    string bitsStr = "11111111";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}
