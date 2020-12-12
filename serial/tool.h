#ifndef TOOL_H
#define TOOL_H

#include <iostream>
#include <string> 
#include <stdio.h> 
#include <string.h> 
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <dirent.h> 
#include <iomanip>

#define MSGSIZE 1024

using namespace std;

vector<string> files(char* path);
vector<vector<float>> store_weight(string file_name);
vector<vector<float>> store_train_data(string file_name);
vector<vector<float>> normalize_data(vector<vector<float>> train);
vector<int> predict_price(vector<vector<float>> weights, vector<vector<float>> normalized_train);
float calc_accuracy(vector<vector<float>> train_data, vector<int> prediction);
#endif