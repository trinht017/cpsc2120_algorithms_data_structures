#include <iostream>
#include "intset.h"

using namespace std;

int h_(int k, int M) {
    unsigned int i = k * 12349 + 3242429;
    return i % M;
}

Intset::Intset()
{
  N = 0;   // start with no elements present
  M = 10;  // start with a 10-element array 
  table = new Node_ *[M];
  for (int i = 0; i < M; i++) 
    table[i] = NULL;
}

Intset::~Intset()
{
  Node_* temp = NULL;
  for (int i = 0; i < M; i++) {
    Node_ *n = table[i];
    while (n != NULL) {
      temp = n->next;
      delete n;
      n = temp;
    }
  }
  delete [] table;
}

/* Return true if key is in the set */
bool Intset::find(int key)
{
  int i = h_(key, M);
  for (Node_ *n = table[i]; n != NULL; n = n->next) {
    if (n->key == key) {
      return true;
    }
  }
  return false;
}

/* Inserts a new key, if it isn't already present */
void Intset::insert(int key)
{  
  if (N == M) {
    M = M * 2;
    Node_ **table2 = new Node_ *[M];
    for (int i = 0; i < M; i++) {
      table2[i] = NULL;
    }
    for (int i = 0; i < N; i++) {
      for (Node_ *n = table[i]; n != NULL; n = n->next) {
        int hIndex = h_(n->key, M);
        table2[hIndex] = new Node_(n->key, table2[hIndex]);
      }
    }
    
    //deallocate old table
  
    Node_* temp = NULL;
    for (int i = 0; i < N; i++) {
      Node_ *n = table[i];
      while (n != NULL) {
        temp = n->next;
        delete n;
        n = temp;
      }
    }
    delete [] table;
    
    table = table2;
  }

  int i = h_(key, M);
  table[i] = new Node_(key, table[i]);

  N++;
}
