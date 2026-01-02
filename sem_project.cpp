//HEADER FILES
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <Windows.h>
using namespace std;

//PROTOTYPING AND DECLARING A STRUCT
struct patient {
	string name;
	int age;
	int severity;
	patient* next;
};
patient* head = NULL;
void enter(patient*& head, patient* new_entry);
bool priority_sort(patient* x, patient* y);
void display(patient* head);
void call(patient*& head, int& doctor_free_time, int current_time, bool mode);
int countQueue(patient* head);
int treat_time_std = 10; //Global Variable for treatment time under standard conditions
int treat_time_emerg = 5; //Global Variable for treatment time under emergency mode
int patients_treated = 0;
int patients_entered = 0;

void save(patient* head);
void load(patient*& head);
void setColor(int color) { 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); 
}

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

		setColor(3);
		cout << "---------------------------------" << endl;
		cout << "HOSPITAL QUEUE MANAGEMENT SYSTEM" << endl;
		cout << "           " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << denotion << endl; //DISPLAYS CURRENT TIME
		cout << "---------------------------------" << endl;
		//USER GIVEN 4 OPTIONS
		cout << "1 - Enter Patient" << endl;
		cout << "2 - Display Queue" << endl;
		if (em == true)
			cout << "3 - Exit Emergency Mode" << endl; //This diplays if emeergency mode is on
		else {
			cout << "3 - Emergency Mode" << endl; //Displays if Emergency mode is off
		}
		cout << "4 - End of Day" << endl;
		cout << "Enter choice: ";
		cin >> choice; //USER ENTERS HIS CHOIICE
		cout << "---------------------------------" << endl << endl;

		if (choice == "1") {
			patient* x = new patient;

			setColor(14);
			cout << "ENTER THE PATIENT'S NAME: ";
			cin.ignore(); // clears buffer
			getline(cin, x->name);

			cout << "ENTER THE PATIENT'S AGE: ";
			cin >> x->age; //AGE ENTERED
			while (cin.fail() || x->age < 1 || x->age > 120) { //MAKES SURE AGE IS BETWEEN AN INT
				cin.clear();
				cin.ignore(1000, '\n');
				setColor(12);
				cout << "INVALID AGE" << endl << endl;
				setColor(14);
				cout << "ENTER THE PATIENT'S AGE: ";
				cin >> x->age;

				if (x->age >= 1 && x->age <= 120)
					break;
			}

			cout << "ENTER THE PATIENT'S SEVERITY (1-50): ";
			cin >> x->severity; //SEVERITY ENTERED

			while (cin.fail() || x->severity < 1 || x->severity > 50) { //MAKES SURE SEVERITY IS BETWEEN 1-50
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "SEVERITY ENTERED IS NOT BETWEEN 1 AND 50" << endl << endl;
				cout << "ENTER THE PATIENT'S SEVERITY (1-50): ";
				cin >> x->severity;

				if (x->severity >= 1 && x->severity <= 50)
					break;
			}
			time_gap = rand() % 11; // PATIENTS ARRIVE RANDOMLY ON A 0-10 MINUTE INTERVAL
			current_time += time_gap;
			call(head, doctor_free_time, current_time, em); //Function to call patients

			patients_entered++;
			x->next = NULL;
			enter(head, x);
		}

		else if (choice == "2") {
			display(head); //FUNCTION CALL TO DISPLAY LIVE QUEUE
		}
		else if (choice == "3") {
			em = !em;

			setColor(4);
			if (em)
				cout << setw(50) << right << "EMERGENCY MODE ACTIVATED" << endl << endl; //ACTIVATES EMERGENCY MODE
			else
				cout << setw(50) << right << "EMERGENCY MODE DEACTIVATED" << endl << endl; //DEACTIVATES EMERGENCY MODE
		}
		else if (choice == "4") {
			save(head); //SAVES DATA OF THE DAY TO FILE
			int patients_in_queue = countQueue(head);
			setColor(14);
			cout << setw(70) << right << "----------------------------------"<<endl;
			cout << setw(70) << right << "THE DAY HAS OFFICIALLY WRAPPED UP"<<endl;
			cout << setw(70) << right << "----------------------------------"<<endl;
			cout << setw(62) << right << "STATS FOR THE DAY"<<endl;
			cout << setw(70) << right << "----------------------------------"<<endl;
			cout << setw(62) << right << "Patients Entered: "<< patients_entered <<"\n";
			cout << setw(62) << right << "Patients Treated: " << patients_treated << "\n";
			cout << setw(64) << right << "Patiients in Queue: " << patients_in_queue << "\n";
			break; //BREAKS FROM LOOP BECAUSE "DAY ENDED"
		}
		else { //PREVENTS INPUT OTHER THAN 1, 2, AND 3
			cout << "INVALID INPUT" << endl;
		}
		cout << endl;
	}

	save(head);  //Saves patient info on txt file

	system("pause");
	return 0;
}

//ENTERING PATIENT INFO FUNCTION DEFINITION
void enter(patient*& head, patient* new_entry) {

	if (head == NULL) {
		head = new_entry; //if head = NULL, new entry is the first node
		return;
	}

	if (priority_sort(new_entry, head)) { //If priority_Sort = true, then new entry will be added to start of the list
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
	if (x->severity > y->severity) //If severity of x > everity of y, true returned
		return true;
	if (x->severity < y->severity) //If severity of x < everity of y, true returned
		return false;

	if ((x->age < 10 || x->age > 55) && (y->age >= 10 && y->age <= 55)) //Prioritising children and elderly people
		return true;

	if ((y->age < 10 || y->age > 55) && (x->age >= 10 && x->age <= 55))
		return false;

	return false;
}

//QUEUE DISPLAY FUNCTION DEFINITION
void display(patient* head) {

	if (head == NULL) { //Can't print if there are no nodes
		cout << "QUEUE IS EMPTY" << endl << endl;
		return;
	}

	//COLUMN HEADINGS
	setColor(13);
	cout << setw(20) << left << "SERIAL NUMBER" << setw(20) << left << "NAME" << setw(20) << left << "AGE" << setw(20) << left << "SEVERITY" << endl;

	//Setting up a temporary constant that traverses through nodes
	patient* temp = head;
	int i = 1; //Helps in serial numbering

	while (temp != NULL) {
		//Printing associated values
		setColor(15);
		cout << setw(20) << left << i << setw(20) << left << temp->name << setw(20) << left << temp->age << setw(20) << left << temp->severity << endl; 

		temp = temp->next; //moves temp to next node
		i++; //Incrementing i so next serial number is greater
	}

}

void call(patient*& head, int& doctor_free_time, int current_time, bool mode) {

	int treat_time; //Treamtent time

	if (head == NULL)
		return; //Can't call a patient if there are no nodes/queue is empty

	if (current_time >= doctor_free_time) {

		setColor(10);
		cout << endl << setw(55) << right << head->name << " PLEASE REPORT TO THE DOCTOR'S OFFICE" << endl; //Prints a call message

		
		patient* temporary = head; 
		head = head->next; //Moves head of linked list to next node, isolating the previous head
		delete temporary; //Deletes previous head (patient with top priority) once the patient gets called
		patients_treated++;

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

	//Loop to export data onto the file as one column
	while (temp != NULL) {
		//Prints data from one node
		file << temp->name << endl;
		file << temp->age << endl;
		file << temp->severity << endl;

		temp = temp->next; //Moves onto next node
	}

	file.close();
}

void load(patient*& head) {

	ifstream file("patient_info.txt");

	if (!file)
		return; 

	while (!file.eof()) {
		patient* x = new patient;

		//Inputting info from file into the queue with the help of linked lists
		getline(file, x->name);
		if (x->name == "")
			break;

		file >> x->age;
		file >> x->severity;
		file.ignore();

		x->next = NULL;
		enter(head, x);
	}

	file.close(); //closes file
}

//To count patients in queue at the end of the day
int countQueue(patient* head) {
	int count = 0;
	while (head != NULL) {
		count++;
		head = head->next;
	}
	return count;
}