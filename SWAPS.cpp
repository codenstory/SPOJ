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

    void fixup( holder &x ) {
        while ( x != root and color(x) != Black ) {
            auto i= which_son(x);
            auto y= x->p->son[i];
            if ( color(y) == Red ) {
                flip(x->p), flip(y), rotate(x->p,i);
                continue ;
            }
            assert( color(y) == Black );
            if ( color(y(L)) == Red and color(y(R)) == Red ) {
                flip(y(L)), flip(y(R)), flip(y), x= x->p;
                continue ;
            }
            if ( color(y(i)) == Red ) {
                flip(y(i)), flip(y), rotate(y,i^1);
                continue ;
            }
            assert( color(y(i^1)) == Red );
            y(i^1)->c= Black, y->c= x->p->c, x->p->c= Black, rotate(x->p,i), x= root;
        }
        x->c= Black;
    }

public:

    size_t rank( vtype key ) {
        size_t rnk= 0;
        for ( auto x= root; x != nil; ) {
            if ( x->val > key )
                x= x(L);
            else if ( x->val < key ) {
                rnk+= x->freq, rnk+= x(L)->freq;
                x= x(R);
            } else {
                assert(x->val == key);
                rnk += x->freq, rnk += x(L)->freq;
                break;
            }
        }
        return rnk;
    }

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
            propagate(y), z= y;
        }
        assert( z(L) == nil or z(R) == nil );
        auto x= z(L)==nil?z(R):z(L);
        if ( z == root ) {
            root= x;
        } else {
            if ( (z->p->son[which_son(z)]= x) != nil )
                x->p= z->p;
        }
        propagate(x);
        if ( color(z) == Black )
            fixup(x);
        return true ;
    }
};

template<typename vtype>
class rangetree {
#define L(v) ((v)<<1u)
#define R(v) (1|L(v))
#define LIMIT (0x400)
    using rbtree_holder= std::shared_ptr<rbtree<vtype>>;
    std::vector<rbtree_holder> tr;
    size_t m,n;
    std::vector<vtype> data;
    void build( int v, int i, int j ) {
        if ( j-i+1 >= LIMIT ) {
            auto k= (i+j)>>1u;
            build(L(v),i,k), build(R(v),k+1,j);
            tr[v]= std::make_shared<rbtree<vtype>>();
            for ( auto l= i; l <= j; ++l )
                tr[v]->push(data[l]);
        }
    }
    void update( int v, int i, int j, int pos, vtype old, vtype newval ) {
        if ( i > pos or pos > j )
            return ;
        assert( i <= pos and pos <= j );
        if ( j-i+1 <= LIMIT ) {
            assert( data[pos] == old );
            data[pos]= newval;
            return ;
        }
        auto k= (i+j)>>1u;
        update(L(v),i,k,pos,old,newval), update(R(v),k+1,j,pos,old,newval);
        auto ok= tr[v]->erase(old);
        assert( ok );
        tr[v]->push(newval);
    }
    inline size_t bf( int i, int j, vtype key ) {
        auto k= key;
        return static_cast<size_t>(std::count_if(data.begin()+i,data.begin()+j+1,[k](auto x) {
            return x > k;
        }));
    }
    size_t query( int v, int i, int j, int qi, int qj, vtype key ) {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            if ( j-i+1 < LIMIT ) {
                return bf(i,j,key);
            }
            auto res= tr[v]->rank(key);
            assert( j-i+1 >= res );
            return (j-i+1)-res;
        }
        auto k= (i+j)>>1u;
        return query(L(v),i,k,qi,qj,key)+query(R(v),k+1,j,qi,qj,key);
    }
public:
    rangetree( const std::vector<vtype> &input ) {
        data= input, n= data.size(), m= 4*n+7, tr.resize(m,nullptr);
        build(1,0,n-1);
    }
    void change_val( int pos, vtype newval ) {
        auto old_val= data[pos];
        update(1,0,n-1,pos,old_val,newval);
    }
    size_t counting( int qi, int qj, vtype key ) {
        return query(1,0,n-1,qi,qj,key);
    }
#undef L
#undef R
};

using rt= rangetree<int>;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    int n,qr;
    size_t ans= 0;
    is >> n;
    std::vector<int> data(n);
    for ( auto &v: data )
        is >> v;
    std::shared_ptr<rt> T= std::make_shared<rt>(data);
    ans= 0;
    for ( int i= 1; i < n; ++i )
        ans+= T->counting(0,i-1,data[i]);
    for ( is >> qr; qr--; ) {
        int x,y;
        is >> x >> y;
        auto old= T->counting(0,x-1,data[x-1]);
        T->change_val(x-1,y);
        data[x-1]= y;
        auto neu= T->counting(0,x-1,data[x-1]);
        ans-= old, ans+= neu;
        os << ans << '\n';
    }
    return 0;
}