// Source: https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <omp.h>


using namespace std;

# define NO_OF_CHARS 256

const string SPLIT_PATTERN_TEXT = "SPLIT_PATTERN_TEXT";


// preprocessing for strong good suffix rule
void preprocess_strong_suffix(int *shift, int *bpos, char *pat, int m) {
    // m is the length of pattern 
    int i = m, j = m+1;
    bpos[i] = j;
  
    while(i > 0)
    {
        /*if character at position i-1 is not equivalent to
          character at j-1, then continue searching to right
          of the pattern for border */
        while(j <= m && pat[i-1] != pat[j-1])
        {
            /* the character preceding the occurrence of t in 
               pattern P is different than the mismatching character in P, 
               we stop skipping the occurrences and shift the pattern
               from i to j */
            if (shift[j]==0)
                shift[j] = j - i;
  
            //Update the position of next border 
            j = bpos[j];
        }
        /* p[i-1] matched with p[j-1], border is found.
           store the  beginning position of border */
        i--;
        j--;
        bpos[i] = j; 
    }
}
  
//Preprocessing for case 2
void preprocess_case2(int *shift, int *bpos, char *pat, int m) {
    int i, j;
    j = bpos[0];
    for(i = 0; i <= m; i++)
    {
        /* set the border position of the first character of the pattern
           to all indices in array shift having shift[i] = 0 */ 
        if(shift[i] == 0)
            shift[i] = j;
  
        /* suffix becomes shorter than bpos[0], use the position of 
           next widest border as value of j */
        if (i == j)
            j = bpos[j];
    }
}
  



void badCharHeuristic(char* str, int size, int badchar[NO_OF_CHARS]){
 
    // Initialize all occurrences as -1
    for (int i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;
 
    // Fill the actual value of last occurrence
    // of a character
    for (int i = 0; i < size; i++)
        badchar[(int) str[i]] = i;


    
}
 
/* A pattern searching function that uses Bad
Character Heuristic of Boyer Moore Algorithm */
vector<int> search(char* txt, char* pat) {
    vector<int> startIndices; 

    int m = strlen(pat);
    int n = strlen(txt);

    int badchar[NO_OF_CHARS];
    int bpos[m+1], shift[m+1];
        

    /* Fill the bad character array by calling
    the preprocessing function badCharHeuristic()
    for given pattern */
    badCharHeuristic(pat, m, badchar);


    for(int i = 0; i < m+1; i++) {
        shift[i] = 0;
    }


    preprocess_strong_suffix(shift, bpos, pat, m);
    preprocess_case2(shift, bpos, pat, m);

    #pragma omp parallel
    {

        int nthreads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int sstart = id * ( (n - m) / nthreads);
        //cout << sstart << endl;
        int send = min((id + 1) * ( (n - m) / nthreads), n - m);

        int j = 0; // index for pat[]


        int s = sstart; // s is shift of the pattern with
                    // respect to text
        while(s <= send) {
            int j = m - 1;
    
            /* Keep reducing index j of pattern while
            characters of pattern and text are
            matching at this shift s */
            while(j >= 0 && pat[j] == txt[s + j]) {
                j--;
            }
    
            /* If the pattern is present at current
            shift, then index j will become -1 after
            the above loop */
            if (j < 0) {
                  #pragma omp critical
                  {  
                      startIndices.push_back(s);
                  }
                /* Shift the pattern so that the next
                character in text aligns with the last
                occurrence of it in pattern.
                The condition s+m < n is necessary for
                the case when pattern occurs at the end
                of text */
                s += max ((s + m < n)? m - badchar[txt[s + m]] : 1, shift[0]);
    
            } else
                /* Shift the pattern so that the bad character
                in text aligns with the last occurrence of
                it in pattern. The max function is used to
                make sure that we get a positive shift.
                We may get a negative shift if the last
                occurrence of bad character in pattern
                is on the right side of the current
                character. */
                s += max(j - badchar[txt[s + j]], shift[j + 1]);
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

    for (int i = 0; i < dataset.size(); i+=2) {
          auto t1 = high_resolution_clock::now();
          vector<int> startIndices = search(dataset[i], dataset[i+1]);
          auto t2 = high_resolution_clock::now();
          duration<double, std::milli> ms_double = t2 - t1;

          cout << ms_double.count() << endl;
    }
}