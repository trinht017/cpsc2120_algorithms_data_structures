#include <iostream>
#include "intset.h"

using namespace std;

int h(int k, int M) {
  unsigned int i = 32123 * k + 3425252;
  return i % M;
}

Intset::Intset()
{
  N = 0;   // start with no elements present
  M = 10;  // start with a 10-element array 
  table = new Node *[M];
  for (int i = 0; i < M; i++) 
    table[i] = NULL;
}

Intset::~Intset()
{
  Node* temp = NULL;
  for (int i = 0; i < M; i++) {
    Node *n = table[i];
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
  int i = h(key, M);
  for (Node *n = table[i]; n != NULL; n = n->next) {
    if (n->key == key) {
      return true;
    }
  }
  return false;
}

/* Inserts a new key, if it isn't already present */
void Intset::insert(int key)
{
  if (find(key)) return;
  /* Enlarge (by 2x) the underlying array if it isn't large enough... */
  
  if (N == M) {
    M = M * 2;
    Node **table2 = new Node *[M];
    for (int i = 0; i < M; i++) {
      table2[i] = NULL;
    }
    for (int i = 0; i < N; i++) {
      for (Node *n = table[i]; n != NULL; n = n->next) {
        int hIndex = h(n->key, M);
        table2[hIndex] = new Node(n->key, table2[hIndex]);
      }
    }
    
    //deallocate old table
  
    Node* temp = NULL;
    for (int i = 0; i < N; i++) {
      Node *n = table[i];
      while (n != NULL) {
        temp = n->next;
        delete n;
        n = temp;
      }
    }
    delete [] table;
    
    table = table2;
  }

  int i = h(key, M);
  table[i] = new Node(key, table[i]);

  N++;
}

/* Removes a key.  It is an error if key isn't in the set */

void Intset::remove(int key) 

{
  if (!find(key)) return;

  unsigned int i = h(key, M);
  
  if (table[i]->key == key && table[i]->next == NULL) {
    delete table[i];
    table[i] = NULL;
    N--;
    return;
  }

  for (Node *n = table[i]; n->next != NULL; n = n->next) {
    if (n->next->key == key) {
      Node *temp = n->next;
      n->next = n->next->next;
      delete temp;
      break;
    }
  }
  N--;
  return;
}



void Intset::print(void)
{
  for (int i=0; i<M; i++) {
    if (table[i] != NULL) {
      for (Node *n = table[i]; n != NULL; n = n->next) {
        cout << n->key << "\n";
      }
    }
  }
}
