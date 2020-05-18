/**
 * GNY07F
 * TOPIC: binary trees, parsing, greedy
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

struct cell {
    std::shared_ptr<cell> left, right;
    int depth;
    cell() { left= right= nullptr, depth= 0; }
};

std::shared_ptr<cell> recover( const std::string &s ) {
    std::shared_ptr<cell> root= nullptr;
    std::stack<std::shared_ptr<cell>> st{};
    for ( auto ch: s ) {
        if ( ch == ']' ) {
            assert( not st.empty() );
            root= st.top(), st.pop();
            if ( not root->left ) {
                root->left= std::make_shared<cell>(), root->left->depth= root->depth+1;
            }
            if ( not root->right ) {
                root->right= std::make_shared<cell>(), root->right->depth= root->depth+1;
            }
            continue ;
        }
        assert( ch == '[' );
        if ( st.empty() ) {
            st.push(std::make_shared<cell>());
            continue ;
        }
        auto v= st.top();
        if ( not v->left ) {
            v->left= std::make_shared<cell>(), v->left->depth= v->depth+1;
            st.push(v->left);
        } else {
            assert( not v->right );
            v->right= std::make_shared<cell>(), v->right->depth= v->depth+1;
            st.push(v->right);
        }
    }
    return root;
}

void walk( const std::shared_ptr<cell> &x, std::map<int,int,std::greater<>> &cnt ) {
    if ( x ) {
        walk(x->left,cnt), walk(x->right,cnt);
        if ( not x->left and not x->right )
            ++cnt[x->depth];
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,ts,cs= 0;
    std::string s;
    for ( std::getline(is,s), ts= strtol(s.c_str(),nullptr,10); ts--; ) {
        std::getline(is,s);
        if ( s.empty() ) {
            os << ++cs << " 1\n";
            continue ;
        }
        auto root= recover(s);
        std::map<int,int,std::greater<>> cnt{};
        walk(root,cnt);
        using i64= std::int64_t;
        i64 ans= 0ll;
        i= 0;
        //for ( auto it= cnt.begin(); it != cnt.end(); ans+= it->second*(1ll<<i), ++i, ++it ) ;
        ans= (1ll<<cnt.begin()->first);
        os << ++cs << ' ' << ans << std::endl;
    }
    return 0;
}
    