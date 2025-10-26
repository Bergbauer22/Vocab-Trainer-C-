#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <numeric>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

class Utils {
public:
    void clearConsole() {
        cout.flush(); // <-- zwingt cout, alles sofort auszugeben
        // Clear console based on the operating system
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    string clearString(string text) {
        text.erase(
            remove_if(text.begin(), text.end(),
            [](char c) { return c == ';' || c == '@' || c == ',' || c == '#' || c == '%'; }),
            text.end()
        );
        return text;
    }

    string cleanTextInput(){
        string input;
        getline(cin, input);
        return clearString(input);
    }

    int getValidatedIntegerInput(int min, int max) {
        int input;
        while (true) {
            cin >> input;
            if (cin.fail() || input < min || input > max) {
                cin.clear(); // clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
                cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard any extra input
                return input;
            }
        }
    }

    template<typename T>
    T randomElement(const vector<T>& vec) {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> distr(0, vec.size() - 1);
        return vec[distr(gen)];
    }

};