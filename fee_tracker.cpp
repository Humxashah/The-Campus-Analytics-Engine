#include "fee_tracker.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

// Parse fee line from CSV
FeeRecord parseFeeLine(const string& line) {
    FeeRecord record;
    vector<string> fields = parseCSVLine(line);
    
    if (fields.size() >= 6) {
        record.rollNumber = fields[0];
        record.semester = fields[1];
        record.amountDue = atof(fields[2].c_str());
        record.amountPaid = atof(fields[3].c_str());
        record.dueDate = fields[4];
        record.paidDate = fields[5];
    }
    
    return record;
}

// Convert FeeRecord to string
string feeToString(const FeeRecord& fee) {
    stringstream ss;
    ss << fee.rollNumber << ","
       << fee.semester << ","
       << fee.amountDue << ","
       << fee.amountPaid << ","
       << fee.dueDate << ","
       << fee.paidDate;
    return ss.str();
}

// Validate date format DD-MM-YYYY with string checks
bool validateDate(const string& date) {
    // Check length
    if (date.length() != 10) {
        return false;
    }
    
    // Check format DD-MM-YYYY
    if (date[2] != '-' || date[5] != '-') {
        return false;
    }
    
    // Check if all characters are digits or hyphens
    for (size_t i = 0; i < date.length(); i++) {
        if (i == 2 || i == 5) {
            if (date[i] != '-') return false;
        } else {
            if (!isdigit(date[i])) return false;
        }
    }
    
    // Extract parts
    int day = atoi(date.substr(0, 2).c_str());
    int month = atoi(date.substr(3, 2).c_str());
    int year = atoi(date.substr(6, 4).c_str());
    
    // Basic validation
    if (year < 2000 || year > 2099) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    // Month-specific day validation
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    } else if (month == 2) {
        // Leap year check
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) return false;
    }
    
    return true;
}

// Parse DD-MM-YYYY to total day count (manual month-length array)
int daysBetween(const string& date1, const string& date2) {
    // Manual month length array
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Parse date1
    int day1 = atoi(date1.substr(0, 2).c_str());
    int month1 = atoi(date1.substr(3, 2).c_str());
    int year1 = atoi(date1.substr(6, 4).c_str());
    
    // Parse date2
    int day2 = atoi(date2.substr(0, 2).c_str());
    int month2 = atoi(date2.substr(3, 2).c_str());
    int year2 = atoi(date2.substr(6, 4).c_str());
    
    // Check for N/A
    if (date1 == "N/A" || date2 == "N/A") {
        return 0;
    }
    
    // Calculate total days for date1
    int totalDays1 = 0;
    for (int y = 2000; y < year1; y++) {
        bool isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        totalDays1 += isLeap ? 366 : 365;
    }
    for (int m = 1; m < month1; m++) {
        if (m == 2) {
            bool isLeap = (year1 % 4 == 0 && year1 % 100 != 0) || (year1 % 400 == 0);
            totalDays1 += isLeap ? 29 : 28;
        } else {
            totalDays1 += daysInMonth[m - 1];
        }
    }
    totalDays1 += day1;
    
    // Calculate total days for date2
    int totalDays2 = 0;
    for (int y = 2000; y < year2; y++) {
        bool isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        totalDays2 += isLeap ? 366 : 365;
    }
    for (int m = 1; m < month2; m++) {
        if (m == 2) {
            bool isLeap = (year2 % 4 == 0 && year2 % 100 != 0) || (year2 % 400 == 0);
            totalDays2 += isLeap ? 29 : 28;
        } else {
            totalDays2 += daysInMonth[m - 1];
        }
    }
    totalDays2 += day2;
    
    return abs(totalDays2 - totalDays1);
}

// Compute late fine: 2% per complete week
double computeLateFine(const string& dueDate, const string& paidDate, double amountDue) {
    if (paidDate == "N/A" || dueDate == "N/A") {
        return 0.0;
    }
    
    int days = daysBetween(dueDate, paidDate);
    
    // No fine if paid on or before due date
    if (days <= 0) {
        return 0.0;
    }
    
    // Calculate complete weeks
    int completeWeeks = days / 7;
    
    // 2% per complete week
    double finePercentage = completeWeeks * 0.02;
    
    // Cap at 50% maximum
    if (finePercentage > 0.50) {
        finePercentage = 0.50;
    }
    
    return amountDue * finePercentage;
}

// Get outstanding balance for a student
double getOutstandingBalance(const string& roll, const string& semester) {
    vector<vector<string> > data = readTXT("fees.txt");
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 6) {
            if (data[i][0] == roll && data[i][1] == semester) {
                double amountDue = atof(data[i][2].c_str());
                double amountPaid = atof(data[i][3].c_str());
                return amountDue - amountPaid;
            }
        }
    }
    
    return 0.0;
}

// Bubble sort for defaulters by outstanding amount
void bubbleSortDefaulters(vector<FeeRecord>& defaulters) {
    int n = defaulters.size();
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            double balance1 = defaulters[j].amountDue - defaulters[j].amountPaid;
            double balance2 = defaulters[j + 1].amountDue - defaulters[j + 1].amountPaid;
            
            if (balance1 < balance2) {
                FeeRecord temp = defaulters[j];
                defaulters[j] = defaulters[j + 1];
                defaulters[j + 1] = temp;
            }
        }
    }
}

// Generate receipt - formatted receipt with breakdown
void generateReceipt(const string& roll, const string& semester) {
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    vector<vector<string> > data = readTXT("fees.txt");
    FeeRecord feeRecord;
    bool found = false;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 6) {
            if (data[i][0] == roll && data[i][1] == semester) {
                feeRecord.rollNumber = data[i][0];
                feeRecord.semester = data[i][1];
                feeRecord.amountDue = atof(data[i][2].c_str());
                feeRecord.amountPaid = atof(data[i][3].c_str());
                feeRecord.dueDate = data[i][4];
                feeRecord.paidDate = data[i][5];
                found = true;
                break;
            }
        }
    }
    
    if (!found) {
        cout << "No fee record found for this student!" << endl;
        return;
    }
    
    double outstanding = feeRecord.amountDue - feeRecord.amountPaid;
    double lateFine = 0.0;
    
    if (feeRecord.paidDate != "N/A") {
        lateFine = computeLateFine(feeRecord.dueDate, feeRecord.paidDate, feeRecord.amountDue);
    }
    
    // Generate receipt using setw from iomanip
    cout << "\n" << string(60, '=') << endl;
    cout << setw(40) << "FEE PAYMENT RECEIPT" << endl;
    cout << string(60, '=') << endl;
    cout << left << setw(20) << "Student Name:" << student.name << endl;
    cout << left << setw(20) << "Roll Number:" << roll << endl;
    cout << left << setw(20) << "Semester:" << semester << endl;
    cout << left << setw(20) << "Department:" << student.department << endl;
    cout << string(60, '-') << endl;
    
    cout << left << setw(30) << "Tuition Fee:" 
         << right << setw(10) << fixed << setprecision(2) << feeRecord.amountDue << endl;
    
    if (lateFine > 0) {
        cout << left << setw(30) << "Late Fine (2% per week):" 
             << right << setw(10) << fixed << setprecision(2) << lateFine << endl;
    }
    
    cout << left << setw(30) << "Total Amount Due:" 
         << right << setw(10) << fixed << setprecision(2) << (feeRecord.amountDue + lateFine) << endl;
    cout << left << setw(30) << "Amount Paid:" 
         << right << setw(10) << fixed << setprecision(2) << feeRecord.amountPaid << endl;
    cout << string(60, '-') << endl;
    cout << left << setw(30) << "Outstanding Balance:" 
         << right << setw(10) << fixed << setprecision(2) << outstanding << endl;
    
    cout << left << setw(20) << "Due Date:" << feeRecord.dueDate << endl;
    if (feeRecord.paidDate != "N/A") {
        cout << left << setw(20) << "Payment Date:" << feeRecord.paidDate << endl;
    }
    
    cout << string(60, '=') << endl;
    cout << setw(45) << "Thank you for your payment!" << endl;
    cout << string(60, '=') << endl;
}

// Record payment - validates date, updates fees.txt
void recordPayment() {
    cout << "\n--- RECORD FEE PAYMENT ---" << endl;
    
    string roll;
    cout << "Enter Student Roll Number: ";
    cin >> roll;
    
    // Check if student exists
    Student student = findStudentByRoll(roll);
    if (student.rollNumber == "NULL") {
        cout << "Error: Student not found!" << endl;
        return;
    }
    
    string semester;
    cout << "Enter Semester: ";
    cin >> semester;
    
    // Check if fee record exists
    vector<vector<string> > data = readTXT("fees.txt");
    bool found = false;
    int index = -1;
    FeeRecord feeRecord;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 6) {
            if (data[i][0] == roll && data[i][1] == semester) {
                found = true;
                index = i;
                feeRecord.rollNumber = data[i][0];
                feeRecord.semester = data[i][1];
                feeRecord.amountDue = atof(data[i][2].c_str());
                feeRecord.amountPaid = atof(data[i][3].c_str());
                feeRecord.dueDate = data[i][4];
                feeRecord.paidDate = data[i][5];
                break;
            }
        }
    }
    
    if (!found) {
        cout << "No fee record found for this student and semester!" << endl;
        return;
    }
    
    string paidDate;
    do {
        cout << "Enter Payment Date (DD-MM-YYYY): ";
        cin >> paidDate;
        
        if (!validateDate(paidDate)) {
            cout << "Error: Invalid date format! Use DD-MM-YYYY" << endl;
            continue;
        }
        
        break;
    } while (true);
    
    double amount;
    do {
        cout << "Enter Amount Paid: ";
        cin >> amount;
        
        if (cin.fail() || amount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Please enter a valid positive amount!" << endl;
        } else {
            break;
        }
    } while (true);
    
    // Update paid amount
    feeRecord.amountPaid += amount;
    feeRecord.paidDate = paidDate;
    
    // Update the row
    vector<string> updatedRow;
    updatedRow.push_back(feeRecord.rollNumber);
    updatedRow.push_back(feeRecord.semester);
    
    stringstream due, paid;
    due << feeRecord.amountDue;
    paid << feeRecord.amountPaid;
    
    updatedRow.push_back(due.str());
    updatedRow.push_back(paid.str());
    updatedRow.push_back(feeRecord.dueDate);
    updatedRow.push_back(feeRecord.paidDate);
    
    data[index] = updatedRow;
    
    // Get header
    vector<string> header;
    header.push_back("roll");
    header.push_back("semester");
    header.push_back("amount_due");
    header.push_back("amount_paid");
    header.push_back("due_date");
    header.push_back("paid_date");
    
    if (writeTXT("fees.txt", header, data)) {
        cout << "Payment recorded successfully!" << endl;
        
        // Calculate late fine
        double lateFine = computeLateFine(feeRecord.dueDate, feeRecord.paidDate, feeRecord.amountDue);
        if (lateFine > 0) {
            cout << "Late Fine: " << fixed << setprecision(2) << lateFine << endl;
        }
        
        generateReceipt(roll, semester);
    } else {
        cout << "Error: Failed to record payment!" << endl;
    }
}

// Get defaulters - returns students with balance > 0 past due date
void getDefaulters() {
    cout << "\n--- FEE DEFAULTERS LIST ---" << endl;
    
    vector<vector<string> > data = readTXT("fees.txt");
    vector<FeeRecord> defaulters;
    
    // Current date for comparison (hardcoded for demonstration)
    string currentDate = "30-01-2024";
    
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].size() >= 6) {
            FeeRecord record;
            record.rollNumber = data[i][0];
            record.semester = data[i][1];
            record.amountDue = atof(data[i][2].c_str());
            record.amountPaid = atof(data[i][3].c_str());
            record.dueDate = data[i][4];
            record.paidDate = data[i][5];
            
            double outstanding = record.amountDue - record.amountPaid;
            
            // Check if balance > 0 and past due date
            if (outstanding > 0) {
                // Check if due date has passed
                int daysDiff = daysBetween(record.dueDate, currentDate);
                if (daysDiff > 0) {
                    defaulters.push_back(record);
                }
            }
        }
    }
    
    if (defaulters.empty()) {
        cout << "No fee defaulters found!" << endl;
        return;
    }
    
    // Sort by outstanding amount using bubble sort
    bubbleSortDefaulters(defaulters);
    
    cout << "\n--- STUDENTS WITH OUTSTANDING FEES ---" << endl;
    cout << left << setw(15) << "Roll Number" 
         << setw(30) << "Name" 
         << setw(15) << "Semester" 
         << setw(15) << "Outstanding" << endl;
    cout << string(75, '-') << endl;
    
    for (size_t i = 0; i < defaulters.size(); i++) {
        Student student = findStudentByRoll(defaulters[i].rollNumber);
        if (student.rollNumber != "NULL") {
            double outstanding = defaulters[i].amountDue - defaulters[i].amountPaid;
            cout << left << setw(15) << student.rollNumber
                 << setw(30) << student.name
                 << setw(15) << defaulters[i].semester
                 << setw(15) << fixed << setprecision(2) << outstanding << endl;
        }
    }
    
    cout << "\nTotal Defaulters: " << defaulters.size() << endl;
}