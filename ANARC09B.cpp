#include <bits/stdc++.h>
using namespace std;

int main() {
    int64_t a,b;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    while ( (cin >> a >> b) and (a or b) ) {
        auto g= __gcd(a,b);
        auto lcm= (a*b)/g;
        cout << lcm/g << endl;
    }
    return 0;
}
