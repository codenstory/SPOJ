/**
 * CYLINDER
 * status: Accepted
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
    if ( t > w ) return ans;
    if ( h-t >= 2*pi*R and w >= 2*pi*R )
        return max(h-t,w)*R*R;
    if ( h-t >= 2*pi*R )
        return w*R*R;
    if ( w >= 2*pi*R and h-t >= 0 )
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
        if ( w > h ) swap(w,h);
        double d=double(w)*1.0/pi;
        double height=double(h)-d;
        double vol1=pi*d*d*height*1.0/(4*1.0);
        d=double(h)*1.0/(pi+1);
        if(d>w)
            d=double(w);
        height=double(w);
        double vol2=pi*d*d*height*1.0/(4*1.0);
        if(vol1>vol2)
            vol2=vol1;
        printf("%.3f\n",vol2);
        /*
        vector<long double> vec= {0,w/pi,h/(pi+1)};
        transform(vec.begin(),vec.end(),vec.begin(),[&]( long double x ) {
            return pi*f(x);
        });
        auto ans= *(max_element(begin(vec),end(vec)));
        cout << fixed << setprecision(3) << ans << endl;
         */
    }
}

