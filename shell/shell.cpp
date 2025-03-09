#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

std::vector<std::string> split_command(const std::string& command) {
    std::vector<std::string> args;
    std::string arg;
    bool in_quotes = false;
    
    for (char ch : command) {
        if (ch == '"') {
            in_quotes = !in_quotes;
        } else if (ch == ' ' && !in_quotes) {
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

std::vector<std::string> split_path(const char* path_env) {
    std::vector<std::string> dirs;
    std::string dir;
    
    while (*path_env) {
        if (*path_env == ':') {
            dirs.push_back(dir);
            dir.clear();
        } else {
            dir += *path_env;
        }
        path_env++;
    }
    if (!dir.empty()) {
        dirs.push_back(dir);
    }
    return dirs;
}

void execute_command(const std::vector<std::string>& args) {
    if (args.empty()) return;

    // Встроенная команда cd
    if (args[0] == "cd") {
        if (args.size() > 2) {
            std::cerr << "cd: too many arguments\n";
            return;
        }
        
        const char* path = args.size() == 1 ? getenv("HOME") : args[1].c_str();
        if (!path) {
            std::cerr << "cd: HOME not set\n";
            return;
        }
        
        if (chdir(path) != 0) {
            std::cerr << "cd: " << strerror(errno) << '\n';
        }
        return;
    }

    // Поиск исполняемого файла
    std::string full_path;
    const std::string& cmd = args[0];
    
    // Если путь содержит '/', проверяем напрямую
    if (cmd.find('/') != std::string::npos) {
        if (access(cmd.c_str(), X_OK) == 0) {
            full_path = cmd;
        } else {
            std::cerr << cmd << ": " << strerror(errno) << '\n';
            return;
        }
    }
    // Ищем в PATH
    else {
        const char* path_env = getenv("PATH");
        std::vector<std::string> path_dirs;
        
        if (path_env) path_dirs = split_path(path_env);
        else path_dirs = {"/bin", "/usr/bin", "/usr/local/bin"};
        
        for (const auto& dir : path_dirs) {
            std::string test_path = dir.empty() ? cmd : dir + "/" + cmd;
            if (access(test_path.c_str(), X_OK) == 0) {
                full_path = test_path;
                break;
            }
        }
        
        if (full_path.empty()) {
            std::cerr << cmd << ": command not found\n";
            return;
        }
    }

    // Подготовка аргументов
    std::vector<char*> argv;
    for (const auto& arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);

    // Запуск процесса
    pid_t pid = fork();
    if (pid == 0) {
        execv(full_path.c_str(), argv.data());
        std::cerr << "exec: " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        waitpid(pid, nullptr, 0);
    } else {
        std::cerr << "fork failed\n";
    }
}

int main() {
    char cwd[PATH_MAX];
    const char* history_file = "/home/myron/.my_shell_history";

    // Загружаем историю команд из файла
    read_history(history_file);

    while (true) {
        // Получаем текущую директорию для приглашения
        if (getcwd(cwd, sizeof(cwd))) {
            std::string prompt = std::string(cwd) + "> ";
            // Функция readline позволяет редактировать ввод (стрелки влево/вправо) 
            // и просматривать историю (стрелки вверх/вниз)
            char* input = readline(prompt.c_str());
            if (!input) break;  // Если получен EOF (например, Ctrl+D)
            std::string command(input);
            free(input);

            // Если команда не пуста, добавляем её в историю
            if (!command.empty()) {
                add_history(command.c_str());
            }

            // Выход из интерпретатора
            if (command == "exit" || command == "quit") break;
            
            // Разбиваем команду и выполняем её
            auto args = split_command(command);
            execute_command(args);
        } else {
            std::cout << "?> ";
        }
    }

    // Сохраняем историю команд в файл
    write_history(history_file);
    return 0;
}
