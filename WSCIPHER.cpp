/**
 * WSCIPHER
 * TOPIC: simulation, STL, rotate
 * status: Accepted
 */
#include <bits/stdc++.h>

std::vector<std::set<char>> group={
        {'a','b','c','d','e','f','g','h','i'},
        {'j','k','l','m','n','o','p','q','r'},
        {'s','t','u','v','w','x','y','z','_'}
};

std::istream &is= std::cin;

int main() {
    int i,j,k[3];
    for ( std::string s; std::getline(is,s); ) {
        if ( 3 != sscanf(s.c_str(),"%d %d %d",k,k+1,k+2) )
            break ;
        if ( not k[0] and not k[1] and not k[2] )
            break ;
        std::getline(is,s);
        std::vector<int> idx(s.size());
        std::iota(idx.begin(),idx.end(),0);
        auto buff= s;
        for ( i= 0; i < 3; ++i ) {
            const auto &st= group[i];
            std::vector<int> pos{};
            std::copy_if(idx.begin(),idx.end(),std::back_inserter(pos),[&st,&s](auto t) {
                return st.count(s[t]);
            });
            auto indices= pos;
            if ( indices.empty() ) continue ;
            k[i] %= indices.size();
            std::rotate(indices.rbegin(),indices.rbegin()+k[i],indices.rend());
            for ( j= 0; j < pos.size(); ++j )
                buff[pos[j]]= s[indices[j]];
        }
        std::cout << buff << std::endl;
    }
    return 0;
}
