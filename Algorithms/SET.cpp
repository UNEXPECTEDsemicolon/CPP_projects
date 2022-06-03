#include <iostream>
#include <set>
#include <string>
#include <map>
#include <ios>

int main() {
    unsigned long long n;
    int m, k;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();
    std::cin >> n >> m >> k;
    auto *sets = new std::set<unsigned long long> [m + 1];
    for (int i = 0; i < m + 1; ++i)
        sets[i] = std::set<unsigned long long>();
    auto maps = std::map<unsigned long long, std::set<int>>();
    for (int q = 0; q < k; ++q) {
        std::string str;
        unsigned long long e;
        int s;
        std::cin >> str;
        if (str == "ADD") {
            std::cin >> e >> s;
            sets[s].insert(e);
            maps[e].insert(s);
        } else if (str == "DELETE") {
            std::cin >> e >> s;
            sets[s].erase(e);
            if (maps.find(e) != maps.end())
                maps[e].erase(s);
        } else if (str == "CLEAR") {
            std::cin >> s;
            for (auto it = sets[s].begin(); it != sets[s].end(); ++it)
                maps[*it].erase(s);
            sets[s].clear();
        } else if (str == "LISTSET") {
            std::cin >> s;
            if (sets[s].empty())
                std::cout << -1 << '\n';
            else {
                for (auto it = sets[s].begin(); it != sets[s].end(); ++it) {
                    std::cout << *it << ' ';
                }
                std::cout << '\n';
            }
        } else if (str == "LISTSETSOF") {
            std::cin >> e;
            if (maps.find(e) == maps.end())
                std::cout << -1 << '\n';
            else {
                auto end = maps[e].end();
                for (auto it = maps[e].begin(); it != end; ++it) {
                    if (sets[*it].find(e) != sets[*it].end())
                        std::cout << *it << ' ';
                }
                std::cout << '\n';
            }
        }
    }
    delete [] sets;
}