/*
 */
#include <queue>
#include <stack>
#include <iostream>
using namespace std;
#define DBG(x) (1)
#include <cassert>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define KEY(p) V[h+*(p)]
#define xchg(a,b) (tmp=*(a),*(a)=*(b),*(b)=tmp)
#define MED(a,b,c) (KEY(a) < KEY(b) ?\
						(KEY(b) < KEY(c) ? b : KEY(a) < KEY(c) ? c : a):\
						(KEY(a) < KEY(c) ? a : KEY(b) < KEY(c) ? c : b))
#define M 10
#define N 100010
#define MAXL (M*N)
#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))
#define oo (1 << 29)
enum { L, R };
enum { Ltype, Stype };

int pref[MAXL],height[MAXL],*V,*I,n,x[MAXL],p[MAXL],h,m,len[M+1],r,*rank,*pos;

void update_group( int *pi, int *pj ) {
	int g = pj-I;

	assert( pi <= pj );
	V[*pi] = g;
	if ( pi == pj ) *pi = -1;
	else 
		do V[*++pi] = g; while ( pi < pj );
}

void select_sort( int *p, int n ) {
	int *pa,*pb,*pi,tmp,val,f;
	for ( pa = p; pa < p+n; update_group(pa,pb-1), pa = pb )
		for ( val = KEY(pa), pb = pi = pa+1; pi < p+n; ++pi )
			if ( (f = KEY(pi)) < val )
				xchg(pi,pa), pb = pa+1, val = f;
			else if ( f == val )
				xchg(pi,pb), ++pb;
}

int choose_pivot( int *p, int n ) {
	int *pl,*pm,*pn,s;
	pm = p+(n>>1);
	if ( n > 7 ) {
		pl = p, pn = p+n-1;
		if ( n > 40 ) {
			s = (n >> 3);
			pl = MED(pl,pl+s,pl+s+s);
			pm = MED(pm-s,pm,pm+s);
			pn = MED(pn-s-s,pn-s,pn);
		}
		pm = MED(pl,pm,pn);
	}
	return KEY(pm);
}

void split_sort( int *p, int n ) {
	int *pa,*pb,*pc,*pd,tmp,*pn = p+n,
		val,f,s,t,*pl,*pm;

	assert( n >= 0 );

	if ( !n ) return ;

	if ( n <= 7 ) {
		select_sort(p,n);
		return ;
	}

	for ( val = choose_pivot(p,n), pa = pb = p, pc = pd = p+n-1;; xchg(pc,pb), ++pb, --pc ) {
		for ( ;pb <= pc && (f = KEY(pb)) <= val; ++pb )
			if ( f == val )
				xchg(pa,pb), ++pa;
		for ( ;pc >= pb && (f = KEY(pc)) >= val; --pc )
			if ( f == val )
				xchg(pc,pd), --pd;
		if ( pb > pc )
			break ;
	}
	/*assert( pc+1 == pb );*/

	s = min(pa-p,pb-pa);
	for ( pl = p,  pm = pb-s; s--; xchg(pl,pm), ++pl, ++pm );
	t = min(pn-pd-1,pd-pc);
	for ( pl = pb, pm = pn-t; t--; xchg(pl,pm), ++pl, ++pm );

	s = pb-pa, t = pd-pc;
	/*assert( s >= 0 );
	assert( t >= 0 );*/
	split_sort(p,s);
	/*assert( p+s <= pn-t-1 );*/
	update_group(p+s,pn-t-1);
	split_sort(pn-t,t);
}

int transform( int *p, int *x, int n, int l, int k, int q ) {
	int *xi,*xj,i,j,s,m,b,c,d,e;

	for ( s = 0, i = (k-l); i; ++s, i >>= 1 );
	e = (INT_MAX >> s);

	for ( r = 0, d = b = 0; r < n && d <= e && (c = (d<<s)|(k-l)) <= q; ++r )
		d = c, b = (b<<s)|(x[r]-l+1);

	m = (1 << ((r-1)*s))-1, x[n] = l-1;

	if ( d <= n ) {
		for ( i = 0; i <= d; p[i++] = 0 );
		for ( c = b, xi = x+r; xi <= x+n; ++xi )
			p[c] = 1, c = ((c&m)<<s)|(*xi-l+1);
		for ( i = 1; i < r; ++i )
			p[c] = 1, c = ((c&m)<<s);
		for ( j = 0, i = 0; i <= d; ++i )
			if ( p[i] && (p[i] = ++j) );
		for ( c = b, xi = x, xj = x+r; xj <= x+n; ++xj )
			*xi++ = p[c], c = ((c&m)<<s)|(*xj-l+1);
		for ( i = 1; i < r; ++i )
			*xi++ = p[c], c = ((c&m)<<s);
	}
	else {
		for ( c = b, xi = x, xj = x+r; xj <= x+n; ++xj )
			*xi++ = c, c = ((c&m)<<s)|(*xj-l+1);
		for ( i = 1; i < r; ++i )
			*xi++ = c, c = ((c&m)<<s);
		j = d;
	}

	x[n] = 0;

	return 1+j;

}

/*
 * bucket sort does the ``update_group()'' operation
 * within itself
 */
void bucket_sort( int *p, int *x, int n, int K ) {
	int *pi,i,c,d,k,g;

	for ( pi = p; pi < p+K; *pi++ = -1 );
	for ( i = 0; i <= n; ++i )
		c = x[i], x[i] = p[c], p[c] = i;

	for ( i = n, k = K-1; k >= 0; --k ) {
		if ( (c = p[k]) == -1 ) continue ;
		assert( i >= 0 );
		d = x[c], x[c] = g = i;
		if ( d < 0 )
			p[i--] = -1;
		else {
			p[i--] = c;
			do d = x[c = d], x[p[i--] = c] = g; while ( d >= 0 );
		}
	}
	assert( i == -1 );
}

void suf_sort( int *p, int *x, int n, int l, int k ) {
	int *pi,*pk,sl,s,i;

	I = p, V = x;
	rank = V, pos = I;
	/*
	 * alphabet = [low,high) = [l,k)
	 */
	if ( n >= (k-l) ) {
		i = transform(p,x,n,l,k,n);
		bucket_sort(p,x,n,i);
	}
	else {
		transform(p,x,n,l,k,INT_MAX);
		for ( i = 0; i <= n; ++i ) I[i] = i;
		h = 0, split_sort(I,n+1);
	}

	for ( h = r; *I+(n+1); h <<= 1 ) {
		assert( h <= n );
		pi = I, sl = 0;
		do {
			if ( (s = *pi) < 0 )
				sl += s, pi -= s;
			else {
				if ( sl )
					*(pi+sl) = sl, sl = 0;
				pk = I+V[s]+1;
				/*assert( pi <= pk );*/
			   	split_sort(pi,pk-pi), pi = pk;
			}
		} while ( pi <= I+n );
		if ( sl )
			*(pi+sl) = sl;
	}
	for ( i = 0; i <= n; ++i ) I[V[i]] = i;
}

int which_suff( int position ) {
	int low,high,mid;
	low = 0, high = m;
	if ( position+1 <= len[1] )
		return 1;
	assert( pref[low] < position+1 );
	assert( position+1 <= pref[high] );
	for (;low+1 != high; pref[mid = (low+high)/2] < position+1 ? (low = mid) : (high = mid) );
	return high;
}

char buff[MAXL],*text = buff;

int conv( char  ch ) {
	if ( '0' <= ch && ch <= '9' )
		return ch-'0'+1;
	if ( 'A' <= ch && ch <= 'Z' )
		return 10 + ch-'A'+1;
	return 10 + 26 + 1 + ch-'a';
}

int read_input() {
	int A = 'a'-1,*xi = x,i,k;
	/* Here we assume m <= 10 */
	for ( n = 0, m = 0; fgets(buff,sizeof(buff),stdin); *xi++ = A--, ++len[m] )	
		for ( len[++m] = 0; buff[len[m]] && buff[len[m]] != '\n';)
			*xi++ = buff[len[m]++];
	n = xi-x-1;
	x[n] = A+1;

	for ( i = 0; i <= n; ++i ) text[i] = x[i];
	text[n+1] = '\0';
	// puts(text);

	for ( i = 1; i <= m; ++i ) {
		pref[i] = pref[i-1] + len[i];
		/*printf("len[%d] = %d\n",i,len[i]);*/
	}
	return A+2;
	/*fgets(buff,sizeof(buff),stdin);
	for ( n = 0; buff[n] && buff[n] != '\n'; x[n] = buff[n], ++n );
	*/
}

void kasai() {
	int i,j,H;
	for ( H = 0, j = 0; j < n; height[rank[j++]] = H ) 
		for ( H = max(H-1,0), i = rank[j]; text[pos[i-1]+H] == text[j+H]; ++H );
}

char idx[MAXL];

typedef struct cell {
	struct cell *son[2],*p;
	int idx;
	char cnt;
} cell;

typedef struct {
	cell *root,*rightmost;
	size_t cnt;
} tree;

tree *init() {
	tree *t = (tree *)malloc(sizeof *t);
	t->cnt = 0, t->rightmost = t->root = NULL;
	return t;
}

void tree_push( tree *t, int idx ) {
	cell *x;

	cell *p = (cell *)malloc(sizeof *p);
	assert( p );
	p->idx = idx, p->cnt = 0, p->son[L] = p->son[R] = p->p = NULL;

	for ( ++t->cnt, x = t->rightmost; x && height[x->idx] > height[idx]; x = x->p );
	if ( !x ) {
		if ( p->son[L] = t->root )
			t->root->p = p, ++p->cnt;
	   	t->root = t->rightmost = p;
	}
	else {
		if ( p->son[L] = x->son[R] )
			x->son[R]->p = p, ++p->cnt;
		else ++x->cnt;
		p->p = x, x->son[R] = p;
		t->rightmost = p;
	}
}

int E[4*MAXL],cur,repr[MAXL],BS,K,level[4*MAXL],nn,*A,*B,**MTAB;
char ***table;
unsigned int *mask;

stack<pair<cell *,int> > st;

void euler_tour( cell *x, int depth ) {
	int i,prev_depth,ndepth;
	cell *xx,*y;
	prev_depth = -1;
	for ( xx = x, st.push(make_pair(xx,0)); !st.empty(); ) {
		pair<cell *,int> rr = st.top();
		st.pop();
		xx = rr.first, depth = rr.second;
		E[repr[xx->idx] = cur] = xx->idx, level[cur++] = prev_depth = depth;
		if ( !xx->cnt ) {
			ndepth = depth;
			for ( y = xx->p; y; ) {
				E[cur] = y->idx, level[cur++] = --ndepth;
				if ( !--y->cnt )
					y = y->p;
				else break ;
			}
		}
		for ( i = L; i <= R; ++i )
			if ( xx->son[i] ) {
				st.push(make_pair(xx->son[i],depth+1));
			}
	}
}

int _max( int x, int y ) { return x < y ? y : x; }

tree *q;

void preprocess() {
	int i,j,k,l,ax[0x100];
	unsigned int u;

	for ( q = init(), i = 0; i <= n; tree_push( q, i++ ) );
	cur = 0, euler_tour( q->root, 0 );
	assert( cur == 2*(n+1)-1 );

	/*
	 */
	for ( nn = cur, K = 0; (1 << K) < nn; ++K );
	for ( BS = max(K/2,2); nn % BS; level[nn] = level[nn-1]+1, ++nn );


	table = (char ***)malloc((1 << BS)*sizeof *table);
	assert( table );
	for ( u = 0; u < (1 << BS); ++u ) {
		table[u] = (char **)malloc(BS*sizeof *table[u]);
		assert( table[u] );
		for ( i = 0; i < BS; ++i ) {
			table[u][i] = (char *)malloc(BS*sizeof *table[u][i]);
			assert( table[u][i] );
		}
	}

	/* compute table */
	for ( u = 0; u < (1 << (BS-1)); ++u ) {
		for ( ax[0] = 0, i = 1; i < BS; ++i )
			ax[i] = ax[i-1] + (((u>>(i-1))&1) ? 1 : -1);
		for ( i = 0; i < BS; table[u][i][i] = i, ++i );
		for ( k = 2; k <= BS; ++k )
			for ( i = 0; (j = i+k-1) < BS; ++i )
				if ( ax[j] > ax[table[u][i][j-1]] )
					table[u][i][j] = table[u][i][j-1];
				else
					table[u][i][j] = j;
	}

	/* superblocks */
	mask = (unsigned int *)calloc((nn/BS+1),sizeof *mask);
	assert( mask ); 
	A = (int *)malloc((nn/BS+1)*sizeof *A);
	B = (int *)malloc((nn/BS+1)*sizeof *B);

	for ( k = 0; k < nn/BS; ++k ) {
		for ( mask[k] = 0, i = k*BS, j = i+BS-1, l = i+1; l <= j; ++l )
			if ( level[l]-level[l-1] == 1 )
				mask[k] |= (1 << (l-i-1));
		A[k] = level[B[k] = table[mask[k]][0][BS-1] + k*BS];
	}

	MTAB = (int **)malloc((nn/BS+1)*sizeof *MTAB);
	for ( k = 0; (1 << k) <= nn/BS; ++k );
	for ( i = 0; i < nn/BS; ++i )
		MTAB[i] = (int *)malloc(k*sizeof *MTAB[i]);

	for ( i = 0; i < nn/BS; ++i ) MTAB[i][0] = i;

	for ( j = 1; (k = (1 << j)) < nn/BS; ++j )
		for ( i = 0; (i+k-1) < nn/BS; ++i )
			if ( A[MTAB[i][j-1]] > A[MTAB[i+k/2][j-1]] )
				MTAB[i][j] = MTAB[i+k/2][j-1];
			else
				MTAB[i][j] = MTAB[i][j-1];
}

char pre_lsb[1 << 21];

int lsb( unsigned int u ) {
	if ( u >= (1 << 21) )
		return 21 + pre_lsb[u >> 21];
	return pre_lsb[u];
	/*
	for ( k = 0; (1 << k) <= u; ++k );
	return k-1;
	*/
}

int sparse_rmq( int i, int j ) {
	int l = lsb(j-i+1),
		k = (1 << l);
	assert( (j-i+1) >= k );
	assert( (j-i+1) < (k*2) );
	assert( i+k-1 >= j-k+1 );
	assert( i+k-1 <= j );
	assert( j-k+1 >= i );
	return A[MTAB[i][l]] < A[MTAB[j-k+1][l]] ? B[MTAB[i][l]] : B[MTAB[j-k+1][l]];
}

int rmq( int i, int j ) {
	int BI,BJ,idx1,idx2,idx3,k;

	if ( i > j )
		k = i, i = j, j = k;

	BI = i/BS, BJ = j/BS;

	if ( BI == BJ ) 
		return table[mask[BI]][i % BS][j % BS] + BI*BS;

	idx1 = table[mask[BI]][i % BS][BS-1] + BI*BS;
	idx2 = table[mask[BJ]][0][j % BS]    + BJ*BS;

	idx1 = level[idx1] < level[idx2] ? idx1 : idx2;

	if ( BI+1 < BJ )
		if ( level[idx1] > level[idx3 = sparse_rmq(BI+1,BJ-1)] )
			idx1 = idx3;

	return idx1;

}

int tr[MAXL*2+MAXL/2];

void build_tree( int v, int i, int j ) {
	int k = (i+j)/2;
	if ( i == j ) {
		tr[v] = height[i];
		return ;
	}
	build_tree(2*v,i,k), build_tree(2*v+1,k+1,j);
	tr[v] = min(tr[2*v],tr[2*v+1]);
}

int query( int v, int i, int j, int qi, int qj ) {
	int k = (i+j)/2,left,right;
	if ( qj < i || qi > j )
		return +oo;
	if ( qi <= i && j <= qj )
		return tr[v];
	left = query( 2*v,i,k,qi,qj );
	right = query( 2*v+1, k+1,j,qi,qj );
	return min(left,right);
}

/*
int lcp( int i, int j ) {
	return query(1,1,n,i+1,j);
}
*/

int lcp( int i, int j ) {
	assert( i < j );
	return height[E[rmq(repr[i+1],repr[j])]];
}

int lcs() {
	int L = 0,i,j,left,right,msk,cnt[M+1] = {0};

	for ( left = i = 1, j = 0, msk = 0; ; ) {
		assert( msk != ((1<<m)-1) );
		while ( j <= n && msk != ((1<<m)-1) )
			if ( !cnt[i = idx[right = ++j]]++ )
				msk |= (1<<(i-1));
		if ( j > n ) break ;
		if ( msk == ((1<<m)-1) )
			L = _max(L,lcp(left,right));
		while ( msk == ((1<<m)-1) ) {
			i = idx[left++];
			if ( !--cnt[i] )
				msk &= ~(1<<(i-1));
			if ( msk == ((1<<m)-1) ) {
				L = _max(L,lcp(left,right));
				/*printf("lcp(%d,%d) = %d\n",left,right,lcp(left,right));*/
			}
		}
	}

	return L;

}

#define TEST(i) (t[(i) >> 3] & (1 << ((i) & 7)))
#define SET(i)  (t[(i) >> 3] |= (1 << ((i) & 7)))
#define isLMS(i)  ((i) > 0 && TEST(i) && !TEST(i-1))
#define chr(i) ((cs == sizeof(int)) ? (((int *)s)[i]) : (((unsigned char *)s)[i]))

void bucketBorders( unsigned char *s, int *bkt, int n, int K, int cs, int tyyppi ) {
	int i, sum = 0;
	assert( Ltype <= tyyppi );
	assert( tyyppi <= Stype );
	for ( i = 0; i <= K; bkt[i++] = 0 );
	for ( i = 0; i < n; ++bkt[chr(i)], ++i );
	for ( i = 0; i <= K; ++i )
		sum += bkt[i], bkt[i] = sum-(1-tyyppi)*bkt[i];
}

void induceSA( unsigned char *t, int *SA, unsigned char *s, int *bkt,
				int n, int K, int cs, int tyyppi ) {
	int i,j;
	bucketBorders( s, bkt, n, K, cs, tyyppi );
	if ( tyyppi == Stype ) {
		for ( i = n-1; i >= 0;)
			if ( (j = SA[i--]-1) >= 0 && TEST(j) )
				SA[--bkt[chr(j)]] = j;
	}
	else {
		for ( i = 0; i < n;)
			if ( (j = SA[i++]-1) >= 0 && !TEST(j) )
				SA[bkt[chr(j)]++] = j;
	}
}


void SAIS( unsigned char *s, int *SA, int n, int K, int cs ) {
	int i,j,k,l,n1,*pi,name,*s1,*SA1,*bkt;
	unsigned char *t;

	assert( chr(n-1) == 0 );

	t = (unsigned char *)calloc(((n >> 3)+8),sizeof *t);

	for ( SET(n-1), i = n-3; i >= 0; --i )
		if ( chr(i) < chr(i+1) || (chr(i) == chr(i+1) && TEST(i+1)) )
			SET(i);

	bkt = (int *)malloc((K+1)*sizeof *bkt);
	bucketBorders( s, bkt, n, K, cs, Stype );
	for ( pi = SA; pi < SA+n; *pi++ = -1 );
	for ( i = 1;  i < n; ++i )
		if ( isLMS(i) && (SA[--bkt[chr(i)]] = i) );

	induceSA( t, SA, s, bkt, n, K, cs, Ltype );
	induceSA( t, SA, s, bkt, n, K, cs, Stype );
	free( bkt );

	for ( pi = SA, i = 0; i < n; ++i )
		if ( isLMS(SA[i]) && (*pi++ = SA[i]) );
	for ( n1 = pi-SA, pi = SA+n1; pi < SA+n; *pi++ = -1 );

	for ( name = -1, i = -1, pi = SA; pi < SA+n1; SA[n1+(j-(j&1))/2] = name )
		for ( j = *pi++, l = 0; l < n; ++l )
			if ( i == -1 || chr(i+l) != chr(j+l) || TEST(i+l) != TEST(j+l) ) {
				++name, i = j;
				break ;
			}
			else if ( l > 0 && (isLMS(j+l) || isLMS(i+l)) ) {
				assert( isLMS(i+l) );
				break ;
			}

	for ( j = n-1, i = n-1; i >= n1; --i )
		if ( (SA[i] >= 0) && (SA[j--] = SA[i]) );

	assert( name < n1 );

	SA1 = SA, s1 = SA+n-n1;

	if ( name == n1-1 ) 
		for ( i = 0; i < n1; SA1[s1[i]] = i, ++i );
	else 
		SAIS((unsigned char *)s1,SA1,n1,name,sizeof *s1);

	bkt = (int *)malloc((K+1)*sizeof *bkt);

	bucketBorders( s, bkt, n, K, cs, Stype );	
	for ( i = 1, pi = s1; i < n; ++i )
		if ( isLMS(i) && (*pi++ = i) );

	for ( i = 0; i < n1; SA1[i] = s1[SA1[i]], ++i );
	for ( i = n1; i < n; SA[i++] = -2 );

	for ( i = n1-1; i >= 0; --i )
		j = SA[i], SA[i] = -1, SA[--bkt[chr(j)]] = j;
	induceSA( t, SA, s, bkt, n, K, cs, Ltype );
	induceSA( t, SA, s, bkt, n, K, cs, Stype );
	free( bkt ); free( t );
}


int main() {
	int l,k,i,j;
	unsigned int u;
#ifndef ONLINE_JUDGE
	freopen("in.txt","r",stdin);
#endif

	for ( pre_lsb[1] = 0, u = 2; u < (1 << 21); ++u )
		if ( (1 << (pre_lsb[u-1]+1)) > u )
			pre_lsb[u] = pre_lsb[u-1];
		else
			pre_lsb[u] = pre_lsb[u-1]+1;

	l = read_input(), k = 'z'+1;
	if ( m == 1 ) {
		puts("0");
		return 0;
	}
	if ( n <= 10000 )
		suf_sort(p,x,n,l,k);
	else {
		for ( i = 0; i < n; ++i ) {
			x[i] -= l, ++x[i];
			assert( 1 <= x[i] );
			assert( x[i] <= k-l );
		}
		x[n] = 0;
		pos = I = p, rank = V = x;
		j = transform(p,x,n,1,k-l+1,n+1);
		SAIS( (unsigned char *)x, p, n+1, j-1, sizeof *x );
		for ( i = 0; i <= n; ++i )
			V[I[i]] = i;
	}
	// for ( i = 0; i <= n; printf("%d\n",I[i]),++i );
	kasai();
	/*
	for ( i = 1; i <= n; ++i )
		printf("%d ",height[i]);
	puts("");
	*/
	for ( i = 0; i <= n; ++i )
		idx[i] = which_suff(pos[i]);

	/*
	for ( i = 0; i <= n; ++i )
		printf("(pos = %d) %d ",pos[i],idx[i]);
	puts("");
	*/
	preprocess();
	// build_tree(1,1,n);
	printf("%d\n",lcs());
	return 0;
}


