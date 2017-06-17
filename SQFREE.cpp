/*
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define N (100000000000001)
typedef long long i64;
using namespace std;

bool e[N],m[N];

int main() {
	i64 i,j,k,ts,n;
	for(e[2]=true,i=3;i<N;e[i]=true,i+=2);
	/*for(i=0;i<N;m[i++]=true);
	for(i=2;i<N;++i)
		if(e[i])
			for(j=i+i,k=2;j<N;e[j]=false,j+=i,++k)
				if(0==(k%i))m[j]=false;
				*/
	for ( scanf("%lld",&ts); ts--; ) {
		scanf("%lld",&n);
	}
	return 0;
}

