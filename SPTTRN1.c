/*
 * SPTTRN1
 * TOPIC: invariants
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x80
#define vc(x,y) (0<=(x)&&(x)<n&&0<=(y)&&(y)<n)

char g[N][N];
int n,
	dx[] = {0,1,0,-1},
	dy[] = {1,0,-1,0};

typedef struct state {
	int x,y,t;
} state;

int move_forward( state *s ) {
	int nx = s->x+dx[s->t], ny = s->y+dy[s->t],
		nnx = nx+dx[s->t], nny = ny+dy[s->t];
	if ( !vc(nx,ny) || vc(nnx,nny) && g[nnx][nny] == '*' )
		return 0;
	s->x = nx, s->y = ny;
}

int can_move_forward( const state *s ) {
	int nx = s->x+dx[s->t], ny = s->y+dy[s->t],
		nnx = nx+dx[s->t], nny = ny+dy[s->t];
	if ( !vc(nx,ny) || vc(nnx,nny) && g[nnx][nny] == '*' )
		return 0;
	return 1;
}

void turn( state *s ) { s->t = (s->t+1)&3; }
void paint( const state *s ) { g[s->x][s->y] = '*'; }

int final_state( const state *s ) {
	int nx,ny,nnx,nny;
	if ( !can_move_forward(s) ) return 1;
	nx = s->x+dx[s->t], ny = s->y+dy[s->t];
	nnx = nx+dx[(s->t+1)&3], nny = ny+dy[(s->t+1)&3];
	if ( vc(nnx,nny) && g[nnx][nny] == '*' )
		return 1;
	return 0;
}

int main() {
	int i,j,k,ts,cs = 0;
	state s;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		if ( ++cs > 1 ) putchar('\n');
		for ( i = 0; i < n; g[i++][j] = '\0' )
			for ( j = 0; j < n; g[i][j++] = '.' ) ;
		for ( s.x = s.y = s.t = 0; !final_state(&s); turn(&s) ) 
			for ( paint(&s); move_forward(&s); paint(&s) ) ;
		for ( i = 0; i < n; puts(g[i++]) ) ;
	}
	return 0;
}

