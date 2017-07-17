/*
 * STACKEZ
 * TOPIC: implementation, pointer-based stack
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Q (1<<21)

typedef struct cell {
	struct cell *next,*prev;
	int val;
} cell;

cell pool[Q],*ptr = pool;

cell *init( int val ) {
	cell *x = ptr++;
	x->next = x->prev = NULL, x->val = val;
	return x;
}

typedef struct stack {
	cell *top;
	int cnt;
} stack;

stack *init_stack() {
	stack *s = (stack *)malloc(sizeof *s);
	s->top = NULL, s->cnt = 0;
	return s;
}

void add_after( cell **item, int val ) {
	cell *p = init(val);
	if ( !*item )
		*item = p;
	else {
		if ( p->next = (*item)->next )
			p->next->prev = p;
		p->prev = (*item), (*item)->next = p;
	}
}

void push( stack *s, int val ) {
	add_after(&s->top,val);
	if ( s->cnt++ )
		s->top = s->top->next;
}

void pop( stack *s ) {
	cell *prev;
	if ( !s->cnt ) return ;
	prev = s->top->prev;
	if ( !--s->cnt )
		s->top = NULL;
	else s->top = prev, s->top->next = NULL;
}

int main() {
	int i,j,k,qr;
	stack *s = init_stack();
	for ( scanf("%d",&qr); qr-- && 1 == scanf("%d",&k); ) {
		if ( k == 1 ) {
			scanf("%d",&i);
			push(s,i);
		}
		else if ( k == 2 ) pop(s);
		else {
			if ( !s->cnt ) puts("Empty!");
			else printf("%d\n",s->top->val);
		}
	}
	return 0;
}

