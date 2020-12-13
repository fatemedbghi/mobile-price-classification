#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <bits/stdc++.h> 
#include <dirent.h> 
#include <iomanip>
#include <time.h>
#include "pthread.h"

#define NUMBER_OF_THREADS 4
#define FEATURES 20

using namespace std;

struct thread_data_1
{
    int tid;
    string train_file_name;
};

struct thread_out
{
    int acc;
    int total;
};

vector<vector<vector<float>>> train(NUMBER_OF_THREADS);
vector<vector<float>> weights;
vector<vector<float>> min_all(FEATURES);
vector<vector<float>> max_all(FEATURES);
vector<float> min_total(20, 99999999);
vector<float> max_total(20, -99999999);

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
    vector <string> dir(1,"");
    while ((de = readdir(dr)) != NULL)
    {
        string str(de->d_name);
        string temp = str_path + str;
        if (str.compare("..") != 0 && str.compare(".") != 0)
        {
            if (str.compare("weights.csv") == 0)
                dir[0] = temp;
            else
                dir.push_back(temp);   
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

vector<vector<float>> normalize_data(vector<vector<float>> train_data)
{
    int i;
    for (i = 0; i < FEATURES; i++)
    {
        if (i!=1 && i!=3 && i!=5 && i!=17 && i!=18 && i!=19) 
        {
            for(int j=0; j<train_data.size();j++)
            {
                train_data[j][i] = (train_data[j][i] - min_total[i])/(max_total[i] - min_total[i]);
            }
        }
    }
    return train_data;
}

vector<int> predict_price(vector<vector<float>> normalized_train)
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

int calc_accuracy(vector<vector<float>> train_data, vector<int> prediction)
{
    float acc = 0;
    for (int i=0; i<prediction.size(); i++)
    {
        if (prediction[i] == train_data[i][20])
            acc++;
    } 
    return acc;
}

void* min_and_max(void* data)
{
    struct thread_data_1* my_data = (struct thread_data_1*) data;
    int thread_id = my_data->tid;
    string file_name = my_data->train_file_name;
    vector<vector<float>> train_data = store_train_data(file_name);
    train[thread_id] = train_data;
    for (int i = 0; i < FEATURES; i++)
    {
        vector<float> temp;
        for(int j=0; j<train_data.size();j++)
        {
            temp.push_back(train_data[j][i]);
            
        }
        min_all[thread_id].push_back(*min_element(temp.begin(), temp.end()));
        max_all[thread_id].push_back(*max_element(temp.begin(), temp.end()));
    }
    pthread_exit(NULL);
}

void* predic_phone_price(void* data)
{
    int thread_id =  (long)data;
    vector<vector<float>> train_data = train[thread_id];
    vector<vector<float>> normalized_train = normalize_data(train_data);
    vector<int> prediction = predict_price(normalized_train);
    int accuracy = calc_accuracy(normalized_train, prediction);
    struct thread_out out;
    out.acc = accuracy;
    out.total = prediction.size();
    pthread_exit((void*)&out);
}

int main(int argc, char** argv)
{
    clock_t tStart = clock();
    char *path = argv[1];
    int return_code;
    void* status;
    vector<string> file_names;
    file_names = files(path); 

    weights =  store_weight(file_names[0]);
    

    struct thread_data_1 data_1_arr[NUMBER_OF_THREADS];
    for (int i=0; i<NUMBER_OF_THREADS;i++){
        data_1_arr[i].tid = i;
        data_1_arr[i].train_file_name = file_names[i+1];
    }

    pthread_t threads[NUMBER_OF_THREADS];

    for(int tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		pthread_create(&threads[tid], NULL, min_and_max,(void*)&data_1_arr[tid]);
	} 
    
    for(int tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		pthread_join(threads[tid], &status);
        for (int i=0; i < FEATURES; i++) {
            if(min_all[tid][i] < min_total[i]) min_total[i] = min_all[tid][i];
            if(max_all[tid][i] > max_total[i]) max_total[i] = max_all[tid][i];
        }
	}
    
    float acc = 0;
    float totall = 0;

    for(long tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		pthread_create(&threads[tid],NULL, predic_phone_price, (void*)tid);
	}
    
    for(int tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		pthread_join(threads[tid], &status);
        struct thread_out* my_data = (struct thread_out*) status;
        acc += my_data->acc;
        totall += my_data->total;
	}

    float accuracy = (acc*100)/totall;
    cout<<"Accuracy: "<<fixed<<setprecision(2)<<accuracy<<"%"<<endl;
    // printf("Time taken: %fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}