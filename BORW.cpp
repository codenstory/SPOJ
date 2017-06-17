/*
 * BORW: Black or White
 * TOPIC: dp
 * status:
 */
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#define N 201
#define oo (1<<30)
enum { W, B };
using namespace std;

int n,x[N];

int main() {
	int i,j,k,t;
	for ( ;1==scanf("%d",&n) && n > 0; ) {
		for ( i=1; i<=n; scanf("%d",&x[i++]) );
	}
	return 0;
}

