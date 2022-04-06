#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <cmath>

const double INF = 1e10;
const double EPS = 1e-6;

struct Comp {
    bool operator()(const std::pair<int ,int>& p1, const std::pair<int, int>& p2) const {
        return p1.second != p2.second ? p1.second < p2.second : p1.first < p2.first;
    }
};

double dist(const std::pair<double, double>& p1, const std::pair<double, double>& p2) {
    return std::sqrt(std::pow(p1.first - p2.first, 2) + std::pow(p1.second - p2.second, 2));
}

bool eq(double a, double b) {
    return std::abs(a - b) < EPS;
}

int main() {
    int n;
    double R;
    std::cin >> n >> R;
    auto points = std::unordered_map<int, std::pair<double, double>>(n);
    for (int i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        points[i] = {x, y};
    }
    auto cost = std::vector<std::vector<double>>(n, std::vector<double>(n));
    auto g = std::vector<std::vector<int>>(n, std::vector<int>(0));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            auto d = dist(points[i], points[j]);
            if (d < R || eq(d, R)) {
                cost[i][j] = cost[j][i] = d;
                g[i].push_back(j);
                g[j].push_back(i);
            }
        }
    }
    std::vector<std::vector<double>> d = {std::vector<double>(n, INF), std::vector<double>(n, INF)};
    for (int s = 0; s <= 1; ++s) {
        auto used = std::vector<bool>(n, false);
        auto heap = std::set<std::pair<int, double>, Comp>();
        d[s][s] = 0;
        heap.insert({s, d[s][s]});
        for (int i = 0; i < n; ++i) {
            auto v = heap.begin()->first;
            heap.erase(heap.begin());
            if (eq(d[s][v], INF))
                break;
            used[v] = true;
            for (auto u: g[v]) {
                if (d[s][v] + cost[v][u] < d[s][u]) {
                    heap.erase({u, d[s][u]});
                    heap.insert({u, d[s][u] = d[s][v] + cost[v][u]});
                }
            }
        }
    }
    double ans = INF;
    for (int i = 2; i < n; ++i)
        if (points[i].first < 0.0)
            ans = std::min(ans, d[0][i] + d[1][i]);
    std::cout << ans << '\n';
}
