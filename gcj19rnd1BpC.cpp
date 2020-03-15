/**
 * TOPIC: range tree, DP, data structures, binary search
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;

class rt {
public:
    using node_type= int;
    using size_type= int;
    using value_type= i64;
private:
   mutable std::vector<value_type> h[2];
   mutable std::vector<bool> flag[2];
   std::vector<value_type> tr[2], sum;
   size_type n,m;
   value_type threshold;
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
   void push_down( node_type v, size_type i, size_type j ) {
       if ( i < j )
           for ( auto t= 0; t <= 1; ++t )
               if ( flag[t][v] ) {
                   h[t][L(v)] = h[t][v], flag[t][L(v)] = true;
                   h[t][R(v)] = h[t][v], flag[t][R(v)] = true;
               }
       for ( auto t= 0; t <= 1; ++t )
           if ( flag[t][v] ) {
               flag[t][v]= false;
               if ( t == 0 )
                   sum[v]= (j-i+1)*h[0][v];
               if ( i == j )
                   tr[t][i]= h[t][v];
           }
   }
   void push_up( node_type v, size_type i, size_type j ) {
       assert( i < j );
       sum[v]= sum[L(v)]+sum[R(v)];
   }
   value_type query_( node_type v, size_type i, size_type j, size_type qi, size_type qj ) {
       push_down(v,i,j);
       if ( qi > j or qj < i )
           return 0;
       if ( qi <= i and j <= qj )
           return sum[v];
       auto k= (i+j)>>1;
       auto l= query_(L(v),i,k,qi,qj), r= query_(R(v),k+1,j,qi,qj);
       push_up(v,i,j);
       return l+r;
   }
   void init_( node_type v, size_type i, size_type j ) {
       assert( i <= j );
       for ( auto t= 0; t <= 1; ++t )
           flag[t][v]= false;
       sum[v]= 0;
       if ( i < j ) {
           auto k= (i+j)>>1;
           init_(L(v),i,k), init_(R(v),k+1,j);
       }
       else {
           tr[0][i]= tr[1][i]= -1;
       }
   }
   std::vector<value_type>
   read_( node_type v, size_type i, size_type j, size_type pos ) {
       assert( i <= pos and pos <= j );
       push_down(v,i,j);
       if ( i == j ) {
           //assert( tr[0][i] != -1 and tr[1][i] != -1 );
           return std::vector<value_type>{tr[0][i],tr[1][i]};
       }
       auto k= (i+j)>>1;
       return pos<=k?read_(L(v),i,k,pos):read_(R(v),k+1,j,pos);
   }
   void update_( node_type v, size_type i, size_type j,
                size_type qi, size_type qj,
                std::pair<value_type,value_type> pr )
   {
       push_down(v,i,j);
       if ( qi > j or qj < i )
           return ;
       if ( qi <= i and j <= qj ) {
           if ( pr.first != -1 ) {
               flag[0][v]= true, h[0][v]= pr.first;
           }
           if ( pr.second != -1 ) {
               flag[1][v]= true, h[1][v]= pr.second;
           }
           push_down(v,i,j);
           return ;
       }
       auto k= (i+j)>>1;
       update_(L(v),i,k,qi,qj,pr), update_(R(v),k+1,j,qi,qj,pr);
       push_up(v,i,j);
       /*
       for ( auto idx= qi; idx <= qj; ++idx ) {
           if ( pr.first != -1 )
               tr[0][idx]= pr.first;
           if ( pr.second != -1 )
               tr[1][idx]= pr.second;
       }
       */
   }
#undef L
#undef R
public:
   void build() { init_(1,0,n-1); }
   value_type query( size_type qi, size_type qj ) {
       return query_(1,0,n-1,qi,qj);
   }
   std::vector<value_type> access( size_type i ) { return read_(1,0,n-1,i); }
   void update( size_type qi, size_type qj, std::pair<value_type,value_type> pr ) {
       update_(1,0,n-1,qi,qj,pr);
   }
   rt( size_t n, value_type threshold ): threshold(threshold) {
       m= 4*(this->n= n)+7, sum.resize(m);
       for ( auto t= 0; t <= 1; ++t )
           h[t].resize(m), flag[t].resize(m), tr[t].resize(n);
   }
   size_type predecessor( size_type t, size_type qi, size_type qj, node_type val ) {
       assert( qi <= qj );
       if ( access(qi)[t] <= val )
           return qi;
       assert( access(qi)[t] > val );
       if ( access(qj)[t] > val )
           return qj+1;
       assert( access(qj)[t] <= val );
       auto low= qi, high= qj;
       for ( ;low+1 < high; ) {
           auto mid= (low+high)/2;
           if ( access(mid)[t] > val )
               low= mid;
           else high= mid;
       }
       return high;
       /*
       if ( access(qi)[t] <= val )
           return qi;
       auto idx= qj;
       for ( ;idx >= qi; --idx )
           if ( access(idx)[t] > val )
               return idx+1;
       return qj+1;
       */
   }
   size_type successor( size_type t, size_type qi, size_type qj, node_type val ) {
       /*
       if ( access(qj)[t] >= val )
           return qj;
       auto idx= qi;
       for ( ;idx <= qj; ++idx )
           if ( access(idx)[t] < val )
               return idx-1;
       return qi-1;
       */
       assert( qi <= qj );
       if ( access(qj)[t] >= val )
           return qj;
       assert( access(qj)[t] < val );
       if ( access(qi)[t] < val )
           return qi-1;
       assert( access(qi)[t] >= val );
       assert( access(qj)[t] <= val );
       auto low= qi, high= qj;
       for ( ;low+1 < high; ) {
           auto mid= (low+high)/2;
           if ( access(mid)[t] < val )
               high= mid;
           else low= mid;
       }
       return low;
   }
};

int main() {
    size_t n;
    i64 i,j,k,l,t,cs= 0,ts,KK;
    std::istream &is= std::cin;
    std::vector<i64> z;
    for ( is >> ts; ts--; ) {
        is >> n >> KK, z.resize(n);
        std::fill(z.begin(),z.end(),0);
        auto T= std::make_unique<rt>(n,KK), Q= std::make_unique<rt>(n,KK);
        T->build(), Q->build();
        std::vector<i64> C(n), D(n);
        for ( auto &v: C ) is >> v;
        for ( auto &v: D ) is >> v;
        T->update(0,0,{C[0],D[0]});
        i64 ans= (z[0]= labs(C[0]-D[0])<=KK?1:0);
        Q->update(0,0,{z[0],-1});
        for ( i= 1; i < n; ++i ) {
            i64 low= 0, high= (i-1), mid;
            if ( T->access(low)[0] <= C[i] ) {
                T->update(k= 0,i,{C[i],-1});
            }
            else if ( T->access(high)[0] > C[i] )
                T->update(k= i,i,{C[i],-1});
            else {
                for ( ;low+1 < high; ) {
                    if ( T->access(mid= (low+high)/2)[0] > C[i] )
                        low= mid;
                    else high= mid;
                }
                assert( T->access(low)[0] > C[i] and T->access(high)[0] <= C[i] );
                T->update(k= high,i,{C[i],-1});
            }
            low= 0, high= (i-1);
            if ( T->access(low)[1] <= D[i] ) {
                T->update(l= 0,i,{-1,D[i]});
            }
            else if ( T->access(high)[1] > D[i] )
                T->update(l= i,i,{-1,D[i]});
            else {
                for ( ;low+1 < high; ) {
                    if ( T->access(mid= (low+high)/2)[1] > D[i] )
                        low= mid;
                    else high= mid;
                }
                assert( T->access(low)[1] > D[i] and T->access(high)[1] <= D[i] );
                T->update(l= high,i,{-1,D[i]});
            }
            if ( k > l ) {
                auto s= T->successor(0,l,i,D[i]-KK);
                auto p= T->predecessor(0,l,i,KK+D[i]);
                if ( s >= p )
                    z[i]+= s-p+1;
                Q->update(p,s,{1,-1}), Q->update(l,p-1,{0,-1}), Q->update(s+1,i,{0,-1});
                z[i]+= Q->query(0,l-1);
            }
            else if ( k < l ) {
                auto s= T->successor(1,k,i,C[i]-KK);
                auto p= T->predecessor(1,k,i,KK+C[i]);
                if ( s >= p )
                    z[i]+= s-p+1;
                Q->update(p,s,{1,-1}), Q->update(k,p-1,{0,-1}), Q->update(s+1,i,{0,-1});
                z[i]+= Q->query(0,k-1);
            }
            else {
                assert( k == l );
                z[i]= labs(C[i]-D[i])<=KK?(i-l+1):0;
                if ( z[i] )
                    Q->update(k,i,{1,-1});
                else Q->update(k,i,{0,-1});
                z[i]+= Q->query(0,l-1);
            }
            /*
            z[i]= 0;
            for ( auto idx= i; idx >= 0; --idx ) {
                auto pr= T->access(idx);
                z[i]+= labs(pr.front()-pr.back()) <= K?1:0;
            }
            */
            ans+= z[i];
        }
        std::cout << "Case #" << ++cs << ": " << ans << std::endl;
        T.reset(), T= nullptr;
    }
    return 0;
}
