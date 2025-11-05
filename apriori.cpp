#include <bits/stdc++.h>
using namespace std;

using Transaction = set<string>;
using Transactions = vector<Transaction>;
using Itemset = set<string>;

// Split CSV line into items
Transaction parseLine(const string &line) {
    Transaction t;
    stringstream ss(line);
    string item;
    while(getline(ss, item, ',')) {
        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
        if(!item.empty()) t.insert(item);
    }
    return t;
}

// Check if transaction contains all items in itemset
bool contains(const Transaction &t, const Itemset &itemset) {
    return includes(t.begin(), t.end(), itemset.begin(), itemset.end());
}

// Generate candidate k-itemsets from previous frequent (k-1)-itemsets
vector<Itemset> aprioriGen(const vector<Itemset> &prev) {
    vector<Itemset> candidates;
    int n = prev.size();
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            Itemset c = prev[i];
            c.insert(prev[j].begin(), prev[j].end());
            if(c.size() == prev[i].size() + 1) candidates.push_back(c);
        }
    }
    return candidates;
}

int main() {
    string filename;
    cout << "Enter CSV filename: ";
    cin >> filename;

    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Error: Cannot open file.\n";
        return 1;
    }

    Transactions transactions;
    string line;
    getline(file, line); // skip header
    while(getline(file, line)){
        if(!line.empty()){
            line.erase(remove(line.begin(), line.end(), '"'), line.end());
            auto pos = line.find(',');
            if(pos != string::npos) line = line.substr(pos+1);
            transactions.push_back(parseLine(line));
        }
    }
    file.close();

    int total = transactions.size();

    // Count 1-itemsets
    map<Itemset,int> allCounts;
    map<string,int> singleCount;
    for(auto &t: transactions)
        for(auto &item: t) singleCount[item]++;
    vector<Itemset> freqItemsets;
    for(auto &p: singleCount){
        Itemset s = {p.first};
        allCounts[s] = p.second;
        freqItemsets.push_back(s);
    }

    // Generate k-itemsets
    int k = 2;
    vector<Itemset> prev = freqItemsets;
    while(!prev.empty()){
        vector<Itemset> candidates = aprioriGen(prev);
        map<Itemset,int> candCount;
        for(auto &c: candidates){
            int cnt = 0;
            for(auto &t: transactions) if(contains(t,c)) cnt++;
            if(cnt>0) candCount[c] = cnt;
        }
        for(auto &p: candCount){
            allCounts[p.first] = p.second;
        }
        prev.clear();
        for(auto &p: candCount) prev.push_back(p.first);
        k++;
    }

    // Show all itemsets with support
    cout << "\nAll itemsets with their support (percentage):\n";
    for(auto &p: allCounts){
        double support = (double)p.second / total * 100.0;
        cout << "{ ";
        for(auto &x: p.first) cout << x << " ";
        cout << "} : " << fixed << setprecision(2) << support << "%\n";
    }

    // Ask user for min support and min confidence
    double minSupportPercent, minConfidencePercent;
    cout << "\nEnter minimum support (percentage 0-100): ";
    cin >> minSupportPercent;
    cout << "Enter minimum confidence (percentage 0-100): ";
    cin >> minConfidencePercent;

  
    map<Itemset,int> frequent;
    for(auto &p: allCounts){
        double support = (double)p.second / total * 100.0;
        if(support >= minSupportPercent) frequent[p.first] = p.second;
    }

    cout << "\nFrequent itemsets with support >= " << minSupportPercent << "%:\n";
    for(auto &p: frequent){
        double support = (double)p.second / total * 100.0;
        cout << "{ ";
        for(auto &x: p.first) cout << x << " ";
        cout << "} : " << support << "%\n";
    }

    // Generate association rules
    cout << "\nAssociation Rules with confidence >= " << minConfidencePercent << "%:\n";
    for(auto &p: frequent){
        if(p.first.size()<2) continue;
        vector<string> items(p.first.begin(), p.first.end());
        int itemsetSupport = p.second;
        int n = items.size();
        for(int mask=1; mask<(1<<n)-1; mask++){
            Itemset antecedent, consequent;
            for(int i=0;i<n;i++){
                if(mask & (1<<i)) antecedent.insert(items[i]);
                else consequent.insert(items[i]);
            }
            if(antecedent.empty() || consequent.empty()) continue;
            int antecedentSupport = frequent[antecedent];
            double conf = (double)itemsetSupport / antecedentSupport * 100.0;
            if(conf >= minConfidencePercent){
                cout << "{ ";
                for(auto &x: antecedent) cout << x << " ";
                cout << "} => { ";
                for(auto &x: consequent) cout << x << " ";
                cout << "} (Confidence: " << conf << "%, Support: "
                     << (double)itemsetSupport/total*100 << "%)\n";
            }
        }
    }

    return 0;
}
