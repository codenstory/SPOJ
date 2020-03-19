/**
 * TOPIC: simple DP
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
#define N 10100
#define AND 1
#define OR  0
#define NONE (-1)
#define oo (1<<29)

int n,targ,optype[N],changeable[N],val[N],z[N][2];
bool is_leaf( int x ) { return x > (n-1)/2; }

int calc_z( int x, int t ) {
    if ( z[x][t] != NONE )
        return z[x][t];
    if ( is_leaf(x) )
        return z[x][t]= (val[x]==t)?0:+oo;
    // this is an internal node
    assert( 2*x+1 <= n );
    auto left_0= calc_z(2*x,0), left_1= calc_z(2*x,1);
    auto right_0= calc_z(2*x+1,0), right_1= calc_z(2*x+1,1);
    z[x][t]= +oo;
    if ( t == 0 ) {
        if ( left_0 < +oo and right_0 < +oo )
            z[x][t]= left_0+right_0;
        if ( left_0 < +oo and right_1 < +oo ) {
            if ( optype[x] == AND )
                z[x][t]= std::min(z[x][t],left_0+right_1);
            if ( optype[x] == OR and changeable[x] )
                z[x][t]= std::min(z[x][t],left_0+right_1+1);
        }
        if ( left_1 < +oo and right_0 < +oo ) {
            if ( optype[x] == AND )
                z[x][t]= std::min(z[x][t],left_1+right_0);
            if ( optype[x] == OR and changeable[x] )
                z[x][t]= std::min(z[x][t],left_1+right_0+1);
        }
    }
    else { // t == 1
        if ( left_1 < +oo and right_1 < +oo )
            z[x][t]= left_1+right_1;
        if ( left_0 < +oo and right_1 < +oo ) {
            if ( optype[x] == OR )
                z[x][t]= std::min(z[x][t],left_0+right_1);
            if ( optype[x] == AND and changeable[x] )
                z[x][t]= std::min(z[x][t],left_0+right_1+1);
        }
        if ( left_1 < +oo and right_0 < +oo ) {
            if ( optype[x] == OR )
                z[x][t]= std::min(z[x][t],left_1+right_0);
            if ( optype[x] == AND and changeable[x] )
                z[x][t]= std::min(z[x][t],left_1+right_0+1);
        }
    }
    return z[x][t];
}

int main() {
    int i,j,k,cs= 0,ts;
    std::istream &is= std::cin;
    for ( is >> ts; ts--; ) {
        is >> n >> targ;
        for ( i= 1; i <= (n-1)/2; is >> optype[i] >> changeable[i], ++i ) ;
        for ( i= 1+(n-1)/2; i <= n; is >> val[i++] ) ;
        for ( i= 1; i <= n; ++i )
            for ( k= 0; k <= 1; z[i][k++]= NONE ) ;
        auto res= calc_z(1,targ);
        std::cout << "Case #" << ++cs << ": ";
        if ( res < +oo )
            std::cout << res << std::endl;
        else std::cout << "IMPOSSIBLE" << std::endl;
    }
    return 0;
}

