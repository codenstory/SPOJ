# INCPOWK: Increasing Powers of K
import math,sys

if __name__ == '__main__':
    ts = int(sys.stdin.readline())
    for t in range(ts):
        [K,n] = map(long,sys.stdin.readline().split())
        i = 0
        ans = 0L
        while n > 0:
            if 1 == (n&1):
                ans += long(math.pow(K,i))
            i,n = i+1,n>>1
        print ans
