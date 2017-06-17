/*
 * ANARC08E: Relax, it's just a game
 * TOPIC: binomial coefficient
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
#define tol (1e-13)
#define N 0x20
using namespace std;
typedef long long i64;

i64 C[N][N];

int main() {
	int i,j,k;
	for ( i = 0; i < N; ++i )
		C[i][0] = 1;
	for ( i = 1; i < N; ++i )
		for ( j = 1; j <= i; ++j )
			C[i][j] = C[i-1][j]+C[i-1][j-1];
	while ( 2 == scanf("%d %d",&i,&j) && (i+1||j+1) ) {
		printf("%d+%d%s%d\n",i,j,C[i+j][i]==i+j?"=":"!=",i+j);
	}
	return 0;
}

