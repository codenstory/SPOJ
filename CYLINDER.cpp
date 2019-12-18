/**
 * CYLINDER
 * status: WIP
 * TOPIC: maths
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
#define N 2207
#define tol (1e-7)

double h,w;

int main() {
    cin.tie(nullptr), ios_base::sync_with_stdio(false);
    int i,j,k,ts;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    while ( cin >> w >> h ) {
        if ( fabs(w) < tol and fabs(h) < tol )
            break ;
        // x<-[0,h/2]:
        // M_PI*max(h-x,w)*pow(min(w/2,x/2),2)
        // x <- [h/2,h]:
        //
    }
}

