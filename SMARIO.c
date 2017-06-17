/*
 * SMARIO: Super Mario Revisited
 * TOPIC: bfs, bitmasks
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define oo 0xfffffffful
#define N 0x10
#define MAXW 10
#define SAVED(k)  (printf("Mario saved the princess in %d steps\n",(int)(k)))
#define FAILED (puts("Mario failed to save princess"))
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
#define BIT(k) (1ULL << (k))
#define MASK(k) (BIT(k)-1ULL)
#define enc(t,x,y,coins) ((x)|((y)<<4)|((t)<<8)|((coins)<<12))
#define PREV(x) (((x)+QS-1)&(QS-1))
#define NEXT(x) (((x)+1)&(QS-1))
#define Q BIT(22)
#define QS BIT(23)
#define MAXC 10
#define ADD_TO_FRONT(x) (head=PREV(head),++cnt,q[head]=(x))
#define ADD_TO_REAR(x)  (q[tail]=(x),++cnt,tail=NEXT(tail))
enum { MARIO = 'S', 
	   FREE = '.', 
	   UP = 'U', 
	   DOWN = 'D', 
	   BRICK = '#', 
	   COIN = 'C', 
	   MONSTER = 'M' 
     };
typedef unsigned long state;

char g[MAXW][N][N],id[MAXW][N][N],TOPMOST,BOTTOMMOST,
	 dx[] = {-1,0,1,0},
	 dy[] = {0,1,0,-1};
int m,n,worlds,coins[MAXC],len,seen[Q],yes,cnt;
state q[QS],head,tail,d[Q];
unsigned int ALL_COINS;

unsigned int bfs( const state src ) {
	int x,y,nx,ny,i,j,k,t,nt;
	state u,v;
	unsigned int c,nc,ans = +oo,dw;

	for ( head=tail=0, cnt=0, seen[src]=++yes,d[src]=0, ADD_TO_REAR(src); cnt; ) {
		u=q[head],--cnt,head=NEXT(head),x=(u&0xf),y=((u>>4)&0xf),t=((u>>8)&0xf),c=(u>>12);
		if ( c == ALL_COINS && g[t][x][y] == MONSTER ) 
			if ( ans > d[u] )
				ans = d[u];
		if ( ans <= d[u] || g[t][x][y] == MONSTER ) continue ;

		for ( i = 0; i < 4; ++i )
			if ( vc(nx=x+dx[i],ny=y+dy[i]) && g[t][nx][ny] != BRICK ) {
				if ( g[t][nx][ny] == MONSTER && c != ALL_COINS ) 
					continue ;
				nt = t, nc = c, dw = 1;
				if ( g[nt][nx][ny] == COIN )
					nc |= BIT(id[nt][nx][ny]);
				else if ( g[t][nx][ny] == UP ) {
					assert( t != TOPMOST );
					for ( nt = t-1, dw = 1; g[nt][nx][ny] == UP; --nt ) ;
					if ( g[nt][nx][ny] == DOWN || g[nt][nx][ny] == BRICK ) 
						continue ;
					if ( g[nt][nx][ny] == COIN )
						nc |= BIT(id[nt][nx][ny]);
				}
				else if ( g[t][nx][ny] == DOWN ) {
					assert( t != BOTTOMMOST );
					for ( nt = t+1, dw = 1; g[nt][nx][ny] == DOWN; ++nt ) ;
					if ( g[nt][nx][ny] == UP || g[nt][nx][ny] == BRICK ) 
						continue ;
					if ( g[nt][nx][ny] == COIN )
						nc |= BIT(id[nt][nx][ny]);
				}
				if ( seen[v=enc(nt,nx,ny,nc)]!=yes || d[v]>d[u]+dw )
					seen[v]=yes, d[v]=d[u]+dw, dw?ADD_TO_REAR(v):ADD_TO_FRONT(v);
			}
	}
	return ans;
}

int main() {
	int i,j,k,t;
	state src;
	unsigned int w;
	for ( ;3 == scanf("%d %d %d",&m,&n,&worlds) && (m||n||worlds); ) {
		memset(id,-1,sizeof id);
		for ( len = 0, t = 0; t < worlds; ++t ) 
			for ( i = 0; i < m; ++i )
				for ( scanf("%s",g[t][i]), j = 0; j < n; ++j )
					if ( g[t][i][j] == MARIO )
						src = enc(t,i,j,0), g[t][i][j] = FREE;
					else if ( g[t][i][j] == COIN )
						coins[id[t][i][j]=len++] = ((i|(j<<4))|((t)<<8));
		ALL_COINS = MASK(len), TOPMOST = 0, BOTTOMMOST = worlds-1;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j ) {
				if ( g[TOPMOST][i][j] == UP )
					g[TOPMOST][i][j] = BRICK;
				if ( g[BOTTOMMOST][i][j] == DOWN )
					g[BOTTOMMOST][i][j] = BRICK;
			}
				
		if ( (w = bfs(src)) < +oo )
			SAVED(w);
		else FAILED;
	}
	return 0;
}

