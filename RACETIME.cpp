/**
 * RACETIME
 * TOPIC: 3-sided range counting with updates, range trees, red-black trees, order statistics
 * status:
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

int n,qr;
std::vector<i64> c;

template<typename T= i64>
class rb_tree {

    enum { L, R };
    using t_direction= unsigned char;
    using t_color= bool;
    static const t_color Red= 0,
                         Black= 1;

    struct cell {
        T val;
        size_t freq, card;
        t_color color;
        std::shared_ptr<cell> son[2], p;
        cell() { son[L]= son[R]= p= nullptr, card= freq= 0, color= Red; }
    };

    std::shared_ptr<cell> make_nil() {
        auto x= std::make_shared<cell>();
        x->son[L]= x->son[R]= x->p= x, x->freq= x->card= 0, x->color= Black;
        return x;
    }

    std::shared_ptr<cell> make_cell() {
        auto x= std::make_shared<cell>();
        x->son[L]= x->son[R]= x->p= nil;
        return x;
    }

    t_direction which_son( const std::shared_ptr<cell> &it ) const {
        assert( it->p != nil );
        return it->p->son[L]==it?L:R;
    }

    void set_card( std::shared_ptr<cell> &x ) {
        x->card= x->son[L]->card+x->son[R]->card+x->freq;
    }

    void flip( std::shared_ptr<cell> &x ) {
        x->color^= 1u;
    }

    void propagate( std::shared_ptr<cell> &x ) {
        for ( auto z= x; z != nil; set_card(z), z= z->p ) ;
    }

    t_color color_of( const std::shared_ptr<cell> &x ) const {
        return x->color;
    }

    void rotate( std::shared_ptr<cell> &x, t_direction i ) {
        assert( i == L or i == R );
        assert( x != nil and x->son[i^1u] != nil );
        auto y= x->son[i^1u];
        if ( (x->son[i^1u]= y->son[i]) != nil )
            y->son[i]->p= x;
        if ( (y->p= x->p) != nil )
            x->p->son[which_son(x)]= y;
        else {
            assert( x == root );
            root= y;
            assert( root->p == nil );
        }
        y->son[i]= x, x->p= y, propagate(x);
    }

    void fixup( std::shared_ptr<cell> &x ) {
        for (;x != root and color_of(x) == Black;) {
            auto i= which_son(x);
            auto y= x->p->son[i^1u];
            if ( color_of(y) == Red ) {
                flip(x->p), flip(y), rotate(x->p,i);
                continue ;
            }
            if ( color_of(y->son[i^1u]) == Red ) {
                y->son[i^1u]->color= Black, y->color= x->p->color, x->p->color= Black;
                rotate(x->p,i), x= root;
                continue ;
            }
            if ( color_of(y->son[i]) == Red ) {
                flip(y), flip(y->son[i]), rotate(y,i^1u);
                continue ;
            }
            if ( color_of(x->p) == Black ) {
                flip(y), x= x->p;
                continue ;
            }
            rotate(x->p,i), x= root;
        }
        x->color= Black;
    }

    void erase( std::shared_ptr<cell> &item ) {

        if ( item == nil )
            return ;

        --item->freq, propagate(item);
        if ( item->freq ) return ;

        auto to_reset= item;

        auto z= (item->son[R]!=nil?item->son[R]:item->son[L]);
        if ( z != nil and z == item->son[R] ) {
            auto y= z;
            assert( y != nil );
            for ( ;y->son[L] != nil; y= y->son[L] ) ;
            assert( y != nil and y->son[L] == nil );
            y->p->son[L]= y->son[R], y->son[R]->p= y->p;
            propagate(y->p);
            item->freq= y->freq, item->val= y->val;
            propagate(item);
            z= y->son[R], to_reset= y;
        } else {
            if ( item == root )
                z->p= nil, root= z, propagate(z);
            else {
                auto i= which_son(item);
                item->p->son[i]= z, z->p= item->p, propagate(z->p);
            }
        }
        if ( color_of(to_reset) == Black )
            fixup(z);
        to_reset.reset();
    }

    std::shared_ptr<cell> root, nil;

    std::shared_ptr<cell> find( T value ) const {
        auto x= root;
        for ( ;x != nil and x->val != value; )
            x= x->son[x->val < value?R:L];
        return x;
    }


public:

    rb_tree() {
        nil= make_nil(), root= nil;
    }

    template<typename RandIt>
    rb_tree( RandIt i, RandIt j ) {
        nil= make_nil(), root= nil;
        std::for_each(i,j,[this](auto it) {
            insert((T)(it));
        });
    }

    void insert( T entry ) {
        std::shared_ptr<cell> x= root, prev= nil, *hold= &root;
        for ( t_direction t; x != nil; prev= x, hold= &x->son[t=(x->val<entry?R:L)], x=x->son[t])
            if ( x->val == entry ) {
                ++x->freq, propagate(x);
                return ;
            }

        *hold= x= make_cell();
        (*hold)->val= entry, (*hold)->freq= (*hold)->card= 1, (*hold)->p= prev;
        assert( x == root or x->p->son[which_son(x)] == x );
        propagate(x);

        for ( ;x != root and color_of(x->p) == Red; ) {
            assert( x->p != nil and x->p->p != nil );
            auto i= which_son(x->p);
            auto g= x->p->p, w= g->son[i^1u];
            if ( color_of(w) == Red )
                flip(g), flip(w), flip(x->p), x= g;
            else flip(g), flip(x->p), rotate(g,i^1), x= root;
        }
        root->color= Black;
    }

    void erase( T val ) {
        auto x= find(val);
        assert( x != nil );
        erase(x);
    }

    size_t rank( T value ) const {
        size_t ans= 0;
        for ( auto x= root; x != nil; ) {
            if ( x->val == value )
                return ans+= (x->son[L]->card+x->freq);
            auto t= x->val<value?R:L;
            if ( t == R )
                ans+= x->son[L]->card, ans+= x->freq;
            x= x->son[t];
        }
        return ans;
    }

};

template<typename T>
class rt {
private:
    std::vector<T> data;
    std::vector<rb_tree<T>> S;
    int n,m;

#define L(v) ((v)<<1)
#define R(v) (1|L(v))

    void build( int v, int i, int j ) {
        if ( i == j ) {
            S[v]= rb_tree<T>(data.begin()+i,data.begin()+i+1);
            return ;
        }
        auto k= (i+j)>>1;
        build(L(v),i,k), build(R(v),k+1,j);
        S[v]= rb_tree<T>(data.begin()+i,data.begin()+j+1);
    }

    size_t query( int v, int i, int j, int qi, int qj, T val ) const {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            return S[v].rank(val);
        }
        auto k= (i+j)>>1;
        return query(L(v),i,k,qi,qj,val)+query(R(v),k+1,j,qi,qj,val);
    }

    void update( int v, int i, int j, int pos, T oldval, T newval ) {
        if ( pos < i or pos > j ) return ;
        if ( i == j ) {
            assert( i == pos );
            assert( data[pos] == oldval );
            data[pos]= newval, S[v]= rb_tree<T>(data.begin()+i,data.begin()+i+1);
            return ;
        }
        auto k= (i+j)>>1;
        update(L(v),i,k,pos,oldval,newval), update(R(v),k+1,j,pos,oldval,newval);
        S[v].erase(oldval), S[v].insert(newval);
    }

#undef L
#undef R

public:
    explicit rt( const std::vector<T> &input ): data(input) {
        this->n= input.size(), m= 4*n+7;
        S.resize(m), build(1,0,n-1);
    }
    void update( int pos, T newVal ) {
        update(1,0,n-1,pos-1,data[pos-1],newVal);
    }
    size_t query( int qi, int qj, T val ) const {
        return query(1,0,n-1,qi-1,qj-1,val);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,pos,p,q;
    i64 newval,val;
    for (;(is >> n >> qr) and n;) {
        for ( c.resize(n), i= 0; i < n; is >> c[i++] ) ;
        auto T= std::make_unique<rt<i64>>(c);
        for ( std::string command; (is >> command); ) {
            if ( command[0] == 'M' ) {
                is >> pos >> newval;
                T->update(pos,newval);
            } else {
                assert( command[0] == 'C' );
                is >> p >> q >> val;
                os << T->query(p,q,val) << std::endl;
            }
        }
    }
    return 0;
}
