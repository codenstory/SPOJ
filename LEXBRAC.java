/*
 * LEXBRAC: Bracket Sequence
 * TOPIC: DP, very good, hard, constructive, stacks
 * status: Accepted
 */
import java.util.*;
import java.io.*;
import java.math.BigInteger;

public class LEXBRAC {

	private final static int N = 0x80;
	private final static String EMPTY = "";
	private final static String []OPENING = {"(","["}, CLOSING = {")","]"};
	private final static int OA = 0, OB = 2, CA = 1, CB = 3;
	private static final BigInteger [][]z = new BigInteger[N][N];
	private static final BigInteger []deg = new BigInteger[N];

	static {
		int i,k;
		for ( i = 0; i < N; ++i ) {
			for ( z[i][i] = BigInteger.ONE, k = i+1; k < N; ++k )
				z[i][k] = BigInteger.ZERO;
			for ( k = i-1; k >= 1; --k )
				z[i][k] = z[i][k+1].add(z[i-1][k-1]);
			if ( i >= 1 ) z[i][0] = z[i][1];
		}
		for ( deg[0] = BigInteger.ONE, i = 1; i < N; ++i )
			deg[i] = deg[i-1].add(deg[i-1]);
	}

	private BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	private BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));

	public static void main( String [] args ) throws Exception {
		new LEXBRAC().go();
	}

	private static BigInteger getPow( int k ) {
		return k<0?BigInteger.ZERO:deg[k];
	}

	void go() throws Exception {
		int i,j,k,t,n,curn,openings;
		String s;
		BigInteger m,oa,ob,ca,cb;
		char []ans = new char[2*N];
		while ( (s = br.readLine()) != null ) {
			n = Integer.parseInt(s); n >>= 1;
			m = (new BigInteger(br.readLine())).subtract(BigInteger.ONE);
			Deque<Integer> st = new LinkedList<>();
			for ( openings = 0, curn = n, i = 0; i < 2*n; ++i ) {
				assert openings <= n;
				if ( openings == n ) {
					assert !st.isEmpty();
					t=st.pollLast();
					ans[i]=(t==OA)?')':']';
					continue ;
				}

				k = st.size();

				if ( st.isEmpty() ) {
					oa = getPow(curn-k-1).multiply(z[curn][k+1]);
					if ( m.compareTo(oa) < 0 ) {
						ans[i] = '('; ++openings;
						st.addLast(OA);
						continue ;
					}
					m = m.subtract(oa);
					ans[i] = '['; ++openings;
					st.addLast(OB);
					continue ;
				}

				t = st.peekLast();
				oa = getPow(curn-k-1).multiply(z[curn][k+1]);
				if ( m.compareTo(oa) < 0 ) {
					ans[i] = '('; ++openings;
					st.addLast(OA);
					continue ;
				}
				m = m.subtract(oa);
				assert m.compareTo(BigInteger.ZERO) >= 0;
				if ( t == OA ) {
					ca = getPow((curn-1)-(k-1)).multiply(z[curn-1][k-1]);
					if ( m.compareTo(ca) < 0 ) {
						st.pollLast(); --curn;
						ans[i] = ')';
						continue ;
					}
					m = m.subtract(ca);
					assert m.compareTo(BigInteger.ZERO) >= 0;
					ob = getPow(curn-k-1).multiply(z[curn][k+1]);
					assert m.compareTo(ob) < 0: m+" "+ob;
					st.addLast(OB);
					ans[i] = '['; ++openings;
					continue ;
				}
				assert t == OB;
				ob = getPow(curn-k-1).multiply(z[curn][k+1]);
				if ( m.compareTo(ob) < 0 ) {
					st.addLast(OB);
					ans[i] = '['; ++openings;
					continue ;
				}
				m = m.subtract(ob);
				cb = getPow((curn-1)-(k-1)).multiply(z[curn-1][k-1]);
				assert m.compareTo(cb) < 0;
				st.pollLast(); --curn;
				ans[i] = ']';
			}
			for ( i = 0; i < 2*n; bw.write(ans[i++]) ) ;
			bw.write("\n");
		}
		bw.flush();
	}
};

