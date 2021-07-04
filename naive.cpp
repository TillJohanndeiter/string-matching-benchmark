#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <cstring>


using namespace std;

const string SPLIT_PATTERN_TEXT = "SPLIT_PATTERN_TEXT";


vector<int> search(char* txt, char* pat) {

    vector<int> startIndices; 

    int m = strlen(pat);
    int n = strlen(txt);
 
    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (txt[i + j] != pat[j])
                break;
        }
            
 
        if (j == m) {
            startIndices.push_back(i);
        }

    }

    return startIndices;
}




int main(int argc, char *argv[]) {

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


    string filename = argv[1];

    int n = stoi(filename.substr(filename.find_last_of('=') + 1, filename.find_last_of('.')));

    cout << "Open File: " << filename << endl;

    vector<char*> dataset;

    ifstream is(filename);
    string str;
    int line = 0;

    while(getline(is, str)) {
        
        int pos = str.find(SPLIT_PATTERN_TEXT);
        string txt = str.substr(0, pos);
        char* char_array = new char[txt.length() + 1];

        strcpy(char_array, txt.c_str());


        dataset.push_back(char_array);
        line++;


        string pattern = str.substr(pos + SPLIT_PATTERN_TEXT.length());
        char_array = new char[pattern.length() + 1];
        strcpy(char_array, pattern.c_str());
        dataset.push_back(char_array);
        line++;
    
    }

    for (int i = 0; i < n; i+=2) {
          auto t1 = high_resolution_clock::now();
          vector<int> startIndices = search(dataset[i], dataset[i+1]);
          auto t2 = high_resolution_clock::now();
          duration<double, std::milli> ms_double = t2 - t1;

          cout << ms_double.count() << endl;
    }
}