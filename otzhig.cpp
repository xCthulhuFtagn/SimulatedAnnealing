#include <iostream>
#include <random>
#include <utility>
#include <set>

using namespace std;

size_t s = 100;

double frand(double min, double max){ 
    return min + (double)(rand()) / ((double)(RAND_MAX / (max - min))); }
double E(double x, double y) { return 100 - x*x - y*y; }
double P(double dE, double ti) { return exp(-dE / ti); }
double T(double ti, int i) { return ti * 0.89; }
struct point{
    point(double x, double y) : x(x), y(y){}
    point(){}
    double x, y;
};
bool operator< (const point& l, const point& r) { return E(l.x, l.y) < E(r.x, r.y); }

point otzhig(double min, double max, double k){ //tmax is starting temp in the beginning
    double tmin = 1e-10, tmax = 1;
    double ti = tmax, xi = frand(min, max), yi = frand(min, max), _x, _y, dE;
    set<point> dots;
    for(int i = 0; i < s; ++i) dots.insert(point(frand(min, max), frand(min, max)));
    knuth_b rand_engine;
    uniform_real_distribution<> uniform_zero_to_one(0.0, 1.0);
    for (int i = 1; ti > tmin; ++i){
        set<point> new_dots;
        double step = (max - min)*exp(-i);
        point ND;
        for(auto dot : dots){
            for(size_t j = 0; j < k; ++j){
                ND.x = dot.x + frand(-step, step);
                ND.y = dot.y + frand(-step, step);
                dE = E(ND.x, ND.y) - E(dot.x, dot.y);
                if(dE > 0 || uniform_zero_to_one(rand_engine) <= P(dE, ti)) new_dots.insert(ND);
            }
            ND.x = dot.x + frand(-step, step);
            ND.y = dot.y + frand(-step, step);
            dE = E(ND.x, ND.y) - E(dot.x, dot.y);
            if(dE > 0 || uniform_zero_to_one(rand_engine) <= P(dE, ti)) new_dots.insert(ND);
        }
        ti = T(ti, i);
        dots.insert(new_dots.begin(), new_dots.end());
        while(dots.size() > s) dots.erase(dots.begin());
    }
    return *dots.rbegin();
}

int main(){
    point ans = otzhig(1e-10, 1, 10);
    printf("x = %f, y = %f => D = %f", ans.x, ans.y, sqrt(ans.x*ans.x + ans.y*ans.y));
}