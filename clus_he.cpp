#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

string trim(const string &s) {
    string result = s;
    result.erase(result.begin(), find_if(result.begin(), result.end(),
        [](unsigned char ch){ return !isspace(ch); }));
    result.erase(find_if(result.rbegin(), result.rend(),
        [](unsigned char ch){ return !isspace(ch); }).base(), result.end());
    return result;
}


vector<double> read_csv_file(const string &filename, int colIndex) {
    vector<double> values;
    ifstream file(filename);
    

    string line;
    getline(file, line); 
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string field;
        int idx = 0;
        bool valid = false;
        while (getline(ss, field, ',')) {
            if (idx == colIndex) {
                field = trim(field);
                if (!field.empty()) {
                    try {
                        values.push_back(stod(field));
                        valid = true;
                    } catch (...) {
                        cout << "Skipping invalid value: " << field << endl;
                    }
                }
                break;
            }
            idx++;
        }
        if (!valid) continue;
    }
    return values;
}



double abs_val(double num1) {
    if (num1 >= 0)
        return num1;
    else
        return -num1;
}

double single_linkage(const vector<double> &c1, const vector<double> &c2) {
    double mini= 1e9;
    for (double v1 : c1)
        for (double v2 : c2) {
            double d1 = abs_val(v1 - v2);
            if (d1 < mini) mini = d1;
        }
    return mini;
}


double complete_linkage(const vector<double> &c1, const vector<double> &c2) {
    double maxi= -1e9;
    for (double v1 : c1)
        for (double v2 : c2) {
            double d2 =  abs_val(v1 - v2);
            if (d2 > maxi) maxi = d2;
        }
    return maxi;
}


double average_linkage(const vector<double> &c1, const vector<double> &c2) {
    double sum = 0;
    int count = 0;
    for (double v1 : c1)
        for (double v2 : c2) {
            sum +=  abs_val(v1 - v2);
            count++;
        }
    return sum / count;
}


void printClusters(const vector<vector<double>> &clusters) {
    cout << "[";
    for (size_t i = 0; i < clusters.size(); i++) {
        cout << "[";
        for (size_t j = 0; j < clusters[i].size(); j++) {
            cout << clusters[i][j];
            if (j != clusters[i].size() - 1) cout << ", ";
        }
        cout << "]";
        if (i != clusters.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

    


int main() {
    string filename;
    cout << "Enter CSV filename: ";
    cin >> filename;

    int col_idx;
    cout << "Enter column_indx  for clustering: ";
    cin >> col_idx;

    vector<double> values = read_csv_file(filename, col_idx);
    

    cout << "Choose linkage type:\n1] Single\n2] Complete\n3] Average\n";
    int your_choice;
    cin >>  your_choice;
    
    vector<vector<double>> clusters;
    for (double v : values) clusters.push_back({v});

    int step = 1;
    while (clusters.size() > 1) {
        double minDist = 1e9;
        int idx1 = -1, idx2 = -1;

        for (size_t i = 0; i < clusters.size(); i++) {
            for (size_t j = i + 1; j < clusters.size(); j++) {
                double dist = 0;
                if (your_choice == 1) dist = single_linkage(clusters[i], clusters[j]);
                else if (your_choice == 2) dist = complete_linkage(clusters[i], clusters[j]);
                else dist = average_linkage(clusters[i], clusters[j]);

                if (dist < minDist) {
                    minDist = dist;
                    idx1 = i;
                    idx2 = j;
                }
            }
        }

        cout << "\nStep " << step++ << ": Merging clusters with distance = " 
             << fixed << setprecision(4) << minDist << endl;

        clusters[idx1].insert(clusters[idx1].end(), clusters[idx2].begin(), clusters[idx2].end());
        clusters.erase(clusters.begin() + idx2);

        printClusters(clusters);
    }

    cout << "\nFinal Cluster: ";
    printClusters(clusters);

    return 0;
}
