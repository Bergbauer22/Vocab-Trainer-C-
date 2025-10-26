#pragma once

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "struct.cpp"

using namespace std;

class VocabSet {

    map<string,SetContent> vocabSets;

    map<string,SetContent> loadVocabSets() {
        map<string,SetContent> vocabSets;
        fstream file;
        file.open("vocabsets.vs", ios::in);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                
                if (!line.empty() && line.back() == '\r') line.pop_back(); // Windows CR

                if (line.empty()) continue;

                size_t p1 = line.find(';');
                if (p1 == string::npos) continue;
                size_t p2 = line.find(';', p1 + 1);
                if (p2 == string::npos) continue;
                size_t p3 = line.find(';', p2 + 1);
                if (p3 == string::npos) {
                    // kein drittes Semikolon gefunden -> ungültige Zeile
                    continue;
                }

                string setName = line.substr(0, p1);
                string lang1   = line.substr(p1 + 1, p2 - p1 - 1);
                string lang2   = line.substr(p2 + 1, p3 - p2 - 1);
                string rest    = (p3 + 1 < line.size()) ? line.substr(p3 + 1) : "";

                SetContent sc;
                sc.language1 = lang1;
                sc.language2 = lang2;
                //cout << "Loaded vocab set: " << setName << " (" << lang1 << " - " << lang2 << ")" << endl;

                size_t start = 0;
                while (start < rest.size()) {
                    size_t commaPos = rest.find(',', start);
                    string vocabPair;
                    if (commaPos == string::npos) {
                        vocabPair = rest.substr(start);
                        start = rest.size();
                    } else {
                        vocabPair = rest.substr(start, commaPos - start);
                        start = commaPos + 1;
                    }

                    size_t atPos = vocabPair.find('@');
                    if (atPos != string::npos) {
                        string word1 = vocabPair.substr(0, atPos);
                        string word2 = vocabPair.substr(atPos + 1);
                        sc.vocabulary.push_back({word1, word2});
                    }
                }
                vocabSets[setName] = sc;
            }
            file.close();
        } else {
            cout << "Error opening file." << endl;
        }
        return vocabSets;
    }

    void saveVocabSet(map<string,SetContent> vocabSet) {
        fstream file;
        file.open("vocabsets.vs", ios::out);
        if (file.is_open()) {
            // Convert vocabSet to JSON and write to file
            for(const auto& element : vocabSet) {
                file << element.first << ";" << element.second.language1 << ";" << element.second.language2 << ";";
                for(const auto& vocab : element.second.vocabulary) {
                    file << vocab.word1 << "@" << vocab.word2 << ",";
                }
                file << "\n";
            }
            file.close();
        } else {
            cout << "Error opening file for writing." << endl;
        }
    }

    public:
        //Constructor
        VocabSet() {
            vocabSets = loadVocabSets();
        }

        void createVocabSet(string setName, string language1, string language2) {
            if(vocabSets.find(setName) != vocabSets.end()) {
                cout << "A vocab set with this name already exists." << endl;
                return;
            }
            vocabSets[setName] = {language1, language2, {}};
            saveVocabSet(vocabSets);
            cout << "Vocab set '" << setName << "' created successfully." << endl;
        }

        void addVocabToSet(string setName, Vocab vocab) {
            if(vocabSets.find(setName) == vocabSets.end()) {
                cout << "Vocab set not found." << endl;
                return;
            }
            vocabSets[setName].vocabulary.push_back(vocab);
            saveVocabSet(vocabSets);
            cout << "Vocabulary added to set '" << setName << "' successfully." << endl;
        }
        void removeVocabFromSet(string setName, string word1) {
            if(vocabSets.find(setName) == vocabSets.end()) {
                cout << "Vocab set not found." << endl;
                return;
            }
            auto& vocabList = vocabSets[setName].vocabulary;
            vocabList.erase(remove_if(vocabList.begin(), vocabList.end(),
                [&](const Vocab& v) { return v.word1 == word1; }), vocabList.end());
            saveVocabSet(vocabSets);
            cout << "Vocabulary removed from set '" << setName << "' successfully." << endl;
        }

        map<string,SetContent> getVocabSets() {
            return vocabSets;
        }
        bool vocabSetExists(string setName) {
            return vocabSets.find(setName) != vocabSets.end();
        }
        void renameVocabSet(string oldName, string newName) {
            if(vocabSets.find(oldName) == vocabSets.end()) {
                cout << "Vocab set not found." << endl;
                return;
            }
            if(vocabSets.find(newName) != vocabSets.end()) {
                cout << "A vocab set with the new name already exists." << endl;
                return;
            }
            vocabSets[newName] = vocabSets[oldName];
            vocabSets.erase(oldName);
            saveVocabSet(vocabSets);
            cout << "Vocab set renamed from '" << oldName << "' to '" << newName << "' successfully." << endl;
        }
        void deleteVocabSet(string setName) {
            if(vocabSets.find(setName) == vocabSets.end()) {
                cout << "Vocab set not found." << endl;
                return;
            }
            vocabSets.erase(setName);
            saveVocabSet(vocabSets);
            cout << "Vocab set '" << setName << "' deleted successfully." << endl;
        }
};