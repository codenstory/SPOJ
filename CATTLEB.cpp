/**
 * CATTLEB
 * TOPIC: simulation, maths
 * status:
 */
#include <bits/stdc++.h>
#define N (50100)
#define tol (1e-6)
#define PQ 0

struct plate {
    std::pair<double,double> p;
    std::pair<double,double> v;
};

struct bruiser: plate {
    double R;
    bruiser() {};
    bruiser( double r ): plate() { this->R= r; }
};

std::istream &operator >> ( std::istream &is, plate &z ) {
    return is >> z.p.first >> z.p.second >> z.v.first >> z.v.second;
}

using time_interval= std::pair<double,double>;

std::pair<double,double> operator - ( const std::pair<double,double> &a, const std::pair<double,double> &b ) {
    return {a.first-b.first,a.second-b.second};
}

double operator * ( const std::pair<double,double> &a, const std::pair<double,double> &b ) {
    return a.first*b.first + a.second*b.second;
}

int n;
double R;
plate s;
bruiser e[N];

constexpr time_interval empty_interval= {1,0};

time_interval operator * ( const plate &S, const bruiser &r ) {
    auto coeff= S.p-r.p, velo= S.v-r.v;
    double a= std::hypot(velo.first,velo.second);
    if ( fabs(a) < tol ) { // they are moving at the same speed
        // we check if "s" is initially inside "r"
        auto diff= S.p-r.p;
        auto dx= diff.first, dy= diff.second;
        return dx*dx+dy*dy<=r.R*r.R ? std::make_pair(std::numeric_limits<double>::min(),std::numeric_limits<double>::max()) :
            empty_interval;
    }
    double b= 2*(coeff*velo);
    double c= std::hypot(coeff.first,coeff.second)-r.R*r.R;
    double discr= b*b-4*a*c;
    if ( discr <= -tol )
        return empty_interval;
    double sq= sqrt(discr);
    double t1= (-b-sq)/2/a, t2= (-b+sq)/2/a;
    return {t1,t2};
}

struct event {
    enum class event_type { opening, closing };
    int id;
    double timestamp;
    event_type type;
};

// the natural order of the events
// we would like to process them in
bool operator < ( const event &a, const event &b ) {
    if ( fabs(a.timestamp-b.timestamp) < tol ) {
        if ( a.type == b.type )
            return a.id < b.id;
        return a.type == event::event_type::opening;
    }
    return a.timestamp < b.timestamp;
}

// since C++ STL priority_queue<> returns the "max" element,
// we need to reverse our natural comparison
struct comparator {
    bool operator ()( const event &a, const event &b ) const {
        return b < a;
    }
};

// id-comparator for status-line
// just compare the ids, since in the status line
// only the existence/non-existence matters
struct id_comparator {
    bool operator ()( const event &a, const event &b ) const {
        return a.id < b.id;
    }
};

#if PQ
std::priority_queue<event,std::vector<event>,comparator> pq;
#else
std::set<event> pq;
#endif

int main() {
    std::cin.tie(nullptr), std::ios_base::sync_with_stdio(false);
    int i,j,k,ans;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for (;std::cin>>n;) {
        assert( pq.empty() );
        for ( std::cin >> R >> s, i= 0; i < n; std::cin >> e[i++], e[i-1].R= R ) ;
        // calculate intersections: the most non-trivial part
        std::vector<time_interval> vec(n);
        for ( i= 0; i < n; vec[i]= s*e[i], ++i ) ;
        // populate the priority queue
        for ( ans= 0, i= 0; i < n; ++i ) {
            if ( vec[i] == empty_interval ) continue ; // no intersection
            event evnt= {
                    .id= i,
                    .timestamp = vec[i].first,
                    .type= event::event_type::opening
            };
#if PQ
            pq.push(evnt);
#else
            pq.insert(evnt);
#endif
        }
        // finally, the sweep-line simulation per se
        for ( std::set<event,id_comparator> status_line; not pq.empty(); ) {
#if PQ
            auto evnt= pq.top(); pq.pop();
#else
            auto evnt= *(pq.begin());
            auto it= pq.begin(); pq.erase(it);
#endif

            if ( evnt.type == event::event_type::opening ) {
                event conjugate_event= {
                        .id= evnt.id,
                        .timestamp= vec[evnt.id].second,
                        .type= event::event_type::closing
                };
#if PQ
                pq.push(conjugate_event);
#else
                pq.insert(conjugate_event);
#endif
                status_line.insert(conjugate_event);
            }
            else {
                ans= std::max(ans,static_cast<int>(status_line.size()));
                auto res= status_line.erase(evnt);
                assert( res );
            }
        }
        std::cout << ans << std::endl;
    }
    return 0;
}

