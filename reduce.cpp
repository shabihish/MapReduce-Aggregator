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

map<string, int> process_file(map<string, int> pairs, string str) {
    string key;
    string value;
    stringstream str_stream(str);
    while (str_stream >> key) {
//        cout << key << endl;
        str_stream >> value;
        auto pair = pairs.find(key);
        if (pair != pairs.end()) {
            pair->second = pair->second + atoi(value.c_str());
        } else
            pairs.insert(make_pair(key, atoi(value.c_str())));
    }
    return pairs;
}


int main(int argc, char *argv[]) {
//    printf("Now in map\n");
    if (argc < 4) {
        perror("Not enough arguments given to reduce.c.");
        exit(EXIT_FAILURE);
    }
    int fd[2][2];
    fd[0][0] = atoi(argv[0]);
    fd[0][1] = atoi(argv[1]);
    fd[1][0] = atoi(argv[2]);
    fd[1][1] = atoi(argv[3]);

    close(fd[0][1]);
    char tmp[MAX_FILENAME_LEN + 1] = {0};
    read(fd[0][0], tmp, MAX_FILENAME_LEN + 1);
    close(fd[0][0]);

    int num_of_maps = atoi(tmp);


    map<string, int> out_map;
    char processed_out[8198];
    for (int i = 0; i < num_of_maps; i++) {
        string fifo_name = "FIFO" + to_string(i);
        mkfifo(fifo_name.c_str(), 0700);
        int fd = open(fifo_name.c_str(), O_RDONLY);
        read(fd, processed_out, sizeof(processed_out) / sizeof(char));
        close(fd);
        out_map = process_file(out_map, string(processed_out));
    }

    string out;
    for (auto pair: out_map) {
        out += pair.first + ", " + to_string(pair.second) + "\n";
    }

    close(fd[1][0]);
    write(fd[1][1], out.c_str(), MAX_FILENAME_LEN + 1);
    close(fd[1][1]);
    return 0;
}