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

vector<int> predict_price(vector<vector<long double>> weights, vector<vector<float>> normalized_train)
{
    vector<int> predicion;
    for (int i=0; i< normalized_train.size(); i++)
    {
        vector<int>temp;
        for (int j=0; j<weights.size(); j++)
        {
            long double price = 0;
            for (int k=0; k<weights[0].size()-1; k++)
                price += weights[j][k]*normalized_train[i][k];
            price += weights[j][weights[0].size()-1];
            temp.push_back(price);
        }
        // for (int k=0;k<temp.size();k++) cout<<temp[k]<<endl;
        int p = max_element(temp.begin(),temp.end()) - temp.begin();
        predicion.push_back(p);
    }
    return predicion;
}