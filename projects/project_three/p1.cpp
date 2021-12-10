#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

/*----------------------------------------------------------------------------*/
struct Candy {
    int weight, value;
    Candy() {};
    Candy(int w, int v) {weight = w; value = v;}
};

vector<Candy> candyList;
int N;
/*----------------------------------------------------------------------------*/

void solve_greedy() {
    
    sort(candyList.begin(), candyList.end(), [](Candy a, Candy b){
        return (double)a.value/a.weight > (double)b.value/b.weight;
    });

    int bags[3];
    for (int i = 0; i < 3; i++) {
        bags[i] = 0;
    }
    int solution = 0;
    int attempt = 0;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            attempt = bags[j] + candyList[i].weight;
            if (attempt <= 2000) {
                bags[j] += candyList[i].weight;
                solution += candyList[i].value;
                break;
            }
        }
    }
    cout << "greedy: " << solution << endl;
}

/*----------------------------------------------------------------------------*/
void assignment(vector<vector<int>> &bags) {

    int attempt = 0;
    int index = 0;

    int bagWeights[3];
    for (int i = 0; i < 3; i++) {
        bagWeights[i] = 0;
    }
    
    bool isUsed[N];
    for (int i = 0; i < N; i++) isUsed[i] = false;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            attempt = bagWeights[j] + candyList[i].weight;
            if (attempt <= 2000) {
                bagWeights[j] = attempt;
                bags[j].push_back(i);
                isUsed[i] = true;
                break;
            }
        }
    }

    for (int i = 0; i<N; i++) {
        if (!isUsed[i]) {
            bags[3].push_back(i);
        }
    }   
}

int findWorst(vector<int> &bag) {
    if (bag.empty()) return -1;

    double min = double(candyList[bag[0]].value)/candyList[bag[0]].weight;
    double worth;
    int index = 0;

    for (int i = 0; i < bag.size(); i++) {
        worth = double(candyList[bag[i]].value)/candyList[bag[i]].weight;
        if (worth < min) {
            min = worth;
            index = i;
        }
    }

    return index;
}

int findBest(vector<int> &bag) {
    if (bag.empty()) return -1;

    double max = double(candyList[bag[0]].value)/candyList[bag[0]].weight;
    double worth;
    int index = 0;

    for (int i = 0; i < bag.size(); i++) {
        worth = double(candyList[bag[i]].value)/candyList[bag[i]].weight;
        if (worth > max) {
            max = worth;
            index = i;
        }
    }

    return index;
}

int calculateWeight(vector<int> &bag) {
    int weight = 0;
    for (int i = 0; i < bag.size(); i++) {
        weight += candyList[bag[i]].weight;
    }
    return weight;
}

void repair(vector<int> &bag, vector<vector<int>> &bags) {
    int bagWeight = calculateWeight(bag);
    int worstIndex;
    int bestIndex;
    int attempt;
    if (bagWeight > 2000) {
        while (bagWeight > 2000) {
            worstIndex = findWorst(bag);
            bagWeight -= candyList[bag[worstIndex]].weight;
            bags[3].push_back(bag[worstIndex]);
            bag.erase(bag.begin() + worstIndex);
        }
    }
    else {
        for (int i = 0; i < bags[3].size(); i++) {
            if (bags[3].empty()) break;
            bestIndex = findBest(bags[3]);
            attempt = bagWeight + candyList[bags[3][bestIndex]].weight;
            if (attempt <= 2000) {
                bagWeight = attempt;
                bag.push_back(bags[3][bestIndex]);
                bags[3].erase(bags[3].begin() + bestIndex);
            }
            else {
                break;
            }
        }
    }
}

void move(int i,  vector<int> &A, vector<int> &B) {
    B.push_back(A[i]);
    A.erase(A.begin() + i);
}

int calculateSolution(vector<vector<int>> &bags) {
    int solution = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < bags[i].size(); j++) {
            solution += candyList[bags[i][j]].value;
        }
    }
    return solution;
}

void clearBags(vector<vector<int>> &bags) {
    for (int i = 0; i < 4; i++) {
        bags[i].clear();
    }
}
void solve_iterative_refinement() {
    vector<vector<int>> bags(4);

    int currSolution = 0;
    int bestSolution = 0;

    for (int i = 0; i < 100; i++) {
        random_shuffle(candyList.begin(), candyList.end());
        assignment(bags);
        
        currSolution = calculateSolution(bags);
        
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < bags[j].size(); k++) {
                move(k, bags[j], bags[(j+1)%3]);
                repair(bags[j], bags);
                repair(bags[(j+1)%3], bags);
                currSolution = calculateSolution(bags);
                if (currSolution > bestSolution) {
                    bestSolution = currSolution;
                }
            }
        }
        clearBags(bags);
    }
    cout << "Refinement: " << bestSolution << endl;
}
/*----------------------------------------------------------------------------*/

//for solve_exhaustive_search
int sol;
int bestSol;
int totalValue;
int currValue;
int A[4];
double highDensity;

void solve_exhaustive_search(int index) {
    if (index == N) {
        if (sol > bestSol) {
            bestSol = sol;
        }
        return;
    }

    if (sol + (totalValue-currValue) < bestSol) return;

    int emptySpace = 6000 - A[0] - A[1] - A[2];
    highDensity = (double)candyList[index].value/candyList[index].weight;
    if (sol + emptySpace * highDensity < bestSol) return;

    if (index == 0) {
        for (int i = 0; i < 4; i++) {
            if (i == 0 || i == 3) {
                A[i] += candyList[index].weight;
                if (i!=3) sol+=candyList[index].value;
                currValue += candyList[index].value;
                solve_exhaustive_search(index+1);
                A[i] -= candyList[index].weight;
                if (i!=3) sol -= candyList[index].value;
                currValue -= candyList[index].value;
            }
        }
    }
    else {
        for (int i = 0; i < 4; i++) {
            if(i == 3 || (A[i] + candyList[index].weight <= 2000)) {
                A[i] += candyList[index].weight;
                if (i!=3) sol += candyList[index].value;
                currValue += candyList[index].value;
                solve_exhaustive_search(index+1);
                A[i] -= candyList[index].weight;
                if (i!=3) sol -= candyList[index].value;
                currValue -= candyList[index].value;
            }
        }
    }
}
/*----------------------------------------------------------------------------*/


int main() {

    ifstream input("candy.txt");   //input file 
    Candy p;
    while (input >> p.weight >> p.value) candyList.push_back(p);
    N = candyList.size();

    for (int i = 0; i < 4; i++) {
        A[i] = 0;
    }
    totalValue = 0;
    for (int i = 0; i < N; i++) {
        totalValue += candyList[i].value;
    }
    currValue = 0;
    sol = 0;
    bestSol = 0;
    
    solve_greedy();
    //exhaustive search assumes that the candyList is sorted in descending density order 
    solve_exhaustive_search(0);
    solve_iterative_refinement();
    cout << "Exhaustive: " << bestSol << endl;

    return 0;
}
