#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <omp.h>
#include <string.h>

#define IDX(i, j, n) ((size_t)(i) * (n) + (j))

struct MaxOverlap {
    int16_t value;
    size_t i;
    size_t j;
};

#pragma omp declare reduction( \
    max_with_index : MaxOverlap : \
    omp_out = omp_in.value > omp_out.value ? omp_in : omp_out \
) initializer( \
    omp_priv = {-1, 0, 0} \
)

auto read_input() -> std::vector<std::string> {
    std::vector<std::string> x;
    std::size_t n;
    std::cin >> n;
    while (n--) {
        std::string s;
        std::cin >> s;
        x.emplace_back(s);
    }
    return x;
}

#pragma omp declare target
int16_t overlap(const char* s1, size_t len1, const char* s2, size_t len2) {
    int16_t max_overlap = 0;
    for (int16_t k = 1; k <= (int16_t) (len1 < len2 ? len1 : len2); ++k) {
        if (strncmp(s1 + len1 - k, s2, k) == 0) {
            max_overlap = k;
        }
    }
    return max_overlap;
}
#pragma omp end declare target

void initialize_overlaps(std::vector<int16_t>& overlaps, std::vector<std::string>& words) {
    const size_t n = words.size();
    if (n == 0) return;

    std::vector<size_t> lengths(n);
    std::vector<size_t> offsets(n);
    size_t total_chars = 0;

    for (size_t i = 0; i < n; ++i) {
        lengths[i] = words[i].length();
        offsets[i] = total_chars;
        total_chars += lengths[i];
    }
    total_chars += n;

    std::vector<char> all_words_buffer(total_chars);
    char* all_words_ptr = all_words_buffer.data();

    for (size_t i = 0; i < n; ++i) {
        offsets[i] += i;
        memcpy(all_words_ptr + offsets[i], words[i].c_str(), lengths[i] + 1);
    }

    int16_t* overlaps_ptr = overlaps.data();
    size_t* lengths_ptr = lengths.data();
    size_t* offsets_ptr = offsets.data();

    #pragma omp target teams distribute parallel for \
        map(to: n, lengths_ptr[0:n], offsets_ptr[0:n], all_words_ptr[0:total_chars]) \
        map(from: overlaps_ptr[0:n*n]) \
        collapse(2) // Collapse the two loops into one parallel iteration space
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (i == j) {
                overlaps_ptr[IDX(i, j, n)] = -1;
            } else {
                const char* s1 = all_words_ptr + offsets_ptr[i];
                const char* s2 = all_words_ptr + offsets_ptr[j];
                size_t len1 = lengths_ptr[i];
                size_t len2 = lengths_ptr[j];

                overlaps_ptr[IDX(i, j, n)] = overlap(s1, len1, s2, len2);
            }
        }
    }
}

MaxOverlap max_overlap(const std::vector<int16_t>& overlaps, size_t n) {
    const int16_t* overlaps_ptr = overlaps.data();

    MaxOverlap max_result = {-1, 0, 0};

    #pragma omp target teams distribute parallel for \
        map(to: overlaps_ptr[0:n*n], n) \
        reduction(max_with_index : max_result)
    for (size_t i = 0; i < n * n; i++) {
        const int16_t current_val = overlaps_ptr[i];
        if (current_val > max_result.value) {
            max_result.value = current_val;
            max_result.i = i / n;
            max_result.j = i % n;
        }
    }

    return max_result;
}

auto merge(std::string a, std::string b, int overlap) -> std::string {
    return a + b.substr(overlap);
}

void calculate_new_overlaps(std::vector<int16_t>& overlaps, std::vector<std::string>& words, size_t st, size_t nd, size_t ogn) {
    auto wn = words.size();

    size_t k;
    for (k = 0; k < words.size() + 1; k++) {
        overlaps[IDX(nd, k, ogn)] = overlaps[IDX(words.size(), k, ogn)];
        overlaps[IDX(words.size(), k, ogn)] = -1;
    }

    for (k = 0; k < words.size(); k++){
        overlaps[IDX(k, nd, ogn)] = overlaps[IDX(k, words.size(), ogn)];
        overlaps[IDX(k, words.size(), ogn)] = -1;
    }

    for (k = 0; k < words.size(); k++) {
        if (st == k) {
            overlaps[IDX(st, k, ogn)] = -1;
            continue;
        }
        overlaps[IDX(st, k, ogn)] = overlap(words[st].c_str(), words[st].size(), words[k].c_str(), words[k].size());
        overlaps[IDX(k, st, ogn)] = overlap(words[k].c_str(), words[k].size(), words[st].c_str(), words[st].size());
    }
}

auto main(int argc, char const *argv[]) -> int {
    auto words = read_input();
    auto ogn = words.size();

    double start_time = omp_get_wtime();

    std::vector<int16_t> overlaps(ogn*ogn);
    initialize_overlaps(overlaps, words);

    while (words.size() > 1) {
        auto max_ol = max_overlap(overlaps, ogn);

        std::string new_word = merge(words[max_ol.i], words[max_ol.j], max_ol.value);
        size_t st = std::min(max_ol.i, max_ol.j);
        size_t nd = std::max(max_ol.i, max_ol.j);
        words[st] = new_word;
        std::swap(words[nd], words[words.size() - 1]);
        words.pop_back();

        calculate_new_overlaps(overlaps, words, st, nd, ogn);
    }

    double end_time = omp_get_wtime();
    std::cerr << end_time - start_time << std::endl;

    if (words.size() > 0) std::cout << words[0] << std::endl;

    return 0;
}
