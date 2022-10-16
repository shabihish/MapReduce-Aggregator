#include "unistd.h"
#include "sys/wait.h"
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "file.h"

using namespace std;

int main() {
    DIR *d;
    struct dirent *f;
    d = opendir(TESTCASES_DIR);
    vector<string> file_names;
    if (d != nullptr) {
        while ((f = readdir(d)) != nullptr) {
            //check if it's a regular file (not a directory)
            if (f->d_type == 8) {
                file_names.emplace_back(f->d_name);
            }
        }
        closedir(d);
    }

    int pipes[file_names.size()][2];
    for (int i = 0; i < file_names.size(); i++) {
        if (pipe(pipes[i]) < 0) {
            perror("Pipe creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    int reduce_pipe[2][2];
    for (int i = 0; i < 2; i++) {
        if (pipe(reduce_pipe[i]) < 0) {
            perror("Pipe creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Fork all the map processes
    for (int i = 0; i < file_names.size(); i++) {
        int pid = fork();
        if (pid == 0) {
            // Currently inside the child process
            char argv[2][256];
            sprintf(argv[0], "%d", pipes[i][0]);
            sprintf(argv[1], "%d", pipes[i][1]);

            execl("./map.o", argv[0], argv[1], NULL);
        } else if (pid > 0) {
            // Currently inside the parent process
            string write_str = file_names[i] + " FIFO" + to_string(i);
            close(pipes[i][0]);
            write(pipes[i][1], write_str.c_str(), MAX_FILENAME_LEN + 1);
            close(pipes[i][1]);
        } else {
            perror("Forking a map process was not successful.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Fork the ruduce process
    int pid = fork();
    if (pid == 0) {
        // Currently inside the child process
        char argv[6][256];
        sprintf(argv[0], "%d", reduce_pipe[0][0]);
        sprintf(argv[1], "%d", reduce_pipe[0][1]);
        sprintf(argv[2], "%d", reduce_pipe[1][0]);
        sprintf(argv[3], "%d", reduce_pipe[1][1]);

        execl("./reduce.o", argv[0], argv[1], argv[2], argv[3], NULL);
    } else if (pid > 0) {
        // Currently inside the parent process
        close(reduce_pipe[0][0]);
        write(reduce_pipe[0][1], to_string(file_names.size()).c_str(), MAX_FILENAME_LEN + 1);
        close(reduce_pipe[0][1]);

        char result[4192];
        close(reduce_pipe[1][1]);
        read(reduce_pipe[1][0], result, MAX_FILENAME_LEN + 1);
        close(reduce_pipe[1][0]);

        cout << result;
        ofstream f("out.csv");
        f << result;
        f.close();
    } else {
        perror("Forking the reduce process was not successful.\n");
        exit(EXIT_FAILURE);
    }

    return (0);
}