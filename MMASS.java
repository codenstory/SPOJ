/*
 * TOPIC: regex, parsing
 * status: Accepted
 */
import java.util.*;
import java.util.regex.*;
import java.io.*;

class Main {
	private final BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	private static final Pattern p1 = Pattern.compile("\\(([()HCO0-9]+)\\)([2-9])");
	private static final Pattern p2 = Pattern.compile("\\(([()HCO0-9]+)\\)");
	private static final Pattern p3 = Pattern.compile("([0-9H,C,O]+)");
	private static final Pattern num  = Pattern.compile("(\\d+)");
	private static final Map<String,Integer> mass = new HashMap<>();
	static {
		mass.put("H",1);
		mass.put("C",12);
		mass.put("O",16);
	}
	private static void myAssert( boolean x ) {
		if ( !x ) {
			int trap = 1/0;
		}
	}
	private int f( String s ) {
		int ans = 0,k;
		if ( s == null || s.equals("") ) return ans;
		Matcher m;
		Deque<String> q = new LinkedList<>();
		s = s.replaceAll("\\s+","");
		s = s.replaceAll("[^2-9,H,C,O,(,)]","");
		s = s.replaceAll("\\d*(\\d)","$1");
		if ( s.equals("") ) return ans;
		//System.out.println(s);
		if ( s.charAt(0) == '(' ) {
			for ( int i = 0; i < s.length(); ++i )
				if ( s.charAt(i) == '(' )
					q.addLast("(");
				else if ( s.charAt(i) == ')' ) {
					q.pollLast();
					if ( !q.isEmpty() ) continue ;
					if ( i+1 < s.length() && '0' <= s.charAt(i+1) && s.charAt(i+1) <= '9' ) {
						Matcher mm = num.matcher(s.substring(i+1));
						mm.find();
						return f(s.substring(0,i+1))*Integer.parseInt(mm.group(1))+f(s.substring(i+1+mm.end()));
					}
					else return f(s.substring(1,Math.min(i+1,s.length())-1))+(i+1<s.length()?f(s.substring(i+1)):0);
				}
		}
		else {
			myAssert( s.charAt(0) == 'H' || s.charAt(0) == 'C' || s.charAt(0) == 'O' );
			m = p3.matcher(s);
			if ( m.find() ) {
				String t = m.group(1);
				for ( int i = 0; i < t.length(); ++i ) 
					if ( '0' <= t.charAt(i) && t.charAt(i) <= '9' ) {
						for ( k = 0; i < t.length() && '0' <= t.charAt(i) && t.charAt(i) <= '9'; ++i )
							k = 10*k+(t.charAt(i)-'0');
						--i; ans += k*mass.get(q.pollLast());
					}
					else q.addLast(t.charAt(i)+"");
				for ( ;!q.isEmpty(); ans += mass.get(q.pollLast()) ) ;
				return ans+f(s.substring(m.end()));
			}
		}
		return ans;
	}
	public static void main( String [] args ) throws Exception {
		new Main().go();
	}
	void go() throws Exception {
		String s; s = br.readLine(); System.out.println(f(s));
	}
}

