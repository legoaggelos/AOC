#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>

bool read_file(std::string& output, const std::string& input) {
    std::ifstream file {input};
    if (!file) {
        return false;
    }
    std::string temp {};
    while (std::getline(file, temp)) {
        output += temp;
    }
    return true;
}
std::vector<unsigned long long> extract_ids(std::string_view input) {
    std::vector<unsigned long long> ids = {};
    std::vector<unsigned long long> part_ids = {};
    for (const auto& str : std::views::split(input, ',')) {
        part_ids.clear();
        for (const auto& str1 : std::views::split(std::string_view(str), '-')) {
            part_ids.emplace_back(std::stoll(std::string(std::string_view(str1))));
        }
        if (part_ids[0] > part_ids[1]) {
            const auto temp = part_ids[0];
            part_ids[0] = part_ids[1];
            part_ids[1] = temp;
        }
        for (unsigned long long i = part_ids[0]; i <= part_ids[1]; ++i) {
            ids.emplace_back(i);
        }
    }
    return ids;
}
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    std::string ids_str;
    read_file(ids_str, "ids.txt");
    const std::vector<unsigned long long> ids = extract_ids(ids_str);
    unsigned long long sum = 0;
    for (const auto id : ids) {
        std::string id_str = std::to_string(id);
        for (unsigned long long i = 0; i <= id_str.size()/2; ++i) {
            std::string separating_string = id_str.substr(0, i);
            auto split = std::views::split(std::string_view(id_str), separating_string);
            bool shallAdd = true;
            for (auto c: split) {
                shallAdd &= (std::string_view(c).empty());
            }
            if (shallAdd) {
                sum += id;
                break;
            }
        }
    }
    std::cout<<sum;
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}
