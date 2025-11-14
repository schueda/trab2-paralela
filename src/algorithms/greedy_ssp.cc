#include <stdio.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <omp.h>
#include <queue>
#include <string>
#include <vector>

struct Pair {
    std::size_t i, j;
    int overlap;
    bool operator<(const Pair &o) const { return overlap < o.overlap; }
};

auto read_input() -> std::vector<std::string> {
    std::vector<std::string> words;
    std::size_t n;
    std::cin >> n;
   for (size_t i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        words.push_back(s);
    }
    return words;
}

auto overlap(const std::string& a, const std::string& b) -> int {
    size_t limit = std::min(a.size(), b.size());
    for (size_t i = limit; i > 0; i--) {
        bool is_overlap = true;
        for (size_t j = 0; j < i; j++) {
            if (a[a.size() - i + j] != b[j]) {
                is_overlap = false;
                break;
            }
        }
        if (is_overlap) return i;
    }
    return 0;
}

auto calculate_initial_overlaps(std::vector<std::string>& words) -> std::priority_queue<Pair> {
    std::priority_queue<Pair> pq;
    for (size_t i = 0; i < words.size(); i++) {
        for (size_t j = 0; j < words.size(); j++) {
            if (i == j) continue;
            int ol = overlap(words[i], words[j]);
            pq.emplace(Pair{i, j, ol});
        }
    }
    return pq;
}

auto merge(Pair& pair, std::vector<std::string>& words, std::vector<bool>& valids, std::priority_queue<Pair>& pq) -> void {
    auto word_i = words[pair.i];
    auto word_j = words[pair.j];
    auto new_word = word_i + word_j.substr(pair.overlap);

    valids[pair.i] = false;
    valids[pair.j] = false;

    valids.push_back(true);
    words.push_back(new_word);

    for (size_t i = 0; i < words.size() - 1; i++) {
        int ol = overlap(words[i], new_word);
        pq.emplace(Pair{i, words.size() - 1, ol});

        ol = overlap(new_word, words[i]);
        pq.emplace(Pair{words.size() - 1, i, ol});
    }
}

auto main(int argc, char const *argv[]) -> int {
    auto words = read_input();

    double start_time = omp_get_wtime();

    auto pq = calculate_initial_overlaps(words);
    std::vector<bool> valids(words.size(), true);

    for (size_t i = 0; i < words.size() - 1; i++) {
        auto pair = pq.top(); pq.pop();
        while (!pq.empty() && (!valids[pair.i] || !valids[pair.j])) {
            pair = pq.top(); pq.pop();
        }
        if (pq.empty()) break;

        merge(pair, words, valids, pq);
    }
    double total_time = omp_get_wtime() - start_time;

    if (words.size() > 0) std::cout << words.back() << std::endl;
    std::cerr << total_time << std::endl;

    return 0;
}
