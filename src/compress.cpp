
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* add pseudo compression with ascii encoding and naive header */
void pseudoCompression(const string& inFileName, const string& outFileName) {
    HCTree Tree;
    string inFilename = "toCompress.txt";
    string outFilename = "compressed.txt";
    ifstream inFile;
    ofstream outFile;
    std::vector<unsigned int> freq(256, 0);
    inFile.open(inFileName);
    outFile.open(outFileName);
    byte nextChar = inFile.get();

    while (!inFile.eof()) {
        freq[nextChar]++;
        nextChar = inFile.get();
    }

    Tree.build(freq);

    for (unsigned int i : freq) {
        outFile << i << endl;
    }

    inFile.clear();
    inFile.seekg(0);

    nextChar = inFile.get();

    while (!inFile.eof()) {
        Tree.encode(nextChar, outFile);
        nextChar = inFile.get();
    }

    // add encoded message at the bottom of file
    inFile.close();
    outFile.close();
}
/*  True compression with bitwise i/o and small header (final) */
void trueCompression(const string& inFileName, const string& outFileName) {
    HCTree Tree;
    string inFilename = "toCompress.txt";
    string outFilename = "compressed.txt";
    ifstream inFile;
    ofstream outFile;
    BitOutputStream bitOut(outFile);
    std::vector<unsigned int> freq(256, 0);
    inFile.open(inFileName);
    outFile.open(outFileName);
    byte nextChar = inFile.get();

    while (!inFile.eof()) {
        freq[nextChar]++;
        nextChar = inFile.get();
    }

    Tree.build(freq);

    for (unsigned int i : freq) {
        outFile << i << endl;
    }

    inFile.clear();
    inFile.seekg(0);

    nextChar = inFile.get();

    while (true) {
        Tree.encode(nextChar, bitOut);
        nextChar = inFile.get();
        if (inFile.eof()) break;
    }
    bitOut.flush();

    // add encoded message at the bottom of file
    inFile.close();
    outFile.close();
}

/* Main program that runs the compression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    // if original file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        return 0;
    }

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}
