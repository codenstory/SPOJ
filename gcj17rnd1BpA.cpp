#include <bits/stdc++.h>
#define tol 1e-9
#define oo (DBL_MAX-0x400)
#define N 0x400
using namespace std;
#define S first
#define K second

int n;
double D;
vector<pair<double,double>> vec;
stack<int> st;

struct comparator {
	bool operator () ( pair<double,double> &a, pair<double,double> &b ) const {
		if ( fabs(a.first-b.first) < tol )
			return a.second < b.second;
		return a.first > b.first;
	}
};

double meeting_time( pair<double,double> &a, pair<double,double> &b ) {
	double t= (b.K-a.K)/(a.S-b.S);
	return t;
}

double meeting_point( pair<double,double> &a, pair<double,double> &b ) {
	double t= meeting_time(a,b);
	return t*a.S+a.K;
}

int main() {
	int i,j,k,ts,cs= 0,m,sz;
	for ( scanf("%d",&ts); ts--; ) {
		for ( vec.clear(), scanf("%lf %d",&D,&n), vec.reserve(n), i = 0; i < n; ++i )
			scanf("%lf %lf",&vec[i].K,&vec[i].S);
		sort(vec.begin(),vec.end(),comparator());
		for ( j = 1; j; )
			for ( i = 0, j = 0; i < n-1; ++i )
				if ( (comparator()(vec[i+1],vec[i])) ) {
					swap(vec[i],vec[i+1]);
					j=1;
				}
		/*
		for ( i = 0; i < n; ++i )
			printf("Here %lf %lf\n",vec[i].S,vec[i].K);
			*/
		vector<pair<double,double>> u;
		for ( u.clear(), i= 0; i < n; u.push_back(vec[i]), i= j ) 
			for ( j= i+1; j < n && fabs(vec[i].S-vec[j].S) < tol; ++j ) ;
		for ( ;!st.empty(); st.pop() ) ;
		for ( i= 0; i < (int)u.size(); ++i ) {
			while ( st.size() >= 2 ) {
				j= st.top(), st.pop(), k= st.top();
				if ( meeting_point(u[i],u[j]) > meeting_point(u[k],u[j]) ) {
					st.push(j);
					break ;
				}
			}
			st.push(i);
		}
		vector<pair<pair<double,double>,double>> line(st.size()+8);
		m= st.size()-1, sz= st.size();
		for ( ;st.size() >= 2; ) {
			j= st.top(), st.pop(), k= st.top();
			line[m--]= {{meeting_point(u[j],u[k]),meeting_time(u[j],u[k])},u[j].S};
		}
		j= st.top(), st.pop();
		line[m--]= {{u[j].K,0},u[j].S};
		for ( k= sz-1; k >= 0 && line[k].first.first >= D; --k ) ;
		assert( k >= 0 );
		pair<double,double> pnt;
		pnt.first= D, pnt.second= line[k].first.second+(D-line[k].first.first)/line[k].second;
		printf("Case #%d: %.6lf\n",++cs,pnt.first/pnt.second);
	}
	return 0;
}

