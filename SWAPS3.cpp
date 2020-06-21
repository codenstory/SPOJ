/**
 * SWAPS
 * TOPIC: Red-Black trees, range trees, range counting
 * status:
 */
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>
#include <istream>
#include <ostream>
#include <chrono>

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

template<typename vtype>
class rbtree {
    struct cell;
    using holder= std::shared_ptr<cell>;
    using t_dir= unsigned char;
    enum { L= 0, R= 1 };
    enum { Red= L, Black= R };
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
    void propagate( holder &z ) {
        for ( auto x= z; x != nil; x->card= x->freq+x->son[L]->card+x->son[R]->card, x= x->p ) ;
        assert( nil->card == 0 and nil->freq == 0 );
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

    holder find( vtype val ) const {
        auto x= root;
        for ( ;x != nil and x->val != val; x= x->son[x->val<val?R:L] ) ;
        assert( x == nil or x->val == val );
        return x;
    }

    void fixup( holder &x ) {
        while ( x != root and color(x) == Black ) {
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
    bool find_key( vtype val ) const {
        return find(val) != nil;
    }
    size_t rank( vtype key ) const {
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

    size_t exclusive_rank( vtype key ) const {
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
        if ( p != nil )
            assert( p->son[L] == x or p->son[R] == x );
        assert( x->val == val and x->c == Red );
        for ( propagate(x); x != root and color(x->p) == Red; ) {
            assert( color(x) == Red );
            auto i= which_son(x->p);
            auto g= x->p->p, y= g->son[i^1u];
            assert( g != nil );
            assert( color(g) == Black );
            if ( color(y) == Red ) {
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
        assert( root->p == nil );
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
#define LIMIT (0x400)
//#define LIMIT (0)

    using rbtree_holder= std::shared_ptr<rbtree<vtype>>;
    std::vector<rbtree_holder> tr;
    size_t m,n;
    std::vector<vtype> data;

    void build( int v, int i, int j ) {
        assert( j-i+1 > 0 );
        assert( v < tr.size() );
        assert( not tr[v] );
        if ( j-i+1 > LIMIT ) {
            if ( i == j ) {
                tr[v]= std::make_shared<rbtree<vtype>>();
                for ( auto l= i; l <= j; ++l )
                    tr[v]->push(data[l]);
                return ;
            }
            assert( i < j );
            auto k= (i+j)>>1u;
            build(L(v),i,k), build(R(v),k+1,j);
            tr[v]= std::make_shared<rbtree<vtype>>();
            for ( auto l= i; l <= j; ++l ) {
                tr[v]->push(data[l]);
            }
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
            if ( not tr[v] ) {
                tr[v]= std::make_shared<rbtree<vtype>>();
                tr[v]->push(newval);
                data[pos]= newval;
                return ;
            }
            auto ok= tr[v]->erase(old);
            assert( ok );
            tr[v]->push(newval);
            data[pos]= newval;
            return ;
        }
        assert( i < j );
        auto k= (i+j)>>1u;
        update(L(v),i,k,pos,old,newval), update(R(v),k+1,j,pos,old,newval);
        if ( not tr[v] ) {
            tr[v]= std::make_shared<rbtree<vtype>>();
            for ( auto l= i; l <= j; tr[v]->push(data[l++]) ) ;
            return ;
        }
        auto ok= tr[v]->erase(old);
        assert( ok );
        tr[v]->push(newval);
		//std::cerr << tr[v]->size() << " " << "After pushing " << newval << " in place of " << old << std::endl;
		//tr[v]->display();
		//std::cerr << std::endl;
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
            if ( not tr[v] ) {
                tr[v]= std::make_shared<rbtree<vtype>>();
                for ( auto l= i; l <= j; tr[v]->push(data[l++]) ) ;
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
            if ( not tr[v] ) {
                tr[v]= std::make_shared<rbtree<vtype>>();
                for ( auto l= i; l <= j; tr[v]->push(data[l++]) ) ;
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
        {
            //mytimer mt("building the tree");
            //build(1, 0, n - 1);
        }
    }
    void change_val( int pos, vtype newval ) {
        auto old_val= data[pos];
		//std::cerr << "Old val: " << old_val << std::endl;
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

#define LSB(v) ((v) & ((~(v))+1u))
template<typename vtype>
class BIT {
#define MAXW (50'001)
    std::vector<size_t> tr;
    int n,K;
    size_t prefix( unsigned int i ) {
        if ( i == 0 )
            return 0;
        size_t ans= 0;
        for ( ;i; ans+= tr[i], i&= ~LSB(i) ) ;
        return ans;
    }
    size_t segment( unsigned int i, unsigned int j ) {
        if ( j < i )
            return 0;
        size_t ans= 0;
        if ( i == 0 )
            return prefix(j);
        return prefix(j)-prefix(i-1);
    }
public:
    BIT( const std::vector<vtype> &data, int i, int j ) {
        tr.resize(MAXW,0);
        for ( auto t= i; t < j; ++t )
            increment(data[t]);
    }
    void increment( unsigned int i ) {
        for ( ;i < MAXW; ++tr[i], i+= LSB(i) ) ;
    }
    void decrement( unsigned int i ) {
        for ( ;i < MAXW; --tr[i], i+= LSB(i) ) ;
    }
    size_t countBelow( unsigned int i ) {
        return segment(0,i-1);
    }
    size_t countAbove( unsigned int i ) {
        return segment(i+1,MAXW-1);
    }
};

template<typename vtype>
void mergesort( std::vector<vtype> &c, int i, int j, size_t &res ) {
    if ( i == j )
        return ;
    assert( i < j );
    auto k= (i+j)>>1u;
    size_t ll= 0, rr= 0;
    mergesort(c,i,k,ll), mergesort(c,k+1,j,rr);
    auto tmp= std::vector<vtype>(j-i+1);
    int ii= i, jj= k+1, idx= 0;
    for ( ;ii <= k and jj <= j; ) {
        if ( c[ii] <= c[jj] )
            tmp[idx++]= c[ii++];
        else {
            res+= k+1-ii, tmp[idx++]= c[jj++];
        }
    }
    for ( ;ii <= k; tmp[idx++]= c[ii++] ) ;
    for ( ;jj <= j; tmp[idx++]= c[jj++] ) ;
    for ( int l= 0; l < j-i+1; ++l )
        c[l+i]= tmp[l];
    //std::cerr << "done with " << i << ", " << j << std::endl;
    res+= ll, res+= rr;
}

template<typename vtype>
size_t find_inversions( const std::vector<vtype> &c ) {
    //auto vec= c;
    size_t res= 0;
    //mergesort(vec,0,vec.size()-1,res);
    auto B= std::make_shared<BIT<std::uint16_t>>(std::vector<uint16_t>{},0,0);
    for ( auto x: c ) {
        res+= B->countAbove(x);
        B->increment(x);
    }
    return res;
}

size_t BS;
std::vector<std::shared_ptr<BIT<std::uint16_t>>> trees;
std::vector<std::uint16_t> data;
int n;

template<typename vtype>
size_t countingAbove( int qi, int qj, vtype key ) {
    if ( qi > qj )
        return 0;
    qi= std::min(n-1,std::max(0,qi));
    qj= std::min(n-1,qj);
    int bi= qi/BS, bj= qj/BS;
    if ( bi == bj ) {
        return std::count_if(data.begin()+qi,data.begin()+qj+1,[key](auto x) {
            return x > key;
        });
    }
    assert( bi < bj );
    size_t ans= 0;
    for ( int t= bi+1; t <= bj-1; ++t ) {
        ans+= trees[t]->countAbove(key);
    }
    for ( int t= qi; t < (bi+1)*BS and t <= qj; ++t )
        if ( data[t] > key )
            ++ans;
    for ( int t= bj*BS; t <= qj; ++t )
        if ( data[t] > key )
            ++ans;
    return ans;
}

template<typename vtype>
size_t countingBelow( int qi, int qj, vtype key ) {
    if ( qi > qj )
        return 0;
    qi= std::min(n-1,std::max(0,qi));
    qj= std::min(n-1,qj);
    int bi= qi/BS, bj= qj/BS;
    if ( bi == bj ) {
        return std::count_if(data.begin()+qi,data.begin()+qj+1,[key](auto x) {
            return x < key;
        });
    }
    assert( bi < bj );
    size_t ans= 0;
    for ( int t= bi+1; t <= bj-1; ++t ) {
        ans+= trees[t]->countBelow(key);
    }
    assert( qi < (bi+1)*BS );
    for ( int t= qi; t < (bi+1)*BS and t <= qj; ++t )
        if ( data[t] < key )
            ++ans;
    assert( qi < bj*BS );
    for ( int t= bj*BS; t <= qj; ++t )
        if ( data[t] < key )
            ++ans;
    return ans;
}

template<typename vtype>
void change_value( int pos, vtype new_val ) {
    int bi= pos/BS;
    auto old_val= data[pos];
    trees[bi]->decrement(old_val);
    trees[bi]->increment(new_val);
    data[pos]= new_val;
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    int qr;
    size_t ans= 0;
    is >> n;
    for ( BS= 1u; BS <= n/BS; ++BS ) ;
    if ( not --BS ) BS= 1;
    data.resize(n);
    {
        mytimer mt("reading input");
        for (auto &v: data)
            is >> v;
    }
    {
        mytimer mt("blocks preprocessing");
        trees.resize(n / BS + 0x10, nullptr);
        for (int i = 0; i < n; ++i) {
            int j = i / BS;
            if (not trees[j])
                trees[j] = std::make_shared<BIT<std::uint16_t>>(data, j * BS, std::min(n,(int)((j + 1) * BS)));
            trees[j]->increment(data[i]);
        }
    }
    {
        mytimer mt("inversion counting");
        ans = find_inversions(data);
    }
    /*
    for ( int i= 0; i < n; ++i )
        ans+= T->countingAbove(0,i-1,data[i])+T->countingBelow(i+1,n-1,data[i]);
    assert( 0 == (ans&1) );
    ans>>= 1;
    */
    //std::cerr << "initially: " << ans << std::endl;
    mytimer mt("answering the queries");
    for ( is >> qr; qr--; ) {
        int x,y;
        is >> x >> y;
        assert( 1 <= x and x <= n );
        --x;
        auto old= countingAbove(0,x-1,data[x])+countingBelow(x+1,n-1,data[x]);
        //std::cerr << "old= " << old << std::endl;
        change_value(x,y);
        auto neu= countingAbove(0,x-1,data[x])+countingBelow(x+1,n-1,data[x]);
        //std::cerr << "new= " << neu << std::endl;
        ans= ans+neu-old;
        os << ans << '\n';
    }
    return 0;
}
