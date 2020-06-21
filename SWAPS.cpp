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
        unsigned char c= Red;
        vtype val{};
        holder son[2]= {nullptr,nullptr}, p= nullptr;
        size_t card{},
               freq{};
    };

#define color(x) ((x)->c)
#define which_son(x) ((x)->p->son[L] == (x)?L:R)
#define flip(x) ((x)->c^=1u)

    holder root, nil;
    size_t sz{};
    holder make_nil() {
        auto x= std::make_shared<cell>();
        x->c= Black, x->son[L]= x->son[R]= x->p= x, x->card= x->freq= 0;
        return x;
    }
    holder make_cell( vtype val ) {
        auto x= std::make_shared<cell>();
        x->c= Red, x->son[L]= x->son[R]= x->p= nil, x->val= val;
        x->freq= x->card= 0;
        return x;
    }
    void propagate( holder &x ) {
        for (;x != nil; x->card= x->freq+x->son[L]->card+x->son[R]->card, x= x->p ) ;
    }
    void rotate( holder &x, t_dir t ) {
        assert( t == L or t == R );
        assert( x != nil );
        auto y= x->son[t^1u];
        assert( y != nil );
        if ( (x->son[t^1u]= y->son[t]) != nil )
            y->son[t]->p= x;
        if ( (y->p= x->p) == nil )
            root= y;
        else x->p->son[which_son(x)]= y;
        y->son[t]= x, x->p= y;
        propagate(x);
    }

    holder find( vtype val ) {
        auto x= root;
        for ( ;x != nil and x->val != val; x= x->son[x->val<val?R:L] ) ;
        assert( x == nil or x->val == val );
        return x;
    }

    void fixup( holder &x ) {
        while ( x != root and color(x) != Black ) {
            auto i= which_son(x);
            auto y= x->p->son[i^1];
            if ( color(y) == Red ) {
                flip(x->p), flip(y), rotate(x->p,i);
                continue ;
            }
            assert( color(y) == Black );
            if ( color(y->son[L]) == Black and color(y->son[R]) == Black ) {
                flip(y), x= x->p;
                continue ;
            }
            if ( color(y->son[i]) == Red ) {
                flip(y->son[i]), flip(y), rotate(y,i^1u);
                continue ;
            }
            assert( color(y->son[i^1u]) == Red );
            y->son[i^1u]->c= Black, y->c= x->p->c, x->p->c= Black, rotate(x->p,i), x= root;
        }
        x->c= Black;
    }

	void dump( holder &x ) {
		if ( x != nil ) {
			dump(x->son[L]), std::cerr << x->val << " ", dump(x->son[R]);
			//std::cerr << std::endl;
		}
	}

public:
    bool find_key( vtype val ) {
        auto x= find(val);
        return x!=nil;
    }
    size_t rank( vtype key ) {
        size_t rnk= 0;
        for ( auto x= root; x != nil; ) {
            if ( x->val > key )
                x= x->son[L];
            else if ( x->val < key )
                rnk+= x->freq, rnk+= x->son[L]->card, x= x->son[R];
            else {
                assert(x->val == key);
                rnk += x->freq, rnk += x->son[L]->card;
                break;
            }
        }
        return rnk;
    }

    size_t exclusive_rank( vtype key ) {
        size_t rnk= 0;
        for ( auto x= root; x != nil; ) {
            if ( x->val > key )
                x= x->son[L];
            else if ( x->val < key )
                rnk+= x->freq, rnk+= x->son[L]->card, x= x->son[R];
            else {
                assert(x->val == key);
                rnk += x->son[L]->card;
                break;
            }
        }
        return rnk;
    }

    void push( vtype val ) {
        holder x= root, *hold= &root, p= nil;
        for ( ++sz; x != nil and x->val != val; ) {
            auto t= x->val<val ? R:L;
            p= x, hold= &(x->son[t]), x= x->son[t];
        }
        if ( x != nil ) {
            assert( x->val == val );
            assert( x->freq >= 1 );
            ++x->freq, propagate(x);
            return ;
        }
        *hold= x= make_cell(val), x->p= p, x->freq= 1;
        assert( x->val == val and x->c == Red );
        for ( propagate(x); x != root and color(x->p) == Red; ) {
            auto g= x->p->p;
            assert( g != nil );
            auto i= which_son(x->p);
            auto y= g->son[i^1u];
            if ( color(y) == Red ) {
                assert( color(g) == Black );
                flip(g), flip(x->p), flip(y), x= g;
                continue ;
            }
            if ( which_son(x) == (i^1u) ) {
                x = x->p, rotate(x, i);
                continue ;
            }
            flip(x->p), flip(g), rotate(g,i^1u);
            break ;
        }
        root->c= Black;
    }

    bool erase( vtype val ) {
        auto z= find(val);
        if ( z == nil ) {
            std::cerr << "Cannot find the key " << val << std::endl;
            std::cerr << "tree size " << sz << std::endl;
            return false;
        }
		assert( sz >= 1 and z->freq >= 1 );
        --sz, --z->freq, propagate(z);
        if ( z->freq )
            return true ;
        assert( z->freq == 0 );
        if ( z->son[L] != nil and z->son[R] != nil ) {
            auto y= z->son[R];
            for ( ;y->son[L] != nil; y= y->son[L] ) ;
            std::swap(z->val,y->val), std::swap(z->freq,y->freq);
            propagate(y), z= y;
        }
        assert( z->son[L] == nil or z->son[R] == nil );
        auto x= z->son[L]==nil?z->son[R]:z->son[L];
        if ( (x->p= z->p) == nil ) {
            root= x;
        } else {
            z->p->son[which_son(z)]= x;
        }
        propagate(x);
        if ( color(z) == Black )
            fixup(x);
        return true ;
    }

	void display() {
		dump(root);
		std::cerr << "\n";
	}

	size_t size() const {
		return sz;
	}

#undef color
#undef which_son
#undef flip

    explicit rbtree() {
        root= nil= make_nil();
    }

};

template<typename vtype>
class rangetree {
#define L(v) ((v)<<1u)
#define R(v) (1u|L(v))
//#define LIMIT (0x400)
#define LIMIT (0)

    using rbtree_holder= std::shared_ptr<rbtree<vtype>>;
    std::vector<rbtree_holder> tr;
    size_t m,n;
    std::vector<vtype> data;

    void build( int v, int i, int j ) {
        assert( j-i+1 > 0 );
        assert( v < tr.size() );
        if ( j-i+1 > LIMIT ) {
            if ( i == j ) {
                tr[v]= std::make_shared<rbtree<vtype>>();
                for ( auto l= i; l <= j; ++l )
                    tr[v]->push(data[l]);
                assert( tr[v]->find_key(data[i]) );
                return ;
            }
            assert( i < j );
            auto k= (i+j)>>1u;
            build(L(v),i,k), build(R(v),k+1,j);
            tr[v]= std::make_shared<rbtree<vtype>>();
            for ( auto l= i; l <= j; ++l ) {
                tr[v]->push(data[l]);
                assert( tr[v]->find_key(data[l]) );
            }
			for ( auto l= i; l <= j; ++l )
				assert( tr[v]->find_key(data[l]) );
        } else {
			//assert( false );
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
        if ( i == j ) {
            assert( data[pos] == old );
            auto ok= tr[v]->erase(old);
            assert( ok );
            tr[v]->push(newval);
            data[pos]= newval;
            return ;
        }
        assert( i < j );
        auto k= (i+j)>>1u;
        update(L(v),i,k,pos,old,newval), update(R(v),k+1,j,pos,old,newval);
        auto ok= tr[v]->erase(old);
        assert( ok );
        tr[v]->push(newval);
		std::cerr << tr[v]->size() << " " << "After pushing " << newval << " in place of " << old << std::endl;
		tr[v]->display();
		std::cerr << std::endl;
    }

    inline size_t bf1( int i, int j, vtype key ) {
        auto k= key;
        return static_cast<size_t>(std::count_if(data.begin()+i,data.begin()+j+1,[k](auto x) {
            return x > k;
        }));
    }

    inline size_t bf2( int i, int j, vtype key ) {
        auto k= key;
        return static_cast<size_t>(std::count_if(data.begin()+i,data.begin()+j+1,[k](auto x) {
            return x < k;
        }));
    }

    size_t query1( int v, int i, int j, int qi, int qj, vtype key ) {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            if ( j-i+1 <= LIMIT ) {
                return bf1(i,j,key);
            }
            auto res= tr[v]->rank(key);
            assert( j-i+1 >= res );
            return (j-i+1)-res;
        }
        auto k= (i+j)>>1u;
        return query1(L(v),i,k,qi,qj,key)+query1(R(v),k+1,j,qi,qj,key);
    }

    size_t query2( int v, int i, int j, int qi, int qj, vtype key ) {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            if ( j-i+1 <= LIMIT ) {
                return bf2(i,j,key);
            }
            auto res= tr[v]->exclusive_rank(key);
            assert( j-i+1 >= res );
            return res;
        }
        auto k= (i+j)>>1u;
        return query2(L(v),i,k,qi,qj,key)+query2(R(v),k+1,j,qi,qj,key);
    }

public:

    rangetree( const std::vector<vtype> &input ) {
        data= input, n= data.size(), m= 4*n+7, tr.resize(m,nullptr);
        build(1,0,n-1);
    }
    void change_val( int pos, vtype newval ) {
        auto old_val= data[pos];
		std::cerr << "Old val: " << old_val << std::endl;
        if ( newval != old_val )
            update(1,0,n-1,pos,old_val,newval);
    }
    size_t countingAbove( int qi, int qj, vtype key ) {
        return query1(1,0,n-1,qi,qj,key);
    }
    size_t countingBelow( int qi, int qj, vtype key ) {
        return query2(1,0,n-1,qi,qj,key);
    }
#undef L
#undef R
};

using rt= rangetree<int>;

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
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
    for ( int i= 0; i < n; ++i )
        ans+= T->countingAbove(0,i-1,data[i])+T->countingBelow(i+1,n-1,data[i]);
    assert( 0 == (ans&1) );
    ans>>= 1;
    //std::cerr << "initially: " << ans << std::endl;
    for ( is >> qr; qr--; ) {
        int x,y;
        is >> x >> y;
        assert( 1 <= x and x <= n );
        --x;
        auto old= T->countingAbove(0,x-1,data[x])+T->countingBelow(x+1,n-1,data[x]);
        //std::cerr << "old= " << old << std::endl;
        T->change_val(x,y);
        data[x]= y;
        auto neu= T->countingAbove(0,x-1,data[x])+T->countingBelow(x+1,n-1,data[x]);
        //std::cerr << "new= " << neu << std::endl;
        ans= ans+neu-old;
        os << ans << '\n';
    }
    return 0;
}
