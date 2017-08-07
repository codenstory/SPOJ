/*
 * TREASURY
 * TOPIC: dp on tree, combinatorics, sum rule, product rule
 * status: TLE
 */
import java.io.*;
import java.util.*;

class Main {

	static class Reader
    {
        final private int BUFFER_SIZE = 1 << 21;
        private DataInputStream din;
        private byte[] buffer;
        private int bufferPointer, bytesRead;
		private boolean ok = true;

        public Reader()
        {
            din = new DataInputStream(System.in);
            buffer = new byte[BUFFER_SIZE];
            bufferPointer = bytesRead = 0;
        }

        public Reader(String file_name) throws IOException
        {
            din = new DataInputStream(new FileInputStream(file_name));
            buffer = new byte[BUFFER_SIZE];
            bufferPointer = bytesRead = 0;
        }

        public String readLine() throws IOException
        {
            byte[] buf = new byte[64]; // line length
            int cnt = 0, c;
            while ((c = read()) != -1)
            {
                if (c == '\n')
                    break;
                buf[cnt++] = (byte) c;
            }
            return new String(buf, 0, cnt);
        }

        public int nextInt() throws IOException
        {
            int ret = 0;
            byte c = read();
			if ( c == -1 ) return -1;
            while (c <= ' ')
                c = read();
            boolean neg = (c == '-');
            if (neg)
                c = read();
            do
            {
                ret = ret * 10 + c - '0';
            }  while ((c = read()) >= '0' && c <= '9');

            if (neg)
                return -ret;
            return ret;
        }

        public long nextLong() throws IOException
        {
            long ret = 0;
            byte c = read();
            while (c <= ' ')
                c = read();
            boolean neg = (c == '-');
            if (neg)
                c = read();
            do {
                ret = ret * 10 + c - '0';
            }
            while ((c = read()) >= '0' && c <= '9');
            if (neg)
                return -ret;
            return ret;
        }

        public double nextDouble() throws IOException
        {
            double ret = 0, div = 1;
            byte c = read();
            while (c <= ' ')
                c = read();
            boolean neg = (c == '-');
            if (neg)
                c = read();

            do {
                ret = ret * 10 + c - '0';
            }
            while ((c = read()) >= '0' && c <= '9');

            if (c == '.')
            {
                while ((c = read()) >= '0' && c <= '9')
                {
                    ret += (c - '0') / (div *= 10);
                }
            }

            if (neg)
                return -ret;
            return ret;
        }

        private void fillBuffer() throws IOException
        {
            bytesRead = din.read(buffer, bufferPointer = 0, BUFFER_SIZE);
            if (bytesRead == -1) {
                buffer[0] = -1;
				ok = false;
			}
        }

        private byte read() throws IOException
        {
			if ( !ok ) return -1;
            if (bufferPointer == bytesRead)
                fillBuffer();
            return buffer[bufferPointer++];
        }

        public void close() throws IOException
        {
            if (din == null)
                return;
            din.close();
        }
    }
	private static final int N = 0x400, MAXE = (1<<15), S = 0, T = 1, oo = (1<<30);
	static private BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));
	static Reader br = new Reader();
	static private char []c = new char[1<<21];
	static private int len, cur, yes;

	static private int []present = new int[N], id = new int[N];

	private static int find( int x ) {
		if ( present[x] == yes )
			return id[x];
		last[id[x]=V++]=-1;
		present[x]=yes;
		return id[x];
	}

	private static class MyBigInteger {
		//private static int BASE = 1000000000, DIGL = 9, LEN = 15;
		private static int DIGL = 9, LEN = 14;
		private static long BASE = (1L<<30), MASK = (BASE-1L);
		private static String fmt = "%09d";
		private long []x = new long[LEN];
		static MyBigInteger ZERO = new MyBigInteger(0), ONE = new MyBigInteger(1);
		private int len;
		static void reset( int n ) {
			if ( n < 25 )
				LEN = 2;
			else if ( n < 50 )
				LEN = 2;
			else if ( n < 100 )
				LEN = 4;
			else if ( n < 300 )
				LEN = 5;
			else if ( n < 701 )
				LEN = 6;
			else if ( n < 1001 )
				LEN = 10;
			else LEN = 15;
		}
		private MyBigInteger( int k ) {
			int n = 0;
			do { 
				x[n++] = (k%MASK);
			} while ( (k>>=31) > 0 );
			len = n;
		}
		private void normalize() {
			for ( int i = 0; i < len; ++i )
				if ( x[i] >= BASE ) {
					x[i+1] += (x[i]>>30);
					x[i] &= MASK;
				}
			for ( int i = len; i < LEN && x[i] >= BASE; ++i ) {
				x[i+1] += (x[i]>>30);
				x[i] &= MASK;
			}
			for ( ;len > 0 && x[len-1] == 0; --len ) ;
			if ( len == 0 ) len = 1;
		}

		int divide( int k ) {
			long r = 0,tmp;
			for ( int i = len-1; i >= 0; --i ) {
				tmp = (BASE*r+x[i])%k;
				assert x[i] >= 0: x[i];
				x[i] = (BASE*r+x[i])/k;
				assert x[i] >= 0: x[i];
				r = tmp;
			}
			normalize();
			assert 0 <= r;
			assert r < k;
			return (int)r;
		}

		MyBigInteger add( MyBigInteger other ) {
			MyBigInteger res = new MyBigInteger(0);
			int i;
			for ( i = 0; i < len && i < other.len; ++i ) 
				res.x[i] += x[i]+other.x[i];
			for ( ;i < len; res.x[i] += x[i], ++i ) ;
			for ( ;i < other.len; res.x[i] += other.x[i], ++i ) ;
			res.len = Math.min(LEN,Math.max(len,other.len)+1);
			res.normalize();
			return res;
		}

		void addInPlace( MyBigInteger other ) {
			int i;
			for ( i = 0; i < len && i < other.len; ++i ) 
				x[i] += other.x[i];
			for ( ;i < other.len; x[i] += other.x[i], ++i ) ;
			len = Math.min(LEN,Math.max(len,other.len)+1);
			normalize();
		}

		MyBigInteger multiply( MyBigInteger other ) {
			MyBigInteger res = new MyBigInteger(0);
			for ( int i = 0; i < len; ++i )
				for ( int j = 0; j < other.len && i+j < LEN; ++j ) 
					res.x[i+j] += x[i]*other.x[j];
			res.len = Math.min(LEN,len+other.len);
			res.normalize();
			return res;
		}

		void multiplyIP( MyBigInteger other ) {
			long []res = new long[LEN];
			for ( int i = 0; i < len; ++i )
				for ( int j = 0; j < other.len && (i+j) < LEN; ++j ) 
					res[i+j] += x[i]*other.x[j];
			int nlen = Math.min(LEN,len+other.len);
			for ( int i = 0; i < nlen; x[i] = res[i], ++i ) ;
			len = nlen;
			normalize();
		}

		public void toStr( BufferedWriter bw ) throws Exception {
			int i;
			for ( i = len-1; i >= 0 && x[i] == 0; --i ) ;
			if ( i < 0 ) {
				bw.write("0\n");
				return;
			}
			char []c = new char[0x80+1];
			int k = 0x80, l = 0;
			for(;!(len==1&&x[len-1]==0);c[k--]=(char)(divide(10)+'0'),++l);
			bw.write(c,k+1,l);
			bw.write("\n");
		}

		public boolean equals( Object obj ) {
			MyBigInteger o;
			if ( !(obj instanceof MyBigInteger) )
				return false ;
			o = (MyBigInteger)obj;
			for ( int i = 0; i < LEN; ++i )
				if ( x[i] != o.x[i] )
					return false;
			return true;
		}
	}

	private static int []next = new int[MAXE], to = new int[MAXE], last = new int[N], L = new int[N];
	private static int V,n,E;

	private static void addArcs( int x, int y ) {
		int i = E++;
		to[i] = y; next[i] = last[x]; last[x] = i;
	}

	private static MyBigInteger [][]ways = new MyBigInteger[2][N], dp = new MyBigInteger[N][N], pref = new MyBigInteger[N][N], post = new MyBigInteger[N][N];
	private static int [][]z = new int[2][N];

	public static void main( String [] args ) throws Exception {
		go();
	}

	static int [][]adj = new int[N][];
	static int []deg = new int[N];

	static void makeGraph() {
		int i,j,k,l;
		for ( i = 0; i < V; deg[i++] = 0 ) ;
		for ( j = 0; j < V; ++j ) {
			for ( k = 0, i = last[j]; i >= 0; i = next[i], ++k ) ;
			adj[j] = new int[k];
			for ( l = 0, i = last[j]; i >= 0; adj[j][l++] = to[i], i = next[i] ) ;
			assert l == k;
			deg[j] = l;
		}
	}

	static int calcZ( int t, int x ) {
		int i,y,k,l;
		if ( z[t][x] < +oo )
			return z[t][x];
		if ( t == 0 ) {
			for ( k=1, z[t][x] = 0, ways[t][x] = new MyBigInteger(1), pref[x][0] = MyBigInteger.ONE, i = last[x]; i >= 0 && (y=to[i]) >= 0; i = next[i], ++k ) {
				z[t][x] += Math.max(calcZ(0,y),calcZ(1,y));
				if ( z[0][y] == z[1][y] ) 
					ways[t][x].multiplyIP(dp[x][y]=ways[0][y].add(ways[1][y]));
				else if ( z[0][y] > z[1][y] )
					ways[t][x].multiplyIP(dp[x][y]=ways[0][y]);
				else 
					ways[t][x].multiplyIP(dp[x][y]=ways[1][y]);
				pref[x][k] = pref[x][k-1].multiply(dp[x][y]);
			}
			for ( l = deg[x]-1, L[x] = k-1, post[x][k] = MyBigInteger.ONE; l >= 0; --k )
				post[x][k-1] = post[x][k].multiply(dp[x][adj[x][l--]]);
			/*
			assert pref[x][L[x]].equals(ways[t][x]);
			assert post[x][1].equals(ways[t][x]);
			*/
		}
		else {
			for ( z[t][x] = 0, ways[t][x] = new MyBigInteger(0), i = last[x]; i >= 0 && (y=to[i]) >= 0; i = next[i] ) 
				if ( z[t][x] < 1+calcZ(0,y)+calcZ(0,x)-Math.max(calcZ(0,y),calcZ(1,y)) ) 
					z[t][x] = 1+calcZ(0,y)+calcZ(0,x)-Math.max(calcZ(0,y),calcZ(1,y));
			for ( k = 1, i = last[x]; i >= 0 && (y=to[i]) >= 0; i = next[i], ++k ) 
				if ( z[t][x] == 1+z[0][y]+z[0][x]-Math.max(z[0][y],z[1][y]) ) 
					ways[t][x].addInPlace(ways[0][y]==dp[x][y]?ways[0][x]:ways[0][y].multiply(pref[x][k-1].multiply(post[x][k+1])));
		}
		return z[t][x];
	}

	static void go() throws Exception {
		int i,j,k,l,t;
		Deque<Integer> lst = new LinkedList<>();
		for ( ;(n = br.nextInt()) != -1; ) {
			V = E = 0; ++yes;
			MyBigInteger.reset(++n);
			for ( i=find(0),j=find(1), addArcs(i,j), l = 0; l < n-1; ++l ) 
				for ( lst.addLast(i = find(br.nextInt())), k = br.nextInt(); k-- > 0; j = find(br.nextInt()), addArcs(i,j) ) ;
			makeGraph();
			for ( t = 0; t <= 1; ++t )
				for ( i = 0; i < V; z[t][i++] = +oo ) ;
			for (;!lst.isEmpty();) 
				for ( i = lst.pollLast(), t = 0; t <= 1; ++t )
					calcZ(t,i);
			bw.write(calcZ(0,0)+"\n");
			ways[0][0].toStr(bw);
		}
		bw.flush();
	}
}

