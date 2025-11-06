#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <omp.h>

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

auto highest_overlap(const std::vector<std::string>& words) -> std::tuple<std::size_t, std::size_t, int> {
    int max_overlap = -1;
    std::size_t max_i = 0, max_j = 0;
    
    #pragma omp parallel
    {
        int l_max_overlap = -1;
        size_t l_max_i = 0, l_max_j = 0;
        
        
        #pragma omp for collapse(2)
        for (size_t i = 0; i < words.size(); i++) {
            for (size_t j = 0; j < words.size(); j++) {
                if (i == j) continue;
                int ol = overlap(words[i], words[j]);
                if (ol > l_max_overlap) {
                    l_max_i = i;
                    l_max_j = j;
                    l_max_overlap = ol;
                }
            }
        }
        
        #pragma omp critical
        {
            if(l_max_overlap > max_overlap) {
                max_i = l_max_i;
                max_j = l_max_j;
                max_overlap = l_max_overlap;
            }
        }
    }
    return {max_i, max_j, max_overlap};
}

auto merge(std::string a, std::string b, int overlap) -> std::string {
    return a + b.substr(overlap);
}

auto remove(std::vector<std::string>& words, size_t i, size_t j) -> void {
    if (j < i) std::swap(i, j);
    words[j] = words.back(); words.pop_back();
    words[i] = words.back(); words.pop_back();
}

auto insert(std::vector<std::string>& words, std::string new_word) -> void {
    words.emplace_back(new_word);
}

auto main(int argc, char const *argv[]) -> int {
    auto words = read_input();
    
    double start_time = omp_get_wtime();
    while (words.size() > 1) {
        auto tuple = highest_overlap(words);
        
        size_t i, j; int max_overlap;
        std::tie(i, j, max_overlap) = tuple;
        
        std::string new_word = merge(words[i], words[j], max_overlap);
        remove(words, i, j);
        insert(words, new_word);
    }    
    double end_time = omp_get_wtime();
    std::cerr << end_time - start_time << std::endl;
    
    if (words.size() > 0) std::cout << words[0] << std::endl;
    
    return 0;
}
