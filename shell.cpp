#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <errno.h>

std::vector<std::string> split_command(const std::string& command) {
    std::vector<std::string> args;
    std::string arg;
    for (char ch : command) {
        if (ch == ' ') {
            if (!arg.empty()) {
                args.push_back(arg);
                arg.clear();
            }
        } else {
            arg += ch;
        }
    }
    if (!arg.empty()) {
        args.push_back(arg);
    }
    return args;
}

void execute_command(std::vector<std::string> args) {
    if (args.empty()) return;

    std::string command = args[0];
    size_t last_slash = command.find_last_of('/');
    if (last_slash != std::string::npos) {
        command = command.substr(last_slash + 1);
    }
    std::string full_path = "/bin/" + command;

    if (access(full_path.c_str(), X_OK) != 0) {
        std::cerr << "Команда не найдена или недоступна для выполнения: " << command << std::endl;
        return;
    }

    args[0] = full_path;

    std::vector<char*> argv;
    for (auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) {
        execv(argv[0], argv.data());
        std::cerr << "Ошибка выполнения команды: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        std::cerr << "Ошибка: не удалось создать дочерний процесс" << std::endl;
    }
}

int main() {
    std::string input;
    while (true) {
        std::cout << "myshell> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        auto args = split_command(input);
        if (!args.empty()) {
            execute_command(args);
        }
    }

    return 0;
}