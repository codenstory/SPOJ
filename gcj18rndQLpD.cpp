/*
 */
#include <bits/stdc++.h>
#define tol (1e-13)
#define DIM 3
#define L (0.5)
using namespace std;

double constexpr sqrt2= sqrt(2.00), sqrt3= sqrt(3.00);

struct cell {
	double x[DIM];
	cell( double *xx ) {
		for ( auto i= 0; i < DIM; x[i]= xx[i], ++i ) ;
	}
	double operator [] ( const int i ) const { return x[i]; }
	cell() {};
	cell( double a, double b, double c ) { x[0]=a,x[1]=b,x[2]=c; }
	cell operator + ( const cell &other ) const {
		return cell(x[0]+other[0],x[1]+other[1],x[2]+other[2]);
	}
	cell operator - ( const cell &other ) const {
		return cell(x[0]-other[0],x[1]-other[1],x[2]-other[2]);
	}
	cell operator * ( const double t ) const {
		return cell(x[0]*t,x[1]*t,x[2]*t);
	}
	cell operator / ( const double t ) const {
		return cell(x[0]/t,x[1]/t,x[2]/t);
	}
	cell rotate_around_absciss( double t ) const {
		double c= cos(t), s= sin(t);
		return cell(x[0],x[1]*c-x[2]*s,x[1]*s+x[2]*c);
	}
	friend ostream &operator << ( ostream &out, const cell &t ) ;
};

ostream &operator << ( ostream &out, const cell &t ) {
	char sx[DIM][0x40];
	for ( auto i= 0; i < DIM; ++i )
		sprintf(sx[i],"%.9lf",t[i]);
	out << string(sx[0]) << " " << string(sx[1]) << " " << string(sx[2]);
	return out;
}

cell median( cell &a, cell &b, cell &c ) { return ((a+b)+c)/3.00; }

double A;
cell c[8],q[8];

int main() {
	int i,j,k,cs= 0,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%lf",&A);
		double phi= acos(A/sqrt3)+acos(sqrt(2.00/3.00));
		c[0]= cell(sqrt2/2,0,L), c[1]= cell(0,sqrt2/2,L), c[2]= cell(-sqrt2/2,0,L), c[3]= cell(0,-sqrt2/2,L);
		c[4]= cell(sqrt2/2,0,-L), c[5]= cell(0,sqrt2/2,-L), c[6]= cell(-sqrt2/2,0,-L), c[7]= cell(0,-sqrt2/2,-L);
		for ( i= 0; i < 8; ++i )
			q[i]= c[i].rotate_around_absciss(phi);
		cell x= median(q[0],q[1],q[2]),
			 y= median(q[0],q[4],q[5]),
			 z= median(q[1],q[2],q[6]);
		x= (q[0]+q[2])/2, y= (q[0]+q[5])/2, z= (q[1]+q[6])/2;
		printf("Case #%d:\n",++cs);
		cout << x << "\n" << y << "\n" << z << "\n";
	}
	return 0;
}

