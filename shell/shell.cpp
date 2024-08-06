#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>

std::vector<std::string> history;

void process_command(std::string command) {
    history.push_back(command);

    // INTERNAL COMMANDS
    if (command == "exit")
        exit(0);

    if (command == "pwd") {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) { // get current working directory == getcwd
            std::cout << cwd << std::endl;
        } else {
            std::cerr << "Error obtaining the current working directory" << std::endl;
        }
        return; // return to avoid duplicate out
    }

    std::string args[8];
    std::istringstream iss(command);
    std::string element;
    char split = ' ';

    int counter = 0;
    while (std::getline(iss, element, split))
    {
        args[counter] = element;
        counter++; 
    }
    
    if (args[0] == "cd") {
        if (counter < 2) {
            // Missing argument for cd
            std::cerr << "cd: missing argument" << std::endl;
            return;
        }

        // The directory to change to is the second argument
        const std::string& directory = args[1];

        if (chdir(directory.c_str()) != 0) {
            // Failed to change directory
            std::cerr << "cd: no such file or directory: " << directory << std::endl;
        }
        return; // Return after changing the directory
    }


    // EXTERNAL COMMANDS

    // * necessário verificar se é para ser executado em background
    /*  Se for caminho relativo, procurar o comando na lista de diretórios
        Se for absoluto verifica se comando existe
    */
    std::string absolute_path = "/bin/" + command; 
    if (access(absolute_path.c_str(), F_OK) == 0) { // Arquivo existe no diretório
        if (access(absolute_path.c_str(), X_OK) == 0) { // Arquivo é executável
            pid_t pid = fork();
            if (pid < 0){ // Erro
                std::cout << "Erro de execução!" << std::endl;
                return;
            } else if (pid == 0){ //processo filho
                char * argv[2] = {(char *)command.c_str(), nullptr};
                execve(absolute_path.c_str(), argv, NULL);
            } else { // Processo pai
                /* Deve adicionar processo filho na lista (std::vector) 
                   de processos em execução para gerenciar background. */
                /* Processo pai espera processo filho terminar. */
                waitpid(pid, nullptr, 0); 
            }
        } else { // Arquivo não é executável
            std::cout << "permission denied: " << command << std::endl;
        }
    } else { // Arquivo não existe
        std::cout << "Command not found: " << command << std::endl;
    }
}

int main() {
    while (true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        std::cout << cwd <<"$ ";
        std::string command;
        getline(std::cin, command);
        process_command(command);
    }
    return 0;
}
