/*
 * TOPIC: stacks, sorting with stacks
 * status: Accepted
 */
import java.util.*;
import java.io.*;

class Main {
	Scanner scan = new Scanner(System.in);
	public static void main( String [] args ) throws Exception {
		new Main().go();
	}
	void go() throws Exception {
		Deque<Integer> approachStreet = new LinkedList<>(), sideStreet = new LinkedList<>(), mainStreet = new LinkedList<>();
		int i,j,k,n;
		for ( ;(n = scan.nextInt()) != 0; ) {
			approachStreet.clear();
			for ( i = 0; i < n; ++i )
				approachStreet.addLast(scan.nextInt());
			sideStreet.clear();
			mainStreet.clear(); mainStreet.addFirst(0);
			do { 
				i = mainStreet.peekLast()+1;
				if ( !approachStreet.isEmpty() && approachStreet.peekFirst() == i ) {
					approachStreet.pollFirst();
					mainStreet.addLast(i);
					continue ;
				}
				if ( !sideStreet.isEmpty() && sideStreet.peekLast() == i ) {
					sideStreet.pollLast();
					mainStreet.addLast(i);
					continue ;
				}
				if ( !approachStreet.isEmpty() ) {
					sideStreet.addLast(approachStreet.pollFirst());
					continue ;
				}
				break ;
			} while ( true );
			System.out.println(mainStreet.size() == n+1?"yes":"no");
		}
	}
}
