#include <iostream>
#include <conio.h>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;

void enter(int age[], int severity[], string names[], int count);
void sort(int age[], int severity[], string names[], int count);
void display(int age[], int severity[], string names[], int count);


int main() {
	int age[200];
	int severity[200];
	string names[200];
	int count = 0;

	while (true) {

		string choice;

		cout << "---------------------------------"<<endl;
		cout << "HOSPITAL QUEUE MANAGEMENT SYSTEM"<<endl;
		cout << "---------------------------------"<<endl;
		cout << "1 - Enter Patient" << endl;
		cout << "2 - Display Queue" << endl;
		cout << "3 - End of Day: "<<endl;

		cout << "Enter choice: ";
		cin >> choice;
		cout << "---------------------------------"<<endl<<endl;

		if (choice == "1") {
			enter(age, severity, names, count);
			count++;
			sort(age, severity, names, count);
		}
		else if (choice == "2") {
			display(age, severity, names, count);
		}
		else if (choice == "3") {
			cout << count << " PATIENTS WERE TREATED TODAY.";
			break;
		}
		else {
			cout << "INVALID INPUT" << endl;
		}
		cout << endl;
	}
	_getch();
	return 0;
}

void enter(int age[], int severity[], string names[], int count) {
	string n;
	int a;
	int s;

	cout << "ENTER THE PATIENT'S NAME: ";
	cin >> n;
	names[count] = n;
	cout << "ENTER THE PATIENT'S AGE: ";
	cin >> a;
	age[count] = a;
	cout << "ENTER THE PATIENT'S SEVERITY (1-50): ";
	cin >> s;
	severity[count] = s;
}

void sort(int age[], int severity[], string names[], int count) {

	//Sorting Severity 
	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j< count - i - 1; j++)

		if (severity[j] < severity[j + 1]) {

			swap(severity[j], severity[j + 1]);
			swap(names[j], names[j + 1]);
			swap(age[j], age[j + 1]);
		}
	}

}

void display(int age[], int severity[], string names[], int count) {

	cout << setw(20) << left << "SERIAL NUMBER" << setw(20) << left << "NAME" << setw(20) << left << "AGE" << setw(20) << left << "SEVERITY" << endl;

	for (int i = 0; i < count; i++) {
		cout << setw(20) << left << i << setw(20) << left << names[i] << setw(20) << left << age[i] << setw(20) << left << severity[i] << endl;
	}
}