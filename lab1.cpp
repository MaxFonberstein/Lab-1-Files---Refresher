#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

// Constants
const int MAX_RECORDS = 100; // Maximum number of valid records
const int MAX_ID_LENGTH = 6;  // Length of Car ID

// Struct to store car records
struct CarRecord {
    string carID;
    string model;
    string manufacturer;
    int quantity;
    double price;
};

// Enum for menu options
enum MenuOptions { PRINT_ALL = 1, PRINT_INVALID, QUIT };

// Function prototypes
void getData(CarRecord records[], int &count, const string &errorFileName);
void printAllRecords(const CarRecord records[], int count);
void printInvalidRecords(const string &errorFileName);
bool isValidRecord(const CarRecord &record, string &errorMessage);
void displayMenu();
void clearInputBuffer();

int main() {
    CarRecord records[MAX_RECORDS]; // Array to store valid records
    int count = 0;                  // Number of valid records
    const string errorFileName = "invalid_records.txt"; // Error file name

    getData(records, count, errorFileName); // Read data from file

    int choice = 0;
    while (true) {
        displayMenu(); // Display menu options
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer(); // Clear input buffer on failure
            choice = 0; // Reset choice to trigger invalid case
        }

        switch (choice) {
            case PRINT_ALL:
                printAllRecords(records, count); // Print valid records
                break;
            case PRINT_INVALID:
                printInvalidRecords(errorFileName); // Print invalid records
                break;
            case QUIT:
                cout << "Exiting the program.\n";
                return 0; // Exit program
            default:
                cout << "Invalid choice. Please try again.\n";
                clearInputBuffer(); // Clear input buffer
        }
    }
}

// Function to get data from file and validate records
void getData(CarRecord records[], int &count, const string &errorFileName) {
    ifstream infile("car_records.txt"); // Input file
    ofstream errorfile(errorFileName);   // Error file

    if (!infile) {
        cout << "Error opening input file.\n";
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(infile, line) && count < MAX_RECORDS) {
        stringstream ss(line);
        CarRecord temp;
        ss >> temp.carID >> temp.model >> temp.manufacturer >> temp.quantity >> temp.price;

        string errorMessage;
        if (isValidRecord(temp, errorMessage)) {
            records[count++] = temp; // Store valid record
        } else {
            errorfile << line << "   " << errorMessage << "\n"; // Log invalid record
        }
    }

    infile.close();
    errorfile.close();
}

// Function to validate a car record
bool isValidRecord(const CarRecord &record, string &errorMessage) {
    bool valid = true;
    errorMessage.clear();

    // Validate Car ID
    if (record.carID.length() != MAX_ID_LENGTH) {
        valid = false;
        errorMessage += "Invalid ID length; ";
    }
    if (!isalpha(record.carID[0]) || record.carID[0] == 'O' || !isalpha(record.carID[1]) || record.carID[1] == 'O') {
        valid = false;
        errorMessage += "ID must start with two alphabetic characters (no 'O'); ";
    }
    if (!isdigit(record.carID[2]) || !isdigit(record.carID[3])) {
        valid = false;
        errorMessage += "ID must contain two digits at positions 3 and 4; ";
    }
    if (!isalnum(record.carID[4]) || record.carID[4] == 'O' || !isalnum(record.carID[5]) || record.carID[5] == 'O') {
        valid = false;
        errorMessage += "ID must contain two alphanumeric characters (no 'O') at positions 5 and 6; ";
    }

    // Validate Model
    if (record.model.length() < 4 || !isalpha(record.model[0])) {
        valid = false;
        errorMessage += "Model must be at least 4 characters long and start with an alphabetic character; ";
    }

    // Validate Quantity
    if (record.quantity < 0) {
        valid = false;
        errorMessage += "Quantity cannot be negative; ";
    }

    // Validate Price
    if (record.price <= 11400.00) {
        valid = false;
        errorMessage += "Price must be above $11,400.00; ";
    }

    return valid;
}

// Function to print all valid records
void printAllRecords(const CarRecord records[], int count) {
    cout << "\nCar ID   Model          Manufacturer   Quantity   Price\n";
    for (int i = 0; i < count; i++) {
        cout << setw(10) << records[i].carID
             << setw(15) << records[i].model
             << setw(15) << records[i].manufacturer
             << setw(15) << records[i].quantity
             << setw(10) << fixed << setprecision(2) << records[i].price << "\n";
    }
}

// Function to print invalid records from the error file
void printInvalidRecords(const string &errorFileName) {
    ifstream errorfile(errorFileName);
    if (!errorfile) {
        cout << "Error opening error file.\n";
        return;
    }

    cout << "Invalid Records:\n";
    string line;
    while (getline(errorfile, line)) {
        cout << line << "\n"; // Print invalid record
    }

    errorfile.close();
}

// Function to display menu options
void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Print all valid car records\n";
    cout << "2. Print invalid car records\n";
    cout << "3. Quit\n";
}

// Function to clear the input buffer
void clearInputBuffer() {
    cin.clear(); // Clear error state
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
}
/*
Test Runs:
Car ID   Model          Manufacturer   Quantity   Price
MP127KZ  RX5           Ford               10      12000.00
MP12RKZ  RX6           Toyota             20      15000.00

Invalid Records:
35KMOP32   RX3  Lexus 5 3000.00   Invalid ID length; price must be above $11,400.00; 
123XYZ     KD8  Subaru 0 11000.00   ID must start with two alphabetic characters (no 'O'); price must be above $11,400.00; 
XX123Y     Accord  Honda 3 12000.00   ID must be 6 characters; 
A12345B    Mustang  Ford 15 15000.00   ID must be 6 characters; 
OO123A     Tucson  Hyundai 10 11500.00   ID must start with two alphabetic characters (no 'O'); 
ZK1234B    Civic  Honda 12 8000.00   Price must be above $11,400.00; 

*/
