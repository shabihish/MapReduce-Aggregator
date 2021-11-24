#include "unistd.h"
#include "sys/wait.h"
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
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

    char *pipe_name = "NFIFO";
    // Continue iterating till all file names are iterated through
    for (int i = 0; i < file_names.size(); i++) {
        int pid = fork();
        if (pid == 0) {
            // Currently inside the child process
            char argv[3][256];
            sprintf(argv[0], "%d", pipes[i][0]);
            sprintf(argv[1], "%d", pipes[i][1]);
            sprintf(argv[2], "%s", pipe_name);
//            printf("Finished initializing argv.\n");
            execl("./map.o", argv[0], argv[1], argv[2], NULL);
        } else if (pid > 0) {
            // Currently inside the parent process
                close(pipes[i][0]);
                write(pipes[i][1], file_names[i].c_str(), MAX_FILENAME_LEN + 1);
                close(pipes[i][1]);
        }
    }
    return (0);


}