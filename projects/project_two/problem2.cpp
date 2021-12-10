#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <tuple>
#include <assert.h>
using namespace std;

struct Node {
  int key;
  int size;    // number of nodes in this node's subtree
  Node *left;
  Node *right;
  Node (int k) { key = k; size = 1; left = right = NULL; }
};

void print_inorder(Node *T)
{
  if (T == NULL) return;
  print_inorder(T->left);
  cout << T->key << "\n";
  print_inorder(T->right);
}

Node *insert(Node *T, int r, int k)
{
    if (T == NULL) return new Node(k);
    int root_rank = T->left == NULL ? 0 : T->left->size;
    if (r <= root_rank) T->left = insert(T->left, r, k);
    else T->right = insert(T->right, r - root_rank - 1, k);
    T->size++;
    return T;
}

int get_rand(int N)
{
  return 123456789 % N;
}

int main() {

    int N;
  
    cin >> N; 
    
    Node *T = NULL;
    
    for (int i = N; i > 0; i--) {
      T = insert(T, get_rand(N - i + 1), i);
    }
    print_inorder(T);
    
    return 0;
}