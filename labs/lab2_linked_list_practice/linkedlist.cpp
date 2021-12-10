#include <iostream>
#include <cstdlib>
using namespace std;

struct Node {
  int val;
  Node *next;
  Node (int v, Node *n) { val = v; next = n; }
};

// Generate a list containing random numbers in the range 0..999
Node *generate_random_list(int length)
{
  return length==0 ? NULL : new Node(rand() % 1000, generate_random_list(length-1));
}

// e.g., generate_sorted_list(4, 3, 5) makes the list 4, 7, 10, 13, 16
Node *generate_sorted_list(int start, int step, int length)
{
  return length==0 ? NULL : new Node(start, generate_sorted_list(start+step, step, length-1));
}

void print_list(Node *head)
{
  for (Node *n = head; n != NULL; n = n->next)
    cout << n->val << "\n";
}


// Insert v into a linked list that is sorted, keeping it sorted
// Returns a pointer to the head node in the resulting list
Node *insert_keep_sorted(Node *head, int v) {
  Node *temp = new Node(v, NULL);

  if (head == NULL) {
    head = temp;
    return head;
  }

  if(v < head->val) {
    temp->next = head;
    head = temp;
    return head;
  }

  for (Node* n = head; n != NULL; n = n->next) {
    if (v >= n->val) {
      if (n->next == NULL) {
        n->next = temp;
        break;
      }
      else if(v < n->next->val) {
        temp->next = n->next;
        n->next = temp;
        break;
      }
    }
  }
  return head;
}

// Insert a new node with value v at index i (e.g., i==0 to insert a
// new head node; the node formerly at index i would become index i+1).
// Return a pointer to the head node in the resulting list
// You may assume i is a valid index from 0 up to the list length
Node *insert_at_pos(Node *head, int i, int v) {
  Node *temp = new Node(v, NULL);

  if (head == NULL) {
    head = temp;
    return head;
  }

  if (i == 0) {
    temp->next = head;
    head = temp;
    return head;
  }

  Node *n = head;
  for (int j = 0; j < i - 1; j++) {
    n = n->next;
  }

  if (n->next == NULL) {
    n->next = temp;
  }
  else {
    temp->next = n->next;
    n->next = temp;
  }

  return head;
}

// Delete the element at index i (e.g., if i==0, delete the head node)
// Return a pointer to the head node in the resulting list
// You may assume i is a valid index from 0 up to the list length minus one
Node *delete_at_pos(Node *head, int i) {
  if (head == NULL) {
    return head;
  }
  Node *n = head;
  if (i == 0) {
    head = head->next;
    delete n;
    return head;
  }

  for (int j = 0; j < i - 1; j++) {
    n = n->next;
  }

  if (n->next->next == NULL) {
    delete n->next;
    n->next = NULL;
    return head;
  }

  Node *toDelete = n->next;
  n->next = n->next->next;
  delete toDelete;

  return head;
}

// Given a linked list that is sorted except with one element "out of
// order" (too high or too low), restore the list to sorted order.
// Return a pointer to the head node of the resulting list
Node *restore_sorted (Node *head) {
  if (head == NULL) {
    return head;
  }
  int val;
  Node *n = head;
  Node *temp = head;

  if (n->val > n->next->val) {
    val = n->val;
    head = head->next;
    delete n;
    head = insert_keep_sorted(head, val);
    return head;
  }

  for (Node *n = head; n->next != NULL; n = n->next) {
    if (n->val > n->next->val) {
      if (n->next->val >= temp->val) {
        val = n->val;
        temp->next = n->next;
        delete n;
      }
      else {
        val = n->next->val;
        temp = n->next;
        n->next = n->next->next;
        delete temp;
      }
      head = insert_keep_sorted(head, val);
      break;
    }
    temp = n;
  }
  return head;
}

// Build a linked list containing a copy of the elements in
// positions i...j of a linked list and return it.  You may assume
// i <= j and that i and j are valid indices (between 0 and the
// list length minus one).
Node *splice(Node *head, int i, int j) {

    Node *n = head;
    Node *newHead = NULL;
    Node *newNode = NULL;

    for (int k = 0; k < i; k++) {
      n = n->next;
    }
    newHead = new Node(n->val, NULL);

    for (int k = i; k < j; k++) {
      if (n->next != NULL) {
        n = n->next;
        if (newHead->next == NULL) {
          newNode = new Node(n->val, NULL);
          newHead->next = newNode;
        }
        else {
          newNode->next = new Node(n->val, NULL);
          newNode = newNode->next;
        }
      }
    }
    return newHead;
}

int main(void)
{
  Node *head = generate_sorted_list(4, 3, 5);
  print_list(head);

  return 0;
}
