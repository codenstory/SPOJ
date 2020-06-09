# MOVIE
# TOPIC: combinatorics, recurrent formulas
# status: Accepted
import sys, math

numrows = 122
numcols = 202


z = [[0 for j in range(numcols)] for i in range(numrows)]
a = [[-1 for j in range(numcols)] for i in range(numrows)]
seen = [[False for j in range(numcols)] for i in range(numrows)]
c = [[0 for j in range(0x100)] for i in range(0x100)]
powers = [[1 for j in range(0x400)] for i in range(0x400)]


def mypow( x, n ):
    ax = 1
    while n > 0:
        if 1 == (n&1):
            ax*= x
        x*= x
        n>>=1
    return ax


def calc_a( k,m ):
    if a[k][m] >= 0:
        return a[k][m]
    a[k][m] = 0
    for i in range(k+1):
        a[k][m]+= c[k][i]*(k-i)*calc_z(k-i,k-i)*calc_z(i,m-(k-i+1))
    return a[k][m]


def calc_z( n, m ):
    if m < 0 or n < 0:
        return 0
    if seen[n][m]:
        return z[n][m]
    seen[n][m]= True
    if n > m:
        z[n][m]= 0
        return z[n][m]
    if n == 0:
        z[n][m]= 1
        return z[n][m]
    if m == 0:
        z[n][m]= 0
        return z[n][m]

    assert m >= 1 and n >= 1 and m >= n

    z[n][m]= powers[m][n]
    for k in range(1,n):
        z[n][m]-= powers[m][n-k-1]*calc_a(k,m)
    return z[n][m]

def mgcd(x,y):
    if y == 0:
        return x
    return mgcd(y,x%y)

if __name__ == '__main__':
    data = sys.stdin.readlines()
    numlines= len(data)
    ts = int(data[0])
    k = 1
    for i in range(0x100):
        c[i][0] = 1
    for i in range(1,0x100):
        for j in range(1,0x100):
            c[i][j]= c[i-1][j]+c[i-1][j-1]
    for n in range(1,numrows):
        for m in range(1,numcols):
            powers[m][n]= powers[m][n-1]*m
    for cs in range(ts):
        while k < numlines and data[k] == "\n":
            k= k+1
        assert k < numlines
        [n,m]= list(map(int,data[k].split()))
        # print("%d and %d" %(m,n))
        k= k+1
        if n > m:
            print("1/1")
            continue
        assert n <= m
        ax = mypow(m,n)
        upper, lower= ax-calc_z(n,m), ax
        # print("Preliminary: %d and %d" %(upper,lower))
        g = mgcd(lower,upper)
        assert lower%g == 0
        assert upper%g == 0
        lower//=g
        upper//=g
        print("%d/%d" %(upper,lower))
