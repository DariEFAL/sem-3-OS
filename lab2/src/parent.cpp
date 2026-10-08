#include <iostream>
#include <string>
#include <fcntl.h>      // open
#include <unistd.h>
#include <cstdlib>      // exit
#include <sys/wait.h>   // waitpid, WIFEXITED, WEXITSTATUS
#include <cerrno>
#include <cstring>      // std::strerror

int main() {
    std::string path;
    std::cout << "Введите имя файла: ";
    std::getline(std::cin, path);

    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error: open failed: " << std::strerror(errno) << '\n';
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Error: pipe failed: " << std::strerror(errno) << '\n';
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Error: fork failed: " << std::strerror(errno) << '\n';
        return 1;
    }
    else if (pid == 0) {
        dup2(fd, 0);
        dup2(pipefd[1], 1);

        close(pipefd[0]);
        close(fd);
        close(pipefd[1]);

        execl("./build/child", "child", NULL);

        std::cerr << "Error: execl failed: " << std::strerror(errno) << '\n';
        _exit(127); //системный вызов, который не делает flush и не вызывает atexit
        
    }
    else {
        close(pipefd[1]);
        close(fd);

        char buf[4096];
        ssize_t n;
        while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
            std::cout.write(buf, n);
            std::cout << std::flush;
        }
        if (n == -1) {
            std::cerr << "read failed: " << std::strerror(errno) << '\n';
        }

        close(pipefd[0]);

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "waitpid failed: " << std::strerror(errno) << '\n';
            return 1;
        }
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code != 0) {
                std::cerr << "child exited with code " << code << '\n';
                return 1;
            }
        } else if (WIFSIGNALED(status)) {
            std::cerr << "child killed by signal " << WTERMSIG(status) << '\n';
            return 1;
        }

        return 0;
    }
}