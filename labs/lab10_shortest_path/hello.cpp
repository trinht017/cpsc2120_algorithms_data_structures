#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

typedef int node;

int inf = 999999;
vector<string> input;
int N;

vector<vector<node>> nbrs;
vector<node> dist;
vector<node> pred;


const int SIZE = 5;

bool match(int a, int b) {
    int mismatch = 0;
    for (int i = 0; i < SIZE; i++) {
        if (input[a][i] != input[b][i]) mismatch++;
        if (mismatch > 1) return false;
    }
    if (mismatch == 1) return true;
    return false;
}

void build_graph(void) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) continue;
            if (match(i, j)) nbrs[i].push_back(j);
        }
    }
}

// int best_length = 0;
// node best_node_start;
// node best_node_goal;
// int curr_length = 0;

void bfs(node start) {
    for (int i = 0; i < N; i++) dist[i] = inf;

    queue<node> to_visit;
    to_visit.push(start);
    dist[start] = 0;
    while (!to_visit.empty()) {
        node n = to_visit.front();
        to_visit.pop();
        for (node nbr : nbrs[n]) { 
            if (dist[nbr] == inf) {
	            dist[nbr] = dist[n] + 1;

                // curr_length = dist[nbr];
                // if (curr_length > best_length) {
                //     best_length = curr_length;
                //     best_node_start = start;
                //     best_node_goal = nbr;
                // }

	            pred[nbr] = n;
	            to_visit.push(nbr);
            }
        }
    }
}

void print_path(node start, node goal) {
    if (start != goal) print_path(start, pred[goal]);
    cout << input[goal] << "\n";
}

void print_shortest_path(node start, node goal) {
    pred.clear();
    bfs(start);
    print_path(start, goal);
}

void find_diameter() {
    int best_length = 0;
    node best_node_start;
    node best_node_goal;

    int curr_length = 0;

    for (int i = 0; i < N; i++) {
        pred.clear();

        bfs(i);

        for (int j = 0; j < N; j++) {
            curr_length = dist[j] == inf ? 0 : dist[j];
            if (curr_length > best_length) {
                best_length = curr_length;
                best_node_start = i;
                best_node_goal = j;
            }
        }
    }
    cout << best_length << "\n";
    print_shortest_path(best_node_start, best_node_goal);
}

int main(void) {
    string s;
    while (cin >> s) input.push_back(s);
    N = input.size();
    nbrs.resize(N);
    dist.resize(N);
    pred.resize(N);
    
    for (int i = 0; i < N; i++) {
        dist[i] = inf;
    }

    int goal;
    int start;
    string x = "graph";
    string y = "paths";

    for (int i = 0; i < N; i++) {
        if (input[i] == x) start = i;
        if (input[i] == y) goal = i;
    }

    build_graph();
    find_diameter();

}