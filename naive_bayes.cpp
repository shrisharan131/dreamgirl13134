#include <bits/stdc++.h>
using namespace std;

vector<string> split(const string &s){
    vector<string> res;
    string temp;
    stringstream ss(s);
    while(getline(ss,temp,',')) res.push_back(temp);
    return res;
}

int main(){
    string filename;
    cout<<"Enter file: ";
    cin>>filename;
    ifstream file(filename);
    vector<vector<string>> data;
    string line;
    getline(file,line);
    while(getline(file,line)) if(!line.empty()) data.push_back(split(line));
    file.close();

    int n = data.size();
    int m = data[0].size()-1; 
    vector<string> decision_classes;
    map<string,int> class_count;

    for(auto &row:data) class_count[row[m]]++;
    for(auto &p:class_count) decision_classes.push_back(p.first);

    cout<<"decision_classes: ";
    for(auto &c:decision_classes) cout<<c<<" ";
    cout<<"\n";

    map<string,double> prior;
    for(auto &c:decision_classes) prior[c] = (double)class_count[c]/n;

    map<string,vector<map<string,int>>> condition;
    for(auto &c:decision_classes) condition[c] = vector<map<string,int>>(m);
    for(auto &row:data)
    {
        string cl = row[m];
        for(int i=0;i<m;i++) condition[cl][i][row[i]]++;
    }

    cout<<"\nEnter condition (comma separated "<<m<<" features): ";
    cin.ignore();
    getline(cin,line);
    vector<string> test = split(line);

    string pred;
    double maxp=0;

    cout<<"\nProbabilities:\n";
    for(auto &c:decision_classes){
        double p = prior[c];
        cout<<" P("<<c<<") = "<<p<<"\n";
        for(int i=0;i<m;i++){
            int cnt = condition[c][i][test[i]];
            double px = (double)cnt / class_count[c];
            cout<<" * P("<<test[i]<<"|"<<c<<") = "<<cnt<<"/"<<class_count[c]<<" = "<<px<<"\n";
            p *= px;
        }
        cout<<"P("<<c<<"|sample) = "<<p<<"\n\n";
        if(p>maxp){ maxp=p; pred=c; }
    }

    cout<<"Predicted class: "<<pred<<"\n";
}
