/*
 * Sudoku Solver
 * TOPIC: Algorithm X, Dancing Links
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 4
#define N (16) /* M*M */
#define BLOCK (256) /* N*N */
#define SQUARE(i,j) (((divM[i])*M)+divM[j])
#define Q (1<<21)
#define NONE (-1)
#define ROW_OFFSET (0)
#define COL_OFFSET (BLOCK)
#define SQ_OFFSET  (512) /* 2*BLOCK */
#define UNO_OFFSET (768) /* 3*BLOCK */
#define NUM_ROWS (4096)
#define NUM_COLS (1024)
enum { HOR, VER };

int divN[Q],modN[Q],divBLOCK[Q],modBLOCK[Q],divM[Q];

/* 
 * Cell structure along with different adding methods 
 * a cell has pointers to its 4 neighbours, and
 * we can append, prepend, put as a lower/upper neighbor
 */
typedef struct cell {
	struct cell *left, *right, 
				*upper, *lower;
	int r,c; /* row and column number in the binary matrix */
} cell;

cell pool[Q], *ptr;

cell *init_cell( int r, int c ) {
	cell *x = ptr++;
	x->left = x->right = x->upper = x->lower = x, x->r = r, x->c = c;
	return x;
}

void add_after( cell **item, cell *p ) {
	if ( !*item )
		*item = p;
	else {
		if ( p->right = (*item)->right )
			p->right->left = p;
		p->left = (*item), (*item)->right = p;
	}
}

void add_below( cell **item, cell *p ) {
	if ( !*item )
		*item = p;
	else {
		if ( p->lower = (*item)->lower )
			p->lower->upper = p;
		p->upper = (*item), (*item)->lower = p;
	}
}
/* End of Cell structure */

/* doubly-linked lists */
typedef struct {
	cell *head, *tail;
	int cnt;
	char mode;
} list;

list *init_list( char mode ) {
	list *q = (list *)malloc(sizeof *q);
	q->head = q->tail = NULL, q->cnt = 0, q->mode = mode;
	return q;
}

void add( list *q, cell *p ) {
	if ( q->mode == HOR ) {
		add_after(&q->tail,p);
		if ( !q->cnt++ )
			q->head = q->tail;
		else q->tail = q->tail->right;
	}
	else {
		add_below(&q->tail,p);
		if ( !q->cnt++ )
			q->head = q->tail;
		else q->tail = q->tail->lower;
	}
}

int ans,soln[N][N],g[N][N],b[N][N];
list *col_list[NUM_COLS],*row_list[NUM_ROWS],*header;
unsigned char binary_matrix[NUM_ROWS][NUM_COLS],colsum[NUM_COLS];

int value( int r, int c ) {
	assert( 0 <= r && r < NUM_ROWS );
	assert( 0 <= c && c < NUM_COLS );
	return modN[r];
}

int orig_x( int r, int c ) {
	assert( 0 <= r && r < NUM_ROWS );
	assert( 0 <= c && c < NUM_COLS );
	return divBLOCK[r];
}

int orig_y( int r, int c ) {
	assert( 0 <= r && r < NUM_ROWS );
	assert( 0 <= c && c < NUM_COLS );
	return divN[modBLOCK[r]];
}

cell *get_column() {
	cell *x,*best = NULL;
	int w = (1<<29);
	for ( x = header->head->right; x != header->head; x = x->right )
		if ( w > col_list[x->c]->cnt )
			w = col_list[(best = x)->c]->cnt;
	return best;
}

void cover( cell *col ) {
	cell *x,*right;

	assert( col );
	col->right->left = col->left, col->left->right = col->right;
	--header->cnt;
	for ( x = col->lower; x != col; x = x->lower ) 
		for ( right = x->right; right != x; right = right->right ) {
			right->upper->lower = right->lower, right->lower->upper = right->upper;
			assert( right->r >= 0 );
			--row_list[right->r]->cnt, --col_list[right->c]->cnt;
		}
}

void uncover( cell *col ) {
	cell *x,*left;
	for ( x = col->upper; x != col; x = x->upper ) 
		for ( left = x->left; left != x; left = left->left ) {
			left->upper->lower = left->lower->upper = left;
			++row_list[left->r]->cnt, ++col_list[left->c]->cnt;
		}
	col->right->left = col->left->right = col;
	++header->cnt;
}

void backtracking() {
	cell *c,*x,*right,*left;
	int i,j;
	
	if ( header->cnt == 1 ) {
		if ( 1 == ++ans )
			for ( i = 0; i < N; ++i )
				for ( j = 0; j < N; ++j )
					soln[i][j] = g[i][j];
		return ;
	}

	c = get_column();
	if ( col_list[c->c]->cnt < 2 )
		return ;
	cover(c);

	for ( x = c->lower; x != c && !ans; x = x->lower ) {
		for ( right = x->right; right != x; right = right->right )
			cover(col_list[right->c]->head);

		g[orig_x(x->r,x->c)][orig_y(x->r,x->c)] = 1+value(x->r,x->c);

		backtracking();
		if ( ans ) return ;

		for ( left = x->left; left != x; left = left->left )
			uncover(col_list[left->c]->head);
	}

	uncover(c);

}

int initial_setup() {
	int i,j,k,t;
	/* initialize the binary matrix for empty board */
	memset(binary_matrix,0,sizeof binary_matrix);
	memset(colsum,0,sizeof colsum);
	for ( i = 0; i < N; ++i )
		for ( j = 0; j < N; ++j )
			for ( k = 0; k < N; ++k ) {
				t = i*BLOCK+j*N+k;
				assert( 0 <= t && t < NUM_ROWS );
				assert( 0 <= SQUARE(i,j) < N );
				assert( (i*N+j)+3*BLOCK < NUM_COLS );
				binary_matrix[t][(i*N+k)+ROW_OFFSET] = 1;
				binary_matrix[t][(j*N+k)+COL_OFFSET] = 1;
				binary_matrix[t][(SQUARE(i,j)*N+k)+SQ_OFFSET] = 1;
				binary_matrix[t][(i*N+j)+UNO_OFFSET] = 1;
			}
	/* initial sanity check */
	for ( i = 0; i < N; ++i )
		for ( j = 0; j < N; ++j )
			if ( g[i][j] = b[i][j] ) {
				k = b[i][j]-1;
				if ( ++colsum[(i*N+k)+ROW_OFFSET] >= 2 ) return 0;
				if ( ++colsum[(j*N+k)+COL_OFFSET] >= 2 ) return 0;
				if ( ++colsum[(SQUARE(i,j)*N+k)+SQ_OFFSET] >= 2 ) return 0;
				if ( ++colsum[(i*N+j)+UNO_OFFSET] >= 2 ) return 0;
			}
	return 1;
}

char grid[N][N+N];

int main() {
	int i,j,k,t,l,cs = 0,ts;
	cell *x;

	/* precalc to save on expensive divisions and mod operations later */
	for ( i = 0; i < Q; ++i )
		divN[i] = (i/N), modN[i] = (i%N), divBLOCK[i] = (i/BLOCK), modBLOCK[i] = (i%BLOCK), divM[i] = (i/M);

	for ( scanf("%d",&ts); ts-- && (ptr = pool); ) {
		if ( ++cs > 1 ) putchar('\n');
		for ( i = 0; i < N; ++i )
			for ( scanf("%s",grid[i]), j = 0; j < N; ++j )
				b[i][j] = grid[i][j]=='-'?0:grid[i][j]-'A'+1;


		if ( !initial_setup()) {
			puts("Find another job");
			continue ;
		}
		
		/* Create separate double-linked lists for each row and column */
		for ( i = 0; i < NUM_ROWS; row_list[i++] = init_list(HOR) ) ;
		for ( j = 0; j < NUM_COLS; col_list[j++] = init_list(VER) );

		/* populate the heads of the column lists with dummy elements */
		add(header=init_list(HOR),init_cell(NONE,NONE));
		for ( j = 0; j < NUM_COLS; ++j ) {
			add(col_list[j],init_cell(NONE,j)), add(header,col_list[j]->head);
		}

		/* populate the lists */
		for ( i = 0; i < NUM_ROWS; ++i )
			for ( j = 0; j < NUM_COLS; ++j ) 
				if ( binary_matrix[i][j] ) {
					x = init_cell(i,j);
					add(row_list[i],x), add(col_list[j],x);
				}

		/* cover the already covered rows */
		for ( i = 0; i < N; ++i )
			for ( j = 0; j < N; ++j )
				if ( b[i][j] ) {
					k = b[i][j]-1, t = (i*BLOCK+j*N+k);
					assert( t < NUM_ROWS );
					assert( row_list[t]->cnt >= 2 );
					for ( x = row_list[t]->head->right; x != row_list[t]->head; x = x->right )
						cover(col_list[x->c]->head);
					assert( x == row_list[t]->head ) ;
					cover(col_list[x->c]->head);
				}

		ans = 0, backtracking();

		switch ( ans ) {
			case 0: puts("Find another job"); break ;
			case 1: for ( i = 0; i < N; printf("%c\n",soln[i++][j]-1+'A') ) 
						for ( j = 0; j < N-1; printf("%c",soln[i][j++]-1+'A') );
					break ;
			default: puts("Non-unique");
		}
	}
	return 0;
}

