#ifndef TOOL_H
#define TOOL_H

#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <bits/stdc++.h> 
#include <dirent.h> 
#include <iomanip>
#include <time.h>

using namespace std;

vector<string> files(char* path);
vector<vector<float>> store_weight(string file_name);
vector<vector<float>> store_train_data(string file_name);
vector<vector<float>> normalize_data(vector<vector<float>> train);
vector<int> predict_price(vector<vector<float>> weights, vector<vector<float>> normalized_train);
float calc_accuracy(vector<vector<float>> train_data, vector<int> prediction);

#endif
