# 
# Rectangles
# TOPIC: dp, excellent for interview starters, smekalka
# status: Accepted
# 
import sys

if __name__ == '__main__':
    while True:
        s = sys.stdin.readline()
        if s == "":
            break 
        n = int(s)
        ans = 0
        r = 1
        while r*r <= n:
            m = n-r*r
            ans += 1+m/r
            r = r+1
        print ans
