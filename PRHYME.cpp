/**
 *
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

struct cell {
    std::map<char,std::shared_ptr<cell>> son;
    std::shared_ptr<cell> p;
    bool flag;
    int card{};
    cell() { flag= false, son.clear(), p= nullptr; }
    std::string s;
};

void push( std::shared_ptr<cell> &root, const std::string &s ) {
    std::shared_ptr<cell> *hold= &root, x= root;
    for ( auto ch: s ) {
        if ( not x->son.count(ch) )
            (*hold)->son[ch]= x->son[ch]= std::make_shared<cell>();
        hold= &(x->son[ch]), (*hold)->p= x, x= x->son[ch];
    }
    x->flag= true, x->s= s, std::reverse(x->s.begin(),x->s.end());
}

int traverse( std::shared_ptr<cell> &x ) {
    if ( x ) {
        x->card= (x->flag ? 1:0);
        for ( auto &[_,v]: x->son )
            x->card+= traverse(v);
        return x->card;
    }
    return 0;
}

std::string find( std::shared_ptr<cell> &root, const std::string &s ) {
    auto x= root, y= x;
    std::shared_ptr<cell> &ans= root;
    for ( auto ch: s ) {
        if ( x->son.count(ch) ) {
            x= x->son[ch];
        } else {
            for ( auto it= x->son.begin(); not x->flag; x= it->second, it= x->son.begin() ) ;
            return x->s;
        }
    }
    if ( not x->flag ) {
        assert( not x->son.empty() );
        for ( auto it= x->son.begin(); not x->flag; x= it->second, it= x->son.begin() ) ;
        return x->s;
    } else {
        if ( not x->son.empty() ) {
            for ( auto it= x->son.begin(); not (x= it->second)->flag; it= x->son.begin() ) ;
            return x->s;
        }
        for ( y= x, x= x->p; x and (not x->flag and x->son.size() == 1); y= x, x= x->p ) ;
        assert( x );
        if ( x->flag )
            return x->s;
        assert( x->son.size() >= 2 );
        for ( auto it= x->son.begin(); it != x->son.end(); ++it )
            if ( it->second != y ) {
                x= it->second;
                for ( auto jt= x->son.begin(); not x->flag; x= jt->second, jt= x->son.begin() ) ;
                return x->s;
            }
        assert( false );
    }
    return ans->s;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::shared_ptr<cell> root= std::make_shared<cell>();
    std::string s;
    for ( ;std::getline(is,s) and not s.empty(); std::reverse(s.begin(),s.end()), push(root,s) ) ;
    for ( traverse(root); std::getline(is,s); std::reverse(s.begin(),s.end()), os << find(root,s) << '\n' ) ;
    return 0;
}
    