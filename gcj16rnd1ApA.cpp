#include <bits/stdc++.h>
#define N 0x400
using namespace std;

int n;
char s[N];

int main() {
	int i,j,k,cs= 0,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%s",s), n= strlen(s);
		deque<char> q;
		for ( i= 0; i < n; ++i ) {
			if ( s[i] >= q.front() )
				q.push_front(s[i]);
			else q.push_back(s[i]);
		}
		stringstream ss;
		for ( ;!q.empty(); ss << q.front(), q.pop_front() ) ;
		cout << "Case #" << (++cs) << ": " << ss.str() << endl;
	}
	return 0;
}

