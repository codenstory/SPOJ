/**
 * RACETIME
 * TOPIC: 3-sided range counting with updates, range trees, red-black trees, order statistics
 * status: Accepted
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
        assert( x != nil );
        assert( x->son[i^1u] != nil );
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
        //assert( x != nil );
        for (;x != root and color_of(x) == Black;) {
            auto i= which_son(x);
            auto y= x->p->son[i^1u];
            if ( color_of(y) == Red ) {
                //assert( y->son[L] != nil and y->son[R] != nil );
                flip(x->p), flip(y), rotate(x->p,i);
                continue ;
            }
            assert( y != nil );
            if ( color_of(y->son[L]) == Black and color_of(y->son[R]) == Black ) {
                flip(y), x= x->p;
                continue ;
            }
            if ( color_of(y->son[i^1u]) == Black ) {
                flip(y), flip(y->son[i]), rotate(y,i^1u);
                continue ;
            }
            y->color= x->p->color, x->p->color= Black, flip(y->son[i^1u]);
            rotate(x->p,i), x= root;
        }
        x->color= Black;
    }

    void erase( std::shared_ptr<cell> &z ) {

        if ( z == nil )
            return ;

        --z->freq, propagate(z);
        if ( z->freq ) return ;

        auto to_reset= z;

        auto y= (z->son[R]!=nil and z->son[L]!=nil ? z->son[R]:z);

        if ( y != z ) {
            for ( ;y->son[L] != nil; y= y->son[L] ) ;
            std::swap(z->val,y->val), std::swap(z->freq,y->freq);
            propagate(y), to_reset= y;
        }
        auto x= (y->son[L]!=nil)?y->son[L]:y->son[R];
        if ( (x->p= y->p) != nil )
            y->p->son[which_son(y)]= x;
        else root= x;
        propagate(x);
        if ( color_of(y) == Black )
            fixup(x);
        to_reset.reset();
    }

    std::shared_ptr<cell> root, nil;

    std::shared_ptr<cell> find( T value ) const {
        auto x= root;
        for ( ;x != nil and x->val != value; )
            x= x->son[x->val < value?R:L];
        return x;
    }

    size_t f( const std::shared_ptr<cell> &r, T value ) const {
        if ( r == nil )
            return 0;
        size_t ans= 0;
        if ( r->val <= value )
            ans+= r->val;
        return f(r->son[L],value)+f(r->son[R],value)+ans;
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
            else {
                if ( which_son(x) != i )
                    x= x->p, rotate(x,i);
                else
                    flip(g), flip(x->p), rotate(g,i^1u), x= root;
            }
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

    size_t bf_query( T value ) const {
        return f(root,value);
    }

};

template<typename T>
class rt {
private:
    std::vector<T> data;
    std::vector<rb_tree<T>> S;
    int n,m;
    static const int LIM= 5000;

#define L(v) ((v)<<1)
#define R(v) (1|L(v))

    void build( int v, int i, int j ) {
        if ( i == j ) {
            //S[v]= rb_tree<T>(data.begin()+i,data.begin()+i+1);
            return ;
        }
        auto k= (i+j)>>1;
        build(L(v),i,k), build(R(v),k+1,j);
        if ( j-i+1 > LIM )
            S[v]= rb_tree<T>(data.begin()+i,data.begin()+j+1);
    }

    size_t query( int v, int i, int j, int qi, int qj, T val ) const {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            if ( j-i+1 <= LIM ) {
                return std::count_if(data.begin()+i,data.begin()+j+1,[val](auto x) {
                    return x <= val;
                });
            }
            return S[v].rank(val);
        }
        auto k= (i+j)>>1;
        return query(L(v),i,k,qi,qj,val)+query(R(v),k+1,j,qi,qj,val);
    }

    size_t bf_query( int v, int i, int j, int qi, int qj, T val ) const {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            return S[v].bf_query(val);
        }
        auto k= (i+j)>>1;
        return bf_query(L(v),i,k,qi,qj,val)+bf_query(R(v),k+1,j,qi,qj,val);
    }

    void update( int v, int i, int j, int pos, T oldval, T newval ) {
        if ( pos < i or pos > j ) return ;
        if ( i == j ) {
            assert( i == pos );
            assert( data[pos] == oldval );
            data[pos]= newval;//, S[v]= rb_tree<T>(data.begin()+i,data.begin()+i+1);
            return ;
        }
        auto k= (i+j)>>1;
        update(L(v),i,k,pos,oldval,newval), update(R(v),k+1,j,pos,oldval,newval);
        if ( j-i+1 > LIM )
            S[v].erase(oldval), S[v].insert(newval);
    }

#undef L
#undef R

public:
    rt() {};
    rt( const std::vector<T> &input ): data(input) {
        this->n= input.size(), m= 4*n+7;
        S.resize(m), build(1,0,n-1);
    }
    virtual void update( int pos, T newVal ) {
        update(1,0,n-1,pos-1,data[pos-1],newVal);
    }
    virtual size_t query( int qi, int qj, T val ) const {
        return query(1,0,n-1,qi-1,qj-1,val);
    }
    virtual size_t bf_query( int qi, int qj, T val ) const {
        size_t ans= 0;
        for ( int i= qi-1; i <= qj-1; ++i )
            if ( data[i] <= val )
                ++ans;
        return ans;
    }
    virtual ~rt() {};
};

template<typename T>
class bfsol: public rt<T> {
private:
    std::vector<T> data;
public:
    explicit bfsol(const std::vector<T> &input): data(input) {
    }

    void update(int pos, T newVal) override {
        data[pos-1]= newVal;
    }

    size_t query(int qi, int qj, T val) const override {
        return std::count_if(data.begin()+qi-1,data.begin()+qj,[val](auto x) {
            return x <= val;
        });
    }

    size_t bf_query(int qi, int qj, T val) const override {
        return 0;
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
        //auto T= std::make_unique<bfsol<i64>>(c);
        for ( std::string command; (is >> command); ) {
            if ( command[0] == 'M' ) {
                is >> pos >> newval;
                T->update(pos,newval);
            } else {
                assert( command[0] == 'C' );
                is >> p >> q >> val;
                auto a1= T->query(p,q,val);
                //auto a2= T->bf_query(p,q,val);
                //assert( a1 == a2 );
                //os << "Passed" << std::endl;
                os << a1 << '\n';
            }
        }
    }
    return 0;
}
