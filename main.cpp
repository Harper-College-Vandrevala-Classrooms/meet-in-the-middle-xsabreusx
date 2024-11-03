#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

class FileReader {
public:
    bool loadFile(const string& fileName) {
        ifstream file(fileName);
        if (!file) {
            cerr << "Error opening file." << endl;
            return false;
        }
        
        values.clear();  // Clear previous values before loading new data
        string line;
        int errorSpot = 0;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {  // Split on commas for CSV support
                try {
                    errorSpot++;
                    values.push_back(stod(value));  // Convert to double
                } catch (const invalid_argument&) {
                    cerr << "Invalid data encountered at position: " << errorSpot << " Skipping." << endl;
                }
            }
        }

        file.close();
        return true;
    }

    double findMiddleValue() const {
        if (values.empty()) {
            cerr << "No data loaded." << endl;
            return 0;
        }

        vector<double> sortedValues = values;
        sort(sortedValues.begin(), sortedValues.end());

        size_t size = sortedValues.size();
        if (size % 2 == 0) {
            return (sortedValues[size / 2 - 1] + sortedValues[size / 2]) / 2.0;
        } else {
            return sortedValues[size / 2];
        }
    }

    string stripQuotes(string filePath) {   // Fixes a very annoying issue with quotes when copying file paths
        if (filePath.size() >= 2 && filePath.front() == '"' && filePath.back() == '"') {
            return filePath.substr(1, filePath.size() - 2);
        }
        return filePath;
    }

    void displayResults() const {
        double middleValue = findMiddleValue();
        cout << "Middle Value: " << middleValue << endl;
    }
// This is the next one is method that is called to start the program. It uses the other methods within the class to do the work.
    void medianExtractor() {  
        string fileName;
        cout << "Enter the path of the file to load: ";
        getline(cin, fileName);

        fileName = stripQuotes(fileName);

        if (!loadFile(fileName)) {
            cerr << "Exiting program due to file error." << endl;
            return;  // Exit the function if file loading failed
        }

        displayResults();        
    } 

private:
    vector<double> values;  
};

/////////////// Main is down here //////////////

int main() {

    /////// --- Test area --- ////////


    //////////////////////////////////

    FileReader fileReader;
    
    fileReader.medianExtractor();
}