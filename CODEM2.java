/*
 * TOPIC: BigInteger, Probability
 * status: Accepted
 */
import java.math.BigInteger;
import java.util.*;
import java.io.*;

class Main {
	Scanner scan = new Scanner(System.in);
	public static void main( String [] args ) throws Exception {
		new Main().go();
	}
	void go() throws Exception {
		int i,j,k,ts = scan.nextInt(),n,x,y;
		for ( ;ts-- > 0; ) {
			n = scan.nextInt();
			x = scan.nextInt();
			y = scan.nextInt();
			BigInteger r = BigInteger.valueOf(x).pow(n).add(BigInteger.valueOf(y).pow(n)), z = BigInteger.valueOf(x<y?y:x);
			BigInteger low = BigInteger.ONE, high = z.add(z), mid;
			for ( ;low.add(BigInteger.ONE).compareTo(high) < 0; ) {
				mid = (low.add(high)).divide(BigInteger.valueOf(2));
				if ( mid.pow(n).compareTo(r) < 0 )
					low = mid;
				else high = mid;
			}
			System.out.println(high.pow(n).compareTo(r) == 0?high.toString():"impossible");
		}
	}
}
