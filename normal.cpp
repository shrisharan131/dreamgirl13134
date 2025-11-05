#include <iostream>
#include <vector>
using namespace std;

double absValue(double x) {
    return (x < 0) ? -x : x;
}

double powFunction(double base, int exp) {
    double result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

double squareRootFunction(double x) {
    if (x == 0) return 0;
    double guess = x;
    for (int i = 0; i < 20; i++) { 
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}

void minMaxNormalization(vector<double> arr, double new_min, double new_max) {
    double minA = arr[0], maxA = arr[0];

    
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < minA) minA = arr[i];
        if (arr[i] > maxA) maxA = arr[i];
    }

    cout << "\nMin-Max Normalization (Range [" << new_min << ", " << new_max << "]):\n";
    for (int i = 0; i < arr.size(); i++) {

        double v_normalized = ((arr[i] - minA) / (maxA - minA)) * (new_max - new_min) + new_min;

        cout << v_normalized << " ";
    }
    cout << endl;
}


void z_score_Normalization(vector<double> arr) {
    double sum = 0, mean,  std_deviation= 0;
    int n = arr.size();

    for (int i = 0; i < n; i++)
      sum += arr[i];
    mean = sum / n;

    for (int i = 0; i < n; i++) {
        double difference = arr[i] - mean;
       std_deviation +=  difference * difference ;//variance
    }

    std_deviation = squareRootFunction(std_deviation/ n); 

    cout << "\nZ-Score Normalization:\n";
    for (int i = 0; i < n; i++) {

        double v_normalized = (arr[i] - mean) /  std_deviation;

        cout <<  v_normalized << " ";
    }
    cout << endl;
}


void decimalScalingNormalization(vector<double> arr) {
    double maxVal = 0;

    
    for (int i = 0; i < arr.size(); i++) {
        if (absValue(arr[i]) > maxVal) maxVal = absValue(arr[i]);
    }

    int j = 0;
    while (maxVal >= 1) {
        maxVal /= 10;
        j++;
    }

    cout << "\nDecimal Scaling Normalization:\n";
    for (int i = 0; i < arr.size(); i++) {
        double v_norm = arr[i] / powFunction(10, j); 
        cout << v_norm << " ";
    }
    cout << endl;
}

int main() {
    int n, choice;
    cout << "Enter size of array: ";
    cin >> n;

    vector<double> v(n);
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) 
    cin >> v[i];

    cout << "\nChoose Normalization Method:\n";
    cout << "1) Min-Max Normalization\n";
    cout << "2) Z-Score Normalization\n";
    cout << "3) Decimal Scaling Normalization\n";
    cout << "Enter Choice: ";
    cin >> choice;

    switch (choice) {
        case 1: {
            double new_min, new_max;
            cout << "Enter new_min and new_max: ";
            cin >> new_min >> new_max;
            minMaxNormalization(v, new_min, new_max);
            break;
        }
        case 2:
            z_score_Normalization(v);
            break;
        case 3:
            decimalScalingNormalization(v);
            break;
        default:
            cout << "Your choice is Invalid." << endl;
    }

    return 0;
}
