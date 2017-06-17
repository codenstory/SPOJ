import random

def f(m,n):
    return random.randint(m,n-1)

A = "abcdefghijklmnopqrstuvwxyz"

if __name__ == '__main__':
    ts = 1
    L = 100000
    n = 100000
    print ts
    for t in range(ts):
        print n
        for i in range(n):
            print f(1,L),
        print ""
