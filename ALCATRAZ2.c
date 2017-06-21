/*
 * TOPIC: easy-peasy
 * status: Accepted
 */
#include <stdio.h>
#define N 8
#define L(k) ((k) & ((~(k))+1ULL))
typedef long long i64;
 
unsigned int conflict[N];
char which[1<<N];
i64 c[N],tot[1<<N],ans;
 
int main() {
    int i,j,k,m;
    unsigned int u,v;
    for ( i = 0; i < N;which[1<<i]=i,++i);
    while ( 1 ) {
for(i=0;i<N;conflict[i]=0,scanf("%lld",&c[i++]));
	for(scanf("%d",&m),k=0;k<m;++k){
	scanf("%d%d",&i,&j),--i,--j;
            conflict[i]|=(1<<j);
            conflict[j] |= (1<<i);
        }
        for ( u = 1; u < (1<<N); ++u )
tot[u]=tot[u&~L(u)]+c[which[L(u)]];
for(ans=0,u=0;u<(1<<N);++u){
for(k=1,v=u;v&&k;v&=~L(v)){
i=which[L(v)];
if(u&conflict[i])k=0;
}
if(k&&ans<tot[u])ans=tot[u];
}
printf("%lld\n",ans);
return 0;
}
    return 0;
}
