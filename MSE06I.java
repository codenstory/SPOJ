/*
 * TOPIC: mincost flow
 * status: Accepted
 */
import java.io.*;
import java.util.*;

/**
 * Created by sj on 27/04/17.
 */
public class MSE06I {
    private final static int N = 0x80, MAXE = (1<<21), MAXV = N, oo = (1<<30);
    int n,V,E,cnt;
    int []last = new int[MAXE], next = new int[MAXE], to = new int[MAXE], d = new int[MAXV], cost = new int[MAXE], cap = new int[MAXE], flow = new int[MAXE];
    int []heap = new int[MAXV<<1], pos = new int[MAXV];
    int []pi = new int[MAXV], p = new int[MAXV];
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out));
    public static void main( String ... args ) throws Exception {
        System.setIn(new FileInputStream(new File("/home/sj/IdeaProjects/MSE06I/src/input.txt")));
        System.setOut(new PrintStream(new File("/home/sj/IdeaProjects/MSE06I/src/output.txt")));
        new MSE06I().go();
    }
    char[] buff;
    int cur;

    int getInt() {
        int n = 0,sign = 1;
        while ( !('0' <= buff[cur] && buff[cur] <= '9' || buff[cur] == '-') ) ++cur ;
        if ( buff[cur] == '-' ) {
            sign = -1;
            ++cur;
        }
        for ( n = buff[cur]-'0'; ++cur < buff.length && ('0' <= buff[cur] && buff[cur] <= '9'); n = 10*n+buff[cur]-'0' ) ;
        return sign*n;
    }

    void addArcs( int x, int y, int c, int w ) {
        int i = E++, j = E++;
        to[i] = y; next[i] = last[x]; last[x] = i; cap[i] = c; cost[i] = w;
        to[j] = x; next[j] = last[y]; last[y] = j; cap[j] = 0; cost[j] = 0;
        flow[i] = flow[j] = 0;
    }
    void bubble( int i, int j ) {
        int t = heap[i];
        heap[i] = heap[j];
        heap[j] = t;
        t = pos[heap[i]]; pos[heap[i]] = pos[heap[j]]; pos[heap[j]] = t;
    }
    void push( int x ) {
        int i,j;
        if ( pos[x] < 0 ) {
            pos[heap[cnt] = x] = cnt;
            ++cnt;
        }
        for ( j = pos[x]; j > 0 && d[heap[i = (j-1)>>1]] > d[heap[j]]; bubble(i,j), j = i ) ;
    }
    int pop() {
        int i,j,x = heap[0];
        if ( (cnt += (pos[x] = -1)) > 0 )
            pos[heap[0] = heap[cnt]] = 0;
        for ( j = 0; ; bubble(i,j) ) {
            i = j; j <<= 1; ++j;
            if ( j >= cnt ) break ;
            if ( j < cnt-1 && d[heap[j]] > d[heap[j+1]] ) ++j;
            if ( d[heap[i]] <= d[heap[j]] ) break ;
        }
        return x;
    }
    boolean dijkstra( int src, int sink ) {
        int x,y,i;
        for ( cnt = 0, x = 0; x < n+n; d[x] = +oo, pos[x++] = -1 ) ;
        for ( p[src] = -1, d[src] = 0, push(src); cnt > 0; )
            for ( i = last[x = pop()]; i >= 0 && (y=to[i]) >= 0; i = next[i] )
                if ( flow[i] < cap[i] && d[y] > d[x]+pi[x]+cost[i]-pi[y] ) {
                    d[y] = d[x]+pi[x]-pi[y]+cost[i];
                    p[y] = i;
                    push(y);
                }
        for ( x = 0; x < n+n; ++x )
            if ( d[x] < +oo )
                pi[x] += d[x];
        return d[sink] < +oo;
    }
    int mincostMaxflow( int src, int sink, int T ) {
        int w = 0, fl = 0, x,i;
        for ( x = 0; x < n+n; pi[x++] = 0 ) ;
        for ( ;fl < T && dijkstra(src,sink); ++fl ) {
            for (i = p[sink], x = to[i ^ 1]; ; ) {
                ++flow[i];
                --flow[i ^ 1];
                //System.out.printf("%d --> %d\n", to[i], to[i ^ 1]);
                if (x == src) break;
                i = p[x];
                x = to[i ^ 1];
            }
        }
        for ( i = 0; i < E; ++i )
            if ( flow[i] > 0 )
                w += flow[i]*cost[i];
        return fl == T ? w : +oo;
    }
    void go() throws Exception {
        int i, j, k, t, cs = 0;
        for ( String s; (s = br.readLine()) != null;) {
            buff = s.toCharArray(); cur = 0;
            n = getInt();
            k = getInt();
            if ( n == 0 && k == 0 ) break ;
            for ( E = 0, i = 0; i < n+n; last[i++] = -1 ) ;
            for ( ;k-->0; ) {
                buff = (s = br.readLine()).toCharArray(); cur = 0;
                i = getInt();
                j = getInt();
                t = getInt();
                if ( i == j ) continue ;
                //addArcs(i+n,j,1,t);
                addArcs(i,j,1,t);
            }
            /*
            for ( i = 0; i < n; ++i )
                if ( i != 0 && i != n-1 )
                    addArcs(i, i + n, 1, 0);
                else addArcs(i, i + n, 2, 0);
                */
            bw.write(String.format("Instance #%d:  ",++cs));
            //t = mincostMaxflow(0,n-1+n,2);
            t = mincostMaxflow(0,n-1,2);
            if ( t < +oo )
                bw.write(t+"\n");
            else bw.write("Not possible\n");
        }
        bw.flush();
    }
}

