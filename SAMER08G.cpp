/**
 * SAMER08G
 * TOPIC: ad hoc, simulation
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

int n;
std::vector<std::pair<int,int>> c;
std::map<int,int> a;

bool allocate( int x, int pos ) {
    if ( pos < 0 or pos >= n )
        return false ;
    if ( a.count(pos) ) {
        assert( a[pos] != x );
        //printf("Attempting to put %d to position %d, already occupied by %d\n",x,pos,a[pos]);
        return false;
    }
    //printf("Success in putting %d at position %d\n",x,pos);
    a[pos]= x;
    return true ;
}

int main() {
    int i,j,k;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( ;(is >> n) and n; ) {
        c.resize(n), a.clear();
        for ( auto &v: c )
            is >> v.first >> v.second;
        for ( i= 0; i < n; ++i ) {
            k= c[i].second;
            if ( k == 0 ) {
                if ( allocate(c[i].first,i) )
                    continue ;
                break ;
            }
            if ( k < 0 ) {
                if ( allocate(c[i].first,i+k) )
                    continue ;
                break ;
            }
            if ( allocate(c[i].first,i+k) )
                continue ;
            break ;
        }
        if ( i < n ) {
            os << "-1\n";
            continue ;
        }
        std::vector<int> ans(n);
        for ( auto it: a )
            ans[it.first]= it.second;
        for ( i= 0; i < n; ++i ) {
            if ( i )
                os << ' ';
            os << ans[i];
        }
        os << '\n';
    }
    return 0;
}
    