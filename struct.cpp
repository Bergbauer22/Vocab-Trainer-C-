#pragma once

#include <string>
#include <vector>

using namespace std;

struct Vocab {
    string word1;  
    string word2;  
};

struct SetContent
{
    string language1;
    string language2;
    vector<Vocab> vocabulary;
};