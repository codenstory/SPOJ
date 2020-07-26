/**
 * BOBALLS2
 * TOPIC: simulation
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { NE, NW, SW, SE };

int dx[]= {1,-1,-1,1},
    dy[]= {1,1,-1,-1};

class Solution {
    using t_node= std::tuple<int, int, int>;
    std::map<t_node, int> node2id;
    int m,n;
    int insert( t_node x ) {
        if ( node2id.count(x) )
            return node2id[x];
        auto z= node2id.size();
        return node2id[x]= z;
    }
public:

    Solution( int m, int n ) {
        this->m= n, this->n= n;
        int i,j;
        for ( j= 0; j <= n; j+= n )
            for ( i= 1; i <= m-1; ++i ) {
                for ( int t= NE; t <= SE; ++t ) {
                    insert(std::make_tuple(i,j,t));
                }
            }
        for ( i= 0; i <= m; i+= m )
            for ( j= 1; j <= n-1; ++j ) {
                for ( int t= NE; t <= SE; ++t )
                    insert(std::make_tuple(i,j,t));
            }
        insert(std::make_tuple(0,0,NE));
        insert(std::make_tuple(0,0,SW);
        insert(std::make_tuple(m,n,NE));
        insert(std::make_tuple(m,n,SW);

        insert(std::make_tuple(m,0,SE));
        insert(std::make_tuple(m,0,NW);
        insert(std::make_tuple(0,n,SE);
        insert(std::make_tuple(0,n,NW));
    }

};

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    return 0;
}
    