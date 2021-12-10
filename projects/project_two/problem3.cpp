
#include <iostream>
#include <cstdlib>
#include <tuple>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

using namespace std;

struct Person{
    string name;
    int work;
    Person() : name(""), work(0) {;}
    Person(string n, int w) {name = n; work = w;}
    bool operator <(const Person &p) const {return name < p.name;}
};

struct Node {
  Person key;
  int size; 
  Node *left, *right, *parent;
  Node (Person k) { key = k; size = 1; left = right = parent = NULL; }
};

void update_size(Node *n)
{
  n->size = 1;
  if (n->left != NULL) {
    n->size += n->left->size;
    n->left->parent = n;
  }
  if (n->right != NULL) {
    n->size += n->right->size;
    n->right->parent = n;
  }
}

// Return a pointer to the rank-r element 
Node *select(Node *root, int r)
{
  assert (r >= 0 && r < root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r == root_rank) return root;
  if (r <  root_rank) return select(root->left, r);
  return select(root->right, r - root_rank - 1);
}

// int get_rank(Node *root, Node *n) {
//   int root_rank = root->left == NULL ? 0 : root->left->size;

//   if (root == n) {
//     return root_rank;
//   }
//   if (n->parent->left == n) {
//     return get_rank(root, n->parent) - n->size + (n->left == NULL ? 0 : n->left->size);
//   }
//   else
//     return get_rank(root, n->parent) + (n->left == NULL ? 0 : n->left->size) + 1; 
// }

// int get_rank(Node *root, Node *n) {
//   int root_rank = n->left == NULL ? 0 : n->left->size;

//   if (root == n) return root_rank;
//   if (n->parent->left == n) 
//     return get_rank(root, n->parent) - n->size + root_rank;
//   else 
//     return get_rank(root, n->parent) + root_rank + 1;
// }

int get_rank(Node *root, Node *n) {
  int root_rank = n->left == NULL ? 0 : n->left->size;

  if (n->parent == NULL) return root_rank;
  if (n->parent->left == n) 
    return get_rank(root, n->parent) - n->size + root_rank;
  else 
    return get_rank(root, n->parent) + root_rank + 1;
}



// Insert new value v so it becomes the element at rank r
Node *insert(Node *root, int r, Person v)
{
  if (root == NULL) return new Node(v);
  assert (r >= 0 && r <= root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r <= root_rank) root->left  = insert(root->left,  r, v);
  else                root->right = insert(root->right, r - root_rank - 1, v);
  update_size (root);
  return root;
}

void print_inorder(Node *root)
{
  if (root == NULL) return;
  print_inorder(root->left);
  cout << root->key.name << " " << root->key.work << endl;
  print_inorder(root->right);
}

void insert_to_vector(Node *root, vector<pair<string, int>> &V) {
  if (root == NULL) return;
  insert_to_vector(root->left, V);
  V.push_back(make_pair(root->key.name, root->key.work));
  insert_to_vector(root->right, V);
}

// Split tree on rank r into tree L (containing keys of ranks < r) and tree R
pair<Node *, Node *> split(Node *root, int r)
{
  pair <Node *, Node *> result = make_pair (root, root);
  if (root == NULL) return result;
  assert (r >= 0 && r <= root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r <= root_rank) tie (result.first, root->left)   = split (root->left, r);
  else tie (root->right, result.second) = split (root->right, r - root_rank - 1);
  update_size (root);
  return result;
}

// Insert new value v so it becomes the element at rank r
Node *insert_keep_balanced(Node *root, int r, Person v)
{
  if (root == NULL) return new Node(v);
  assert (r >= 0 && r <= root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (rand() % (1 + root->size) == 0) {
    Node *new_root = new Node(v);
    tie (new_root->left, new_root->right) = split (root, r);
    update_size (new_root);
    return new_root;
  } 
  if (r <= root_rank) root->left  = insert_keep_balanced(root->left,  r, v);
  else root->right = insert_keep_balanced(root->right, r - root_rank - 1, v);
  update_size (root);
  return root;
}

Node *join(Node *L, Node *R)
{
  if (L == NULL) return R;
  if (R == NULL) return L;
  Node *root = (rand() % (L->size + R->size) < L->size) ? L : R;
  if (root == L) L->right = join(L->right, R); 
  else           R->left  = join(L, R->left);   
  update_size (root);
  return root;
}

// Remove the rank-r element from a tree (should only be called on a non-empty tree)
Node *remove(Node *root, int r)
{
  assert (root != NULL);
  assert (r >= 0 && r < root->size);
  int root_rank = root->left == NULL ? 0 : root->left->size;
  if (r == root_rank) {
    Node *result = join(root->left, root->right);
    delete root;
    return result;
  }
  if (r < root_rank) root->left  = remove (root->left, r);
  else               root->right = remove (root->right, r - root_rank - 1);
  update_size (root);
  return root;
}

int main() {

    string p1, p2;
    
    vector<pair<string, string>> moves;

    while (cin >> p1 >> p2) {
        moves.push_back(make_pair(p1, p2));
    }
    
    set<Person> P;
    Person temp;
    for (auto &e : moves) {
        temp.name = e.first;
        P.insert(temp);
        temp.name = e.second;
        P.insert(temp);
    }

    Node *T = NULL;
    int i = 0;
    for (auto e : P) {
        T = insert_keep_balanced(T, i, e);
        i++;
    }

    map<string, Node *> M;

    Node *ptr = NULL;
    for (int i = 0; i < P.size(); i++) {
        ptr = select(T, i);
        M.insert(pair<string, Node *>(ptr->key.name, ptr));
    }

    int rank_of_first = 0;
    int rank_of_second = 0;
    int rank_of_first_after_move = 0;
    int w;

    Node *temp2;
    Person temp3;
    Node *person1;
    Node *person2;

    for (auto &e : moves) {

      person1 = M[e.first];
      person2 = M[e.second];

      rank_of_first = get_rank(T, person1);
      rank_of_second = get_rank(T, person2);
      if (rank_of_first - rank_of_second == 1) continue;

      temp3.name = person1->key.name;
      temp3.work = person1->key.work;

      T = remove(T, rank_of_first);
      rank_of_second = get_rank(T, person2);
      T = insert_keep_balanced(T, rank_of_second + 1, temp3);

      temp2 = select(T, rank_of_second + 1);
      M[temp2->key.name] = temp2;

      rank_of_first_after_move = get_rank(T, temp2);
      temp2->key.work += abs(rank_of_first - rank_of_first_after_move);
    }

    vector<pair<string, int>> V;
    insert_to_vector(T, V);
    sort(V.begin(), V.end(), [](pair<string, int> &a, pair<string,int> &b) 
                              {return a.first < b.first;});
    for (auto &e : V) {
      cout << e.first << " " << e.second << endl;
    }

}