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
        //std::cout << header << " took " << time_span.count() << " seconds\n";
    }
};

template<typename vtype>
class rbtree {
    struct cell;
    //using holder= std::shared_ptr<cell>;
    using holder= cell*;
    using t_dir= unsigned char;
    enum { L= 0, R= 1 };
    enum { Red= L, Black= R };
    struct cell {
        holder son[2]= {nullptr,nullptr};
        holder p= nullptr;
        std::uint32_t card{};
        std::uint32_t freq{};
        vtype val{};
        unsigned char c= Red;
    };
    std::vector<holder> pool;
    int curr;

#define color(x) ((x)->c)
#define which_son(x) ((x)->p->son[L] == (x)?L:R)
#define flip(x) ((x)->c^=1u)

    holder root, nil;
    size_t sz{};
    holder make_nil() {
        auto x= pool[curr++];
        x->c= Black, x->son[L]= x->son[R]= x->p= x, x->card= x->freq= 0;
        return x;
    }
    holder make_cell( vtype val ) {
        if ( curr == pool.size() ) {
            pool.push_back(new cell());
        }
        auto x= pool[curr++];
        x->c= Red, x->son[L]= x->son[R]= x->p= nil, x->val= val;
        x->freq= x->card= 0;
        return x;
    }
    void propagate( holder &z ) {
        for ( auto x= z; x != nil; x->card= x->freq+x->son[L]->card+x->son[R]->card, x= x->p ) ;
        //assert( nil->card == 0 and nil->freq == 0 );
    }
    void rotate( holder &x, t_dir t ) {
        //assert( t == L or t == R );
        //assert( x != nil );
        auto y= x->son[t^1u];
        //assert( y != nil );
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
        //assert( x == nil or x->val == val );
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
            //assert( color(y) == Black );
            if ( color(y->son[L]) == Black and color(y->son[R]) == Black ) {
                flip(y), x= x->p;
                continue ;
            }
            if ( color(y->son[i]) == Red ) {
                flip(y->son[i]), flip(y), rotate(y,i^1u);
                continue ;
            }
            //assert( color(y->son[i^1u]) == Red );
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
                //assert(x->val == key);
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
                //assert(x->val == key);
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

    ~rbtree() {
        for ( auto &v: pool )
            if ( v ) {
                delete v;
            }
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

    explicit rbtree( size_t sz ) {
        pool.resize(sz+0x10,nullptr), curr= 0;
        for ( auto &v: pool )
            //v= std::make_shared<cell>();
            v= new cell();
        root= nil= make_nil();
    }

};

using container= rbtree<std::uint16_t>;

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
    auto vec= c;
    size_t res= 0;
    //std::cerr << "n= " << vec.size() << std::endl;
    mergesort(vec,0,vec.size()-1,res);
    return res;
}

size_t BS,K;
std::vector<std::shared_ptr<container>> trees;
std::vector<std::uint16_t> data;
int n;

template<typename vtype>
size_t countingAbove( int qi, int qj, vtype key ) {
    if ( qi > qj )
        return 0;
    qi= std::min(n-1,std::max(0,qi));
    qj= std::min(n-1,qj);
    int bi= (qi>>K), bj= (qj>>K);
    if ( bi == bj ) {
        return std::count_if(data.begin()+qi,data.begin()+qj+1,[key](auto x) {
            return x > key;
        });
    }
    assert( bi < bj );
    size_t ans= 0;
    for ( int t= bi+1; t <= bj-1; ++t ) {
        ans+= BS-trees[t]->rank(key);
    }
    for ( int t= qi; t < ((bi+1)<<K); ++t )
        if ( data[t] > key )
            ++ans;
    for ( int t= (bj<<K); t <= qj; ++t )
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
    int bi= (qi>>K), bj= qj>>K;
    if ( bi == bj ) {
        return std::count_if(data.begin()+qi,data.begin()+qj+1,[key](auto x) {
            return x < key;
        });
    }
    assert( bi < bj );
    size_t ans= 0;
    for ( int t= bi+1; t <= bj-1; ++t ) {
        ans+= trees[t]->exclusive_rank(key);
    }
    assert( qi < ((bi+1)<<K) );
    for ( int t= qi; t < (bi+1)*BS and t <= qj; ++t )
        if ( data[t] < key )
            ++ans;
    assert( qi < (bj<<K) );
    for ( int t= (bj<<K); t <= qj; ++t )
        if ( data[t] < key )
            ++ans;
    return ans;
}

template<typename vtype>
void change_value( int pos, vtype new_val ) {
    int bi= pos>>K;
    assert( pos < n );
    auto old_val= data[pos];
    auto ok= trees[bi]->erase(old_val);
    assert( ok );
    trees[bi]->push(new_val);
    data[pos]= new_val;
}

int getint() {
    int ch, n= 0;
    static int dig[256]= {0};
    if ( not dig['0'] )
        for ( ch= '0'; ch <= '9'; dig[ch++]= 1 ) ;
    for ( ;(ch= getchar_unlocked()) != EOF and not dig[ch] and ch != '-'; ) ;
    assert( ch != '-' );
    for ( n= ch-'0'; (ch= getchar_unlocked()) != EOF and dig[ch]; n= 10*n+ch-'0' ) ;
    return n;
}

int main() {
    //std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
#endif
    //std::istream &is = std::cin;
    //std::ostream &os = std::cout;
    int qr;
    size_t ans= 0;
    {
        mytimer mt("reading input");
        n = getint();
        for (BS = 1u; BS <= n / BS; ++BS);
        if (not --BS) BS = 1;
        for ( K= 0; (1<<K) < BS; ++K ) ;
        BS= (1<<K);
        data.resize(n);
        for (auto &v: data)
            v = getint();
    }
    {
        mytimer mt("counting inversions");
        ans = find_inversions(data);
    }
    trees.resize((n>>K)+BS,nullptr);
    {
        mytimer mt("preprocessing blocks");
        for (int i = 0; i < n; ++i) {
            int j = i>>K;
            if (not trees[j])
                trees[j] = std::make_shared<container>(1<<K);
            trees[j]->push(data[i]);
        }
    }
    /*
    for ( int i= 0; i < n; ++i )
        ans+= T->countingAbove(0,i-1,data[i])+T->countingBelow(i+1,n-1,data[i]);
    assert( 0 == (ans&1) );
    ans>>= 1;
    */
    //std::cerr << "initially: " << ans << std::endl;
    mytimer mt("answering the queries");
    for ( qr= getint(); qr--; ) {
        int x,y;
        x= getint(), y= getint();
        assert( 1 <= x and x <= n );
        --x;
        auto old= countingAbove(0,x-1,data[x])+countingBelow(x+1,n-1,data[x]);
        //std::cerr << "old= " << old << std::endl;
        change_value(x,y);
        auto neu= countingAbove(0,x-1,data[x])+countingBelow(x+1,n-1,data[x]);
        //std::cerr << "new= " << neu << std::endl;
        ans= ans+neu-old;
        printf("%lu\n",ans);
    }
    return 0;
}
