/*
 * FAMILY
 * TOPIC: math. expectation
 * status: WA (same code in C++ Accepted)
 */
import java.math.BigDecimal;
import java.util.*;
import java.io.*;

class Main {
	private final static int N = 0x200;
	Scanner scan = new Scanner(System.in);
	public static void main( String [] args ) throws Exception {
		new Main().go();
	}
	BigDecimal [][]z = new BigDecimal[N][N];
	int [][]parent = new int[N][2];
	int n,m,tick;
	boolean []seen = new boolean[N];
	int []tm = new int[N];
	BigDecimal f( int x, int y ) {
		if ( z[x][y] != null )
			return z[x][y];
		if ( x == y ) 
			return z[x][y] = BigDecimal.ONE;
		if ( parent[x][0] == -1 && parent[y][0] == -1 ) 
			return z[y][x]=z[x][y]=BigDecimal.ZERO;
		if ( tm[x] < tm[y] ) {
			int k = x; x = y; y = k;
		}
		if ( parent[x][0] == -1 ) {
			int k = x; x = y; y = k;
		}
		assert parent[x][0] != -1;
		return z[y][x]=z[x][y]=f(parent[x][0],y).add(f(parent[x][1],y)).divide(BigDecimal.valueOf(2.00));
	}
	void dfs( int x ) {
		if ( seen[x] ) return ;
		if ( parent[x][0] != -1 ) {
			dfs(parent[x][0]);
			dfs(parent[x][1]);
		}
		seen[x] = true ;
		tm[x] = ++tick;
	}
	String g( String s ) {
		int i,j,k,n = s.length();
		i = s.indexOf('.');
		if ( i == -1 ) return s;
		assert i != -1;
		for ( j = i+1; j < n && s.charAt(j) == '0'; ++j ) ;
		if ( j == n ) 
			return s.substring(0,i);
		for ( j = n-1; j >= i+1 && s.charAt(j) == '0'; --j ) ;
		return s.substring(0,j+1);
	}
	void go() throws Exception {
		int l,t,i,j,k,ts = scan.nextInt();
		for (;ts-->0;) {
			n = scan.nextInt();
			m = scan.nextInt();
			for ( tick = -1, i = 0; i < n; parent[i][0]=parent[i][1]=-1, seen[i++]=false ) ;
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n;	z[i][j++] = null ) ;
			for ( l = 0; l < m; ++l ) {
				i = scan.nextInt()-1;
				parent[i][0] = scan.nextInt()-1;
				parent[i][1] = scan.nextInt()-1;
			}
			for ( i = 0; i < n; dfs(i++) ) ;
			for ( l = scan.nextInt(); l-->0; ) {
				i = scan.nextInt()-1;
				j = scan.nextInt()-1;
				String ans = f(i,j).movePointRight(2).toString();
				System.out.println(g(ans)+"%");
			}
		}
	}
}

