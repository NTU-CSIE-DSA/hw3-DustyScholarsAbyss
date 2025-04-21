// Copied from DSA 2024 HW3 Dynamic Spectrum Artistry
// Repo: https://github.com/NTU-CSIE-DSA/hw3-DynamicSpectrumArtistry/blob/main/gen/gen_copy.cpp
#include "jngen.h"
#include <climits>
#include <cassert>
#include <set>
#include <queue>
#include <utility>
using namespace std;

// Usage: genCopy <filename>
int main(int argc, char* argv[]) {
    registerGen(argc, argv);
    assert(argc == 2);
    string filename = argv[1];
    filename = "../tests/" + filename + ".in";
    ifstream inputFile(filename);
    vector<string> lines;
    assert(inputFile.is_open());

    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();

    lines[0].replace(lines[0].end() - 1, lines[0].end(), "2");

    for (const auto& updatedLine : lines) {
        cout << updatedLine << "\n";
    }
    return 0;
}