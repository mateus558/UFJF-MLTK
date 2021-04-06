#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <execution>
#include <thread>

#include "Point.hpp"

using namespace std;
using namespace mltk;

void optimize(Point<double>& set, std::vector<double>& perm, int _k, double total_sum,
              std::vector<Point<double>>& combinations){
    auto sum = std::accumulate(perm.begin(), perm.end(), 0.0);

    if(sum > total_sum){
        return;
    }

    if(_k == 0){
        if(sum == total_sum){
            combinations.emplace_back(perm);
        }
        perm.clear();
        return;
    }

    for (int i = 0; i < set.size(); i++){
        std::vector<double> p = perm;
        p.push_back(set[i]);

        optimize(set, p, _k - 1, total_sum, combinations);
    }
}

void optimize(const size_t step, const double max_weight, const size_t set_size, const size_t seed = 0){
    Point<double> values = mltk::linspace<double>(0, max_weight, step);
    int _k = set_size;
    size_t N = 0;
    std::vector<double> perm;
    std::vector<Point<double>> configs;
    std::mt19937 engine(seed);

    optimize(values, perm, _k, max_weight, configs);

    std::cout << "Combinations: " << configs.size() << std::endl;
    std::cout << "----------------------------------------------------------------------------------------------------------------\n";
    std::cout << "\tstep\t\t\tweights\t\t\taccuracy\t\tbest_weights\t\tbest_acc\n";
    std::cout << "----------------------------------------------------------------------------------------------------------------\n";

    std::shuffle(configs.begin(), configs.end(), engine);
    std::for_each(std::execution::par_unseq, configs.begin(), configs.end(), [&N](auto& config){
        double acc = 0, best_acc = 0;
        auto best = config;
        for(int i = 0; i < 200000; i++);
        std::cout <<"\t"<< N << "\t\t" << config << "\t\t" << acc << "\t\t" << best << "\t\t" << best_acc << std::endl;
        N++;
    });
}
int main()
{
    optimize(9, 104, 7);
}