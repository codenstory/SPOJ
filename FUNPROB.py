# Ballot numbers
import sys,math

def f(n,m):
    return (m+1.0-n)/(m+1.0)


if __name__ == '__main__':
    while True:
        [n,m] = map(lambda x: int(x), sys.stdin.readline().split())
        if m > 0 or n > 0:
            print "%.6f" %(max(0.00,f(n,m)))
        else:
            break
