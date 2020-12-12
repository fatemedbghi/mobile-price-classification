#include "tool.h"

using namespace std;

vector<string> files(char* path)
{
    struct dirent *de;
    DIR *dr = opendir(path); 
  
    if (dr == NULL) 
    { 
        printf("Could not open current directory\n"); 
        exit(1); 
    } 
    string str_path(path);
    vector <string> dir;
    while ((de = readdir(dr)) != NULL)
    {
        string str(de->d_name);
        string temp = str_path + str;
        if (str.compare("..") != 0 && str.compare(".") != 0)
            dir.push_back(temp);
    }
    
    closedir(dr);

    return dir;
}

vector<vector<long double>> store_weight(string file_name)
{
    vector<vector<long double>> weights;

    fstream fin; 
    fin.open(file_name, ios::in);   
    string line, word; 
    int i = 0;
    while (fin.good()){
        
        getline(fin, line);
        vector<long double> temp;
        stringstream l(line); 
        string token; 
        while(getline(l, token, ',') && i!=0) 
        {
            temp.push_back(stod(token));
        }
        if (i!=0) weights.push_back(temp);
        i++;
    }
    return weights;
}

vector<vector<float>> store_train_data(string file_name)
{
    vector<vector<float>> train;

    fstream fin; 
    fin.open(file_name, ios::in);   
    string line, word; 
    int i = 0;
    while (fin.good()){
        
        getline(fin, line);
        vector<float> temp;
        stringstream l(line); 
        string token; 
        while(getline(l, token, ',') && i!=0) 
        {
            temp.push_back(stof(token));
        }
        if (i!=0) train.push_back(temp);
        i++;
    }
    return train;
}

vector<vector<float>> normalize_data(vector<vector<float>> train)
{
    int i;
    for (i = 0; i < 20; i++)
    {
        if (i!=1 && i!=3 && i!=5 && i!=17 && i!=18 && i!=19) 
        {
            vector<float> temp;
            for(int j=0; j<train.size();j++)
            {
                temp.push_back(train[j][i]);
            }
            float min = *min_element(temp.begin(), temp.end());
            float max = *max_element(temp.begin(), temp.end());
            for(int j=0; j<train.size();j++)
            {
                train[j][i] = (train[j][i] - min)/(max - min);
            }
        }
    }
    return train;
}