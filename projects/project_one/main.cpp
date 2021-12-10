
#include <iostream>
#include <fstream>
#include "intset.h"

using namespace std;

const int k = 100;   // PROBE SIZE RIGHT HERE
const int x = 2;
const int M = 1000000007;

struct LocNode {
    int seqID, index;
    LocNode *next;
    LocNode(int i, int j, LocNode *next) {seqID = i; index = j; this->next = next;}
};

struct Node {
    int key;
    Node *next;
    LocNode *loc;
    Node(int key, Node *next, LocNode *loc) {this->key = key; this->next = next; this->loc = loc;}
};

struct variant {
    int initHash;
    string label;
    string sequence;
};

bool isMatch(std::string &a, int aStart, std::string &b, int bStart) {

    int probeSize = k/2;
    int count = 0;
    for (int i = 0; i < probeSize; i++) {
        if (a[aStart+i] != b[bStart+i]) {
            count++;
        }
        if (count > 1) {
        return false;
        }
    }
    return true;
}

int h(int hash, int N) {
    unsigned int i = hash * 12349 + 3242429;
    return i % N;
}

int main() {
    ifstream inputFile("covid.txt");  //INPUT FILE RIGHT HERE
    string label, sequence;

    int variantCount = 0, deltaCount = 0;
    while (inputFile >> label >> sequence) {
        variantCount++;
        if (label == "delta_variant") {
            deltaCount++;
        }
    }
    inputFile.clear();
    inputFile.seekg(0, std::ios_base::beg);

    variant *A = new variant[variantCount];
    for (int i = 0; i < variantCount; i++) {
        inputFile >> A[i].label >> A[i].sequence;
    }

    inputFile.close();

    int sum = 0;
    for (int i = 0; i < variantCount; i++) {
        sum += (int)A[i].sequence.length();
    }
    int averageSize = sum / variantCount;
    const int N = variantCount * averageSize + 1;
    cout << N << endl;

    Node **table = new Node *[N];
    for (int i = 0; i < N; i++) {
        table[i] = NULL;
    }
    
    int p = 1;
    for (int i = 0; i < k/2 - 1; i++) {
        p = ((long long)p * x) % M;
    }
    
    int hash;
    int myHash;

    for (int i = 0; i < variantCount; i++) {
        hash = 0;
        for (int j = 0; j < k/2; j++) {
            hash = ((((long long)hash * x) % M) + A[i].sequence[j]) % M;
        }
        A[i].initHash = hash;
    }

    bool found = false;
    int **B = new int *[variantCount];
    for (int i = 0; i < variantCount; i++) {
        found = false;
        hash = 0;
        myHash = 0;

        B[i] = new int[(int)A[i].sequence.length() - k/2];
        hash = A[i].initHash;
        
        for (int j = 0; j < (int)A[i].sequence.length() - k/2; j++) {
            if (j != 0) {
                hash = ((((long long)((hash - (((long long)A[i].sequence[j - 1] * p) % M) + M) % M) * x) % M) + A[i].sequence[j - 1 + k/2]) % M;
            }
            B[i][j] = hash;
            myHash = h(hash, N);

            if (table[myHash] == NULL) {
                table[myHash] = new Node(hash, NULL, new LocNode(i, j, NULL));
            }
            else {
                for (Node *n = table[myHash]; n != NULL; n = n->next) {
                    if (n->key == hash) {
                        found = true;
                        n->loc = new LocNode(i, j, n->loc);
                        break;
                    }
                }
                if (!found) {
                    table[myHash] = new Node(hash, table[myHash], new LocNode(i, j, NULL));
                    found = false;
                }     
            }
        }
    }
   
    int deltaMatch = 0, notDeltaMatch = 0;
    double fpr, fnr, errorRate = 0, previousRate = 10000;
    int falsePositive = 0, falseNegative = 0;
    double bestProbefpr, bestProbefnr, bestProbeER;
    string bestProbe;

    for (int i = 0; i < variantCount; i++) {
        if (A[i].label != "delta_variant") continue;
        for (int j = 0; j < (int)A[i].sequence.length() - k; j++) {
            Intset Q;
            hash = B[i][j];
            myHash = h(hash, N);
            for (Node *n = table[myHash]; n != NULL; n = n->next) {
                if (n->key == hash) {
                    for(LocNode *m = n->loc; m != NULL; m = m->next) {
                        if (isMatch(A[i].sequence, j + k/2, A[m->seqID].sequence, m->index + k/2)) {
                            Q.insert(B[m->seqID][m->index]);
                            if (A[m->seqID].label == "delta_variant") {
                                deltaMatch++;
                            }
                            else {
                                notDeltaMatch++;
                            }
                        }
                    }
                    break;
                }
            }
            hash = B[i][j+k/2];
            myHash = h(hash, N);
            for (Node *n = table[myHash]; n != NULL; n = n->next) {
                if (n->key == hash) {
                    for(LocNode *m = n->loc; m != NULL; m = m->next) {
                        if (isMatch(A[i].sequence, j, A[m->seqID].sequence, m->index - k/2)) {
                            if (Q.find(B[m->seqID][m->index - k/2])) continue;
                            if (A[m->seqID].label == "delta_variant") {
                                deltaMatch++;
                            }
                            else {
                                notDeltaMatch++;
                            }
                        }
                    }
                    break;
                }
            }
            fpr = (double)notDeltaMatch / (variantCount - deltaCount);
            fnr = (double)(deltaCount - deltaMatch) / deltaCount;
            errorRate = 2.0 * fpr + 1.0 * fnr;
            if (errorRate < previousRate) {
                previousRate = errorRate;
                bestProbe = A[i].sequence.substr(j, k);
                falsePositive = notDeltaMatch;
                falseNegative = deltaCount - deltaMatch;
                bestProbefpr = fpr;
                bestProbefnr = fnr;
                bestProbeER = errorRate;
            }
            deltaMatch = 0;
            notDeltaMatch = 0; 
        }
    }
    std::cout << "Best Probe Sequence: \n" << bestProbe << std::endl;
    std::cout << "Number of False Positives: " << falsePositive << std::endl;
    std::cout << "False Positive Rate: "  << bestProbefpr << std::endl;
    std::cout << "Number of False Negatives: " << falseNegative << std::endl;
    std::cout << "False Negative Rate: " << bestProbefnr << std::endl;
    std::cout << "Erorr Rate: " << bestProbeER << std::endl;
    
    for (int i = 0; i < N; i++) {
        Node *n = table[i];
        while (n != NULL) {
            LocNode *m = n->loc;
            while (m != NULL) {
                LocNode *temp = m->next;
                delete m;
                m = temp;
            }
            Node *temp = n->next;
            delete n;
            n = temp;
        }
    }
    /*
    for (int i = 0; i < variantCount; i++) {
        cout << i << " " << A[i].initHash << endl;
    }
    */
    for (int i = 0; i < (int)A[9].sequence.length() - k/2; i++) {
        cout << i << " " << B[9][i] << endl;
    }
    delete [] table;
    for (int i = 0; i < variantCount; i++) {
        delete B[i];
    }
    delete [] B;
    delete [] A;
    
    return 0;
}