#include <stdio.h>
typedef long long u;
u i,j,k;
u f(u x){u a=0,t=0,r,g=0,d=1,p=0;for(;x;a+=(k==(r=x%10))*(1+g)+d*(r>k)+(r*t*p),g+=r*d,p=d,d*=10,x/=10,++t);return a;}
int main(){for(;scanf("%lld%lld",&i,&j)&&i;puts(""))for(k=1;k<=9;printf("%lld ",f(j)-f(i-1)),++k);return 0;}
