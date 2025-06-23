#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct EmployeeData //STRUCT DATA
{
	string employeeName;
	int employeeID;
	int PCNumber;
	vector<string> absences;
	vector<double> hours;
};
//CLASS DATA
class MenuScreens // data class to handle both menu screens
{
public:
	void mainMenuScreen()//Main menu function screen to user
	{
		cout << "********************************************" << endl;
		cout << "\t Main Menu" << endl;
		cout << "1 - Add Employee Absence" << endl;
		cout << "2 - Create New Employee Entry" << endl;
		cout << "3 - View/Search for Employee Information" << endl;
		cout << "4 - Delete an Employee from database" << endl;
		cout << "0 - Quit Program" << endl;
		cout << "********************************************" << endl;
	}
	void absenceType()// Menu to display the absences types 
	{
		cout << "********************************************" << endl;
		cout << "1 - Local Leave" << endl;
		cout << "2 - State Leave" << endl;
		cout << "3 - Unpaid Leave" << endl;
		cout << "4 - Workers Comp." << endl;
		cout << "5 - Off Campus Duty" << endl;
		cout << "6 - Jury Duty" << endl;
		cout << "7 - Funeral Leave" << endl;
		cout << "8 - ASLT" << endl;
		cout << "9 - COMP Time" << endl;
		cout << "10 - Other" << endl;
		cout << "********************************************" << endl;
	}
};

class fileOpperations //Class to handle file opperations
{
public:
	string openFileTest(fstream& fout, string fileName) // try ... catch for exeption handling to handle opening the file
	{
		try 
		{
			fout.open(fileName);
			if (!fout.is_open())
				throw runtime_error("Failed to open " + fileName);
			return "Successful!";
		}
		catch (const exception& e) 
		{
			return "Error opening file: " + string(e.what());
		}
	}
	void writeToFile(vector<EmployeeData>& employee) // Function used in choice 2 in main() used to input new employees into the file
	{
		ofstream outfile("employeeData.txt", ios::app);
		if (outfile.is_open())
		{
			for (const auto& emp : employee)
			{
				outfile << emp.employeeID << " " << emp.employeeName << " " << emp.PCNumber << endl;
			}
			employee.clear();
			outfile.close();
		}
		else
		{
			cout << "Error opening the file!";
		}
	}
	bool searchEmployeeByID(const vector<EmployeeData>& employee, int id) // Function used to search for an employee
	{
		fstream infile("employeeData.txt", ios::in);
		string line;
		bool found = false;

		while (getline(infile, line))
		{
			stringstream ss(line);
			int empID;
			ss >> empID;

			if (empID == id)
			{
				cout << line << endl;
				return true;
				break;
			}
		}
		infile.close();

		if (!found)
		{
			cout << "Employee with ID " << id << " wasn't found." << endl;
		}
		return found;
	}
	bool updateEmployeeRecord(int id, const string& newAbsence, double hours)
	{
		vector<string> fileCopy; // To store all file data temporarily
		fstream file("employeeData.txt", ios::in); // Open file for reading
		string line;
		bool found = false;
		while (getline(file, line)) // Read the file and modify the matching line
		{
			stringstream ss(line);
			int empID;
			ss >> empID;
			if (empID == id)
			{
				found = true; // Mark that we've found the employee
				stringstream formattedLine;
				formattedLine << fixed << setprecision(2);  // Set precision
				formattedLine << " " << newAbsence << " " << hours;  // Build the formatted string
				line += formattedLine.str();
			}
			fileCopy.push_back(line); // Add the modified line to the vector
		}
		file.close(); // Close the file after reading

		if (!found)
		{
			cout << "Employee ID " << id << " wasn't found." << endl;
			return false;
		}
		file.open("employeeData.txt", ios::out | ios::trunc); // Reopen the file for writing (truncating existing content)
		if (!file.is_open())
		{
			cout << "Error opening file for editing." << endl;
			return false;
		}
		for (const auto& updatedLine : fileCopy) // Write all the lines back to the file
		{
			file << updatedLine << "\n";
		}
		file.close(); // Close the file after writing
		return true;
	}
	void deleteEmployeeEntry(vector<EmployeeData>& employees, int id)
	{
		vector<string> lines;
		ifstream infile("employeeData.txt");
		string line;
		bool found = false;

		if (!infile.is_open())
		{
			throw runtime_error("Unable to open file.");
		}

		while (getline(infile, line)) // Read all lines into memory
		{
			stringstream ss(line);
			int empID;
			ss >> empID;
			if (empID != id)
			{
				lines.push_back(line); // Keep all lines that don't match the ID
			}
			else
			{
				found = true; // Identify that we found and are 'deleting' this line
			}
		}
		infile.close();
		if (!found)
		{
			cout << "Employee with ID " << id << " not found." << endl;
		}
		else
		{
			ofstream outfile("employeeData.txt", ios::trunc); // Rewrite the file from the in-memory lines excluding the deleted entry
			if (!outfile.is_open())
			{
				throw runtime_error("Unable to open file for writing.");
			}

			for (const auto& savedLine : lines)
			{
				outfile << savedLine << "\n";
			}

			outfile.close();
			cout << "Employee with ID " << id << " has been deleted." << endl;
		}
	}
};

void addEmployeeEntry(vector<EmployeeData>& employee) //Function used when adding a new employee. Data is written to the employee struct then written to the file
{
	EmployeeData NewEmployee;
	cout << "Enter Employee Name: ";
	cin.ignore();
	getline(cin, NewEmployee.employeeName);
	cout << "Enter Employee ID: ";
	cin >> NewEmployee.employeeID;
	cout << "Enter Employee PC#: ";
	cin >> NewEmployee.PCNumber;

	NewEmployee.hours.resize(10, 0);
	employee.push_back(NewEmployee);
}

int main() //MAIN FUNCTION
{
	vector<EmployeeData> employee; // create a data object
	MenuScreens Menu;
	fileOpperations file;
	int choice, employeeSearch;
	char keepGoing;
	fstream fout;

	// do-while loop to select an option
	do {
		Menu.mainMenuScreen();
		cin >> choice;
		if (choice == 1)
		{
			cout << "File open status: " << file.openFileTest(fout, "employeeData.txt") << endl;
			cout << "Enter Employee ID: ";
			cin >> employeeSearch;
			cout << endl;

			if (file.searchEmployeeByID(employee, employeeSearch))
			{
				Menu.absenceType();
				int absenceChoice;
				double hours;

				cout << "What kind of absence would you like to enter: ";
				cin >> absenceChoice;


				string absence;
				switch (absenceChoice)
				{
				case 1: absence = "Local Leave"; break;
				case 2: absence = "State Leave"; break;
				case 3: absence = "Unpaid Leave"; break;
				case 4: absence = "Workers Comp"; break;
				case 5: absence = "Off Campus Duty"; break;
				case 6: absence = "Jury Duty"; break;
				case 7: absence = "Funeral Leave"; break;
				case 8: absence = "ASLT"; break;
				case 9: absence = "COMP Time"; break;
				case 10: absence = "Other"; break;
				default: cout << "Invalid absence type selected." << endl; continue;
				}

				cout << "How long was the absence in hours: ";
				cin >> hours;

				if (file.updateEmployeeRecord(employeeSearch, absence, hours))
				{
					cout << "File Update Success!" << endl;
				}
				else
				{
					cout << "File Update Failed!" << endl;
				}
			}
			else
			{
				cout << "Either employee is not in database or employee ID was entered wrong." << endl;
			}
			cout << "Would you like to try again? (y/n)";
			cin >> keepGoing;
		}
		else if (choice == 2)
		{
			file.openFileTest(fout, "employeeData.txt");
			addEmployeeEntry(employee);
			file.writeToFile(employee);
			cout << "Would you like to keep running the program? (y/n) \t";
			cin >> keepGoing;
		}
		else if (choice == 3)
		{
			int employeeID;
			cout << "Enter the ID of the employee whos attendence information youd like to see:  ";
			cin >> employeeID;
			if (file.searchEmployeeByID(employee, employeeID))
			{
				NULL;
			}
			else
			{
				cout << "Either employee is not in database or employee ID was entered wrong." << endl;
			}
			cout << "Would you like to keep running the program? (y/n)\t";
			cin >> keepGoing;
		}
		else if (choice == 4)
		{
			cout << "Enter the employees ID of which you'd like to delete:  ";
			cin >> employeeSearch;
			file.deleteEmployeeEntry(employee, employeeSearch);
			cout << "Would You like to keep running the program? (y/n)\t";
			cin >> keepGoing;

		}
		else if (choice == 0)
		{
			cout << "Quitting Program";
			break;
		}
		else
		{
			cout << "Wrong Option Chosen, Please try again" << endl;
			keepGoing = 'y';
		}
	} while (tolower(keepGoing) == 'y');

	return 0;
}


