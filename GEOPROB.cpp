/**
 * GEOPROB
 * TOPIC: geometry, Boost
 * status: Accepted
 */
#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

std::istream &is = std::cin;
std::ostream &os = std::cout;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    boost::multiprecision::cpp_int a,b,c,d;
    int ts;
    for ( is >> ts; ts--; ) {
        is >> b >> c >> d;
        a= 2*c-b-d;
        os << a << std::endl;
    }
    return 0;
}
    