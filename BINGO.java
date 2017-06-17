/*
 * BINGO: Bullshit Bingo
 * TOPIC: regex
 * status: Accepted
 */
import java.util.regex.*;
import java.util.*;
import java.io.*;

class Main {
	private static final Pattern p = Pattern.compile("([A-Za-z]+)");
	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	public static void main( String [] args ) throws Exception {
		new Main().go();
	}
	private static long gcd( long x, long y ) {
		return 0==y?x:gcd(y,x%y);
	}
	void go() throws Exception {
		String s,t;
		int i,j,k = 0,games = 0;
		Set<String> set = new HashSet<>();
		long sum = 0, g;
		for ( ;(s = br.readLine()) != null; ) {
			for ( Matcher m = p.matcher(s); m.find(); ) 
				if ( (t = m.group(1)).equals("BULLSHIT") ) {
					++games;
					sum += set.size();
					set.clear();
				}
				else {
					++k;
					set.add(t.toLowerCase());
				}
		}
		g = gcd(sum,(long)games);
		System.out.printf("%d / %d\n",sum/g,games/g);
	}
}

