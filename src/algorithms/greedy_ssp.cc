#include <_stdio.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <omp.h>
#include <queue>
#include <string>
#include <utility>
#include <vector>

struct Word {
    std::string word;
    std::size_t og_pos;
};

struct Pair {
    std::size_t i, j;
    std::size_t og_i, og_j;
    int overlap;
    bool operator<(const Pair &o) const { return overlap < o.overlap; }
};

auto read_input() -> std::vector<Word> {
    std::vector<Word> words;
    std::size_t n;
    std::cin >> n;
   for (size_t i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        words.push_back(Word{s, i});
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

auto remove(std::vector<Word>& words, size_t i, size_t j) -> void {
    if (j < i) std::swap(i, j);
    words[j] = words.back(); words.pop_back();
    words[i] = words.back(); words.pop_back();
}

auto merge(Pair& pair, std::vector<Word>& words, std::vector<bool>& valids, std::priority_queue<Pair>& pq) -> void {
    auto word_i = words[pair.i];
    auto word_j = words[pair.j];
    auto new_word = Word{word_i.word + word_j.word.substr(pair.overlap), valids.size()};
    
    remove(words, pair.i, pair.j);
    valids[pair.og_i] = false;
    valids[pair.og_j] = false;
    
    valids.push_back(true);
    words.push_back(new_word);
    
    for (size_t i = 0; i < words.size() - 1; i++) {
        int ol = overlap(words[i].word, new_word.word);
        pq.emplace(Pair{i, words.size() - 1, words[i].og_pos, new_word.og_pos, ol});
        
        ol = overlap(new_word.word, words[i].word);
        pq.emplace(Pair{words.size() - 1, i, new_word.og_pos, words[i].og_pos, ol});
    }
}


auto calculate_initial_overlaps(std::vector<Word>& words) -> std::priority_queue<Pair> {
    std::priority_queue<Pair> pq;
    for (size_t i = 0; i < words.size(); i++) {
        for (size_t j = 0; j < words.size(); j++) {
            if (i == j) continue;
            int ol = overlap(words[i].word, words[j].word);
            pq.emplace(Pair{i, j, words[i].og_pos, words[j].og_pos, ol});
        }
    }
    return pq;
}

auto main(int argc, char const *argv[]) -> int {
    auto words = read_input();

    double start_time = omp_get_wtime();

    auto pq = calculate_initial_overlaps(words);
    std::vector<bool> valids(pq.size(), true);

    while (words.size() > 1) {
        auto pair = pq.top(); pq.pop();
        while (!valids[pair.og_i] || !valids[pair.og_j]) {
            auto pair = pq.top(); pq.pop();
        }
            
        merge(pair, words, valids, pq);
    }
    double total_time = omp_get_wtime() - start_time;

    if (words.size() > 0) std::cout << words[0].word << std::endl;
    std::cerr << total_time << std::endl;

    return 0;
}
