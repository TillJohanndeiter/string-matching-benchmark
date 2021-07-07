// Source: https://www.geeksforgeeks.org/boyer-moore-algorithm-for-pattern-searching/ and https://www.geeksforgeeks.org/boyer-moore-algorithm-good-suffix-heuristic/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <cstring>
#include <omp.h>

using namespace std;

const string SPLIT_PATTERN_TEXT = "SPLIT_PATTERN_TEXT";

  
void computeLPSArray(char* pat, int M, int* lps);
  
// Prints occurrences of txt[] in pat[]
vector<int> search(char* txt, char* pat)
{
    vector<int> startIndices; 

    int M = strlen(pat);
    int N = strlen(txt);
  
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];
  
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);

    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int istart = id * (N / nthreads);
        int iend = min((id + 1) * (N / nthreads), N);
        int i = istart; // index for txt[]
        int j = 0; // index for pat[]


        
        while (i < iend) {

        
            if (pat[j] == txt[i]) {
                j++;
                i++;
            }
    
            if (j == M) {
                #pragma omp critical
                {   
                    startIndices.push_back(i - j);
                }
                j = lps[j - 1];
            }
    
            // mismatch after j matches
            else if (i < iend && pat[j] != txt[i]) {
                // Do not match lps[0..lps[j-1]] characters,
                // they will match anyway
                if (j != 0)
                    j = lps[j - 1];
                else
                    i = i + 1;
            }
        }

    }

    

    return startIndices;
}
  
// Fills lps[] for given patttern pat[0..M-1]
void computeLPSArray(char* pat, int M, int* lps)
{
    // length of the previous longest prefix suffix
    int len = 0;
  
    lps[0] = 0; // lps[0] is always 0
  
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0) {
                len = lps[len - 1];
  
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
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

    for (int i = 0; i < dataset.size(); i+=2) {
          auto t1 = high_resolution_clock::now();
          vector<int> startIndices = search(dataset[i], dataset[i+1]);
          auto t2 = high_resolution_clock::now();
          duration<double, std::milli> ms_double = t2 - t1;

          cout << ms_double.count() << endl;
    }
}