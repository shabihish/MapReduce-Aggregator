#include "cstdio"
#include "stdlib.h"
#include "unistd.h"
#include <dirent.h>
#include "file.h"
#include <iostream>
#include <fstream>
#include <map>
#include<bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>


using namespace std;

string process_file(char *fname) {
    string dir = TESTCASES_DIR;
    ifstream f(dir + "/" + fname);
    string content;
    f >> content;
    f.close();

    map<string, int> pairs;
    char *tok = strtok((char *) content.c_str(), ",");
    while (tok != nullptr) {
        auto pair = pairs.find(tok);
        if (pair != pairs.end()) {
            pair->second = pair->second + 1;
        } else
            pairs.insert(make_pair(tok, 1));
        tok = strtok(nullptr, ",");
    }

    string out;
    for (auto pair: pairs)
        out += pair.first + " " + to_string(pair.second) + "\n";

    return out;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Not enough arguments given to map process.");
        exit(EXIT_FAILURE);
    }
    int fd0 = atoi(argv[0]);
    int fd1 = atoi(argv[1]);


    close(fd1);
    char tmp[MAX_FILENAME_LEN + 1] = {0};
    read(fd0, tmp, MAX_FILENAME_LEN + 1);
    close(fd0);

    char fname[MAX_FILENAME_LEN + 1] = {0};
    char pipe_name[MAX_FILENAME_LEN + 1] = {0};

    stringstream tmpstream(tmp);
    tmpstream >> fname;
    tmpstream >> pipe_name;

    string processed_out = process_file(fname);

    mkfifo(pipe_name, 0700);
    int fd = open(pipe_name, O_WRONLY);
    write(fd, processed_out.c_str(), processed_out.size() + 1);
    close(fd);

    return 0;
}