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
  // TBD
  // We covered this in lecture, but please make sure the code still makes sense to you...
  if (T == NULL) return;
  print_inorder(T->left);
  cout << T->key << ",";
  print_inorder(T->right);
}

//return a pointer to the rank r element in tree T
//r=0 mean the 1st element stored in the tree's sequence
//r=T->size-1 is the last element stored in the tree's sequence
Node *select(Node *T, int r) {
    assert (r >= 0 && r < T->size);
    int root_rank = T->left == NULL ? 0: T->left->size;
    if (r == root_rank) return T;
    if (r < root_rank) return select(T->left, r);
    return select(T->right, r - root_rank - 1);
}

Node *insert(Node *T, int r, int k)
{
    if (T == NULL) return new Node(k);
    int root_rank = T->left == NULL ? 0 : T->left->size;
    if (r < root_rank) T->left = insert(T->left, r, k);
    else T->right = insert(T->right, r - root_rank - 1, k);
    T->size++;
    return T;
}

Node *join(Node *L, Node *R)
{
  // TBD
  // choose either the root of L or the root of R to be the root of the joined tree
  // (where we choose with probabilities proportional to the sizes of L and R)

  // Probably need some base cases here or else the next line will crash...
  if (L == NULL) return R;
  if (R == NULL) return L;
  int total = L->size + R->size;
  int randn = rand() % total;  // Pick random number in {0, 1, ..., total-1}
  if (randn < L->size) { // Happens with probability L->size / total
    // Make L the root of the joined tree in this case
    L->right = join(L->right, R);
    L->size = 1;
    if (L->right != NULL) L->size += L->right->size;
    if (L->left != NULL) L->size += L->left->size;
    return L;
  } else {               // Happens with probability R->size / total
    // Make R the root of the joined tree in this case
    R->left = join(L, R->left);
    R->size = 1;
    if (R->right != NULL) R->size += R->right->size;
    if (R->left != NULL) R->size += R->left->size;
    return R;
  }
}

Node *remove(Node *T, int r)
{
    if (T == NULL) return T;
    int root_rank = T->left == NULL ? 0 : T->left->size;
    if (r < root_rank) T->left = remove(T->left, r);
    else if (r > root_rank) T->right = remove(T->right, r - root_rank - 1);
    else {
        Node *temp = join(T->left, T->right);
        delete T;
        return temp;
    }
    T->size = 1;
    if (T->left != NULL) T->size += T->left->size;
    if (T->right != NULL) T->size += T->right->size;
    return T;
}

pair<Node *, Node *> split(Node *T, int r)
{
  // TBD
  if (T == NULL) return make_pair(T, T);
  int root_rank = T->left == NULL ? 0 : T->left->size;
  if (root_rank > r) {
    pair<Node *, Node *> l = split(T->left, r);
    T->left = l.second;
    T->size = 1;
    if (T->left != NULL) T->size += T->left->size;
    if (T->right != NULL) T->size += T->right->size;
    return make_pair(l.first, T);
  }
  else {
    pair<Node *, Node *> r2 = split(T->right, r - root_rank - 1);
    T->right = r2.first;
    T->size = 1;
    if (T->left != NULL) T->size += T->left->size;
    if (T->right != NULL) T->size += T->right->size;
    return make_pair(T, r2.second);
  }
}

Node *insert_keep_balanced(Node *T, int r, int k)
{
  if (T == NULL) return new Node(k);
  int root_rank = T->left == NULL ? 0 : T->left->size;
  if (rand() % (1 + T->size) == 0) {
    // With probability 1/N, insert at root...
    pair<Node *, Node *> result = split (T, r);
    T = new Node(k); // ok to re-use T for new root, since result keeps pointers to remnants of original tree after splitting it
    T->left = result.first;
    T->right = result.second;
  } else {
    // Otherwise, recurseively insert on left or right side...
    if (r < root_rank) T->left  = insert_keep_balanced(T->left,  r, k);
    else            T->right = insert_keep_balanced(T->right, r - root_rank - 1, k);
  }
  // Make sure T's size is correct, since its subtrees may have changed
  T->size = 1;
  if (T->left != NULL) T->size += T->left->size;
  if (T->right != NULL) T->size += T->right->size;
  return T;

}

int main() {
    Node *T = NULL;
    int N;
    //cout << "please enter a number" << endl;

    int i = 0;
    while (T->size != 1) {
        i = (i + 2120) % T->size;
        T = remove(T, i);
    }
    cout << "Last guy: " << T->key << endl;
    return 0;
}