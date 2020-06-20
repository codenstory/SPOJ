/**
 * TOPIC: Diophantine equations, Fibonacci
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
#define N 23

i64 F[N],S[N];

void ext_euclid( i64 x, i64 y, i64 *a, i64 *b, i64 *d ) {
    (not y) ? (*a= 1, *b= 0, *d= x) : (ext_euclid(y,x%y,b,a,d), *b-= (*a)*(x/y));
}

// solves ax+by = c
void diophantine_solver( i64 A, i64 B, i64 C, i64 *x, i64 *y ) {
    if ( not B ) {
        assert( not (C%A) );
        *x= C/A, *y= 0;
        return ;
    }
    diophantine_solver(B,A%B,C,y,x);
    *y-= (*x)*(A/B);
    assert( A*(*x) + B*(*y) == C );
}

i64 my_gcd( i64 x, i64 y ) { return (not y)?x:my_gcd(y,x%y); }

void simulate( int r, i64 k, i64 T, i64 x, i64 y ) {
    std::vector<i64> c(r+1);
    c[r]= x, c[r-1]= y;
    for ( int i= r-2; i >= 1; --i )
        c[i]= c[i+1]+c[i+2];
    auto sum= std::accumulate(c.begin()+1,c.end(),0ll);
    assert( sum == T );
}

i64 solve( int r, i64 k, i64 T, i64 &bestx, i64 &besty ) {
    assert( r >= 3 );
    auto A= S[r-2]+1, B= S[r-1];
    i64 alpha, beta;
    //std::cerr << "A= " << A << ", B= " << B << ", C= " << T << std::endl;
    diophantine_solver(A,B,T,&alpha,&beta);
    assert( alpha*A+beta*B == T );
    auto g= my_gcd(A,B);
    auto LB= (alpha-beta)/(A/g+B/g), UB= (alpha*g)/B;
    i64 best= std::numeric_limits<i64>::min();
    for ( i64 t: {LB-1,LB,LB+1,UB-1,UB,UB+1} ) {
        auto x= alpha-t*B/g, y= beta+t*A/g;
        if ( y > x and x > 0 )
            if ( A*x+B*y == T ) {
                auto candidate= x*F[r-2]+y*F[r-1];
                if ( candidate > best or (candidate == best and bestx > x) )
                    best= candidate, bestx= x, besty= y;
            }
    }
    return best*k;
}

int main() {
    int i,j;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( F[1]= F[2]= 1, i= 3; i < N; ++i )
        F[i]= F[i-1]+F[i-2];
    for ( i= 1; i < N; ++i )
        S[i]= S[i-1]+F[i];
    std::istream &is= std::cin;
    int r;
    i64 k,T;
    for ( ;(is >> r) and r; ) {
        is >> k >> T;
        if ( r == 2 ) {
            std::cout << 1 << " " << (T-1) << " " << k*(T-1) << std::endl;
            continue ;
        }
        assert( r >= 3 );
        i64 x,y;
        auto ans= solve(r,k,T,x,y);
        //simulate(r,k,T,x,y);
        std::cout << x << " " << y << " " << ans << std::endl;
    }
    return 0;
}

