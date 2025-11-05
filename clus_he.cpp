// #include <bits/stdc++.h>
// using namespace std;

// struct Cluster {
//     vector<string> labels;
//     vector<double> values;
// };

// // Trim leading/trailing whitespace
// string trim(const string &s) {
//     auto start = s.find_first_not_of(" \t\r\n");
//     auto end = s.find_last_not_of(" \t\r\n");
//     if (start == string::npos) return "";
//     return s.substr(start, end - start + 1);
// }

// // Compute centroid of a cluster
// double centroid(const Cluster &c) {
//     double sum = 0;
//     for (double v : c.values) sum += v;
//     return sum / c.values.size();
// }

// // Distance between two clusters based on linkage type
// double clusterDistance(const Cluster &c1, const Cluster &c2, int linkageType) {
//     if (linkageType == 1) { // Single
//         double minD = DBL_MAX;
//         for (double a : c1.values)
//             for (double b : c2.values)
//                 minD = min(minD, abs(a - b));
//         return minD;
//     } else if (linkageType == 2) { // Complete
//         double maxD = 0;
//         for (double a : c1.values)
//             for (double b : c2.values)
//                 maxD = max(maxD, abs(a - b));
//         return maxD;
//     } else { // Average
//         double sum = 0;
//         int count = 0;
//         for (double a : c1.values)
//             for (double b : c2.values) { sum += abs(a - b); count++; }
//         return sum / count;
//     }
// }

// // Print current clusters with centroids
// void printClusters(const vector<Cluster> &clusters) {
//     int idx = 1;
//     cout << "\nCurrent Clusters and Centroids:\n";
//     for (auto &c : clusters) {
//         cout << "Cluster " << idx++ << ": " << c.labels.size() << " points [";
//         for (size_t i = 0; i < c.labels.size(); i++) {
//             cout << c.labels[i];
//             if (i != c.labels.size() - 1) cout << ", ";
//         }
//         cout << "], Centroid = [" << fixed << setprecision(2) << centroid(c) << "]\n";
//     }
// }

// // Print distance matrix between clusters
// void printDistanceMatrix(const vector<Cluster> &clusters, int linkageType) {
//     int n = clusters.size();
//     vector<vector<double>> mat(n, vector<double>(n, 0));
//     for (int i = 0; i < n; i++)
//         for (int j = i + 1; j < n; j++)
//             mat[i][j] = mat[j][i] = clusterDistance(clusters[i], clusters[j], linkageType);

//     cout << "\nDistance Matrix:\n            ";
//     for (auto &c : clusters) {
//         for (auto &lbl : c.labels) cout << lbl;
//         cout << "       ";
//     }
//     cout << "\n";
//     for (int i = 0; i < n; i++) {
//         for (auto &lbl : clusters[i].labels) cout << lbl;
//         cout << "      ";
//         for (int j = 0; j < n; j++)
//             cout << fixed << setprecision(2) << mat[i][j] << "       ";
//         cout << "\n";
//     }
// }

// int main() {
//     string filename;
//     cout << "Enter CSV filename: ";
//     cin >> filename;

//     ifstream file(filename);
//     if (!file.is_open()) { cerr << "Cannot open file.\n"; return 1; }

//     string line;
//     getline(file, line); // read header
//     stringstream ss(line);
//     vector<string> headers;
//     string col;
//     while (getline(ss, col, ',')) headers.push_back(trim(col));

//     cout << "Columns in CSV:\n";
//     for (int i = 0; i < headers.size(); i++)
//         cout << i << "] " << headers[i] << "\n";

//     int colIndex;
//     cout << "Enter column index to cluster on: ";
//     cin >> colIndex;

//     vector<Cluster> clusters;
//     int rowNum = 0;
//     while (getline(file, line)) {
//         rowNum++;
//         stringstream ss(line);
//         string label, valueStr;
//         if (!getline(ss, label, ',')) { cerr << "Row " << rowNum << " invalid, skipping.\n"; continue; }
//         label = trim(label);

//         // Read the desired column
//         bool valid = false;
//         for (int i = 0; i <= colIndex; i++) {
//             if (!getline(ss, valueStr, ',')) break;
//         }

//         valueStr = trim(valueStr);
//         try {
//             double val = stod(valueStr);
//             clusters.push_back({{label}, {val}});
//         } catch (...) {
//             cerr << "Row " << rowNum << " invalid numeric value, skipping.\n";
//         }
//     }
//     file.close();

//     cout << "Choose linkage type:\n1] Single\n2] Complete\n3] Average\n";
//     int linkageType; cin >> linkageType;

//     int step = 1;
//     while (clusters.size() > 1) {
//         printClusters(clusters);
//         printDistanceMatrix(clusters, linkageType);

//         // Find closest pair
//         double minDist = DBL_MAX;
//         int idx1 = -1, idx2 = -1;
//         for (size_t i = 0; i < clusters.size(); i++)
//             for (size_t j = i + 1; j < clusters.size(); j++) {
//                 double d = clusterDistance(clusters[i], clusters[j], linkageType);
//                 if (d < minDist) { minDist = d; idx1 = i; idx2 = j; }
//             }

//         // Merge clusters
//         cout << "\nStep " << step++ << ": Merging clusters " 
//              << idx1+1 << " and " << idx2+1 
//              << " (distance=" << fixed << setprecision(2) << minDist << ")\n";

//         clusters[idx1].labels.insert(clusters[idx1].labels.end(),
//                                      clusters[idx2].labels.begin(), clusters[idx2].labels.end());
//         clusters[idx1].values.insert(clusters[idx1].values.end(),
//                                      clusters[idx2].values.begin(), clusters[idx2].values.end());
//         clusters.erase(clusters.begin() + idx2);
//     }

//     cout << "\n--- Final Cluster ---\n";
//     printClusters(clusters);
// }


//simple
#include <bits/stdc++.h>
using namespace std;

// Trim whitespace
string trim(const string &s) {
    string result = s;
    result.erase(result.begin(), find_if(result.begin(), result.end(),
        [](unsigned char ch){ return !isspace(ch); }));
    result.erase(find_if(result.rbegin(), result.rend(),
        [](unsigned char ch){ return !isspace(ch); }).base(), result.end());
    return result;
}

// Cluster structure
struct Cluster {
    vector<string> items;
    Cluster() {}
    Cluster(const string &name) { items.push_back(name); }
};

// Get distance between two items
double getDistance(const map<pair<string, string>, double> &distMap,
                   const string &a, const string &b) {
    if (a == b) return 0;
    auto it = distMap.find({a, b});
    if (it != distMap.end()) return it->second;
    return distMap.at({b, a});
}

// Compute distance between two clusters
double clusterDistance(const Cluster &c1, const Cluster &c2,
                       const map<pair<string, string>, double> &distMap,
                       const string &method) {
    double minD = DBL_MAX, maxD = 0, sum = 0;
    int count = 0;

    for (auto &i : c1.items)
        for (auto &j : c2.items) {
            double d = getDistance(distMap, i, j);
            minD = min(minD, d);
            maxD = max(maxD, d);
            sum += d;
            count++;
        }

    if (method == "single") return minD;
    if (method == "complete") return maxD;
    return sum / count; // average linkage
}

// Print clusters in compact format
void printClusters(const vector<Cluster> &clusters) {
    cout << "[";
    for (size_t i = 0; i < clusters.size(); i++) {
        cout << "[";
        for (size_t j = 0; j < clusters[i].items.size(); j++) {
            cout << clusters[i].items[j];
            if (j != clusters[i].items.size() - 1) cout << ", ";
        }
        cout << "]";
        if (i != clusters.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Hierarchical clustering
void hierarchicalClustering(const vector<string> &labels,
                            const map<pair<string, string>, double> &distMap,
                            const string &method) {
    vector<Cluster> clusters;
    for (auto &l : labels) clusters.push_back(Cluster(l));

    cout << "\n" << method << " LINKAGE CLUSTERING:\n";
    int step = 1;

    while (clusters.size() > 1) {
        double minDist = DBL_MAX;
        int a = -1, b = -1;

        for (int i = 0; i < clusters.size(); i++) {
            for (int j = i + 1; j < clusters.size(); j++) {
                double d = clusterDistance(clusters[i], clusters[j], distMap, method);
                if (d < minDist) { minDist = d; a = i; b = j; }
            }
        }

        cout << "Step " << step++ << ": Merging clusters with distance = "
             << fixed << setprecision(4) << minDist << endl;

        clusters[a].items.insert(clusters[a].items.end(),
                                 clusters[b].items.begin(), clusters[b].items.end());
        clusters.erase(clusters.begin() + b);

        printClusters(clusters);
    }

    cout << "\nFinal Cluster: ";
    printClusters(clusters);
}

int main() {
    string filename;
    cout << "Enter CSV filename (distance matrix): ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) { cerr << "Error opening file\n"; return 1; }

    string line;
    getline(file, line); // header
    stringstream ss(line);
    string val;
    getline(ss, val, ','); // skip first empty
    vector<string> headers;
    while (getline(ss, val, ',')) headers.push_back(trim(val));

    map<pair<string, string>, double> distMap;
    while (getline(file, line)) {
        stringstream ss(line);
        string rowLabel;
        getline(ss, rowLabel, ',');
        for (int i = 0; i < headers.size(); i++) {
            string cell;
            getline(ss, cell, ',');
            distMap[{rowLabel, headers[i]}] = stod(cell);
        }
    }
    file.close();

    cout << "Choose linkage type:\n1] Single\n2] Complete\n3] Average\n";
    int choice; cin >> choice;
    string method = (choice == 1 ? "single" : (choice == 2 ? "complete" : "average"));

    hierarchicalClustering(headers, distMap, method);

    return 0;
}
