/**
 * TOPIC: computational geometry, sorting, interviews
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
#define N 2207

int m,n;
pair<i64,i64> c[N];
double angle[N];

int largest_run( double *x, int n ) {
    int i,j,k= 0;
    for ( i= 0; i < n; k= max(k,j-i), i= j )
        for ( j= i+1; j < n and fabs(x[i]-x[j]) < 1e-7; ++j ) ;
    return k;
}

int main() {
    cin.tie(nullptr), ios_base::sync_with_stdio(false);
    int i,j,k,ts;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( cin >> ts; ts--; ) {
        cin >> m;
        for ( i= 0; i < m; ++i )
            cin >> c[i].first >> c[i].second;
        sort(c,c+m);
        int ans= 0;
        for ( i= 0; i < m; ++i ) {
            for ( k= 0, j= i+1; j < m; ++j, ++k )
                angle[k]= atan((c[j].second-c[i].second+0.00)/(c[j].first-c[i].first));
            sort(angle,angle+k);
            ans= max(ans,largest_run(angle,k));
        }
        cout << ans << endl;
    }
}
