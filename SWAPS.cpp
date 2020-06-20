/**
 * SWAPS
 * TOPIC: Red-Black trees, range trees, range counting
 * status:
 */
#include <bits/stdc++.h>

template<typename vtype>
class rbtree {
    struct cell;
    using holder= std::shared_ptr<cell>;
    using t_dir= int;
    enum { L, R };
    enum { Red, Black };
    struct cell {
        unsigned char c;
        vtype val;
        holder son[2], p;
        size_t card,
               freq;
        holder operator() (int t) const {
            return son[t];
        }
        holder& operator() (int t) {
            return son[t];
        }
    };

#define color(x) ((x)->c)
#define which_son(x) ((x)->p->son[L] == (x)?L:R)
#define flip(x) ((x)->c^=1)

    holder root, nil;
    holder operator ++ ( holder x ) {
        return x->p;
    }
    holder make_nil() {
        auto x= std::make_shared<cell>();
        x->c= Black, x(L)= x(R)= x->p= x;
        x->card= x->freq= 0;
        return x;
    }
    holder make_cell( vtype val ) {
        auto x= std::make_shared<cell>();
        x->c= Red, x(L)= x(R)= x->p= nil, x->val= val;
        return x;
    }
    void propagate( holder &x ) {
        for ( ;x != nil; x->card= x->freq+x(L)->card+x(R)->card, x++ ) ;
    }
    void rotate( holder &x, t_dir t ) {
        assert( t == L or t == R );
        assert( x != nil );
        auto y= x(t^1);
        assert( y != nil );
        if ( (x(t^1)= y(t)) != nil )
            y(t)->p= x;
        y(t)= x, x->p= y;
        propagate(x);
    }

    holder find( vtype val ) {
        auto x= root;
        for ( ;x != nil and x->val != val; x= x->val<val?x->son[R]:x->son[L] ) ;
        return x;
    }

public:

    void push( vtype val ) {
        holder x= root, *hold= &root, p= nil;
        for ( ;x != nil and x->val != val; ) {
            auto t= x->val<val ? R:L;
            p= x, hold= &x->son[t], x= x->son[t];
        }
        if ( x != nil ) {
            ++x->freq, propagate(x);
            return ;
        }
        *hold= x= make_cell(val), x->p= p, x->freq= 1;
        propagate(x);
        while ( x != root and color(x->p) == Red ) {
            auto g= x->p->p;
            auto i= which_son(x->p);
            auto y= g->son[i^1];
            if ( color(y) == Red ) {
                flip(g), flip(x->p), flip(y), x= g;
                continue ;
            }
            if ( which_son(x) == (i^1) )
                x= x->p, rotate(x,i);
            flip(x->p), flip(g), rotate(g,i^1), x= root;
        }
        root->c= Black;
    }

    bool erase( vtype val ) {
        auto z= find(val);
        if ( val == nil )
            return false ;
        if ( --z->freq ) {
            propagate(z);
            return true ;
        }
        assert( z->freq == 0 );
        if ( z(L) != nil and z(R) != nil ) {
            auto y= z(R);
            for ( ;z(L) != nil; z= z(L) ) ;
            std::swap(z->val,y->val), std::swap(z->freq,y->freq);
            propagate(y);
            z= y;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &out = std::cout;
    return 0;
}