#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>

class Diagram {
    std::vector<std::vector<char>> diagram = {};
public:
    explicit Diagram(const std::vector<std::vector<char>> &diagram) {
        for (const auto& row : diagram) {
            unsigned long length = -1;
            if (!row.empty()) {
                length = row.size();
            }
            for (const auto& character : row) {
                if (row.size() != length) {
                    throw std::invalid_argument("Inconsistent row length!");
                }
                if (character != '@' && character != '.') {
                    throw std::invalid_argument("Invalid characters!");
                }
            }
        }
        this->diagram = diagram;
    }

    bool pos_exists(int x, int y) const {
        if (diagram.empty()) {
            throw std::invalid_argument("diagram empty");
        }
        if (diagram[0].empty()) {
            throw std::invalid_argument("diagram rows empty");
        }
        if (x < diagram[0].size() && y < diagram.size()) {
            return true;
        }
        throw std::invalid_argument("x or y out of range");
    }

    bool set_pos(int x, int y, char new_char) {
        pos_exists(x, y);
        diagram[x][y] = new_char;
        return true;
    }

    [[nodiscard]] char get_at_pos(int x, int y) const {
        this->pos_exists(x, y);
        return diagram[x][y];
    }
    [[nodiscard]] short amount_of_chars_near_pos(const int x, const int y, const char c) const {
        this->pos_exists(x, y);
        const bool check_over = y > 0;
        const bool check_under = y < diagram.size()-1;
        const bool check_left = x > 0;
        const bool check_right = x < diagram[0].size() - 1;
        short amount = 0;
        if (check_over) {
            amount += get_at_pos(x, y - 1) == c ? 1 : 0;
        }
        if (check_under) {
            amount += get_at_pos(x, y + 1) == c ? 1 : 0;
        }
        if (check_left) {
            amount += get_at_pos(x - 1, y) == c ? 1 : 0;
        }
        if (check_right) {
            amount += get_at_pos(x + 1, y) == c ? 1 : 0;
        }
        if (check_left && check_over) {
            amount += get_at_pos(x - 1, y - 1) == c ? 1 : 0;
        }
        if (check_right && check_under) {
            amount += get_at_pos(x + 1, y + 1) == c ? 1 : 0;
        }
        if (check_left && check_under) {
            amount += get_at_pos(x - 1, y + 1) == c ? 1 : 0;
        }
        if (check_right && check_over) {
            amount += get_at_pos(x + 1, y - 1) == c ? 1 : 0;
        }
        return amount;
    }
    void print() const {
        for (const auto& vec : diagram) {
            std::cout<<"[";
            for (const auto& c : vec) {
                std::cout<<c;
            }
            std::cout<<"]\n";
        }
    }

    unsigned long remove_as_many_as_possible() {
        std::vector<std::pair<unsigned long, unsigned long>> to_be_removed = {};
        for (int x = 0; x < diagram[0].size(); ++x) {
            for (int y = 0; y < diagram.size(); ++y) {
                if (this->get_at_pos(x,y) != '@') {
                    continue;
                }
                if (this->amount_of_chars_near_pos(x, y, '@') < 4) {
                    to_be_removed.emplace_back(std::pair{x,y});
                }
            }
        }
        for (const auto& pair : to_be_removed) {
            this->set_pos(pair.first, pair.second, '.');
        }
        return to_be_removed.size();
    }
};
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    std::vector<std::vector<char>> matrice_vec = {};
    std::ifstream file {"vec.txt"};
    std::string temp {};
    while (getline(file, temp)) {
        std::vector<char> vec = {};
        for (const char c : temp) {
            vec.emplace_back(c);
        }
        matrice_vec.emplace_back(vec);
    }
    Diagram diag = Diagram(matrice_vec);
    //diag.print();

    unsigned long last_removed = diag.remove_as_many_as_possible();
    unsigned long long removed_amount = last_removed;
    while (last_removed != 0) {
        last_removed = diag.remove_as_many_as_possible();
        removed_amount += last_removed;
    }
    std::cout<<removed_amount<<"\n";
    //diag.print();
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}