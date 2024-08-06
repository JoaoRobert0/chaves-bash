#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <cstdlib>

std::vector<std::string> history;
// Before running ChavesBash, change the path to your initial directory here!
const std::string initial_directory = "/home/joao";

void process_command(std::string command) {
    history.push_back(command);

    // INTERNAL COMMANDS
    if (command == "exit") {
        exit(0);
    }

    if (command == "pwd") {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) { // get current working directory == getcwd
            std::cout << cwd << std::endl;
        } else {
            std::cerr << "Error obtaining the current working directory" << std::endl;
        }
        return; // return to avoid duplicate output
    }

    if (command == "clear") {
        // Clear the terminal screen
        std::system("clear");
        return; // Return after clearing the screen
    }

    std::string args[8];
    std::istringstream iss(command);
    std::string element;
    char split = ' ';

    int counter = 0;
    while (std::getline(iss, element, split) && counter < 8) {
        args[counter] = element;
        counter++; 
    }
    
    if (args[0] == "cd") {
        if (counter == 1) {
            // No arguments for cd, change to the initial directory
            if (chdir(initial_directory.c_str()) != 0) {
                std::cerr << "cd: no such file or directory: " << initial_directory << std::endl;
            }
        } else if (counter >= 2) {
            // Change to the directory specified by the second argument
            const std::string& directory = args[1];
            if (chdir(directory.c_str()) != 0) {
                std::cerr << "cd: no such file or directory: " << directory << std::endl;
            }
        } else {
            // Missing argument for cd
            std::cerr << "cd: missing argument" << std::endl;
        }
        return; // Return after changing the directory
    }

    if (args[0] == "history") {
        if (counter == 1) {
            // Show the last 10 commands
            int start = (history.size() > 10) ? history.size() - 10 : 0;
            for (int i = start; i < history.size(); i++) {
                std::cout << (history.size() - i - 1) << " " << history[i] << std::endl;
            }
        } else if (counter == 2 && args[1] == "-c") {
            // Clear the history
            history.clear();
        } else if (counter == 2) {
            // Executes a command from the history
            try {
                int offset = std::stoi(args[1]);
                if (offset < 0 || offset >= static_cast<int>(history.size())) {
                    std::cerr << "history: invalid offset" << std::endl;
                } else {
                    std::string cmd_to_execute = history[history.size() - offset - 2];
                    process_command(cmd_to_execute); // Recursively processes the command from the history
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "history: invalid argument" << std::endl;
            }
        } else {
            std::cerr << "history: invalid argument" << std::endl;
        }
        return;
    }

    // EXTERNAL COMMANDS
    std::string absolute_path = "/bin/" + command; 
    if (access(absolute_path.c_str(), F_OK) == 0) {
        if (access(absolute_path.c_str(), X_OK) == 0) {
            pid_t pid = fork();
            if (pid < 0) {
                std::cout << "Execution error!" << std::endl;
                return;
            } else if (pid == 0) {
                char *argv[2] = {(char *)command.c_str(), nullptr};
                execve(absolute_path.c_str(), argv, NULL);
            } else {
                waitpid(pid, nullptr, 0); 
            }
        } else {
            std::cout << "Permission denied: " << command << std::endl;
        }
    } else {
        std::cout << "Command not found: " << command << std::endl;
    }
}

int main() {
    std::cout << "Welcome to ChavesBash! Type 'exit' to quit, 'pwd' to print the current working directory, 'history' to view command history, or 'clear' to clear the terminal screen." << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << '.';
        std::cout.flush();
        sleep(1);
    }
    std::system("clear");
    while (true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        // Define colors using ANSI escape codes
        const std::string purple = "\033[1;35m";
        const std::string reset = "\033[0m";    // Reset color

        std::cout << purple << cwd << reset << "$> ";
        std::string command;
        std::getline(std::cin, command);
        process_command(command);
    }
    return 0;
}
