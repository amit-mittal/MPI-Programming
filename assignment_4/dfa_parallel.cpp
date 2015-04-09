/*
    Input file must have the following format:
        N M
        F a1 a2 ... af
        s1 y1 t1
        s2 y2 y2
        :
        :
        Len c1 c2 c3 ...... cLen
    Here,   
        N -> No. of states in the DFA (states are numbered 0 ... N-1)
        M -> Size of input alphabet (input symbols are numbered 1 ... M)
        F -> No. of final states followed by F states ( 0 <= ai <= N-1)
        si -> Previous state
        yi -> Input symbol
        ti -> Next state
        Len -> Length of input string
        ci -> Alphabets of string
    Each line until the end of file contains one transition ( si yi ti ).
    '0' is the start state.
    The input strings contain space-separated input symbols (1 ... M).
*/

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <omp.h>
#include <cmath>

#define MAX_DFA_STATES 10
#define MAX_ALPHABET_SIZE 10

using namespace std;

int transitions[MAX_DFA_STATES][MAX_ALPHABET_SIZE];
bool finalStates[MAX_DFA_STATES];
int input_string[1000000];

int main()  {
    int N, M, F, X, Y, A, state, symbol, i, j, len;
    char* p;

    // Initializing the DFA
    cin >> N >> M >> F;
    for(i=0; i<F; i++)  {
        cin >> X;
        finalStates[X] = true;
    }

    memset(transitions, -1, MAX_DFA_STATES * MAX_ALPHABET_SIZE * sizeof(int));
    for(i=0; i<(N*M); i++){
        cin >> X >> A >> Y;
        transitions[X][A] = Y;
    }

    // Taking input string
    cin >> len;
    for (i = 0; i < len; ++i)
        cin >> input_string[i];
    // ==============INPUT PART DONE==============
    

    // ==============PARALLEL IMPLEMENTATION==============
    int total_threads = 2;
    int each_part = ceil((double)len/total_threads);
    int states[total_threads][N];
    
    #pragma omp parallel for
    for(int i=0;i<total_threads;++i){
        for (int j = 0; j < N; ++j){
            states[i][j] = j;
        }
    }

    // simulate the DFA
    omp_set_nested(true);
    #pragma omp parallel num_threads(total_threads)
    {
        int thread_id = omp_get_thread_num();
        int start = thread_id*each_part;
        int end = min(len, start + each_part);
        if(thread_id == 0)
        {
            int q = 0;
            for (int i = start; i < end; ++i)
            {
                q = transitions[q][input_string[i]];
            }
            states[0][0] = q;
        }
        else
        {
            // Below is slowing down the program
            #pragma omp parallel num_threads(total_threads)
            for (int q = 0; q < N; ++q)
            {
                int state = q;
                for (int i = start; i < end; ++i)
                {
                    state = transitions[state][input_string[i]];
                }
                states[thread_id][q] = state;
            }
        }
    }

    /*for(i=0;i<total_threads;++i){
        for (int j = 0; j < N; ++j){
            cout << states[i][j] << " ";
        }
        cout << endl;
    }*/

    int final = 0;
    for(i=0;i<total_threads;++i){
        final = states[i][final];
    }

    if(finalStates[final])
        printf("String accepted.\n");
    else
        printf("String rejected.\n");

    return 0;
}