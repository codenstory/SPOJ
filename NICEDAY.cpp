/**
 * NICEDAY
 * TOPIC: range tree, 3d offline dominance queries
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (100100)

std::istream &is = std::cin;
std::ostream &os = std::cout;

using point= std::tuple<int,int,int>;

std::vector<point> pts;

struct cell {
    int i,j;
    std::shared_ptr<cell> left, right;
    int mini;
    cell() { left= right= nullptr, mini= N; }
};

void insert( std::shared_ptr<cell> &root, point pt ) {
    assert( root );
    std::shared_ptr<cell> x,*hold;
    for ( hold= &root, x= root, (*hold)->mini= std::min((*hold)->mini, std::get<2>(pt)); x->i != x->j; ) {
        auto k= (x->i+x->j)>>1;
        if ( std::get<1>(pt) <= k ) {
            hold= &x->left, x= x->left;
        } else {
            hold= &x->right, x= x->right;
        }
        (*hold)->mini= std::min((*hold)->mini,std::get<2>(pt));
    }
    (*hold)->mini= std::min((*hold)->mini,std::get<2>(pt));
}

bool dominatesSomeone( const std::shared_ptr<cell> &root, point pt ) {
    assert( root );
    std::shared_ptr<cell> x= root;
    for ( ;x->i != x->j; ) {
        auto k= (x->i+x->j)>>1;
        if ( std::get<1>(pt) <= k ) {
            x= x->left;
        } else {
            if ( std::get<2>(pt) > x->left->mini )
                return true ;
            x= x->right;
        }
    }
    return false ;
}

void build( std::shared_ptr<cell> &root, int i, int j ) {
    if ( not root )
        root= std::make_shared<cell>();
    root->i= i, root->j= j;
    if ( i < j ) {
        auto k = (i+j)>>1;
        build(root->left,i,k), build(root->right,k+1,j);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int ts,n,i,j,k;
    for ( is >> ts; ts--; ) {
        for ( is >> n, pts.resize(n), i= 0; i < n; ++i ) {
            point tpl;
            is >> std::get<0>(tpl) >> std::get<1>(tpl) >> std::get<2>(tpl);
            --std::get<0>(tpl), --std::get<1>(tpl), --std::get<2>(tpl);
            pts[i]= tpl;
        }
        std::sort(pts.begin(),pts.end(),[](const point &a, const point &b) {
            assert( std::get<0>(a) != std::get<0>(b) );
            return std::get<0>(a) < std::get<0>(b);
        });
        std::shared_ptr<cell> root= nullptr;
        build(root,0,n-1);
        for ( k= n, i= 0; i < n; ++i ) {
            if ( dominatesSomeone(root,pts[i]) )
                --k;
            insert(root,pts[i]);
        }
        os << k << std::endl;
    }
    return 0;
}
    