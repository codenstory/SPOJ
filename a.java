public class Solution {
    class Pair implements Comparable<Pair> {
        int x,y;
        Pair( int x, int y ) {
            this.x = x;
            this.y = y;
        }
        public int compareTo( Pair other ) {
            if ( this.x == other.x )
                return this.y-other.y;
            return this.x-other.x;
        }
    }
    List<Pair> allOccs( char []text, char []s ) {
        int i,j,k,n = text.length, m = s.length;
        int []p = new int[n+10+m];
        int []t = new int[m+n+10], idx = new int[m+n+10];
        for ( j = 1, i = 0; i < m; ++i )
            t[j++] = s[i];
        t[j++] = (1<<30);
        int limit = j-1;
        for ( i = 0; i < n; ++i ) {
            t[j++] = text[i++];
            idx[j-1] = i-1;
        }
        k = --j;
        for ( p[0] = j = -1, i = 1; i <= k; p[i++] = ++j )
            for ( ;j+1 != 0 && t[i] != t[j+1]; j = p[j] ) ;
        List<Pair> res = new ArrayList<>();
        for ( i = limit+1; i <= k; ++i )
            if ( p[i] == m )
                res.add(new Pair(idx[i]-m+1,idx[i]));
        return res;
    }
    public String addBoldTag(String s, String[] dict) {
        int i,j,k,m = dict.length, n = s.length();
        List<Pair> lst = new ArrayList<>();
        char []t = s.toCharArray();
        for ( i = 0; i < m; ++i ) 
            lst.addAll(allOccs(t,dict[i].toCharArray()));
        int left = 0, right = -1;
        List<Pair> res = new ArrayList<>();
        Collections.sort(lst);
        for ( i = 0; i < lst.size(); ++i ) {
            Pair r = lst.get(i);
            if ( right < r.x ) {
                if ( right <= left ) res.add(new Pair(left,right));
                left = r.x;
                right = r.y;
                continue ;
            }
            else if ( right < r.y ) right = r.y;
        }
        if ( left <= right )
            res.add(new Pair(left,right));
        StringBuilder sb = new StringBuilder();
        List<Pair> e = new ArrayList<>();
        Pair pr = null;
        for ( i = 0; i < res.size(); i = j ) {
            for ( j = i+1; j < res.size() && res.get(j).x == res.get(j-1).y+1; ++j ) ;
            e.add(new Pair(res.get(i).x,res.get(j-1).y));
        }
        List<String> result = new ArrayList<>();
        for ( i = 0; i < s.length(); ++i )
            result.add(s.charAt(i)+"");
        for ( k = 0, i = 0; i < e.size(); ++i ) {
            left = e.get(i).x;
            right = e.get(i).y;
            result.add(k+left,"<b>");
            result.add(k+right+1,"</b>");
            k += 2;
        }
        for ( i = 0; i < result.size(); ++i )
            sb.append(result.get(i));
        return sb.toString();
    }
}
