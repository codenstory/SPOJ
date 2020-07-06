/**
 * ANARC08H
 * TOPIC: Red-Black trees
 * status: Accepted
 */
#include <bits/stdc++.h>

class rbtree {
private:
    enum { L , R };
    enum { Red, Black };
    struct cell;
    using holder= cell *;
    struct cell {
        holder son[2],p;
        int val, freq, card;
        unsigned char c;
    };
    holder nil, root;
    holder make_cell() {
        holder x= new cell();
        x->son[L]= x->son[R]= x->p= nil, x->c= Red;
        return x;
    }
    holder make_nil() {
        holder x= new cell();
        x->son[L]= x->son[R]= x->p= x, x->c= Black;
        x->freq= x->card= 0;
        return x;
    }
    void propagate( holder &x ) {
        for ( auto z= x; z != nil; z->card= z->freq+z->son[L]->card+z->son[R]->card, z= z->p ) ;
    }

#define color(x) ((x)->c)
#define flip(x) ((x)->c^=1)
#define which_son(x) ((x)->p->son[L]==(x)?L:R)

    void rotate( holder &x, int t ) {
        assert( t == L or t == R );
        auto y= x->son[t^1];
        assert( y != nil );
        if ( (x->son[t^1]= y->son[t]) != nil )
            y->son[t]->p= x;
        if ( (y->p= x->p) == nil )
            root= y;
        else x->p->son[which_son(x)]= y;
        y->son[t]= x, x->p= y;
        propagate(x);
    }

    void push( holder &root, int val ) {
        holder x= root, *hold= &root, p= nil;
        for ( ;x != nil and x->val != val; ) {
            auto t= x->val<val?R:L;
            hold= &(x->son[t]), p= x, x= x->son[t];
        }
        if ( x != nil ) {
            ++x->freq, propagate(x);
            return ;
        }
        (*hold)= x= make_cell(), x->p= p, x->val= val, x->freq= 1;
        for ( propagate(x); x != root and color(x->p) == Red; ) {
            auto g= x->p->p;
            assert( g != nil );
            auto i= which_son(x->p);
            auto y= g->son[i^1];
            if ( color(y) == Red ) {
                assert( color(g) == Black );
                flip(x->p), flip(g), flip(y), x= g;
                continue ;
            }
            if ( which_son(x) == (i^1) )
                x= x->p, rotate(x,i);
            flip(x->p), flip(g), rotate(g,i^1), x= root;
        }
        root->c= Black;
    }

    void fixup( holder &x ) {
        for ( ;x != root and color(x) == Black; ) {
            auto i= which_son(x);
            auto y= x->p->son[i^1];
            if ( color(y) == Red ) {
                flip(x->p), flip(y), rotate(x->p,i);
                continue ;
            }
            if ( color(y->son[L]) == Black and color(y->son[R]) == Black ) {
                flip(y), x= x->p;
                continue ;
            }
            if ( color(y->son[i]) == Red ) {
                flip(y->son[i]), flip(y), rotate(y,i^1);
                continue ;
            }
            flip(y->son[i^1]), y->c= x->p->c, x->p->c= Black;
            rotate(x->p,i), x= root;
        }
        x->c= Black;
    }

    holder find( int val ) {
        auto x= root;
        for ( ;x != nil and x->val != val; x=x->son[x->val<val?R:L] ) ;
        return x;
    }

    bool erase( int val ) {
        auto z= find(val);
        if ( z == nil )
            return false ;
        --z->freq, propagate(z);
        if ( z->freq )
            return true ;
        if ( z->son[L] != nil and z->son[R] != nil ) {
            auto y= z->son[R];
            for ( ;y != nil and y->son[L] != nil; y= y->son[L] ) ;
            assert( y != nil and y->son[L] == nil ) ;
            std::swap(z->val,y->val), std::swap(z->freq,y->freq);
            propagate(y), z= y;
        }
        assert( z->son[L] == nil or z->son[R] == nil );
        auto x= (z->son[L] == nil ? z->son[R]:z->son[L]);
        if ( (x->p= z->p) == nil )
            root= x;
        else z->p->son[which_son(z)]= x;
        propagate(x);
        if ( color(z) == Black )
            fixup(x);
        return true ;
    }

    int kth( holder &root, int k ) {
        for ( auto x= root; x != nil; ) {
            assert( x->card > k );
            if ( x->son[L]->card > k ) {
                x= x->son[L];
                continue ;
            }
            if ( x->son[L]->card+x->freq > k )
                return x->val;
            k-= x->son[L]->card, k-= x->freq;
            assert( k >= 0 );
            x= x->son[R];
        }
        return -1;
    }

    int rank( int val ) {
        int ans= 0;
        for ( auto x= root; x != nil; ) {
            if ( x->val == val )
                return ans+x->son[L]->card;
            if ( x->val > val ) {
                x= x->son[L];
                continue ;
            }
            ans+= x->freq, ans+= x->son[L]->card;
            x= x->son[R];
        }
        return ans;
    }

public:

    rbtree() {
        root= nil= make_nil();
    }

    void ins( int val ) {
        push(root,val);
    }

    bool del( int val ) {
        return erase(val);
    }

    int ordered_stats( int k ) {
        return kth(root,k);
    }

    int rnk( int val ) {
        return rank(val);
    }

};

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif

    std::istream &is = std::cin;
    std::ostream &out = std::cout;

    for ( int n,D; (is >> n >> D) and (n or D); ) {
        if ( n == 1 ) {
            out << n << ' ' << D << ' ' << "1\n";
            continue ;
        }
        assert( n >= 2 );
        auto T= std::make_shared<rbtree>();
        int x,y,r;
        for ( x= 0; x < n; T->ins(x++) ) ;
        r= T->rnk(y= (D-1)%n);
        auto flag= T->del(y);
        assert( flag );
        //std::cerr << "First deleted " << y+1 << std::endl;
        for ( int remains= n-1; remains >= 2; ) {
            if ( (r+= D-1) >= remains )
                r%= remains;
            y= T->ordered_stats(r);
            auto ok= T->del(y);
            //std::cerr << "next deleted " << y+1 << std::endl;
            assert( ok );
            --remains;
        }
        out << n << ' ' << D << ' ' << T->ordered_stats(0)+1 << '\n';
    }

    return 0;

}