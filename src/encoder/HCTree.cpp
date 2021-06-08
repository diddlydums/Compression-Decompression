#include "HCTree.hpp"

#include <stack>
void clean(HCNode* n) {
    if (n != nullptr) {
        clean(n->c0);
        clean(n->c1);
        delete n;
    }
}
/* Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() {
    clean(root);
    root = nullptr;
    delete root;
}

/**
 * Build the HCTree from the given frequency vector. Assume the
 * vector must have size 256 and each value at index i represents the frequency
 * of char with ASCII value i. Only non-zero frequency symbols should be used to
 * build the tree. The leaves vector must be updated so that it can be used in
 * encode() to improve performance.
 *
 *    1. HCNode with lower count should have higher priority. If count is the
 * same, then HCNode with a larger ascii value symbol should have higher
 * priority. (This should be already defined properly in the comparator in
 * HCNode.hpp)
 *    2. When popping two highest priority nodes from PQ, the higher priority
 * node will be the ‘c0’ child of the new parent HCNode.
 *    3. The symbol of any parent node should be taken from its 'c0' child.
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    // priority queue
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> que;
    // through frequency vector creat node for each symbol
    for (int i = 0; i < freqs.size(); ++i) {
        if (freqs[i] != 0) {
            // new HCnode
            HCNode* newN = new HCNode(freqs[i], i);
            // push node to queue
            que.push(newN);
            // add to leaves
            leaves[i] = newN;
        }
    }
    // check for non-empty vector
    if (que.size() != 0) {
        // more than 1 elements
        while (que.size() > 1) {
            HCNode* c0;
            HCNode* c1;
            // first element
            c0 = que.top();
            que.pop();
            // second element
            c1 = que.top();
            que.pop();
            // newnode parent for these Nodes
            int add = c0->count + c1->count;
            HCNode* newN = new HCNode(add, c0->symbol);
            // newnod for c1 , c2
            newN->c0 = c0;
            newN->c1 = c1;
            // parent for c1, c2
            c0->p = newN;
            c1->p = newN;
            // push to queue
            que.push(newN);
        }
        // set last elemt to root and then pop the element
        root = que.top();
        que.pop();
    }
}

/**
 * Write the encoding bits of the given symbol to the ostream. 
 * Write each encoding bit as ascii char either '0' or '1' to the ostream.
 * Must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    if (root) {
        stack<unsigned int> stack;
        string bitString;
        HCNode* myleaf = leaves[symbol];
        // If just 1 character
        if (myleaf == root) {
            out.writeBit(0);
        } else {
            // make the tree upwards, pushing pattern in stackk
            while (myleaf != root) {
                if (myleaf->p->c1 == myleaf) {
                    stack.push(1);
                } else {
                    stack.push(0);
                }
                myleaf = myleaf->p;
            }
            // empty the stack and build encode string
            while (!stack.empty()) {
                out.writeBit(stack.top());
                stack.pop();
            }
        }
    }
}

/**
 * Write the encoding bits of the given symbol to ostream.
 * Write each encoding bit as ascii char either '0' or '1' to the ostream. Must  * not perform a comprehensive search to find the encoding bits of the
 * given symbol, and should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been called
 * beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    // if root is null so check tree has been built
    if (root) {
        stack<char> stack;
        string bitString;
        HCNode* myleaf = leaves[symbol];
        // If just 1 character
        if (myleaf == root) {
            bitString = bitString + '0';
        } else {
            // make the tree upwards, pushing pattern in stackk
            while (myleaf != root) {
                if (myleaf->p->c1 == myleaf) {
                    stack.push('1');
                } else {
                    stack.push('0');
                }
                myleaf = myleaf->p;
            }
            // empty the stack and build encode string
            while (!stack.empty()) {
                bitString = bitString + stack.top();
                stack.pop();
            }
        }
        out << bitString;
    }
}

/**
 * Decode the sequence of bits (represented as a char of either '0' or
 * '1') from the istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */

byte HCTree::decode(BitInputStream& in) const {
    if (root) {
        HCNode* myleaf = root;
        byte tmp;
        // Traverse down the tree until reach a leaf node
        while (myleaf->c0 && myleaf->c1) {
            // get single left-character to 0 and right to 1
            tmp = in.readBit();
            if (tmp == 0) {
                myleaf = myleaf->c0;
            } else {
                myleaf = myleaf->c1;
            }
        }
        return myleaf->symbol;
    }
}

/**
 * Decode the sequence of bits (represented as char of either '0' or '1')
 * from istream to return the coded symbol. For this function to work, build()
 * must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const {
    if (root) {
        HCNode* myleaf = root;
        char tmp;
        // Traverse down the tree until reach a leaf node
        while (myleaf->c0 && myleaf->c1) {
            // get single left-character to 0 and right to 1
            tmp = in.get();
            if (tmp == '0') {
                myleaf = myleaf->c0;
            } else {
                myleaf = myleaf->c1;
            }
        }
        return myleaf->symbol;
    }
}
