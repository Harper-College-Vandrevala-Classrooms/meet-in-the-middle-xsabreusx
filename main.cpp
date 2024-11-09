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

        // Check for and skip BOM. Fixes a very annoying issue with certain types of csv files.
        char firstChar = file.peek();
        if (firstChar == '\xEF') { // BOM's first byte
            char bom[3];
            file.read(bom, 3); // Read and discard the BOM
        }
        
        while (getline(file, line)) {
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {  // Split on commas for CSV support
                errorSpot++;

                value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
                
                //cout << "Processing value: '" << value << "'" << endl;  // Debug output

                if (isNumeric(value)) {
                    values.push_back(stod(value));  // Convert to double
                } else {
                    cerr << "Invalid data encountered at position: " << errorSpot << " Skipping." << endl;
                }
            }
        }

        file.close();
        return true;
    }

    double findMiddleValue() const {
        if (values.empty()) {
            cerr << "\nNo data loaded." << endl;
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
        cout << "\nMedian Value: " << middleValue << endl;
        cout << "\nProgram ran successfully. Goodbye.";
    }

    void medianExtractor() {  
        string fileName;
        cout << "\nEnter the path of the file to load: ";
        getline(cin, fileName);

        fileName = stripQuotes(fileName);

        if (!loadFile(fileName)) {
            cerr << "\nExiting program due to file error." << endl;
            return;  // Exit the function if file loading failed
        }

        displayResults();        
    } 

private:
    vector<double> values;  

    bool isNumeric(const string& str) const {
        if (str.empty()) return false;

        bool decimalPoint = false;
        size_t start = 0;

        // Allow optional leading minus for negative numbers
        if (str[start] == '-') {
            start++;
        }

        for (size_t i = start; i < str.size(); ++i) {
            if (str[i] == '.') {
                if (decimalPoint) return false;  // More than one decimal point
                decimalPoint = true;
            } else if (!isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }
};

/////////////// Main is down here //////////////

int main() {

    /////// --- Test area --- ////////

    //To test the functions, I included a test .csv file
    //This is a small file which can be adjusted, making it easy to predict the expected output.

    FileReader fileReader;
    
    fileReader.medianExtractor();

    //End of testing


}