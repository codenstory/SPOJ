/**
 * SWTHIN
 * TOPIC: sorting, polar angles
 * status: Accepted
 */
#include <bits/stdc++.h>
#define tol (1e-7)
#define pi (2*acos(0.00))

using cell= std::pair<double,double>;
std::istream &is= std::cin;

double operator ^ ( const cell &a, const cell &b ) {
    auto u= b.first-a.first,
         v= b.second-a.second;
    auto len= hypot(u,v);
    auto co= u/len, si= v/len;
    if ( co >= 0 and si >= 0 )
        return acos(co);
    if ( co >= 0 and si <= 0 )
        return asin(si);
    if ( co <= 0 and si >= 0 )
        return acos(co);
    return -pi+acos(-co);
}

int main() {
    int n,i,j,k,ans,cs= 0;
    std::vector<cell> v;
    for ( ;(is >> n) and n; printf("Photo %d: %d points eliminated\n",++cs,ans+1>=4?ans+1:0) ) {
        v.resize(n);
        for ( auto &z: v ) is >> z.first >> z.second;
        for ( ans= 0, i= 0; i < n; ++i ) {
            std::vector<double> angles{};
            for ( j= 0; j < n; ++j )
                if (not(i==j or std::max(fabs(v[i].first-v[j].first),fabs(v[i].second-v[j].second))<tol))
                    angles.push_back(v[i]^v[j]);
            std::sort(angles.begin(),angles.end());
            for ( j= 0; j < angles.size(); ans= std::max(ans,k-j), j= k )
                for ( k= j+1; k < angles.size() and fabs(angles[j]-angles[k]) < tol; ++k ) ;
        }
    }
    return 0;
}
