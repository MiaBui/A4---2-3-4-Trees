#include <stdio.h>
#include <stdlib.h>
#define M 4

struct node {
	int n; /* n < M Number of keys in the node will always be less than the order of the B tree */
	int keys[M - 1]; /* Array of keys */
	struct node *p[M]; /* (n + 1 pointers will be in use) */
}*root = NULL;

enum temp { Duplicate, SearchError, Success, InsertThis, SmallerKey };

void insert_24_tree(int key);
void print(struct node *root, int);
void delete_24_tree(int x);
void search(int x);
enum temp ins(struct node *r, int x, int* y, struct node** u);
int findPos(int x, int *keyArray, int n);
enum temp del(struct node *r, int x);