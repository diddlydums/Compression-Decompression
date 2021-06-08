
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

//Pseudo decompression with ascii encoding and naive header (checkpoint)

void pseudoDecompression(const string& inFileName, const string& outFileName) {
    HCTree Tree;
    string inFilename = "toCompress.txt";
    string outFilename = "compressed.txt";
    ifstream inFile;
    ofstream outFile;
    std::vector<unsigned int> freq(256);
    inFile.open(inFileName);
    outFile.open(outFileName);
    string str;
    unsigned int x;
    unsigned int count = 0;

    for (int i = 0; i < 256; ++i) {
        getline(inFile, str);
        x = stoi(str);
        freq[i] = x;
        count += freq[i];
    }

    Tree.build(freq);

    for (int i = 0; i < count; ++i) {
        outFile << Tree.decode(inFile);
    }

    inFile.close();
    outFile.close();
}
/* True decompression with bitwise i/o and small header  */
void trueDecompression(const string& inFileName, const string& outFileName) {
    HCTree Tree;
    string inFilename = "toCompress.txt";
    string outFilename = "compressed.txt";
    ifstream inFile;
    ofstream outFile;
    std::vector<unsigned int> freq(256);
    BitInputStream bitIn(inFile);
    inFile.open(inFileName);
    outFile.open(outFileName);
    string str;
    unsigned int x;
    unsigned int count = 0;

    for (int i = 0; i < 256; ++i) {
        getline(inFile, str);
        x = stoi(str);
        freq[i] = x;
        count += freq[i];
    }

    Tree.build(freq);

    for (int i = 0; i < count; ++i) {
        outFile << Tree.decode(bitIn);
    }

    inFile.close();
    outFile.close();
}

/* Main program that runs the decompression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Uncompresses files using Huffman Encoding");
    options.positional_help(
        "./path_to_compressed_input_file ./path_to_output_file");

    bool isAscii = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Read input in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAscii))("input", "",
                                       cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit.");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }

    // if compressed file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        exit(0);
    }

    if (isAscii) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }

    return 0;
}
