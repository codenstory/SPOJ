/*
 * SQFREE
 * TOPIC: Mobius function, sieve, square-free numbers
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#define N 10000001
typedef long long i64;
using namespace std;

bool o[N];
int m[N];
vector<i64> s;

int main() {
	i64 i,j,k,t,ts,n;
	for(m[i=1]=1;++i<N;m[i]=-1);
	for(i=2;i<N;o[i++]=true);
	for(i=2;i<N;++i)
		for(j=i+i,k=2;o[i]&&j<N;o[j]=false,j+=i,++k){
			if(o[j]){
				m[j]=-1;
				if(!(k%i))m[j]=0;
			}
			else{
				if(!(k%i))m[j]=0;
				else m[j]*=-1;
			}
		}
	for(i=1;i<N;++i)if(m[i])s.push_back(i);
	for(scanf("%lld",&ts);ts--;printf("%lld\n",k))
		for(k=0,scanf("%lld",&n),i=0;i<(int)s.size()&&(j=s[i]*s[i])<=n;k+=n/j*m[s[i++]]);
	return 0;
}

