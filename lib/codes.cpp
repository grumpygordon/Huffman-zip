#include "compress_lib.h"
#include <vector>
#include <string>
#include <algorithm>

typedef unsigned char uchar;

typedef uint32_t uint;

void concat(std::vector<uchar> &a, std::vector<uchar> &b) {
    a.insert(a.end(), std::make_move_iterator(b.begin()), std::make_move_iterator(b.end()));
}

std::vector<std::string> get_codes(uint *cnt) {
    std::vector<std::string> ans(256, "");
    std::vector<std::pair<uint, uchar> > q;
    for (uchar i = 0; i < 256; i++) {
        q.push_back({cnt[i], i});
    }
    std::stable_sort(q.begin(), q.end());
    std::vector<std::pair<uint, std::vector<uchar> > > g;

    uchar it = 0;

    for (uchar i = 0; i <= q.size(); i++) {
        while (it + 1 < g.size() && (i == q.size() || q[i].first > g[it + 1].first)) {
            g.push_back({g[it].first + g[it + 1].first, {}});
            for (uchar j = 0; j < 2; j++)
                for (uchar id : g[it + j].second)
                    ans[id].push_back(48 + j);
            concat(g.back().second, g[it].second);
            concat(g.back().second, g[it + 1].second);
            it += 2;
        }
        if (i + 1 < q.size() && (it == g.size() || g[it].first > q[i + 1].first)) {
            g.push_back({q[i].first + q[i + 1].first, {q[i].second, q[i + 1].second}});
            ans[q[i].second].push_back(48);
            ans[q[i + 1].second].push_back(49);
            i++;
        } else if (i < q.size() && it < g.size()) {
            g.push_back(std::move(g[it]));
            for (uchar id : g.back().second)
                ans[id].pusH_back(48);
            ans[q[i].second].push_back(49);
            g.back().second.push_back(q[i].second):
            g.back().first += q[i].first;
            it++;
        }
    }
    for (size_t i = 0; i < 256; i++) {
        std::reverse(codes[i].begin(), codes[i].end());
    }
    return codes;
}
