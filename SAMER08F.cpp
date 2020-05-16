/**
 * SAMER08F
 * TOPIC: STL, inner_product
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
#define N 0x100

std::istream &is = std::cin;
std::ostream &os = std::cout;

int main() {
    int i,j,k,n;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( ;(is >> n) and n; ) {
        std::vector<i64> c(n);
        std::iota(c.begin(),c.end(),1);
        os << std::inner_product(c.begin(),c.end(),c.begin(),0ll,[](i64 acc, i64 ir){return acc+ir;},[](i64 x,i64 y){return x*x;}) << std::endl;
    }
    return 0;
}
    