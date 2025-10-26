#include <string>
#include <vector>
#include <iostream>
#include "utils.cpp"
#include "vocab_set.cpp"
#include "programm_functions.cpp"

using namespace std;

int main() {
    Utils utils = Utils();
    VocabSet vs = VocabSet();
    ProgrammFunctions pf = ProgrammFunctions(vs, utils);
    pf.startProgram();
    return 0;
}

