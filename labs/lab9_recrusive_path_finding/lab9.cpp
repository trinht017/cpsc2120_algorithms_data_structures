
#include <iostream>
#include <vector>
/**

state: (fullness of jug1, fullness of jug 2)
start: (0,0)
goal: (a, b) where a+b = target 

transitions:
(a,b) ----> empty out 1st jug ----> (0, b);
(a, b) ----->empty out 2nd jug ----->(a, 0);
(a,b) -------> fill 1st jug --------> (A, b);
(a, b) ------->pour 1st jug into 2nd ----> 2 cases
*/
using namespace std;

struct Node{
    int a, b, c;
    Node(int x, int y, int z) {a = x; b = y; c = z;}
};

int A, B;
int x;
bool beenThere[1001][1001]; 

pair<int, int> pour(int a, int b, bool john) {
    int a_ = a;
    int b_ = b;

    if (john) {
        // b = a + b >= B ? B : a + b;
        // a = a - b_;
        // a = a < 0 ? 0 : a;
        if (b == B || a == 0) return make_pair(a, b);
        
        if (a + b > B) {
            b_ = B - b;
            b = B;
            a = a - b_;
        }
        else {
            b = a + b;
            a = 0;
        }
    }
    else {
        if (a == A || b == 0) return make_pair(a, b);
        if (a + b > A) {
            a_ = A - a;
            a = A;
            b = b - a_;
        }
        else {
            a = a + b;
            b = 0;
        }
    }

    return make_pair(a, b);
}

vector<Node> V;
bool visit(int a, int b) {
    /*
    - if reach goal state, done
    -mark(a,b) as having been visited
    -push (a,b) onto stack representing current path
    -visit all unvisited neighbors
    -pop from stack

    at end print path
    1. keep predecessor pointers
        from every state(a,b), keep (prev_a, prev_b)
    2. use a stack
    */

    if(a + b == x) return true;
    beenThere[a][b] = true;
    if (!beenThere[0][b] && visit(0, b)) {
        Node temp(0, b, 0);
        V.push_back(temp);
        return true;
    }
    if (!beenThere[a][0] && visit(a, 0)) {
        Node temp(a, 0, 1);
        V.push_back(temp);
        return true;
    }
    if (!beenThere[A][b] && visit(A, b)) {
        Node temp(A, b, 2);
        V.push_back(temp);
        return true;
    }
    if (!beenThere[a][B] && visit(a, B)) {
        Node temp(a, B, 3);
        V.push_back(temp);
        return true;
    }
    pair<int, int> hiDean = pour(a, b, true);
    pair<int, int> hiJohn = pour(a, b, false);
    if (!beenThere[hiDean.first][hiDean.second] && visit(hiDean.first, hiDean.second)) {
        Node temp(hiDean.first, hiDean.second, 4);
        V.push_back(temp);
        return true;
    }
    if (!beenThere[hiJohn.first][hiJohn.second] && visit(hiJohn.first, hiJohn.second)) {
        Node temp(hiJohn.first, hiJohn.second , 5);
        V.push_back(temp);
        return true;
    }

    return false;
}
void printPath() {
    for(int i = V.size() - 1; i >= 0; i--) {
        if(V[i].c == 0) {
            cout << "Empty jug 1 " << "[a=" << V[i].a << ", b=" << V[i].b << "]\n";
        }
        if(V[i].c == 1) {
            cout << "Empty jug 2 " << "[a=" << V[i].a + ", b=" << V[i].b << "]\n";
        }
        if(V[i].c == 2) {
            cout << "Fill jug 1  " << "[a=" << V[i].a << ", b=" << V[i].b << "]\n";
        }
        if(V[i].c == 3) {
            cout << "Fill jug 2  " << "[a=" << V[i].a << ", b=" << V[i].b << "]\n";
        }
        if(V[i].c == 4) {
            cout << "Pour 1->2   " << "[a=" << V[i].a << ", b=" << V[i].b << "]\n";
        }
        if(V[i].c == 5) {
            cout << "Pour 2->1   " << "[a=" << V[i].a << ", b=" << V[i].b << "]\n";
        }
    }
}

int main() {
    cout << "Enter a ";
    cin >> A;
    cout << "Enter b ";
    cin >> B;
    cout << "Enter x ";
    cin >> x;

    if (visit(0, 0)) {
        cout << "\n";
        printPath();
        cout << "\n";
    }
    else {
        cout << "Impossible!";
    }
    return 0;
}