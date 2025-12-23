//HEADER FILES
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

//PROTOTYPING AND DECLARING A STRUCT
struct patient {
	string name;
	int age;
	int severity;
	patient* next;
};
patient* head = NULL;
void enter(patient* &head, patient* new_entry);
bool priority_sort (patient* x, patient* y);
void display(patient* head);
void call(patient*& head, int& doctor_free_time, int current_time, bool mode);
int treat_time_std = 10;
int treat_time_emerg = 5;
void save(patient* head);
void load(patient*& head);

//MAIN FUNCTION
int main() {
	//DECLARATION OR INITIALIZATION OF VARIABLES
	int doctor_free_time = 0;
	int current_time = 0;
	int time_gap = 0;
	bool em = false;
	srand(time(0));

	load(head); //Importing data from file

	//LOOP THAT RUNS THROUGHOUT THE DAY UNTIL USER ENDS THE DAY
	while (true) {

		//DECLARATION OF VARIABLES CONCERNED WITH THE LOOP ONLY
		string choice; // USER'S CHOICES THROUGHOUT THE DAY
		//FOR PRINTING TIME
		int hours = 8 + current_time / 60;
		int minutes = current_time % 60;
		string denotion = (hours >= 12 ? "PM" : "AM"); //12 HOUR CLOCK (AM/PM)

		if (hours > 12) {
			hours -= 12; // CONVERSION FROM 24 HR TO 12 HR (e.g 13 --> 1)
		}
		if (hours == 0) {
			hours = 12;
		}

		cout << "---------------------------------" << endl;
		cout << "HOSPITAL QUEUE MANAGEMENT SYSTEM" << endl;
		cout << "           " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << denotion << endl; //DISPLAYS CURRENT TIME
		cout << "---------------------------------" << endl;
		//USER GIVEN 4 OPTIONS
		cout << "1 - Enter Patient" << endl;
		cout << "2 - Display Queue" << endl;
		if (em == true)
			cout << "3 - Exit Emergency Mode" << endl;
		else
		    cout << "3 - Emergency Mode" << endl;
		cout << "4 - End of Day: " << endl;
		cout << "Enter choice: ";
		cin >> choice; //USER ENTERS HIS CHOIICE
		cout << "---------------------------------" << endl << endl;

		if (choice == "1") {
			patient* x = new patient;
			cout << "ENTER THE PATIENT'S NAME: ";
			cin.ignore(); // clears buffer
			getline(cin, x->name);
			cout << "ENTER THE PATIENT'S AGE: ";
			cin >> x->age; //AGE ENTERED
			cout << "ENTER THE PATIENT'S SEVERITY (1-50): ";
			cin >> x->severity; //SEVERITY ENTERED
			while (x->severity < 1 || x->severity > 50) { //MAKES SURE SEVERITY IS BETWEEN 1-50
				cout << "SEVERITY ENTERED IS NOT BETWEEN 1 AND 50" << endl << endl;
				cout << "ENTER THE PATIENT'S SEVERITY (1-50): ";
				cin >> x->severity;
			}
			time_gap = rand() % 11; // PATIENTS ARRIVE RANDOMLY ON A 0-10 MINUTE INTERVAL
			current_time += time_gap;
			call(head, doctor_free_time, current_time, em);

			x->next = NULL;
			enter(head, x);
		}

		else if (choice == "2") {
			display(head); //FUNCTION CALL TO DISPLAY LIVE QUEUE
		}
		else if (choice == "3") {
			em = !em;

			if (em)
				cout << setw(50) << right << "EMERGENCY MODE ACTIVATED" << endl << endl; //ACTIVATES EMERGENCY MODE
			else
				cout << setw(50) << right << "EMERGENCY MODE DEACTIVATED" << endl << endl; //DEACTIVATES EMERGENCY MODE
		}
		else if (choice == "4") {
			save(head); //SAVES DATA OF THE DAY TO FILE
			break; //BREAKS FROM LOOP BECAUSE "DAY ENDED"
		}
		else { //PREVENTS INPUT OTHER THAN 1, 2, AND 3
			cout << "INVALID INPUT" << endl;
		}
		cout << endl;
	}
	system("pause");
	return 0;
}

//ENTERING PATIENT INFO FUNCTION DEFINITION
void enter(patient*& head, patient* new_entry) {

	if (head == NULL) {
		head = new_entry;
		return;
	}

	if (priority_sort(new_entry, head)) {
		new_entry->next = head;
		head = new_entry;
		return;
	}

	patient* curr = head;
	while (curr->next != NULL && !priority_sort(new_entry, curr->next)) {
		curr = curr->next;
	}

	new_entry->next = curr->next;
	curr->next = new_entry;
}

//QUEUE SORTING FUNCTION DEFINITION
bool priority_sort(patient* x, patient* y) {
	if (x->severity > y->severity)
		return true;
	if (x->severity < y->severity)
		return false;

	if ((x->age < 10 || x->age > 55) && (y->age >= 10 && y->age <= 55))
		return true;

	if ((y->age < 10 || y->age > 55) && (x->age >= 10 && x->age <= 55))
		return false;

	return false;
	}

//QUEUE DISPLAY FUNCTION DEFINITION
void display(patient* head) {

	if (head == NULL) {
		cout << "QUEUE IS EMPTY"<<endl<<endl;
		return;
	}

	//COLUMN HEADINGS
	cout << setw(20) << left << "SERIAL NUMBER" << setw(20) << left << "NAME" << setw(20) << left << "AGE" << setw(20) << left << "SEVERITY" << endl;

	patient* temp = head;
	int i = 1;

	while (temp != NULL) {
		cout << setw(20) << left << i << setw(20) << left << temp->name << setw(20) << left << temp->age << setw(20) << left << temp->severity << endl;
		temp = temp->next;
		i++;
	}
	
}

void call(patient*& head, int& doctor_free_time, int current_time, bool mode) {

	int treat_time;

	if (head == NULL)
		return;

	if (current_time >= doctor_free_time) {

		cout << endl << setw(50)<< right<< "PATIENT " << head->name << " PLEASE REPORT TO THE DOCTOR'S OFFICE" << endl;

		patient* temporary = head;
		head = head->next;
		delete temporary;

		if (mode == true) {
			treat_time = treat_time_emerg;
		}
		else
			treat_time = treat_time_std;

		doctor_free_time = current_time + treat_time;
	}
}

void save(patient* head) {
	
	ofstream file("patient_info.txt");

	patient* temp = head;
	
	while (temp != NULL) {
		file << temp->name << endl;
		file << temp->age << endl;
		file << temp->severity << endl;

		temp = temp->next;
	}

	file.close();
}

void load(patient*& head) {

	ifstream file("patient_info.txt");

	if (!file)
		return;

	while (!file.eof()) {
		patient* x = new patient;

		getline(file, x->name);
		if (x->name == "")
			break;

		file >> x->age;
		file >> x->severity;
		file.ignore();

		x->next = NULL;
		enter(head, x);
	}

	file.close();
}