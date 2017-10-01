/*
 * CF25E
 * TOPIC: KMP, greedy
 * status: WA (AC on CodeForces)
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<19)
#define M 3
#define P 6
#define FG (ptr = fgets(buff,sizeof buff,stdin))

char buff[N],s[M][N],res1[N],res2[N],t[N],*ptr;
int n[M],p[N],len[M],delim,m,pos[M],ans,
	perm[P][M] = {0,1,2,
				  0,2,1,
				  1,0,2,
				  1,2,0,
				  2,0,1,
				  2,1,0};

void pack_string( char *x, char *y, int m, int n, char *res, int *length ) {
	int i,j,k,max_overlap = 0;
	for ( k = 0, i = 0; i < n; t[++k] = y[i++] ) ;
	for ( t[++k] = 'z'+1, i = 0; i < m; t[++k] = x[i++] ) ;
	for ( j = p[0] = -1, i = 1; i <= k; p[i++] = ++j )
		for ( ;j+1 && t[j+1] != t[i]; j = p[j] ) ;
	for ( i = m+2; i <= k; ++i )
		if ( p[i] > max_overlap )
			max_overlap = p[i];
	if ( max_overlap == n ) {
		for ( *length = m, i = 0; i < m; res[i] = x[i], ++i ) ;
		return ;
	}
	for ( max_overlap = 0, k = 0, i = 0; i < n; t[++k] = y[i++] ) ;
	for ( t[++k] = 'z'+1, i = 0; i < m; t[++k] = x[i++] ) ;
	for ( j = p[0] = -1, i = 1; i <= k; p[i++] = ++j )
		for ( ;j+1 && t[j+1] != t[i]; j = p[j] ) ;
	max_overlap = p[k];
	assert( 0 <= max_overlap && max_overlap <= n );
	for ( j = 0, *length = m+n-max_overlap, i = 0; i < m; res[j++] = x[i++] ) ;
	for ( i = max_overlap; i < n; res[j++] = y[i++] ) ;
	assert( j == *length ) ;
}

int main() {
	int i,j,k,l,t,length1,length2;
	for ( ;FG; ) {
		for ( i = 0, j = 0; buff[j] && buff[j] != '\n'; s[i][j] = buff[j], ++j ) ;
		for ( len[i] = j; ++i < M && FG; len[i] = j )
			for ( j = 0; buff[j] && buff[j] != '\n'; s[i][j] = buff[j], ++j ) ;
		for ( ans = (N<<4), l = 0; l < P; ++l ) {
			pack_string(s[perm[l][0]],s[perm[l][1]],len[perm[l][0]],len[perm[l][1]],res1,&length1);
			pack_string(res1,s[perm[l][2]],length1,len[perm[l][2]],res2,&length2);
			if ( ans > length2 ) ans = length2;
		}
		printf("%d\n",ans);
	}
	return 0;
}

/*
 *
#include <cstring>
#include <stdio.h>      /* printf */
#include <math.h>       /* ceil */
#include <map>
#include <set>
#include <queue>
#include <iostream>
using namespace std;
 
#define pb push_back
#define lb lower_bound
#define ub upper_bound
 
typedef vector<int> vi;
typedef vector<vi> vv;
 
int b[1000005];
int n, m;
string s1, s2, s3, T;
string s;
void kmpPre()
{
    int j = b[0] = -1;
 
    for(int i = 0 ; i < s.size(); )
    {
        while(j >= 0 && s[i] != s[j])
            j = b[j];
        j++;
        i++;
        b[i] = j;
    }
}
int mini  = 0;
 
bool kmpSearch()   // this is similar as kmpPreprocess(), but on string T
{
    int i = 0, j = 0; // starting values
    while (i < T.size())   // search through string T
    {
        while (j >= 0 && T[i] != s[j]) j = b[j]; // if different, reset j using b
        i++;
        j++; // if same, advance both pointers
        if (j == s.size())   // a match found when j == m
        {
            return true;
            j = b[j]; // prepare j for the next possible match
        }
    }
    return false;
}
 
int makeString(string s1, string s2, string s3)
{
    int maxL = 0;
    s = s2;
    T = s1;
    kmpPre();
    if(kmpSearch()){
        maxL = s2.size();
    }else{
        s = s2 + s1;
        m = s.size();
        kmpPre();
        int l = s.size();
        do{
            maxL = b[l];
            l = b[l];
        }while(maxL > s1.size());
    }
    s = s1;
    if(maxL == 0)
        s.append(s2);
    else
    {
        for(int i = maxL ; i < s2.size(); i++)
            s += s2[i];
    }
    string ss = s;
    T = ss;
    s = s3;
    kmpPre();
    if(kmpSearch()){
        maxL += s3.size();
    }else{
        s = s3 + ss;
        m = s.size();
        kmpPre();
        int l = s.size();
        int maxL2;
        do{
            maxL2 = b[l];
            l = b[l];
        }while(maxL2 > ss.size());
        maxL += maxL2;
    }
 
    return s1.size() + s2.size() + s3.size() - maxL;
 
}
 
int min(int x, int y)
{
    return x < y ? x : y;
}
 
int main()
{
    char c[1000005];
    int T;
    int t;
 
    while(scanf("%s", c) != EOF)
    {
        s1 = c;
        scanf("%s", c);
        s2 = c;
        scanf("%s", c);
        s3 = c;
        mini = makeString(s1, s2, s3);
        mini = min(mini, makeString(s1, s3, s2));
        mini = min(mini, makeString(s2, s1, s3));
        mini = min(mini, makeString(s2, s3, s1));
        mini = min(mini, makeString(s3, s2, s1));
        mini = min(mini, makeString(s3, s1, s2));
        printf("%d\n", mini);
    }
    return 0;
}
 */
