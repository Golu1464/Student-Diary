#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <limits>
#include <sstream>

using namespace std;

class Student {
public:
    string name;
    vector<int> grades;

    void setGrades(const vector<int>& newGrades) {
        grades = newGrades;
    }

    void displayInfo() const {
        cout << "Name: " << name << "\nGrades: ";
        for (int grade : grades) {
            cout << grade << " ";
        }
        cout << endl;
    }
};

class GradeTracker {
private:
    vector<Student> students;

public:
    void addStudent(const Student& newStudent) {
        students.push_back(newStudent);
    }

    void displayAllStudents() const {
        for (const auto& student : students) {
            student.displayInfo();
        }
    }

    double calculateAverage() const {
        double total = 0;
        int count = 0;
        for (const auto& student : students) {
            for (int grade : student.grades) {
                total += grade;
                count++;
            }
        }
        return count == 0 ? 0 : total / count;
    }

    void findTopScorer() const {
        if (students.empty()) {
            cout << "No students to find a top scorer." << endl;
            return;
        }

        auto compareTotalGrades = [](const Student& a, const Student& b) {
            int totalA = accumulate(a.grades.begin(), a.grades.end(), 0);
            int totalB = accumulate(b.grades.begin(), b.grades.end(), 0);
            return totalA > totalB;
        };

        auto topScorer = max_element(students.begin(), students.end(), compareTotalGrades);
        cout << "Top Scorer: ";
        topScorer->displayInfo();
    }

    void displayPassingStudents() const {
        cout << "Passing Students:\n";
        for (const auto& student : students) {
            int total = accumulate(student.grades.begin(), student.grades.end(), 0);
            double average = static_cast<double>(total) / student.grades.size();
            if (average >= 50) {
                student.displayInfo();
            }
        }
    }

    void updateStudentInfo(const string& studentName, const vector<int>& newGrades) {
        auto it = find_if(students.begin(), students.end(),
                           [&studentName](const Student& student) { return student.name == studentName; });

        if (it != students.end()) {
            it->setGrades(newGrades);
            cout << "Student information updated successfully." << endl;
        } else {
            cout << "Student not found." << endl;
        }
    }

    void deleteStudent(const string& studentName) {
        auto it = remove_if(students.begin(), students.end(),
                             [&studentName](const Student& student) { return student.name == studentName; });

        if (it != students.end()) {
            students.erase(it, students.end());
            cout << "Student deleted successfully." << endl;
        } else {
            cout << "Student not found." << endl;
        }
    }

    void saveToFile(const string& fileName) const {
        ofstream outFile(fileName);
        for (const auto& student : students) {
            outFile << student.name;
            for (int grade : student.grades) {
                outFile << "," << grade;
            }
            outFile << "\n";
        }
        outFile.close();
        cout << "Student details saved to " << fileName << endl;
    }

    void loadFromFile(const string& fileName) {
        ifstream inFile(fileName);
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            Student student;
            getline(ss, student.name, ',');
            int grade;
            while (ss >> grade) {
                student.grades.push_back(grade);
                if (ss.peek() == ',') ss.ignore();
            }
            students.push_back(student);
        }
        inFile.close();
    }

    void searchStudent(const string& studentName) const {
        auto it = find_if(students.begin(), students.end(),
                           [&studentName](const Student& student) { return student.name == studentName; });

        if (it != students.end()) {
            cout << "Student found:\n";
            it->displayInfo();
        } else {
            cout << "Student not found." << endl;
        }
    }

    void viewSubjectPerformance(int subjectIndex) const {
        if (subjectIndex < 0 || subjectIndex >= students.front().grades.size()) {
            cout << "Invalid subject index." << endl;
            return;
        }

        cout << "Subject-wise Performance (Subject " << subjectIndex + 1 << "):\n";
        for (const auto& student : students) {
            cout << student.name << ": " << student.grades[subjectIndex] << "\n";
        }
    }
};

void displayMenu() {
    cout << "======= Menu =======\n"
         << "1. Add a new student\n"
         << "2. Display all students\n"
         << "3. Calculate average grades\n"
         << "4. Find the top scorer\n"
         << "5. Display passing students\n"
         << "6. Update a student's information\n"
         << "7. Delete a student\n"
         << "8. Save student details to a file\n"
         << "9. Load student details from a file\n"
         << "10. Search for a student\n"
         << "11. View subject-wise performance\n"
         << "0. Exit\n"
         << "====================\n";
}

int main() {
    GradeTracker gradeTracker;
    int choice;

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Student newStudent;
                cout << "Enter student name: ";
                cin >> newStudent.name;

                int numGrades;
                cout << "Enter the number of grades: ";
                cin >> numGrades;

                newStudent.grades.resize(numGrades);
                cout << "Enter grades for the student: ";
                for (int i = 0; i < numGrades; ++i) {
                    while (true) {
                        string grade;
                        cin >> grade;
                        try {
                            newStudent.grades[i] = stoi(grade);
                            break;
                        } catch (const invalid_argument& e) {
                            cout << "Invalid grade. Please enter a valid integer.\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }
                }

                gradeTracker.addStudent(newStudent);
                cout << "Student added successfully.\n";
                break;
            }
            case 2:
                gradeTracker.displayAllStudents();
                break;
            case 3:
                cout << "Average grades: " << gradeTracker.calculateAverage() << "\n";
                break;
            case 4:
                gradeTracker.findTopScorer();
                break;
            case 5:
                gradeTracker.displayPassingStudents();
                break;
            case 6: {
                string studentName;
                cout << "Enter student name to update: ";
                cin >> studentName;
                vector<int> newGrades;
                int numGrades;
                cout << "Enter the number of grades: ";
                cin >> numGrades;
                newGrades.resize(numGrades);
                cout << "Enter new grades for the student: ";
                for (int i = 0; i < numGrades; ++i) {
                    cin >> newGrades[i];
                }
                gradeTracker.updateStudentInfo(studentName, newGrades);
                break;
            }
            case 7: {
                string studentName;
                cout << "Enter student name to delete: ";
                cin >> studentName;
                gradeTracker.deleteStudent(studentName);
                break;
            }
            case 8: {
                string fileName;
                cout << "Enter file name to save: ";
                cin >> fileName;
                gradeTracker.saveToFile(fileName);
                break;
            }
            case 9: {
                string fileName;
                cout << "Enter file name to load: ";
                cin >> fileName;
                gradeTracker.loadFromFile(fileName);
                break;
            }
            case 10: {
                string studentName;
                cout << "Enter student name to search: ";
                cin >> studentName;
                gradeTracker.searchStudent(studentName);
                break;
            }
            case 11: {
                int subjectIndex;
                cout << "Enter subject index to view performance: ";
                cin >> subjectIndex;
                gradeTracker.viewSubjectPerformance(subjectIndex - 1);
                break;
            }
            case 0:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
