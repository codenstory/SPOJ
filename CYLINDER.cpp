/**
 * CYLINDER
 * status: WA
 * TOPIC: maths
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
#define tol (1e-9)

long double h,w,T,Q;
const double pi= 2*acos(0.00);

long double f( long double t ) {
    long double R= min(w/2,t/2);
    long double ans= numeric_limits<long double>::min();
    if ( h-t >= 2*pi*R and w >= 2*pi*R )
        return max(h-t,w)*R*R;
    if ( h-t >= 2*pi*R )
        return w*R*R;
    if ( w >= 2*pi*R )
        return (h-t)*R*R;
    return ans;
}

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
        // pi*max(h-x,w)*pow(min(w/2,x/2),2)
        // x <- [h/2,h]:
        //
        T= (pi*w > h-w ? h/(pi+1) : h-pi*w);
        T= max((long double)0.00,T), T= min(h,T), Q= w/pi;
        if ( T < Q ) swap(T,Q);
        long double a,b,na,nb;
        for ( a= 0, b= h; fabs(b-a) >= tol; ) {
            na= (2*a+b)/3, nb= (a+2*b)/3;
            if ( f(na) < f(nb) )
                a= na;
            else b= nb;
        }
        vector<long double> vec= {0,h,w,
                                  min(h,max((long double)0,h/(pi+1))),
                                  w/pi,
                                  min(h,max((long double)0.00,h-pi*w))};
        transform(vec.begin(),vec.end(),vec.begin(),[&]( long double x ) {
            return pi*f(x);
        });
        auto ans= *(max_element(begin(vec),end(vec)));
        printf("%.3Lf\n",(ans));
    }
}

