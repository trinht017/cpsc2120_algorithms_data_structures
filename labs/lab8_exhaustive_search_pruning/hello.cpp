#include <iostream>
#include <vector>


using namespace std;
//loop over all places to put queens in row r
// for each of those, 
//     tentatively place a queen there
//     solve(r+1)
//     pick up the queen


//exploit symmetry
//n = 8 : 92
//n = 14 : 365596

bool **A;
int n;
int count;

bool isValid(int x, int y) {
    int i = x;
    int j = y;

    while (i > 0) {
        i--;
        if (A[i][j] == true) return false;
    }
    
    i = x;
    j = y;

    while (i > 0 && j > 0) {
        i--;
        j--;
        if (A[i][j] == true) return false;
    }

    i = x;
    j = y;

    while (i > 0 && j < n - 1) {
        i--;
        j++;
        if (A[i][j] == true) return false;
    }

    return true;

}

bool isOdd = false;

void check_row(int r) { 

    if (r == n) {
        if (isOdd) {
            count++;
        }
        else {
            count += 2;
        }
        return;
    }
    
    if (r == 0) {
        for (int i = 0; i < n/2; i++) {
            A[r][i] = true;
            check_row(r+1);
            A[r][i] = false;
        }
        if (n % 2 != 0) {
            isOdd = true;
            A[r][n/2] = true;
            check_row(r+1);
            A[r][n/2] = false;
        }
    }
    else {
        for (int i = 0; i < n; i++) {
            if (isValid(r, i)) {
                A[r][i] = true;
                check_row(r+1);
                A[r][i] = false;
            }
        }
    }
}

// n = 5 : 10
// n = 6 : 4
// n = 14 : 365596
// n = 15 : 2279184 : 1m16s (slow)
int main() {

    cin >> n;
    
    A = new bool *[n];
    for (int i = 0; i < n; i++) {
        A[i] = new bool;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = false;
        }
    }
    count = 0;
    check_row(0);
    cout << count;

    return 0;
}