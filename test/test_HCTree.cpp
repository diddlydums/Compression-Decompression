#include <gtest/gtest.h>

#include <bitset>
#include <iostream>
#include <string>
#include <vector>

#include "HCNode.hpp"
#include "HCTree.hpp"

using namespace std;
using namespace testing;

TEST(HCNode_TEST, comparator_test) {
    HCNode* ptrS = new HCNode(23, (byte)'A');
    HCNode* ptrL = new HCNode(53, (byte)'E');
    HCNode* ptrS2 = new HCNode(23, (byte)'E');
    HCNodePtrComp comp;
    /* Check if returns the right bool for when the freq differ
     * lower priority is before higher so smaller freq < higher freg
     */
    bool test = comp(ptrS, ptrL);
    ASSERT_EQ(test, false);

    /* Check if returns the right bool for when freq are the same but symbols
     * differ. Smaller symbol should have less priority so should return smaller
     * < larger
     */
    bool test_sameFreq = comp(ptrS, ptrS2);
    ASSERT_EQ(test_sameFreq, true);
}

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 1;
        freqs['b'] = 2;
        freqs['c'] = 1;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "01");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("1");
    ASSERT_EQ(tree.decode(is), 'b');
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE_Bits) {
    stringstream ss;
    BitOutputStream bitOut(ss);
    vector<unsigned int> freq(256, 0);
    char in[] = {'a', 'b', 'b', 'c'};
    for (char c : in) {
        freq[c] += 1;
    }
    HCTree tree;
    tree.build(freq);

    for (char c : in) {
        tree.encode((byte)c, bitOut);
    }
    bitOut.flush();
    string test;
    ss >> test;

    // 0111 0000
    // c- 00 a- 01, b- 1
    ASSERT_EQ(test, "p");
}
TEST_F(SimpleHCTreeFixture, TEST_DECODE_NULL) {
    stringstream ss;
    stringstream out;
    BitInputStream in(ss);
    byte test = 'p';  //'for 01110000--> abbc
    ss << test;

    for (int n = 0; n < 4; ++n) {
        out << tree.decode(in);
    }

    ASSERT_EQ(out.str(), "abbc");
}
