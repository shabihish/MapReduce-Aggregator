#include "cstdio"
#include "stdlib.h"
#include "unistd.h"
#include <dirent.h>
#include "file.h"
#include <iostream>
#include <fstream>
#include <map>
#include<bits/stdc++.h>
#include <cstring>

using namespace std;

map<string, int> *process_file(char *fname) {
    printf("Starting process.\n");
    string dir = TESTCASES_DIR;
    ifstream f(dir + "/" + fname);
    string content;
    f >> content;
    f.close();

    map<string, int> out;
    char *tok = strtok((char *) content.c_str(), ",");
    while (tok != nullptr) {
        auto pair = out.find(tok);
        if (pair != out.end()) {
            pair->second = pair->second + 1;
        } else
            out.insert(make_pair(tok, 1));
        tok = strtok(nullptr, ",");
    }
    for (auto pair: out)
        cout << pair.first << " " << pair.second << endl;
    return nullptr;
}

int main(int argc, char *argv[]) {
//    printf("Now in map\n");
    if (argc <= 2) {
        perror("Not enough arguments given to map.c.");
        exit(EXIT_FAILURE);
    }
    int fd0 = atoi(argv[0]);
    int fd1 = atoi(argv[1]);
    char *pipe_name = argv[2];


    close(fd1);
    char fname[MAX_FILENAME_LEN + 1] = {0};
    read(fd0, fname, MAX_FILENAME_LEN + 1);
    close(fd0);

//    printf("Map file name: %s\n", fname);
    process_file(fname);

    return 0;
}