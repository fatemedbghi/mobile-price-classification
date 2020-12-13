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
    vector <string> dir(2, "");
    while ((de = readdir(dr)) != NULL)
    {
        string str(de->d_name);
        string temp = str_path + str;
        if (str.compare("..") != 0 && str.compare(".") != 0)
        {
            if (str.compare("weights.csv") == 0)
                dir[1] = temp;
            else
                dir[0] = temp;   
        }
    }
    
    closedir(dr);
    return dir;
}

vector<vector<float>> store_weight(string file_name)
{
    vector<vector<float>> weight;

    fstream fin; 
    fin.open(file_name, ios::in);   
    string line, word; 
    int i = 0;
    while (fin.good()){
        
        getline(fin, line);
        vector<float> temp;
        stringstream l(line); 
        string token; 
        if (!line.empty())
        {
            while(getline(l, token, ',') && i!=0) 
                temp.push_back(stof(token));

            if (i!=0) weight.push_back(temp);
            i++;
        }
    }
    return weight;
}

vector<vector<float>> store_train_data(string file_name)
{
    vector<vector<float>> train_data;
    fstream fin; 
    fin.open(file_name, ios::in);   
    string line, word; 
    int i = 0;
    
    while (fin.good()){
        
        getline(fin, line);
        vector<float> temp;
        stringstream l(line); 
        string token; 
        if (!line.empty())
        {
            while(getline(l, token, ',') && i!=0)
            {
                temp.push_back(stof(token));
            }
            if (i!=0) train_data.push_back(temp);
            i++;
        }
    }
    return train_data;
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

vector<int> predict_price(vector<vector<float>> weights, vector<vector<float>> normalized_train)
{
    vector<int> predicion;
    for (int i=0; i< normalized_train.size(); i++)
    {
        vector<float>temp;
        for (int j=0; j<weights.size(); j++)
        {
            float price = 0;
            for (int k=0; k<weights[0].size()-1; k++)
                price += weights[j][k]*normalized_train[i][k];
            price += weights[j][weights[0].size()-1];
            temp.push_back(price);
        }
        
        int p = max_element(temp.begin(),temp.end()) - temp.begin();
        predicion.push_back(p);       
        temp.clear();
    }
    return predicion;
}

float calc_accuracy(vector<vector<float>> train_data, vector<int> prediction)
{
    float acc = 0;
    for (int i=0; i<prediction.size(); i++)
    {
        if (prediction[i] == train_data[i][20])
            acc++;
    } 
    return (acc*100/prediction.size());
}