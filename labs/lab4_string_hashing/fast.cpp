#include <iostream>
#include <fstream>

using namespace std;

const int M = 1000000007;
const int x = 2;

int main() {
    int k = 0;

    string cmd;
    int val;

    cin >> k;
    int *A = new int[k * 2];

    for (int i = 0; i < k; i++) {
        cin >> cmd >> val;
        if (cmd == "F") A[i * 2] = 0;
        if (cmd == "L") A[i * 2] = 1;
        if (cmd == "R") A[i * 2] = 2;
        A[i * 2 + 1] = val;
    }

    int index = 0;

    int hash = 0;
    for (int i = 0; i < k * 2; i++) {
        hash = ((((long long)hash * x) % M) + A[i]) % M;
    }

    int newHash = hash;
    bool isMatch = true;

    int p = 1;
    for (int i = 0; i < k * 2 - 1; i++) { p = ((long long)p * x) % M; }

    for (int i = 0; i < k * 2 - 1; i++) {
        newHash = ((((long long)((newHash - (((long long)A[i] * p) % M) + M) % M) * x) % M) + A[i]) % M;
        
        if (hash == newHash) {
            for (int j = 0; j < k*2; j++) {
                if (A[j] != A[(j+i+1) % (k*2)]) {
                    isMatch = false;
                    break;
                }
            }
            if (isMatch) {
                index = i;
                break;
            }    
        }
    }
    cout << double(k * 2) / (index + 1) << endl;
    delete [] A;
    return 0;
}