#pragma once

#include <string>
#include <map>
#include <iostream>
#include <limits>
#include <numeric>
#include <cstdlib>
#include <fstream>
#include <iterator>

#include "utils.cpp"
#include "vocab_set.cpp"
#include "struct.cpp"


using namespace std;

class ProgrammFunctions {

private:
    VocabSet vocabSet;
    Utils utils;

    int selectMode() {
        cout << "Please select an option:" << endl;
        cout << "---------------------------" << endl;
        cout << "1. Open Vocabulary Set" << endl;
        cout << "2. Create Vocabulary Set" << endl;

        int mode = utils.getValidatedIntegerInput(1, 2);

        return mode;
    }
    void startCreateVocabSet() {
        cout << "Creating new vocab set..." << endl;
        cout << "Please enter the name of the new vocab set: ";
        string setName = utils.cleanTextInput();
        cout << "Please enter the first language: ";
        string language1 = utils.cleanTextInput();
        cout << "Please enter the second language: ";
        string language2 = utils.cleanTextInput();
        vocabSet.createVocabSet(setName, language1, language2);
    }
    void startOpenVocabSet() {
        listVocabSets();
        if(vocabSet.getVocabSets().empty()) {
            cout << "---------------------------" << endl;
            cout << "No vocab sets available." << endl;
            cout << "---------------------------" << endl;
            startCreateVocabSet();
            startProgram();
        }

        cout << "Please enter the name of the vocab set you want to open: ";
        string name = utils.cleanTextInput();

        if(vocabSet.vocabSetExists(name)) {
            cout << "Opening vocab set: " << name << endl;
            selectSetMenu(name);
        } else {
            cout << "The vocab set \"" << name << "\" does not exist." << endl;
            startOpenVocabSet();
        }
    }

    void selectSetMenu(string setName) {
        cout << "Select an option:" << endl;
        cout << "---------------------------" << endl;
        cout << "1. Train Set" << endl;
        cout << "2. Edit Set" << endl;
        cout << "---------------------------" << endl;
        cout << "3. Back to Main Menu" << endl;

        int option = utils.getValidatedIntegerInput(1, 3);
        switch (option)
        {
            case 1:
                startTrainSet(setName);
                break;
            case 2:
                editSetMenu(setName);
                break;
            case 3:
                startProgram();
                break;
        }
    }
    void startTrainSet(string setName) {
        if(vocabSet.getVocabSets().at(setName).vocabulary.empty()) {
            cout << "The selected vocab set is empty. Please add vocabulary before training." << endl;
            editSetMenu(setName);
            return;
        }
        cout << "Starting training for vocab set: " << setName << endl;

        SetContent sc = vocabSet.getVocabSets().at(setName);
        int correctAnswers = 0;
        int totalQuestions = sc.vocabulary.size();
        
        // Zufallsgenerator
        random_device rd;
        mt19937 gen(rd());
        vector<Vocab> vocabs = sc.vocabulary;
        // Reihenfolge zufällig mischen
        shuffle(vocabs.begin(), vocabs.end(), gen);

        for (const auto& vocab : vocabs) {
            cout << "Please translate the word (" << sc.language1 << "): " << vocab.word1 << endl;
            string answer = utils.cleanTextInput();
            if(answer == vocab.word2) {
                cout << "Correct!" << endl;
                correctAnswers++;
            } else {
                cout << "Incorrect. The correct translation is: " << vocab.word2 << endl;
            }
        }
        cout << "---------------------------" << endl;
        cout << "Training completed. You answered " << correctAnswers << " out of " << totalQuestions << " correctly." << endl;
        cout << "Your score: " << (static_cast<double>(correctAnswers) / totalQuestions) * 100 << "%" << endl;
        cout << "---------------------------" << endl;
        startProgram();
    }


    void editSetMenu(string setName) {
        cout << endl;
        cout << "Select an option:" << endl;
        cout << "---------------------------" << endl;
        cout << "1. Add Vocabulary" << endl;
        cout << "2. Remove Vocabulary" << endl;
        cout << "3. List Vocabulary" << endl;
        cout << "---------------------------" << endl;
        cout << "4. Rename Set" << endl;
        cout << "5. Delete Set" << endl;
        cout << "---------------------------" << endl;
        cout << "6. Back to Main Menu" << endl;

        int option = utils.getValidatedIntegerInput(1, 6);
        switch (option)
        {
            case 1:
                addVocabulary(setName);
                editSetMenu(setName);
                break;
            case 2:
                removeVocabulary(setName);
                editSetMenu(setName);
                break;
            case 3:
                listVocabulary(setName);
                editSetMenu(setName);
                break;
            case 4:
                renameSet(setName);
                startProgram();
                break;
            case 5:
                deleteSet(setName);
                startProgram();
                break;
            case 6:
                startProgram();
                break;
        }
    }

    void addVocabulary(string setName) {
        SetContent sc = vocabSet.getVocabSets().at(setName);
        cout << "Adding new vocabulary..." << endl;
        cout << "Please enter the (" << sc.language1 << ") word: ";
        string word1 = utils.cleanTextInput();
        cout << "Please enter the (" << sc.language2 << ") word: ";
        string word2 = utils.cleanTextInput();
        Vocab vocab = {word1, word2};
        vocabSet.addVocabToSet(setName, vocab);
    }

    void removeVocabulary(string setName) {
        cout << "Removing vocabulary..." << endl;
        cout << "Please enter the word in the first language to remove: ";
        string word1 = utils.cleanTextInput();
        vocabSet.removeVocabFromSet(setName, word1);
    }

    void listVocabulary(string setName) {
        SetContent sc = vocabSet.getVocabSets().at(setName);
        cout << "Vocabulary in set '" << setName << "':" << endl;
        for (const auto& vocab : sc.vocabulary) {
            cout << "- " << vocab.word1 << " : " << vocab.word2 << endl;
        }
    }

    void renameSet(string setName) {
        cout << "Renaming vocab set..." << endl;
        cout << "Please enter the new name for the vocab set: ";
        string newName = utils.cleanTextInput();
        vocabSet.renameVocabSet(setName, newName);
    }

    void deleteSet(string setName) {
        vocabSet.deleteVocabSet(setName);
    }

    void listVocabSets() {
        map<string,SetContent> sets = vocabSet.getVocabSets();
        cout << "Available Vocabulary Sets:" << endl;
        for (const auto& element : sets) {
            cout << "- " << element.first << " (" << element.second.language1 << " - " << element.second.language2 << ")" << endl;
        }
    }

    public:
    //Constructor
    ProgrammFunctions(VocabSet vocabSet, Utils utils) {
        this->vocabSet = vocabSet;
        this->utils = utils;
    }

    void startProgram() {
        cout << "Welcome to your Vocabulary Trainer!" << endl;
        label_main_menu:
        int mode = selectMode();
        switch (mode)
        {
            case 1:
                startOpenVocabSet();
                break;
            case 2:
                startCreateVocabSet();
                goto label_main_menu;
        }
    }
};