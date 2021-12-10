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
    bool isMatch = false;

    k = k * 2;

    for (int i = 1; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (A[j] != A[(i + j) % k]) {
                break;
            }
            if (j == k - 1) isMatch = true;
        }
        if (isMatch) {
            index = i;
            break;
        }
    }
    cout << (double)k / index << endl;
    
    delete [] A;
    return 0;
}