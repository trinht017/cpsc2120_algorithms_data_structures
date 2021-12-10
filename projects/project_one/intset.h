#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

struct Node_ {
  int key;
  Node_ *next;
  Node_ (int k, Node_ *n) { key = k; next = n; }
};

class Intset {

 private: 
  int N; // number of elements currently stored in set
  int M; // size of array allocated for the set
  Node_ **table;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
};

#endif
