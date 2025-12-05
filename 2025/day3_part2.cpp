#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

class BatteryBank {
    std::vector<short> batteries;
    std::vector<short> best_pair = {-1, -1};

public:
    explicit BatteryBank(std::vector<short> batteries) {
        this->batteries=std::move(batteries);
        for (const auto& battery : this->batteries) {
            if (battery < 1 || battery > 9) {
                throw "Battery index out of bounds";
            }
        }
    }
    explicit BatteryBank(const std::string_view str) {
        std::vector<short> batteries = {};
        for (const auto chr : str) {
            auto* string = new std::string;
            *string+=chr;
            batteries.push_back(std::stoi(*string));
            delete string;
        }
        this->batteries=std::move(batteries);
        for (const auto& battery : this->batteries) {
            if (battery < 1 || battery > 9) {
                throw "Battery index out of bounds";
            }
        }
    }
    [[nodiscard]] std::string to_string() const {
        std::string result;
        for (const auto& battery : batteries) {
            result+=std::to_string(battery);
        }
        return result;
    }

    std::vector<short> get_best_pair(short digitnum) {
        digitnum--;
        if (this->best_pair[0] != -1) {
            return best_pair;
        }
        if (batteries.size() < 2) {
            throw "Batteries less than 2";
        }
        std::vector<short> results = {};
        long previous_index = 0;
        for (int i = digitnum; i >= 0; --i) {
            short next = batteries.at(previous_index+1);
            if (i == digitnum) {
                next = batteries.at(0);
                previous_index = -1;
            }
            int index = previous_index + 1;
            for (unsigned long j = index; j <= batteries.size()-1-i; ++j) {
                //std::cout<<i<<"\n";
                if (batteries.at(j) > next) {
                    next = batteries.at(j);
                    index = j;
                }
            }
            results.push_back(next);
            previous_index = index;
        }
        return results;
    }
};

std::vector<BatteryBank> read_batteries(std::string_view filename) {
    std::vector<BatteryBank> banks = {};
    std::ifstream file {(filename.data())};
    std::string temp {};
    while (std::getline(file, temp)) {
        banks.emplace_back(BatteryBank(temp));
    }
    return banks;
}

long long pair_as_long(std::vector<short> short_pair) {
    std::string result {};
    for (const auto value : short_pair) {
        result+=std::to_string(value);
    }
    return std::stoll(result);
}

int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    std::vector<BatteryBank> banks = read_batteries("banks.txt");
    long long sum = 0;
    for (auto& bank : banks) {
        std::cout<<bank.to_string()<<"\n";
        std::cout<<pair_as_long(bank.get_best_pair(12))<<"\n";
        sum+=pair_as_long(bank.get_best_pair(12));
    }
    std::cout<<sum;
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}