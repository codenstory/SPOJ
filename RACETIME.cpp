/**
 * RACETIME
 * TOPIC: 3-sided range counting with updates, range trees, red-black trees, order statistics
 * status:
 */
#include <bits/stdc++.h>
#define N (1<<17)
enum { L, R };
enum { Red, Black };
using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

int n,qr;
std::vector<i64> c;

template<typename T= i64>
class rb_tree {

    struct cell {
        std::shared_ptr<cell> son[2], p;
        int freq;
        T val;
        char color, card;
        cell() { son[L]= son[R]= p= nullptr, card= freq= 0, color= Red; }
    };

    int which_son( std::shared_ptr<cell> &it ) const {
        assert( it->p != nil );
        return it->p->son[L]==it?L:R;
    }

    void rotate( std::shared_ptr<cell> &x, int dir ) {
        assert( dir == L or dir == R );
        assert( x and x->son[dir^1] );
        auto y= x->son[dir^1];
        if ( (x->son[dir^1]= y->son[dir]) != nil )
            y->son[dir]->p= x;
        if ( (y->p= x->p) != nil )
            x->son[which_son(x)]= y;
        y->son[dir]= x;
    }

    //TODO: update and maintain cardinalities
    //TODO: maintain the nil-node
    void insert_( T entry ) {
        std::shared_ptr<cell> x= root, *hold= &root;
        for ( ;x; ) {
            if ( x->val == entry ) {
                ++x->freq;
                return ;
            }
            auto t= x->val < entry ? R:L;
            hold= &x->son[t], x= x->son[t];
        }

        *hold= x= std::make_shared<cell>();
        x->val= entry;

        for ( ;x != root and x->p->color == Red; ) {
            auto g= x->p->p;
            auto i= which_son(x->p);
            auto w= g->son[i^1];
            if ( w->color == Red ) {
                g->color^= 1, w->color^= 1, x->p->color^= 1, x= g;
                continue ;
            }
            if ( w->son[L]->color == Red and w->son[R]->color == Red ) {
                 w->son[L]->color^= 1, w->son[R]->color^= 1, w->color^= 1;
                 continue ;
            }
            if ( w->son[i]->color == Red ) {
                rotate(w,i^1);
                continue ;
            }
            if ( w->son[i]->color == Black ) {
                w->color^= 1, w->son[i^1]->color^= 1, rotate(w,i^1);
                continue ;
            }
        }
        root->color= Black;
    }

    void fixup( std::shared_ptr<cell> &x ) {
        for ( ;x != root and x->color == Black; ) {
            auto i= which_son(x);
            auto y= x->p->son[i^1];
            if ( y->color == Red ) {
                x->p->color^= 1, y->color^= 1, rotate(x->p,i);
                continue ;
            }
            assert( y->color == Black );
            if ( y->son[i^1]->color == Red ) {
                y->son[i^1]->color= Black, y->color= x->p->color, x->p->color= Black;
                rotate(x->p,i);
                x= root;
                continue ;
            }
            if ( y->son[i]->color == Red ) {
                y->color^= 1, y->son[i^1]->color^= 1, rotate(y,i^1);
                continue ;
            }
            if ( x->p->color == Black ) {
                y->color^= 1, x= x->p;
                continue ;
            }
            rotate(x->p,i);
            x= root;
        }
        x->color= Black;
    }

    // decrease the frequency of an item
    // if the frequency reached zero -- delete it
    void erase( std::shared_ptr<cell> &item ) {
        if ( item == nil )
            return ;
        if ( --item->freq ) return ;
        auto z= (item->son[R] != nil ? item->son[R] : item->son[L]);
        if ( z == item->son[R] ) {
            auto y= z->son[L];
            for ( ;y != nil and y->son[L] != nil; y= y->son[L] ) ;
            if ( (y->p->son[L]= y->son[R]) != nil )
                y->son[R]->p= y->p;
            item->freq= y->freq, item->val= y->val;
        }
        auto i= which_son(item);
        if ( (item->p->son[i]= z) != nil )
            z->p= item->p;
        if ( item->color == Black ) {
            fixup(z);
        }
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
        root= nullptr, nil= std::make_shared<cell>(), nil->color= Black;
    }
    void clear() {
        //TODO
    }
    void insert( T x ) {
        insert_(x);
    }
    void erase( T val ) {
        auto x= find(val);
        erase(x);
    }
    friend rb_tree operator + ( const rb_tree &a, const rb_tree &b ) ;
};

class rt {
private:
    std::vector<i64> data;
    std::vector<rb_tree<>> S;
    int n,m;
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
    void build( int v, int i, int j ) {
        if ( i == j ) {
            S[v].clear(), S[v].insert(data[i]);
            return ;
        }
        auto k= (i+j)>>1;
        build(L(v),i,k), build(R(v),k+1,j);
        S[v]= S[L(v)]+S[R(v)];
    }
    int query( int v, int i, int j, int qi, int qj, i64 val ) {
        if ( qi > j or qj < i )
            return 0;
        if ( qi <= i and j <= qj ) {
            return 0; //TODO
        }
        auto k= (i+j)>>1;
        return query(L(v),i,k,qi,qj,val)+query(R(v),k+1,j,qi,qj,val);
    }
    void update( int v, int i, int j, int pos, i64 oldval, i64 newval ) {
        if ( pos < i or pos > j )
            return ;
        if ( i == j ) {
            S[v].clear(), data[pos]= newval, S[v].insert(newval);
            return ;
        }
        auto k= (i+j)>>1;
        update(L(v),i,k,pos,oldval,newval);
        update(R(v),k+1,j,pos,oldval,newval);
        S[v].erase(oldval), S[v].insert(newval);
    }
public:
    explicit rt( const std::vector<i64> &input ): data(input) {
        this->n= input.size(), m= 4*n+7;
        S.resize(m);
    }
    void update( int pos, i64 newVal ) {
        update(1,0,n-1,pos-1,data[pos-1],newVal);
    }
    int query( int qi, int qj, i64 val ) {
        return query(1,0,n-1,qi-1,qj-1,val);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,j,k,pos,p,q;
    i64 newval,val;
    for (;(is >> n >> qr) and n;) {
        for ( c.resize(n), i= 0; i < n; is >> c[i++] ) ;
        auto T= std::make_unique<rt>(c);
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
    