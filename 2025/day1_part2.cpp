#include <iostream>
#include <vector>
#include <fstream>


bool read_file(std::vector<std::string>& output, const std::string& input) {
    std::ifstream file {input};
    if (!file) {
        return false;
    }
    std::string temp {};
    while (std::getline(file, temp)) {
        output.push_back(temp);
    }
    return true;
}

int execute_command(int value, std::string_view command, int& zerocount) {
    if (command.size() < 2) {
        return -1;
    }
    if ((command[0] != 'L' && command[0] != 'R')) {
        return -2;
    }
    if (value < 0 || value > 99) {
        return -3;
    }
    int command_value;
    char command_dir = command[0];
    try {
        command_value = std::stoi(std::string(command.substr(1)));
    } catch (std::exception&) {
        return -4;
    }
    zerocount += command_value/100;
    command_value %= 100;
    if (command_value == 0) {
        return value;
    }
    if (command_dir == 'L') {
        value -= command_value;
        if (value == 0 || (value < 0 && value + command_value != 0/*needed so we dont count it twice if alr 0*/)) {
            zerocount++;
        }
        if (value < 0) {
            value = 100 + value;
        }

    } else if (command_dir == 'R') {
        value += command_value;
        if (value >= 100) {
            value %= 100;
            zerocount++;
        }
    }
    return value;
}

int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    std::vector<std::string> rotations = {};
    bool success = read_file(rotations, "rots.txt");
    if (!success) {
        return -1;
    }
    int value = 50;
    int zerocount = 0;
    for (const auto& rot : rotations) {
        value = execute_command(value, rot, zerocount);
        std::cout<<rot<<"\n"<<value<<"\n"<<zerocount<<"\n";
    }
    std::cout<<zerocount;

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}