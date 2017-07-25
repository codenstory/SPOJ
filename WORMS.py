# Accepted
n,a,z=501,{},{}
for i in range(1,7):
    a[1,i]=0L
a[1,0]=z[1,0]=z[1,3]=z[1,4]=z[1,5]=z[1,6]=1L
z[1,1]=z[1,2]=2L
for m in range(2,n):
    for r in range(7):
        z[m,r]=a[m,r]=0
        for t in range(10):
            z[m,r]+=z[m-1,5*(r-t+7)%7]
            if t!=7:
                a[m,r]+=a[m-1,5*(r-t+7)%7]
        a[m,r]+=z[m-1,5*r%7]
for r in range(7):
    for m in range(2,n):
        a[m,r]+=a[m-1,r]
for k in range(input()):
    n=int(input())
    print 10**n/7+10**n-9**n-a[n,0]
