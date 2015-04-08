/*
 * Description :	Program to simulate a given DFA on an input string. It reads in the 
			DFA from "DFA.txt" and the input strings from the console and prints out
			whether the strings are accepted or rejected on the console.
					
			"DFA.txt" must have the following format:
				N M
				F a1 a2 ... af
				s1 y1 t1
				s2 y2 y2
				:
				:
			Here, 	
                N -> No. of states in the DFA (states are numbered 0 ... N-1)
				M -> Size of input alphabet (input symbols are numbered 1 ... M)
				F -> No. of final states followed by F states ( 0 <= ai <= N-1)
				si -> Previous state
				yi -> Input symbol
				ti -> Next state
			Each line until the end of file contains one transition ( si yi ti ).
 *			'0' is the start state.
 *			The input strings contain space-separated input symbols (1 ... M).
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
int input_string[100000];

int main()  {
    int N, M, F, X, Y, A, state, symbol, i, j, len;
    char* p;

    // read in the underlying DFA
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

    // check if the DFA is well defined
    for(i=0; i<N; i++){
        for(j=1; j<=M; j++){
            if(transitions[i][j] < 0 || transitions[i][j] >= N) {
                printf("DFA not defined properly.\n");
                return -1;
            }
        }
    }

    // simulate the DFA
    cin >> len;
    for (i = 0; i < len; ++i)
        cin >> input_string[i];
    // ==============INPUT PART DONE==============
    

    // ==============PARALLEL IMPLEMENTATION==============
    int total_threads = 3;
    int each_part = ceil((double)len/total_threads);
    int states[total_threads][N];
    
    #pragma omp parallel for
    for(i=0;i<total_threads;++i){
        for (int j = 0; j < N; ++j){
            states[i][j] = j;
        }
    }

    #pragma omp parallel num_threads(total_threads) shared(states)
    {
        int start = omp_get_thread_num()*each_part;
        int end = min(len, start + each_part);
        if(omp_get_thread_num() == 0)
        {
            int q = 0;
            for (i = start; i < end; ++i)
            {
                symbol = input_string[i];
                q = transitions[q][symbol];
            }
            states[0][0] = q;
        }
        else
        {
            #pragma omp single
            for (int q = 0; q < N; ++q)
            {
                state = q;
                for (i = start; i < end; ++i)
                {
                    symbol = input_string[i];
                    state = transitions[state][symbol];
                }
                states[omp_get_thread_num()][q] = state;    
            }
        }
    }

    for(i=0;i<total_threads;++i){
        for (int j = 0; j < N; ++j){
            cout << states[i][j] << " ";
        }
        cout << endl;
    }

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