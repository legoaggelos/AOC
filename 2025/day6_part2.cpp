#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>

template<typename T>
std::vector<std::vector<T> > transpose_matrix(const std::vector<std::vector<T> > &matrix) {
    if (matrix.empty()) {
        return matrix;
    }
    if (matrix[0].empty()) {
        return {};
    }
    std::vector<std::vector<T> > new_matrix = {};
    for (unsigned long long i = 0; i < matrix[0].size(); ++i) {
        std::vector<T> vec = {};
        for (unsigned long long y = 0; y < matrix.size(); ++y) {
            vec.emplace_back(0);
        }
        new_matrix.emplace_back(vec);
    }
    for (unsigned long long x = 0; x < matrix[0].size(); ++x) {
        for (unsigned long long y = 0; y < matrix.size(); ++y) {
            new_matrix[x][y] = matrix[y][x];
        }
    }
    return new_matrix;
}

template<typename T>
void print_matrix(const std::vector<std::vector<T> > &matrix) {
    for (unsigned long long x = 0; x < matrix[0].size(); ++x) {
        std::cout << "[";
        for (unsigned long long y = 0; y < matrix.size(); ++y) {
            std::cout << matrix[y][x];
        }
        std::cout << "]\n";
    }
}

class Operation {
    std::vector<unsigned long long> numbers = {};

    enum Operator {
        Plus = '+',
        Multiplicator = '*',
    };

    static Operator get_from_char(const char c) {
        if (c == '+') {
            return Plus;
        }
        if (c == '*') {
            return Multiplicator;
        }
        throw std::invalid_argument("Char does not equate to an operator!");
    }

    static char get_from_oper(const Operator oper) {
        if (oper == Plus) {
            return '+';
        }
        if (oper == Multiplicator) {
            return '*';
        }
        throw std::invalid_argument("Char does not equate to an operator!");
    }

    Operator oper;

public:
    explicit Operation(const std::vector<unsigned long long> &numbers, const char oper = '+') {
        this->numbers = numbers;
        this->oper = get_from_char(oper);
    }

    [[nodiscard]] unsigned long long apply_operator_to_values(const char oper) const {
        unsigned long long result;
        if (oper == Plus) {
            result = 0;
            for (const auto &value: numbers) {
                result += value;
            }
        } else {
            result = 1;
            for (const auto &value: numbers) {
                result *= value;
            }
        }
        return result;
    }

    [[nodiscard]] unsigned long long apply_operator_to_values() const {
        return this->apply_operator_to_values(oper);
    }

    void print() {
        for (const auto &n: numbers) {
            std::cout << n << " ";
        }
        std::cout << get_from_oper(oper) << " =" << apply_operator_to_values() << "\n";
    }
};

template<typename T>
static std::vector<T> extractWords(const std::string &str) {
    std::stringstream ss;

    /* Storing the whole string into string stream */
    ss << str;
    std::vector<T> output = {};
    /* Running loop till the end of the stream */
    std::string temp;
    T found;
    while (!ss.eof()) {
        /* extracting word by word from stream */
        ss >> temp;

        /* Checking the given word is integer or not */
        if (std::stringstream(temp) >> found)
            output.emplace_back(std::stoll(temp));

        /* To save from space at the end of string */
        temp = "";
    }
    return output;
}

static std::vector<char> extractChars(const std::string &str, const std::set<char> &acceptable_chars) {
    std::vector<char> output = {};
    for (const auto &c: str) {
        if (acceptable_chars.contains(c)) {
            output.emplace_back(c);
        }
    }
    return output;
}

bool is_blank(const std::vector<char>& char_vec) {
    for (const auto& c : char_vec) {
        if (c != 0 && c != ' ') {
            return false;
        }
    }
    return true;
}

std::vector<char> construct_empty_column(const int size) {
    std::vector<char> column = {};
    for (int i = 0; i < size; ++i) {
        column.emplace_back(' ');
    }
    return column;
}

std::string string_from_char_vec(const std::vector<char> &vec) {
    std::string result = "";
    for (char c : vec) {
        if (!std::string("0123456789 +*").contains(c)) {
            continue;
        }
        result += c;
    }
    return result;
}

bool transpose_file(std::string_view filename) {
    std::ifstream file{std::string(filename)};
    if (!file) {
        return false;
    }
    std::string temp{};
    std::vector<std::vector<char>> columns = {};
    std::vector<int> space_column_indexes = {};
    while (std::getline(file, temp)) {
        columns.emplace_back();
        std::vector<char>& current_column = columns[columns.size()-1];
        for (char & i : temp) {
            current_column.emplace_back(i);
        }
    }
    columns = transpose_matrix(columns);
    std::cout<<columns[0][1]<<"\n\n";
    //print_matrix(columns);
    std::vector<std::string> operations = {};
    std::string last_column_series = {};
    std::vector<std::string> operators = {};
    for (int i = 0; i < columns.size(); ++i) {
        std::cout<<last_column_series<<"  "<<i<<"  \n";
        if (is_blank(columns[i])) {
            space_column_indexes.emplace_back(i);
            operations.emplace_back(last_column_series);
            last_column_series = "";
            continue;
        }
        std::string result = " ";
        for (const auto& c : columns[i]) {
            if (c=='\n') {
                result += ' ';
                continue;
            }
            result += c;
        }
        last_column_series += result;
        char c = columns[i][columns[i].size()-1];
        std::string temp {c};
        temp += " ";
        operators.emplace_back(temp);
        if (operators[operators.size() - 1] == "  ") {
            operators.pop_back();
        }
        last_column_series.erase(last_column_series.end() - 1);
    }
    operations.emplace_back(last_column_series);
    //result.emplace_back(temp);
    unsigned long long sum = 0;
    for (int i = 0; i < operations.size(); ++i) {
        std::vector<unsigned long long> numbers = {};
        for (const auto& str : extractWords<unsigned long long>(operations[i])) {
            numbers.emplace_back(str);
            //std::cout<<str<<"\n";
        }
        std::cout<<operators[i]<<"\n";
        sum += Operation(numbers, operators[i][0]).apply_operator_to_values();

    }
    std::cout<<"\n";
    std::cout<<sum;
    return true;
}


class Assignment {
    std::vector<Operation> operations = {};

public:
    explicit Assignment(std::string_view filename) {
        std::ifstream file{std::string(filename)};
        std::string temp{};
        std::vector<std::vector<unsigned long long> > rows = {};
        std::vector<char> operators = {};
        while (std::getline(file, temp)) {
            if (temp.contains('+') || temp.contains('*')) {
                for (const auto &str: extractChars(temp, {'+', '*'})) {
                    operators.emplace_back(str);
                }
                continue;
            }
            std::vector<unsigned long long> row = {};
            for (const auto &str: extractWords<unsigned long long>(temp)) {
                row.emplace_back(str);
            }
            rows.emplace_back(row);
        }

        rows = transpose_matrix(rows);
        for (int y = 0; y < rows.size(); ++y) {
            operations.emplace_back(Operation(rows[y], operators[y]));
        }
    }

    void print() {
        for (auto &oper: operations) {
            oper.print();
        }
    }

    std::vector<unsigned long long> results() {
        std::vector<unsigned long long> results = {};
        for (auto &oper: operations) {
            results.emplace_back(oper.apply_operator_to_values());
        }
        return results;
    }
};

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    /*auto hw = Assignment("new.txt");
    auto results = hw.results();
    hw.print();
    unsigned long long actual = 0;
    for (const auto &result: results) {
        actual += result;
    }
    std::cout << actual;*/
    //std::cout<<"5";
    transpose_file("hw.txt");

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}
