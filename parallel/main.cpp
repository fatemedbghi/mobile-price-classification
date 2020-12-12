#include "tool.h"

using namespace std;

int main(int argc, char** argv)
{
    char *path = argv[1];
    vector<string> file_names;
    file_names = files(path);
    vector<vector<float>> train_data = store_train_data(file_names[0]);
    vector<vector<long double>> weights =  store_weight(file_names[1]);
    vector<vector<float>> normalized_train = normalize_data(train_data);
    vector<int> prediction = predict_price(weights, normalized_train);
    return 0;
}