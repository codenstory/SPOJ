/**
 * KQUERY2
 * TOPIC: orthogonal range counting, Red-Black trees, order statistics, ranks
 * status: Accepted
 */
#include <bits/stdc++.h>
using vtype= int;
using namespace std::chrono;

class mytimer {
private:
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    std::string header;
public:
    mytimer( std::string action ): header(std::move(action)) {
    }
    ~mytimer() {
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        std::cout << header << " took " << time_span.count() << " seconds\n";
    }
};

class rbtree {
private:
    enum { L= 0, R= 1 };
    enum { Red= L, Black= R };

    struct cell {
        std::shared_ptr<cell> son[2],p;
        vtype val{};
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

    void update_upwards( std::shared_ptr<cell> &z ) {
        for (auto x= z;x!=nil;x->card=x->freq+x->son[L]->card+x->son[R]->card,x= x->p) ;
        assert( nil->card == 0 and nil->freq == 0 );
    }

#define color(x)     ((x)->c)
#define which_son(x) ((x)->p->son[L]==(x)?L:R)
#define flip(x)      ((x)->c^= 1u)

    void rotate( std::shared_ptr<cell> &x, const unsigned i ) {
        assert( i == L or i == R );
        auto y= x->son[i^1u];
        assert( y != nil );
        if ( (x->son[i^1u]= y->son[i]) != nil )
            y->son[i]->p= x;
        if ( (y->p= x->p) == nil )
            root= y;
        else
            x->p->son[which_son(x)]= y;
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
            auto i= which_son(x);
            auto w= x->p, y= w->son[i^1u];
            if ( color(y) == Red ) {
                flip(w), flip(y), rotate(w,i);
                continue ;
            }
            assert( color(y) == Black );
            if ( color(y->son[L]) == Black and color(y->son[R]) == Black ) {
                flip(y), x= w;
                continue ;
            }
            if ( color(y->son[i]) == Red ) {
                flip(y), flip(y->son[i]), rotate(y,i^1u);
                continue ;
            }
            assert( color(y->son[i^1u]) == Red );
            flip(y->son[i^1u]), y->c= w->c, w->c= Black, rotate(w,i);
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
                res+= x->freq, res+= x->son[L]->card;
                return res;
            }
            assert( x->val < key );
            res+= x->freq, res+= x->son[L]->card, x= x->son[R];
        }
        return res;
    }

public:

    rbtree() {
        nil= make_nil(), root= nil;
    }

    void push( const vtype entry ) {
        //mytimer tm("push "+std::to_string(entry));
        std::shared_ptr<cell> *hold= &root, x= root, p= nil;
        for (;x != nil;) {
            if ( x->val == entry ) {
                assert( x->freq >= 1 );
                ++x->freq, update_upwards(x);
                return ;
            }
            auto t= x->val<entry?R:L;
            p= x, hold= &(x->son[t]), x= x->son[t];
        }
        assert( x == nil );
        *hold= x= make_cell(), x->val= entry, x->p= p;
        assert( x->c == Red );
        ++x->freq, update_upwards(x);
        while ( x != root and color(x->p) == Red ) {
            assert( color(x) == Red );
            auto i= which_son(x->p);
            auto g= x->p->p, y= g->son[i^1u];
            assert( g != nil );
            if ( color(y) == Red ) {
                assert( color(g) == Black );
                flip(x->p), flip(y), flip(g), x= g;
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

    bool erase( const vtype key ) {
        auto z= find_key(key);
        if ( z == nil )
            return false;
        --z->freq, update_upwards(z);
        if ( z->freq ) {
            assert( z->freq >= 1 );
            return true;
        }
        assert( z != nil and z->val == key and not z->freq );
        if ( z->son[L] != nil and z->son[R] != nil ) {
            auto y= z->son[R];
            for ( ;y->son[L] != nil; y= y->son[L] ) ;
            assert( y != nil and y->son[L] == nil );
            std::swap(z->val,y->val), std::swap(z->freq,y->freq);
            update_upwards(y), z= y;
        }
        assert( z->son[L] == nil or z->son[R] == nil );
        auto x= (z->son[L]!=nil?z->son[L]:z->son[R]);
        if ( (x->p= z->p) == nil ) {
            root= x, root->p= nil, root->c= Black;
            update_upwards(root);
            return true;
        }
        assert( z->p != nil );
        auto i= which_son(z);
        z->p->son[i]= x;
        update_upwards(x);
        if ( color(z) == Black )
            fixup(x);
        z.reset();
        return true ;
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
#define THRE (0x400)
    std::vector<std::shared_ptr<rbtree>> tr;
    int n,m;
    std::vector<vtype> data;

    void build( int v, int i, int j ) {
        if ( i < j and (j-i+1) > THRE ) {
            auto k= (i+j)>>1u;
            build(L(v),i,k), build(R(v),k+1,j);
        }
        if ( j-i+1 > THRE ) {
            tr[v]= std::make_shared<rbtree>();
            for ( auto key= data.begin()+i; key <= data.begin()+j; tr[v]->push(*key++) ) ;
        } else tr[v]= nullptr;
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
            assert( i <= pos and pos <= j );
            data[pos]= newval;
            return ;
        }
        auto k= (i+j)>>1;
        update_(L(v),i,k,pos,newval,oldval), update_(R(v),k+1,j,pos,newval,oldval);
        auto ok= tr[v]->erase(oldval);
        assert( ok );
        tr[v]->push(newval);
    }

public:

    range_tree( const std::vector<vtype> &data ): data(data) {
        n= data.size(), m= 4*n+7;
        tr.resize(m);
        {
            //mytimer t("building the range tree");
            build(1, 0, n - 1);
        }
    }

    int query( int qi, int qj, vtype key ) {
        return query_(1,0,n-1,qi,qj,key);
    }

    void modify( int pos, vtype newval ) {
        assert( 0 <= pos and pos < n );
        if ( newval != data[pos] )
            update_(1,0,n-1,pos,newval,data[pos]);
    }

    int bf( int qi, int qj, vtype key ) {
        auto k= key;
        return std::count_if(data.begin()+qi,data.begin()+qj+1,[k](auto x) {
            return x > k;
        });
    }

};

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;

    //mytimer tm("whole_thing");

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
                        auto ans= T->query(i-1,j-1,k);
                        /*
                        auto bf= T->bf(i-1,j-1,k);
                        if ( ans != bf ) {
                            std::cerr << "ans: " << ans << ", and bf= " << bf << std::endl;
                        } else std::cerr << "OK" << std::endl;
                        assert( ans == bf );
                         */
                        os << ans << '\n';
                        break ;
            }
        }
    }

    return 0;
}
    