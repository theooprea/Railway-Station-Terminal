#include <iostream>
#include <list>
#include <vector>

using namespace std;

int digits(int n) {
	int count = 0;
	while (n > 0) {
		count++;
		n /= 10;
	}
	return count;
}
// TODO implement counting the number of - to add per railway
/*int lines_per_railway(Railway railway) {
	int counter = 0;
	for ()
}*/

class Train {
private:
	int number;
	string type;
	string abreviation;
public:
	Train() {
		number = 0;
		type = "NotDefined";
		abreviation = "ND";
	}
	Train(int number) {
		this->number = number;
		if (number >= 500 && number < 600) {
			type = "InterCity";
			abreviation = "IC";
		}
		else if (number >= 2000 && number < 10000) {
			type = "Regio";
			abreviation = "R";
		}
		else {
			type = "InterRegio";
			abreviation = "IR";
		}
	}
	int getNumber() {
		return number;
	}
	string getType() {
		return type;
	}
	string getAbreviation() {
		return abreviation;
	}
};
class Railway {
private:
	list<Train> trains;
	int railway_nr;
public:
	Railway(int railway_nr) {
		this->railway_nr = railway_nr;
	}
	void add_train(Train train) {
		trains.push_front(train);
	}
	void print_railway() {
		cout << "Railway nr: " << railway_nr << " ";
		int lines = 80 - 8 * trains.size();
		for (int i = 0; i < lines; i++) {
			cout << "-";
		}
		for (Train current : trains) {
			cout << current.getNumber() << "---";
		}
		cout << endl;
	}
	int getSize() {
		return trains.size();
	}
};
class Station {
private:
	string name;
	int nr_railways;
	vector<Railway> railways;
public:
	Station(int nr_railways, char *name) {
		this->nr_railways = nr_railways;
		this->name = name;
		for (int i = 0; i < nr_railways; i++) {
			Railway new_railway(i);
			railways.push_back(new_railway);
		}
	}
	void print_station() {
		cout << "-----" << name << "-----" << endl;
		for (int i = 0; i < nr_railways; i++) {
			railways[i].print_railway();
		}

	}
	void print_railways() {
		for (int i = 0; i < nr_railways; i++) {
			railways[i].print_railway();
		}
	}
	void print_railway(int railway_nr) {
		railways[railway_nr].print_railway();
	}
	void add_train(int train_number, int railway_number) {
		if (train_number >= 200 && train_number < 17000) {
			if (railway_number >= nr_railways) {
				cout << "The given railway doesn't exist!(" << railway_number << ")" << endl;
			}
			else {
				if (railways[railway_number].getSize() == 8) {
					cout << "Railway full, please select another one" << endl;
				}
				else {
					Train new_train(train_number);
					railways[railway_number].add_train(new_train);
				}
			}
		}
		else {
			cout << "The given train doesn't exist according to CFR!(" << train_number << ")" << endl;
		}
		
	}
};

int main() {
	FILE *in_file = fopen("./config_file.txt", "r");
	char station_name[255];
	int nr_railways;
	int already_configured = 0;

	if (in_file == NULL) {
		in_file = fopen("./config_file.txt", "w");
		cout << "Starting initial configuration..." << endl;
		cout << "Please insert Railway Station name: ";
		fgets(station_name, 255, stdin);
		station_name[strlen(station_name) - 1] = '\0';
		cout << "And the number of available railways: ";
		cin >> nr_railways;
		cout << "Welcome " << station_name << "! For help type 'help'" << endl;
		fprintf(in_file, "%s\n%d\n", station_name, nr_railways);
		already_configured = 1;
	}
	else {
		fgets(station_name, 255, in_file);
		station_name[strlen(station_name) - 1] = '\0';
		fscanf(in_file, "%d", &nr_railways);
	}

	Station station(nr_railways, station_name);
	char command[255];
	fgets(command, 255, stdin);
	command[strlen(command) - 1] = '\0';
	while (strcmp(command,"stop") && strcmp(command, "quit") && strcmp(command, "q") && strcmp(command, "s")) {
		if (!strcmp(command, "pr")) {
			station.print_railways();
		}
		else if (command[0] == 'p' && command[1] == 'r') {
			int railway = atoi(command + 3);
			station.print_railway(railway);
		}
		else if (!strcmp(command, "ps")) {
			station.print_station();
		}
		else if (command[0] == 'a' && command[1] == 'd' && command[2] == 'd') {
			if (command[3] && command[4]) {
				char* aux = command + 4;
				int poz = 0;
				int size = strlen(aux);
				for (int i = 0; i < size; i++) {
					if (aux[i] == ' ') {
						poz = i;
					}
				}
				if (poz) {
					int railway = atoi(aux + poz + 1);
					char train[10];
					strcpy(train, aux);
					train[poz] = '\0';
					int train_nr = atoi(train);
					station.add_train(train_nr, railway);
				}
				else {
					cout << "The add command must be run as 'add train railway'" << endl;
				}
			}
			else {
				cout << "The add command must be run as 'add train railway'" << endl;
			}
		}
		else if (!strcmp(command, "-help")) {
			cout << "help" << endl;
		}
		else {
			cout << "Command " << command << "not found, use -help for more details";
		}
		fgets(command, 255, stdin);
		command[strlen(command) - 1] = '\0';
	}
	fclose(in_file);
	return 0;
}