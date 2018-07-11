/*
 */
import java.util.*;
import java.math.BigInteger;

public class gcj10rndQLpB {
	private Scanner scan= new Scanner(System.in);
	private BigInteger []d,t;
	private BigInteger T;
	private int n;
	public static void main( String [] args ) { new gcj10rndQLpB().go(); }
	private void go() {
		int i,j,k,cs= 0,ts= scan.nextInt();
		for ( ;ts-->0; ) {
			n= scan.nextInt();
			t= new BigInteger[n];
			d= new BigInteger[n*(n-1)/2];
			for ( i= 0; i < n; t[i++]= new BigInteger(scan.next()) ) ;
			for ( k= 0, i= 0; i < n; ++i )
				for ( j= i+1; j < n; ++j ) {
					if ( t[i].compareTo(t[j]) < 0 )
						d[k++]= t[j].subtract(t[i]);
					else d[k++]= t[i].subtract(t[j]);
				}
			for ( T= d[0], i= 1; i < k; T= T.gcd(d[i++]) ) ;
			BigInteger remainder= t[0].mod(T), y= T.subtract(remainder);
			if ( remainder.equals(BigInteger.ZERO) ) y= BigInteger.ZERO;
			System.out.printf("Case #%d: %s\n",++cs,y.toString());
		}
	}
};

