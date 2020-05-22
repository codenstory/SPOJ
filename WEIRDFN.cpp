/**
 * WEIRDFN
 * TOPIC: priority queues, medians
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

class median_manager {
private:
    std::priority_queue<int,std::vector<int>,std::less<>> max_heap;
    std::priority_queue<int,std::vector<int>,std::greater<>> min_heap;
    bool invariant() const {
        int m= max_heap.size(),
            n= min_heap.size();
        return abs(n-m) <= 1;
    }
    void promote() {
        min_heap.push(max_heap.top()), max_heap.pop();
    }
    void demote() {
        max_heap.push(min_heap.top()), min_heap.pop();
    }
    void normalize() {
        int m= max_heap.size(),
            n= min_heap.size();
        if ( invariant() ) return ;
        for (;m >= n+2; promote(), m= max_heap.size() ) ;
        for (;n >= m+2; demote(), n= min_heap.size() ) ;
        assert( invariant() );
    }
public:
    median_manager() {
        for (;not min_heap.empty(); min_heap.pop() ) ;
        for (;not max_heap.empty(); max_heap.pop() ) ;
    }
    int get_median() const {
        int m= max_heap.size(),
            n= min_heap.size();
        if ( m == n )
            return max_heap.top();
        assert( abs(m-n) <= 1 );
        if ( m+1 == n )
            return min_heap.top();
        assert( n+1 == m );
        return max_heap.top();
    }
    void push( int entry ) {
        int m= max_heap.size(),
            n= min_heap.size();
        if ( m == 0 and n == 0 ) {
            max_heap.push(entry);
            assert( invariant() );
            return ;
        }
        if ( m == 1 and n == 0 ) {
            if ( entry >= max_heap.top() ) {
                min_heap.push(entry);
                assert( invariant() );
                return ;
            }
            promote(), max_heap.push(entry);
            assert( invariant() );
            return ;
        }
        if ( m == 0 and n == 1 ) {
            if ( entry <= min_heap.top() ) {
                max_heap.push(entry);
                assert( invariant() );
                return ;
            }
            demote(), min_heap.push(entry);
            assert( invariant() );
            return ;
        }
        assert( m >= 1 and n >= 1 );
        if ( entry <= max_heap.top() ) {
            max_heap.push(entry);
            normalize();
            return ;
        }
        min_heap.push(entry);
        normalize();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    using i64= std::int64_t;
    int ts;
    for ( is >> ts; ts--; ) {
        i64 a,b,c,n,i, sum= 1;
        is >> a >> b >> c >> n;
        auto m= std::make_unique<median_manager>();
        m->push(1);
        for ( i= 2; i <= n; ++i ) {
            auto x= m->get_median();
            auto y= (a*x + b*i + c) % 1000000007ll;
            m->push(y), sum+= y;
        }
        os << sum << std::endl;
    }
    return 0;
}
    