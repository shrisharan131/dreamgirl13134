// #include <bits/stdc++.h>
// using namespace std;

// vector<string> split(const string &s){
//     vector<string> res;
//     string temp;
//     stringstream ss(s);
//     while(getline(ss,temp,',')) res.push_back(temp);
//     return res;
// }

// int main(){
//     string filename;
//     cout<<"Enter file: ";
//     cin>>filename;
//     ifstream file(filename);
//     vector<vector<string>> data;
//     string line;
//     getline(file,line);
//     while(getline(file,line)) if(!line.empty()) data.push_back(split(line));
//     file.close();

//     int n = data.size();
//     int m = data[0].size()-1; 
//     vector<string> decision_classes;
//     map<string,int> class_count;

//     for(auto &row:data) class_count[row[m]]++;
//     for(auto &p:class_count) decision_classes.push_back(p.first);

//     cout<<"decision_classes: ";
//     for(auto &c:decision_classes) cout<<c<<" ";
//     cout<<"\n";

//     map<string,double> prior;
//     for(auto &c:decision_classes) prior[c] = (double)class_count[c]/n;

//     map<string,vector<map<string,int>>> condition;
//     for(auto &c:decision_classes) condition[c] = vector<map<string,int>>(m);
//     for(auto &row:data)
//     {
//         string cl = row[m];
//         for(int i=0;i<m;i++) condition[cl][i][row[i]]++;
//     }

//     cout<<"\nEnter condition (comma separated "<<m<<" features): ";
//     cin.ignore();
//     getline(cin,line);
//     vector<string> test = split(line);

//     string pred;
//     double maxp=0;

//     cout<<"\nProbabilities:\n";
//     for(auto &c:decision_classes){
//         double p = prior[c];
//         cout<<" P("<<c<<") = "<<p<<"\n";
//         for(int i=0;i<m;i++){
//             int cnt = condition[c][i][test[i]];
//             double px = (double)cnt / class_count[c];
//             cout<<" * P("<<test[i]<<"|"<<c<<") = "<<cnt<<"/"<<class_count[c]<<" = "<<px<<"\n";
//             p *= px;
//         }
//         cout<<"P("<<c<<"|sample) = "<<p<<"\n\n";
//         if(p>maxp){ maxp=p; pred=c; }
//     }

//     cout<<"Predicted class: "<<pred<<"\n";
// }
#include <bits/stdc++.h>
using namespace std;

vector<string> split(const string &s, char d) {
    vector<string> t; string x; stringstream ss(s);
    while (getline(ss, x, d)) t.push_back(x);
    return t;
}

int main() {
    string filename;
    cout << "Enter CSV filename: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file.\n";
        return 1;
    }

    string line;
    getline(file, line);
    vector<string> headers = split(line, ',');
    int n = headers.size() - 1;

    vector<vector<string>> data;
    while (getline(file, line)) {
        vector<string> row = split(line, ',');
        if (row.size() == headers.size()) data.push_back(row);
    }
    file.close();

    map<string, int> classCount;
    for (auto &r : data) classCount[r.back()]++;
    int total = data.size();

    cout << "\nPrior Probabilities:\n";
    map<string, double> prior;
    for (auto &c : classCount) {
        prior[c.first] = (double)c.second / total;
        cout << "P(" << c.first << ") = " << classCount[c.first] << "/" << total
             << " = " << fixed << setprecision(3) << prior[c.first] << endl;
    }

    cout << "\nConditional Probabilities:\n";
    map<string, map<string, map<string, double>>> featureProb;
    for (int i = 0; i < n; i++) {
        map<string, map<string, int>> freq;
        for (auto &r : data) freq[r.back()][r[i]]++;

        for (auto &c : classCount) {
            cout << "\nFor Class = " << c.first << " (" << headers[i] << "):\n";
            for (auto &f : freq[c.first]) {
                featureProb[headers[i]][c.first][f.first] =
                    (double)f.second / classCount[c.first];
                cout << "P(" << f.first << " | " << c.first << ") = "
                     << f.second << "/" << classCount[c.first]
                     << " = " << featureProb[headers[i]][c.first][f.first] << endl;
            }
        }
    }

    cout << "\nEnter values for a test case:\n";
    map<string, string> inst;
    for (int i = 0; i < n; i++) {
        string v;
        cout << headers[i] << ": ";
        cin >> v;
        inst[headers[i]] = v;
    }

    cout << "\nCalculation of Posterior Probabilities:\n";
    map<string, double> post;

    for (auto &c : classCount) {
        double prob = prior[c.first];
        cout << "\nFor " << c.first << ":\n";
        cout << "P(" << c.first << ") = " << prior[c.first] << endl;
        for (int i = 0; i < n; i++) {
            string f = headers[i], v = inst[f];
            double cond = featureProb[f][c.first].count(v) ?
                          featureProb[f][c.first][v] : 0.0;
            cout << "P(" << v << " | " << c.first << ") = " << cond << endl;
            prob *= cond;
        }
        cout << "=> Final P(" << c.first << " | Case) = " << prob << endl;
        post[c.first] = prob;
    }

    cout << "\nComparison:\n";
    for (auto &p : post)
        cout << "P(" << p.first << " | Case) = " << p.second << endl;

    string pred = max_element(post.begin(), post.end(),
                              [](auto &a, auto &b) {
                                  return a.second < b.second;
                              })->first;

    cout << "\nBased on calculations:\nPredicted classification for case is "
         << pred << endl;

    return 0;
}
