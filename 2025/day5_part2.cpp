#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <ranges>
#include <algorithm>
#include <set>

/*class IngredientDatabase {
    std::vector<std::pair<unsigned long long, unsigned long long> > fresh_id_ranges = {};

public:
    explicit IngredientDatabase(const std::vector<std::pair<unsigned long long, unsigned long long> > &id_ranges) {
        this->fresh_id_ranges = id_ranges;
    }

    static std::pair<unsigned long long, unsigned long long> string_to_id_range(const std::string &str) {
        std::vector<unsigned long long> range_start_end = {};
        for (const auto &str1: std::views::split(std::string_view(str), '-')) {
            range_start_end.emplace_back(std::stoll(std::string(std::string_view(str1))));
        }
        if (range_start_end.size() != 2) {
            throw std::invalid_argument("Id range not in valid format!");
        }
        return std::pair{range_start_end[0], range_start_end[1]};
    }

    static std::vector<unsigned long long> range_to_valid_id_list(
        const std::pair<unsigned long long, unsigned long long> &range) {
        std::vector<unsigned long long> list = {};
        for (unsigned long long i = range.first; i <= range.second; ++i) {
            list.emplace_back(i);
        }
        return list;
    }

    static bool is_fresh(const unsigned long long id, const std::pair<unsigned long long, unsigned long long> &range) {
        return range.first <= id && id <= range.second;
    }

    bool is_fresh(const unsigned long long id) const {
        for (const auto &range: fresh_id_ranges) {
            if (range.first <= id && id <= range.second) {
                return true;
            }
        }
        return false;
    }

    unsigned long long amount_fresh(const std::vector<unsigned long long> &ids) {
        unsigned long long amount_fresh = 0;
        for (const auto &id: ids) {
            amount_fresh += is_fresh(id);
        }
        return amount_fresh;
    }

    template<typename T>
    static T max(T first, T second) {
        return first > second ? first : second;
    }
    template<typename T>
    static bool vec_contains(std::vector<T> vec, T item) {
        if (vec.empty()) {
            return false;
        }
        auto find_check = std::ranges::find(vec, item);
        if (find_check == vec.end()) {
            return vec[vec.size()-1] == item;
        }
        return true;
    }

    unsigned long long amount_fresh() {
        unsigned long long amount_fresh = 0;
        std::pair<unsigned long long, unsigned long long> merged_first = {1, 1};
        std::pair<unsigned long long, unsigned long long> merged_second = {1, 1};
        std::pair<unsigned long long, unsigned long long> merged_result = {1, 1};
        while (merged_first != std::pair<unsigned long long, unsigned long long>{0, 0}) {
            merged_first = {0, 0};
            merged_second = {0, 0};
            merged_result = {0, 0};
            for (const auto &range: fresh_id_ranges) {
                for (const auto &range1: fresh_id_ranges) {
                    if (range.first == range1.first && range.second == range1.second) {
                        continue;
                    }

                    //for example 1..4 and 2..3, 1 < 2 and 4 > 3
                    if (range1.first <= range.first && range1.second >= range.second) {
                        merged_first = range1;
                        merged_second = range;
                        merged_result = range1;
                        break;
                    }
                    if (range.first <= range1.first && range.second >= range1.second) {
                        merged_first = range;
                        merged_second = range1;
                        merged_result = range;
                        break;
                    }

                    //for example 1-16, 12-18, 1 < 12 and 16 < 18 and 12 < 16
                    if (range.first <= range1.first && range.second <= range1.second && range1.first < range.second) {
                        merged_first = range;
                        merged_second = range1;
                        merged_result = {range.first, range1.second};
                        break;
                    }
                    if (range1.first <= range.first && range1.second <= range.second && range.first < range1.second) {
                        merged_first = range1;
                        merged_second = range;
                        merged_result = {range1.first, range.second};
                        break;
                    }
                    //for example 1-5, 6-9, 5 == 6 - 1
                    if (range.second == range1.first - 1 || range.second == range1.first) {
                        merged_first = range1;
                        merged_second = range;
                        merged_result = {range.first, range1.second};
                        break;
                    }
                    if (range1.second == range.first - 1 || range1.second == range.first) {
                        merged_first = range1;
                        merged_second = range;
                        merged_result = {range1.first, range.second};
                        break;
                    }
                }
            }
            if (merged_result == std::pair<unsigned long long, unsigned long long>{0, 0}) {
                break;
            }
            if (vec_contains(fresh_id_ranges, merged_first)) {
                fresh_id_ranges.erase(std::ranges::find(fresh_id_ranges, merged_first));
            }
            if (vec_contains(fresh_id_ranges, merged_second)) {
                fresh_id_ranges.erase(std::ranges::find(fresh_id_ranges, merged_second));
            }
            fresh_id_ranges.emplace_back(merged_result);
        }
        std::cout << "dadada";
        std::pmr::set set_range = {fresh_id_ranges[0]};
        for (const auto &range: fresh_id_ranges) {
            set_range.insert(range);
        }
        for (const auto &range: set_range) {
            amount_fresh += range.second - range.first + 1;
        }
        std::cout << "dadada";
        return amount_fresh;
    }
};

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    std::vector<unsigned long long> checked_ids = {};
    std::vector<std::pair<unsigned long long, unsigned long long> > ranges = {};
    bool reading_ranges = true;
    std::ifstream file{"db.txt"};
    std::string temp{};
    while (getline(file, temp)) {
        if (temp.empty()) {
            reading_ranges = false;
            continue;
        }
        if (reading_ranges) {
            ranges.emplace_back(IngredientDatabase::string_to_id_range(temp));
        } else {
            checked_ids.emplace_back(std::stoll(temp));
        }
    }
    auto db = IngredientDatabase(ranges);
    std::cout << db.amount_fresh();
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}*/
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::pair<long, long>> loadRanges(const std::string &filename) {
    std::vector<std::pair<long, long>> result;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        long start = std::stol(line.substr(0, line.find('-')));
        long end = std::stol(line.substr(line.find('-') + 1));
        result.emplace_back(start, end);
    }
    std::sort(result.begin(), result.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });
    return result;
}

std::vector<long> loadIds(const std::string &filename) {
    std::vector<long> result;
    std::ifstream file(filename);
    std::string line;
    bool parseIds = false;
    while (std::getline(file, line)) {
        if (line.empty()) {
            parseIds = true;
            continue;
        }
        if (parseIds) {
            long long id = std::stol(line);
            result.push_back(id);
        }
    }
    return result;
}

void partOne() {
    std::string filename("db.txt");
    const auto ids = loadIds(filename);
    const auto ranges = loadRanges(filename);
    long result = 0;
    for (const auto &id : ids) {
        for (const auto &[start, end] : ranges) {
            if (id >= start && id <= end) {
                result++;
                break;
            }
        }
    }
    std::cout << result << std::endl;
    //assert(result == 558);
}

std::vector<std::pair<long, long>> mergeRanges(const std::vector<std::pair<long, long>> &ranges) {
    std::vector<std::pair<long, long>> result;
    auto [start, end] = ranges[0];
    for (int i = 1; i < ranges.size(); ++i) {
        auto [nextStart, nextEnd] = ranges[i];
        if (nextStart > end) {
            result.emplace_back(start, end);
            start = nextStart;
            end = nextEnd;
        } else if (nextEnd > end) {
            end = nextEnd;
        }
    }
    result.emplace_back(start, end);
    return result;
}

void partTwo() {
    auto ranges = loadRanges("db.txt");
    auto mergedRanges = mergeRanges(ranges);
    long result = 0;
    for (auto [first, end] : mergedRanges) {
        result += end - first + 1;
    }
    std::cout << result << std::endl;
    //assert(result == 344813017450467);
}

int main() {
    auto l = mergeRanges({{2,5}, {   8,10}, {9,11}, {4,7}});
    for (const auto& l : l) {
        std::cout<<l.first<<" "<<l.second<<"\n";
    }
    return 0;
}
