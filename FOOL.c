/*
 * FOOL
 * TOPIC: dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x100

int is_set[N][N],is_element[N][N],yes,is_list[N][N],n;
char s[N];

int main() {
	int i,j,k,t,ts,cs = 0;
	for ( scanf("%d",&ts); ts-- && ++yes; ) {
		scanf("%s",s+1), n = strlen(s+1);
		for ( i = 1; i <= n; ++i )
			is_list[i][i] = is_element[i][i] = yes;
		for ( i = 1; i <= n; ++i )
			for ( j = 0; j <= i-1; ++j ) 
				is_list[i][j] = yes;
		for ( k = 2; k <= n; ++k )
			for ( i = 1; (j = i+k-1) <= n; ++i ) {
				if ( s[i] == '{' && s[j] == '}' && ((i+1>j-1) || is_list[i+1][j-1] == yes) ) 
					is_element[i][j] = is_set[i][j] = yes;
				for ( t = i+1; t <= j-1; ++t )
					if ( s[t] == ',' && is_element[i][t-1] == yes && is_list[t+1][j] == yes )
						is_list[i][j] = yes;
				if ( is_list[i][j] != yes && is_element[i][j] == yes )
					is_list[i][j] = yes;
			}
		printf("Word #%d: %s\n",++cs,is_set[1][n]==yes?"Set":"No Set");
	}
	return 0;
}

