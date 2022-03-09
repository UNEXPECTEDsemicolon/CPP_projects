#include <iostream>

const int INF = 1e8;

int main() {
    int n, m;
    std::cin >> n >> m;
    auto x = new int [n];
    for (int i = 0; i < n; ++i)
        std::cin >> x[i];
    auto ps = new int [n];
    ps[0] = x[0];
    for (int i = 1; i < n; ++i)
        ps[i] = ps[i-1] + x[i];
    auto dists = new int** [n];
    for (int i = 0; i < n; ++i) {
        dists[i] = new int* [n];
        for (int j = 0; j < n; ++j) {
            dists[i][j] = new int[2];
            dists[i][j][0] = (j - i) * x[j] - ((j > 0 ? ps[j-1] : 0) - (i > 0 ? ps[i-1] : 0));
            dists[i][j][1] = (ps[j] - ps[i]) - (j - i) * x[i];
        }
    }
    auto min_dists = new int* [n];
    for (int i = 0; i < n; ++i) {
        min_dists[i] = new int [n];
        for (int j = 0; j < n; ++j) {
            min_dists[i][j] = INF;
            for (int m = i; m <= j; ++m) {
                min_dists[i][j] = std::min(dists[i][m][1] + (m < j ? dists[m+1][j][0] : 0), min_dists[i][j]);
                min_dists[i][j] = std::min((m > i ? dists[i][m-1][1] : 0) + dists[m][j][0], min_dists[i][j]);
            }
        }
        if (i > 0) min_dists[i][i-1] = 0;
    }

    auto dp = new int** [m];
    for (int i = 0; i < m; ++i) {
        dp[i] = new int* [n];
        for (int j = 0; j < n; ++j) {
            dp[i][j] = new int [2];
            dp[i][j][0] = INF;
            dp[i][j][1] = -1;
        }
    }
    for (int j = 0; j < n; ++j) {
        dp[0][j][0] = dists[0][j][0] + dists[j][n - 1][1];
        dp[0][j][1] = j;
    }
    for (int i = 1; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k <= j; ++k) {
                if (dp[i][j][0] >= dp[i-1][k][0] - dists[k][n-1][1] + min_dists[k][j] + dists[j][n-1][1]) {
                    dp[i][j][0] = dp[i-1][k][0] - dists[k][n-1][1] + min_dists[k][j] + dists[j][n-1][1];
                    dp[i][j][1] = k;
                }
            }
        }
    }
//    for (int i = 0; i < m; ++i) {
//        for (int j = 0; j < n; ++j)
//            std::cout << '(' << dp[i][j][0] << ", " << dp[i][j][1] << ") ";
//        std::cout << '\n';
//    }
    int best_j = 0;
    int min_len = INF;
    for (int j = 0; j < n; ++j) {
        if (dp[m-1][j][0] < min_len) {
            min_len = dp[m-1][j][0];
            best_j = j;
        }
    }
    auto ans = new int [m];
    ans[m-1] = best_j;
    for (int i = 1; i < m; ++i) {
        ans[m-i-1] = dp[m-i][ans[m-i]][1];
    }
    std::cout << dp[m-1][best_j][0] << '\n';
    for (int i = 0; i < m; ++i)
        std::cout << x[ans[i]] << ' ';
    std::cout << '\n';

    delete [] ans;
    delete [] x;
    delete [] ps;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            delete [] dists[i][j];
        delete [] dists[i];
    }
    delete [] dists;
    for (int i = 0; i < n; ++i) delete [] min_dists[i];
    delete [] min_dists;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            delete [] dp[i][j];
        delete [] dp[i];
    }
    delete [] dp;
}