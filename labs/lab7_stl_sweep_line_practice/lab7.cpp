#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int abs(int n) {
    if (n < 0) return n * -1;
    return n;
} 

int main() {
    vector<pair<string, int>> input;
    string name;
    int height;

    while (cin >> name >> height) {
        input.push_back(make_pair(name, height));
    }

    vector<pair<int, int>> A;

    for (int i = 0; i < (int)input.size(); i++) {
        A.push_back(make_pair(input[i].second, i));
    }

    sort(A.begin(), A.end(), [](pair<int, int> &a, pair<int, int> &b) 
                                {return a.first > b.first;});
    
    int currBest = 0, best = 0;
    int prevBest = -1;
    int distance = 0;
    int distance2 = 0;
    pair<int, int> result;

    set<int> B;
    set<int>::iterator it, pred, succ;
    for (int i = 0; i < (int)A.size(); i++) {
        B.insert(A[i].second);
        it = B.find(A[i].second);
        pred = it;
        succ = it;

        if (++succ != B.end()) distance = abs(*it - *succ);
        if (--pred != B.end()) distance = abs(*it - *pred);
        currBest = distance > distance2 ? distance : distance2;
        if (currBest > prevBest) {
            prevBest = currBest;
            best = currBest;
            result = make_pair(*it, distance > distance2 ? *succ : *pred);
        }
    }

    int h1, h2;
    for (auto &e : A) {
        if (e.second == result.first) h1 = e.first;
        if (e.second == result.second) h2 = e.first;
    }

    string name1, name2;
    for (auto &e : input) {
        if (e.second == h1) name1 = e.first;
        if (e.second == h2) name2 = e.first;
    }
    cout << best << endl;
    cout << name1 << endl;
    cout << name2 << endl;
    
    return 0;
}



