/*
 * SPTTRN3
 * TOPIC: invariants, neat
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N 0x80
#define vc(x,y) (0<=(x) && (x)<n && 0<=(y) && (y)<n)
using namespace std;

char g[N][N];
int n,
	dx[] = {0,1,0,-1},
	dy[] = {1,0,-1,0};

struct state {
	int x,y,t,phase,mode,steps_made_in_phase,
		unlucky_strike;
	state() {};
	state( int x, int y, int t, int phase, int mode ) {
		this->x = x, this->y = y, this->t = t, this->phase = phase, this->mode = mode;
		steps_made_in_phase = unlucky_strike = 0;
	}
	bool can_advance() {
		if ( phase == 4 && steps_made_in_phase == 2 ) {
			++unlucky_strike;
			return false ;
		}
		int nx = x+dx[t], ny = y+dy[t],
			nnx = nx+dx[t], nny = ny+dy[t];
		if ( !vc(nx,ny) || vc(nnx,nny) && g[nnx][nny] == '*' ) {
			++unlucky_strike;
			return false ;
		}
		int left = (t+3)&3, right = (t+1)&3,
			lx = nx+dx[left], ly = ny+dy[left],
			rx = nx+dx[right], ry = ny+dy[right];
		if ( vc(lx,ly) && g[lx][ly] == '*' || vc(rx,ry) && g[rx][ry] == '*' ) {
			++unlucky_strike;
			return false ;
		}
		return true ;
	}
	void advance() {
		x += dx[t], y += dy[t], unlucky_strike = 0;
	}
	void turn() {
		t += (mode?3:1), t &= 3;
		if ( ++phase == 5 ) 
			phase = 0, mode ^= 1;
		steps_made_in_phase = 0;
	}
	void mark() {
		if ( g[x][y] == '.' )
			++steps_made_in_phase;
		g[x][y] = '*';
	}
	bool final_state() {
		return unlucky_strike >= 4;
	}
};

int main() {
	int i,j,k,ts,cs = 0;
	state s;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		if ( ++cs > 1 ) putchar('\n');
		for ( i = 0; i < n; g[i++][j] = '\0' )
			for ( j = 0; j < n; g[i][j++] = '.' ) ;
		for ( s = state(0,0,0,0,0); !s.final_state(); s.turn() ) 
			for ( s.mark(); s.can_advance(); s.advance(), s.mark() ) ;
		for ( i = 0; i < n; puts(g[i++]) ) ;
	}
	return 0;
}

