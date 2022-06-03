#include <iostream>
#include <vector>
#include <queue>

bool OK(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

int main() {
    char s1[] = "aa";
    char s2[] = "bb";
    std::cin >> s1;
    int s = (s1[0] - 'a') * 8 + (s1[1] - '0') - 1;
    std::cin >> s2;
    int dest = (s2[0] - 'a') * 8 + (s2[1] - '0') - 1;
    const int n = 64;
    auto g = std::vector<std::vector<int>>(n, std::vector<int>());
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (OK(x + 2, y + 1))
                g[8 * x + y].push_back((x + 2) * 8 + y + 1);
            if (OK(x - 2, y + 1))
                g[8 * x + y].push_back((x - 2) * 8 + y + 1);
            if (OK(x + 2, y - 1))
                g[8 * x + y].push_back((x + 2) * 8 + y - 1);
            if (OK(x - 2, y - 1))
                g[8 * x + y].push_back((x - 2) * 8 + y - 1);
            if (OK(x + 1, y + 2))
                g[8 * x + y].push_back((x + 1) * 8 + y + 2);
            if (OK(x - 1, y + 2))
                g[8 * x + y].push_back((x - 1) * 8 + y + 2);
            if (OK(x + 1, y - 2))
                g[8 * x + y].push_back((x + 1) * 8 + y - 2);
            if (OK(x - 1, y - 2))
                g[8 * x + y].push_back((x - 1) * 8 + y - 2);
        }
    }
    const int INF = 100000000;
    std::vector<int> dist(n, INF);
    std::vector<int> parent(n, -1);
    std::queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int to: g[v]) {
            if (dist[to] == INF) {
                dist[to] = dist[v] + 1;
                q.push(to);
                parent[to] = v;
            }
            if (to == dest)
                break;
        }
    }
    std::vector<int> ans;
    for (int v = dest; v != s; v = parent[v]) {
        ans.push_back(v);
    }
    ans.push_back(s);
    for (auto rit = ans.rbegin(); rit != ans.rend(); ++rit)
        printf("%c%i\n", *rit / 8 + 'a', *rit % 8 + 1);
}