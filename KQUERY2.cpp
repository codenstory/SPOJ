/**
 * KQUERY2
 * TOPIC: orthogonal range counting, Red-Black trees, order statistics, ranks
 * status:
 */
#include <bits/stdc++.h>
using vtype= int;

class rbtree {
private:
    enum { L= 0, R= 1 };
    enum { Red= L, Black= R };
    struct cell {
        std::shared_ptr<cell> son[2],p;
        vtype val;
        int freq, card;
        unsigned char c;
        explicit cell() {
            son[L]= son[R]= nullptr, p= nullptr, c= Red;
            freq= card= 0;
        }
    };
    std::shared_ptr<cell> root, nil;
    std::shared_ptr<cell> make_cell() {
        auto x= std::make_shared<cell>();
        x->son[L]= x->son[R]= nil, x->p= nil;
        return x;
    }
    std::shared_ptr<cell> make_nil() {
        auto x= std::make_shared<cell>();
        x->c= Black, x->son[L]= x->son[R]= x->p= x;
        return x;
    }

    void update_upwards( std::shared_ptr<cell> &x ) {
        for (;x!=nil;x->card=x->freq+x->son[L]->card+x->son[R]->card,x= x->p) ;
    }

#define color(x)     ((x)->c)
#define which_son(x) ((x)->p->son[L]==(x)?L:R)
#define flip(x)      ((x)->c^= 1u)

    void rotate( std::shared_ptr<cell> &x, const int i ) {
        auto y= x->son[i^1u];
        assert( y != nil );
        if ( (x->son[i^1u]= y->son[i]) != nil )
            y->son[i]->p= x;
        y->son[i]= x, x->p= y;
        update_upwards(x);
    }

    std::shared_ptr<cell> find_key( const vtype key ) {
        std::shared_ptr<cell> x= root;
        for ( ;x != nil and x->val != key; x= x->son[x->val<key?R:L] ) ;
        return x;
    }

    void fixup( std::shared_ptr<cell> &x ) {
        while ( x != root and color(x) == Black ) {
            auto w= x->p;
            auto i= which_son(x);
            auto y= w->son[i^1u];
            if ( color(y) == Red ) {
                flip(w), flip(y), rotate(w,i);
                continue ;
            }
            assert( color(y) == Black );
            if ( color(y->son[L]) == Black and color(y->son[R]) == Black ) {
                flip(y), x= x->p;
                continue ;
            }
            if ( color(y->son[i]) == Red ) {
                flip(y), flip(y->son[i]), rotate(y,i^1u);
                continue ;
            }
            assert( color(y->son[i^1u]) == Red );
            flip(y->son[i^1u]), y->c= x->p->c, x->p->c= Black, rotate(x->p,i);
            x= root;
        }
        x->c= Black;
    }

    int rank_( vtype key ) {
        int res= 0;
        for ( auto x= root; x != nil; ) {
            if ( key < x->val ) {
                x= x->son[L];
                continue ;
            }
            if ( x->val == key ) {
                res+= x->freq, res+= x->son[L]->freq;
                return res;
            }
            assert( x->val < key );
            res+= x->freq, res+= x->son[L]->freq, x= x->son[R];
        }
        return res;
    }

public:

    rbtree() {
        nil= make_nil(), root= nil;
    }

    void push( const vtype entry ) {
        std::shared_ptr<cell> *hold= &root, x= root, p= nil;
        for (;x != nil;) {
            if ( x->val == entry ) {
                ++x->freq, update_upwards(x);
                return ;
            }
            auto t= x->val < entry ? R:L;
            p= x, hold= &(x->son[t]), x= x->son[t];
        }
        assert( x == nil );
        *hold= x= make_cell(), x->val= entry, x->p= p;
        while ( color(x->p) == Red ) {
            auto g= x->p->p;
            auto i= which_son(x->p);
            auto y= g->son[i^1u];
            if ( color(y) == Red ) {
                flip(x->p), flip(y), x= g;
                continue ;
            }
            if ( which_son(x) == (i^1u) )
                x= x->p, rotate(x,i);
            flip(x->p), flip(g), rotate(g,i^1u), x= root;
        }
        root->c= Black;
    }

    void erase( const vtype key ) {
        auto z= find_key(key);
        if ( z == nil )
            return ;
        assert( z != nil and z->val == key );
        if ( z->son[L] != nil and z->son[R] != nil ) {
            auto y= z->son[R];
            for ( ;y->son[L] != nil; y= y->son[L] ) ;
            assert( y != nil and y->son[L] == nil );
            std::swap(z->val,y->val), std::swap(z->freq,y->freq);
            z= y;
        }
        assert( z->son[L] == nil or z->son[R] == nil );
        auto x= (z->son[L]!=nil?z->son[L]:z->son[R]);
        if ( z == root ) {
            root= x, root->p= nil, root->c= Black;
            return ;
        }
        auto i= which_son(z);
        if ( (z->p->son[i]= x) != nil )
            x->p= z->p;
        update_upwards(z->p);
        if ( color(z) == Black )
            fixup(x);
    }
#undef color
#undef which_son
#undef flip
    // return the number of elements <= key
    int rank( vtype key ) {
        return rank_(key);
    }
};

class range_tree {
#define L(v) ((v)<<1u)
#define R(v) (1u|L(v))
#define THRE (0x40)
    std::vector<std::shared_ptr<rbtree>> tr;
    int n,m;
    std::vector<vtype> data;
    void build( int v, int i, int j ) {
        if ( i < j ) {
            auto k= (i+j)>>1u;
            build(L(v),i,k), build(R(v),k+1,j);
        }
        if ( j-i+1 > THRE ) {
            tr[v]= std::make_shared<rbtree>();
            for ( auto key= data.begin()+i; key <= data.begin()+j; tr[v]->push(*key++) ) ;
        }
    }
    int query_( int v, int i, int j, int qi, int qj, vtype key ) {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            if ( j-i+1 <= THRE ) {
                auto k= key;
                return std::count_if(data.begin()+i,data.begin()+j+1,[k](auto x) {
                    return x > k;
                });
            }
            return (j-i+1)-tr[v]->rank(key);
        }
        auto k= (i+j)>>1;
        return query_(L(v),i,k,qi,qj,key)+query_(R(v),k+1,j,qi,qj,key);
    }
    void update_( int v, int i, int j, int pos, vtype newval, vtype oldval ) {
        if ( pos < i or pos > j )
            return ;
        if ( j-i+1 <= THRE ) {
            data[pos]= newval;
            return ;
        }
        auto k= (i+j)>>1;
        update_(L(v),i,k,pos,newval,oldval), update_(R(v),k+1,j,pos,newval,oldval);
        tr[v]->erase(oldval), tr[v]->push(newval);
    }
public:
    range_tree( const std::vector<vtype> &data ): data(data) {
        n= data.size(), m= 4*n+7;
        tr.resize(m), build(1,0,n-1);
    }
    int query( int qi, int qj, vtype key ) {
        return query_(1,0,n-1,qi,qj,key);
    }
    void modify( int pos, vtype newval ) {
        update_(1,0,n-1,pos,newval,data[pos]);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( int n, qr, i,j,k; (is >> n) and n; ) {
        std::vector<vtype> data(n);
        for ( auto &v: data )
            is >> v;
        std::shared_ptr<range_tree> T= std::make_shared<range_tree>(data);
        vtype val;
        for ( is >> qr; qr--; ) {
            int comm;
            is >> comm;
            switch ( comm ) {
                case 0: is >> i >> val, T->modify(i-1,val); break ;
                default: assert( comm == 1 ); is >> i >> j >> k;
                        os << T->query(i-1,j-1,k) << '\n';
                        break ;
            }
        }
    }
    return 0;
}
    