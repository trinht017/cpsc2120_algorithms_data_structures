#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

struct Node {
  int key;
  Node *next;
  Node (int k, Node *n) { key = k; next = n; }
};

class Intset {

 private: 
  int N; // number of elements currently stored in set
  int M; // size of array allocated for the set
  //int *A;
  Node **table;

  /* When you switch from an array implementation to a hash table,
     you'll keep N and M above (M will now represent the size of the
     array you've allocated for your hash table).  You'll get rid of
     the array A and add the following elements instead:

     struct Node {
       int key;
       Node *next;
       Node (int k, Node *n) { key = k; next = n; }
     };
     Node **table;  // array of head pointers
  */

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif
