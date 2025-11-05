#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Function to compute Pearson correlation
double pearsonCorrelation(const vector<double>& X, const vector<double>& Y) {
    int n = X.size();
    double meanX = 0, meanY = 0;
    for (int i = 0; i < n; i++) {
        meanX += X[i];
        meanY += Y[i];
    }
    meanX /= n;
    meanY /= n;

    double sumXY = 0, sumX2 = 0, sumY2 = 0;
    for (int i = 0; i < n; i++) {
        sumXY += (X[i] - meanX) * (Y[i] - meanY);
        sumX2 += (X[i] - meanX) * (X[i] - meanX);
        sumY2 += (Y[i] - meanY) * (Y[i] - meanY);
    }

    return sumXY / sqrt(sumX2 * sumY2);
}

int main() {
    string filename;
    cout << "Enter CSV file name (with .csv): ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string line;
    vector<string> headers;
    getline(file, line); // Read header
    stringstream ss(line);
    string col;
    while (getline(ss, col, ',')) headers.push_back(col);

    // Display column options
    cout << "Columns found: ";
    for (int i = 0; i < headers.size(); i++)
        cout << i << ":" << headers[i] << "  ";
    cout << "\nEnter index of first column: ";
    int col1, col2;
    cin >> col1;
    cout << "Enter index of second column: ";
    cin >> col2;

    vector<double> X, Y;
    while (getline(file, line)) {
        stringstream ss(line);
        string val;
        vector<double> row;
        while (getline(ss, val, ',')) row.push_back(stod(val));
        if (row.size() > max(col1, col2)) {
            X.push_back(row[col1]);
            Y.push_back(row[col2]);
        }
    }
    file.close();

    if (X.empty() || Y.empty()) {
        cerr << "No data found for selected columns!" << endl;
        return 1;
    }

    double r = pearsonCorrelation(X, Y);
    cout << "Pearson correlation coefficient (r) = " << r << endl;

    if (fabs(r) < 1e-6) 
        cout << "Correlation is approximately zero (no linear relationship)." << endl;
    else if (r > 0)
        cout << "Positive correlation." << endl;
    else
        cout << "Negative correlation." << endl;

    return 0;
}
