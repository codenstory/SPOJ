/**
 * CUBERT
 * TOPIC:
 * status:
 */
#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_dec_float.hpp>

template <class T>
T cbrt_2deriv_lambda(T x)
{
    // return cube root of x using 1st and 2nd derivatives and Halley.
    //using namespace std;  // Help ADL of std functions.
    using namespace boost::math::tools;
    int exponent;
    frexp(x, &exponent);                                // Get exponent of z (ignore mantissa).
    T guess = ldexp(1., exponent / 3);                    // Rough guess is to divide the exponent by three.
    T min = ldexp(0.5, exponent / 3);                     // Minimum possible value is half our guess.
    T max = ldexp(2., exponent / 3);                      // Maximum possible value is twice our guess.
    const int digits = std::numeric_limits<T>::digits;  // Maximum possible binary digits accuracy for type T.
    // digits used to control how accurate to try to make the result.
    int get_digits = static_cast<int>(digits * 0.4);    // Accuracy triples with each step, so stop when just
    // over one third of the digits are correct.
    boost::uintmax_t maxit = 20;
    T result = halley_iterate(
            // lambda function:
            [x](const T& g){ return std::make_tuple(g * g * g - x, 3 * g * g, 6 * g); },
            guess, min, max, get_digits, maxit);
    return result;
}

template <typename T>
T show_cube_root(T value, std::ostream &os )
{ // Demonstrate by printing the root using all definitely significant digits.
    os.precision(std::numeric_limits<T>::digits10);
    T r = cbrt_2deriv_lambda(value);
    os << r;
    return r;
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    using namespace boost::multiprecision;
    int ts,i,j,k,t;
    using mf= cpp_dec_float_100;
    for ( is >> ts; ts--; ) {
        mf x[2];
        std::string s;
        is >> s;
        auto R= mf(s.c_str());
        /*
        x[t= 0]= mf("1.00"), x[t^1]= mf("0.00");
        while ( fabs(x[1]-x[0]) > 1e-13 ) {
            t^= 1;
            auto z= x[t^1]*x[t^1];
            x[t]= x[t^1]-(x[t^1]*z-R)/3/z;
        }
        std::stringstream str{};
        str.precision(std::numeric_limits<mf>::digits10);
        if ( fabs(x[t]*x[t]*x[t]-R) < 1e-13 ) {
            str << x[t];
        } else {
            show_cube_root(R, str);
        }
        auto res= str.str();
        auto pos= res.find('.');
        if ( pos == std::string::npos )
            res.push_back('.'), pos= res.find('.');
        else {
            std::stringstream str2{};
            show_cube_root(R, str2);
            res= str2.str();
            pos= res.find('.');
            if ( pos == std::string::npos )
                res.push_back('.'), pos= res.find('.');
        };
         */
        std::stringstream str2{};
        show_cube_root(R, str2);
        auto res= str2.str();
        auto pos= res.find('.');
        if ( pos == std::string::npos )
           res.push_back('.'), pos= res.find('.');
        res= res.substr(0,pos+11);
        for ( ;res.size() < pos+11; res.push_back('0') ) ;
        for ( k= 0, i= 0; i < res.size(); ++i )
            if ( res[i] != '.' )
                k+= res[i]-'0', k%= 10;
        os << k << ' ' << res << '\n';
    }
    return 0;
}