/*
 * TREEVERSE
 * TOPIC: binary search tree, interview
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cell {
	struct cell *left,*right;
	int val;
} cell;

typedef struct tree {
	cell *root;
	int cnt;
} tree;

tree *init() {
	tree *t = (tree *)malloc(sizeof *t);
	t->root = NULL, t->cnt = 0;
	return t;
}

void push( tree *t, int val ) {
	cell **hold,*x,*y;
	for ( hold = &t->root, x = t->root; x; ) {
		if ( x->val > val )
			hold = &x->left, x = x->left;
		else hold = &x->right, x = x->right;
	}
	*hold = (cell *)malloc(sizeof **hold), (*hold)->val = val;
}

void preorder( cell *x ) {
	if ( x ) 
		printf(" %d",x->val), preorder(x->left), preorder(x->right);
}

void inorder( cell *x ) {
	if ( x ) 
		inorder(x->left), printf(" %d",x->val), inorder(x->right);
}

void postorder( cell *x ) {
	if ( x ) 
		postorder(x->left), postorder(x->right), printf(" %d",x->val);
}



int main() {
	int i,j,k,n;
	tree *t;
	for ( ;1 == scanf("%d",&n); ) {
		t = init();
		for ( i = 0; i < n; ++i )
			scanf("%d",&k), push(t,k);
		printf("Pre order :"), preorder(t->root), putchar('\n');
		printf("In order  :"), inorder(t->root), putchar('\n');
		printf("Post order:"), postorder(t->root), putchar('\n');
	}
	return 0;
}

