#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

int main() {
    string filename;
    cout << "Enter CSV file path: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    vector<double> X, Y;
    string line, x_val, y_val;

    // Read CSV assuming first line has headers
    getline(file, line); // skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, x_val, ',');
        getline(ss, y_val, ',');
        X.push_back(stod(x_val));
        Y.push_back(stod(y_val));
    }
    file.close();

    int n = X.size();
    if (n == 0) {
        cerr << "No data found!" << endl;
        return 1;
    }

    // Calculate means
    double sumX = 0, sumY = 0;
    for (int i = 0; i < n; i++) {
        sumX += X[i];
        sumY += Y[i];
    }
    double meanX = sumX / n;
    double meanY = sumY / n;

    // Calculate slope (w) and intercept (b)
    double num = 0, den = 0;
    for (int i = 0; i < n; i++) {
        num += (X[i] - meanX) * (Y[i] - meanY);
        den += (X[i] - meanX) * (X[i] - meanX);
    }
    double w = num / den;
    double b = meanY - w * meanX;

    // Calculate R-squared
    double ss_tot = 0, ss_res = 0;
    for (int i = 0; i < n; i++) {
        double y_pred = b + w * X[i];
        ss_res += (Y[i] - y_pred) * (Y[i] - y_pred);
        ss_tot += (Y[i] - meanY) * (Y[i] - meanY);
    }
    double r2 = 1 - (ss_res / ss_tot);
    double adj_r2 = 1 - (1 - r2) * (n - 1) / (n - 2); // for 1 predictor

    // Output results
    cout << "\nLinear Regression Results:\n";
    cout << "Intercept (b): " << b << endl;
    cout << "Slope (w): " << w << endl;
    cout << "R-Squared: " << r2 << endl;
    cout << "Adjusted R-Squared: " << adj_r2 << endl;

    return 0;
}
