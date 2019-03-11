#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cerrno>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "shell.h"
#include "utility.h"
#include "limitsfix.h"

int main()
{
    // Load prompt
    std::string prompt = load_prompt();

    if (prompt.empty())
    {
        std::cout << "Couldn't load " << SHELL_CFG << std::endl;
        std::cout << "Exiting..." << std::endl;
        return 1;
    }

    // Shell main loop
    std::string input;
    while (true)
    {
        std::cout << prompt << ": ";
        std::getline(std::cin, input);
        if (input == "new_file")
            new_file();
        if (input == "ls")
            list();
        if (input == "find")
            find();
        if (input == "python")
            python();
        if (input == "exit" || input == "quit")
            return 0;
        if (input == "error")
            return 1;
        if (input == "version")
            version();
    }
}

void new_file()
{
    // Filename
    std::cout << "File name: " << std::flush;
    std::string filename;
    std::getline(std::cin, filename);

    // Content
    std::cout << "Content, close with <EOF>" << std::endl;
    std::vector<std::string> content;
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (line == "<EOF>")
            break;

        content.push_back(line + "\n");
    }

    // Create file
    int flags = O_CREAT | O_TRUNC | O_WRONLY; // Create if not exist | Overwrite existing content
    mode_t mode = 0775;

    // We use the SYS_open call because SYS_creat is not defined on macOS (not anymore)
    // Return value is file handle or NULL if file couldn't be opened/created
    int file = syscall(SYS_open, filename.c_str(), flags, mode);

    // If error
    if (file == -1)
    {
        std::cout << "Couldn't create file" << std::endl;
        std::cout << "Error: " << errno << " - " << std::strerror(errno) << std::endl;
        return;
    }

    // Write to file
    for (auto line : content)
    {
        int res = syscall(SYS_write, file, line.c_str(), line.length());

        // If error
        if (res == -1)
        {
            std::cout << "Couldn't write line" << std::endl;
            std::cout << "Error: " << errno << " - " << std::strerror(errno) << std::endl;
            return;
        }
    }

    // Close file
    int res = syscall(SYS_close, file);

    // If error
    if (res == -1)
    {
        std::cout << "Couldn't close file" << std::endl;
        std::cout << "Error: " << errno << " - " << std::strerror(errno) << std::endl;
        return;
    }
}

void list()
{
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cout << "Couldn't fork" << std::endl;
        std::cout << "Error: " << errno << " - " << std::strerror(errno) << std::endl;
        return;
    }
    else if (pid == 0)
    {
        // Child
        const char *arg1 = "-la";
        const char *arg2 = ".";
        const char *argv[] = {arg1, arg2, NULL};
        const char *env = NULL;

        syscall(SYS_execve, "/bin/ls", env, argv);
    }
    else
    {
        wait(NULL);
    }
}

void find()
{
    std::cout << "What to find: ";
    std::string target;
    std::getline(std::cin, target);

    // Pipes
    int pipefd[2];
    pipe(pipefd);

    // Find
    pid_t pid = fork();
    if (pid == -1)
    {
        std::cout << "Couldn't fork" << std::endl;
        std::cout << "Error: " << errno << " - " << std::strerror(errno) << std::endl;
        return;
    }
    else if (pid == 0)
    {
        close(pipefd[0]);

        // Redirect stdout to pipe
        dup2(pipefd[1], 1);

        // Child 1 - Find
        const char *arg1 = ".";
        const char *argv[] = {arg1, NULL};
        const char *env = NULL;

        syscall(SYS_execve, "/usr/bin/find", env, argv);

        // Close pipe
        close(pipefd[1]);
    }
    else
    {
        close(pipefd[1]);
        wait(NULL); // Wait for find
    }

    // Grep
    pid = fork();
    if (pid == -1)
    {
        std::cout << "Couldn't fork" << std::endl;
        std::cout << "Error: " << errno << " - " << std::strerror(errno) << std::endl;
        return;
    }
    else if (pid == 0)
    {
        dup2(pipefd[1], STDIN_FILENO);

        const char *argv[] = {target.c_str(), NULL};
        const char *env = NULL;

        syscall(SYS_execve, "/usr/bin/grep", env, argv);
    }
    else
    {
        wait(NULL); // Wait for grep
    }
}

void python()
{
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cout << "Couldn't fork" << std::endl;
        std::cout << "Error: " << errno << " - " << std::strerror(errno) << std::endl;
        return;
    }
    else if (pid == 0)
    {
        // Child
        const char *argv[] = {NULL};
        const char *env = NULL;

        int res = syscall(SYS_execve, "/usr/local/bin/python", argv, env);
    }
    else
    {
        wait(NULL);
    }
}

void version()
{
    std::cout << "    ____                        _____ __         ____" << std::endl;
    std::cout << "   / __ )____ __________  ____ / ___// /_  ___  / / /" << std::endl;
    std::cout << "  / __  / __ `/ ___/ __ \\/ __ \\\\__ \\/ __ \\/ _ \\/ / / " << std::endl;
    std::cout << " / /_/ / /_/ / /__/ /_/ / / / /__/ / / / /  __/ / /  " << std::endl;
    std::cout << "/_____/\\__,_/\\___/\\____/_/ /_/____/_/ /_/\\___/_/_/   " << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Version: " << baconshell_VERSION << std::endl;
}

// Return prompt string or empty string if prompt couldn't be loaded
std::string load_prompt()
{
    // Load prompt config if possible
    std::ifstream ifs(SHELL_CFG);
    if (ifs.fail())
        return std::string();

    std::string prompt = slurp(ifs);

    // Load hostname and username
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    getlogin_r(username, LOGIN_NAME_MAX);

    // Replace prompt vars
    replace(prompt, "$host", std::string(hostname));
    replace(prompt, "$user", std::string(username));

    return prompt;
}
