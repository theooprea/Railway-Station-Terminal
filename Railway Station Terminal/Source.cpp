#include <iostream>
#include <list>
#include <vector>

using namespace std;

class Station {
	private:
		string name;
		int nr_railways;
		vector<list<int>> railways;
	public:
		Station(int nr_railways, char *name) {
			this->nr_railways = nr_railways;
			this->name = name;
			for (int i = 0; i < nr_railways; i++) {
				railways.push_back(list<int>());
			}
		}
		void print_station() {
			cout << "-----" << name << "-----" << endl;
			print_railways();
		}
 		void print_railways() {
			for (int i = 0; i < nr_railways; i++) {
				print_railway(i);
			}
		}
		void print_railway(int railway) {
			if (railway >= nr_railways) {
				cout << "No such railway" << endl;
			}
			else {
				list<int>::iterator it = railways[railway].begin();
				int lines = 80 - 8 * railways[railway].size();
				for (int i = 0; i < lines; i++) {
					cout << "-";
				}
				// TODO fix showing it
				for (int i = 0; i < railways[i].size(); i++) {
					cout << *it << "----";
				}
				cout << endl;
			}
		}
		void add_train(int train, int railway) {
			if (railway >= nr_railways) {
				cout << "The given railway doesn't exist!(" <<railway<<")"<< endl;
			}
			else {
				if (railways[railway].size() == 6) {
					cout << "Railway full, please select another one" << endl;
				}
				else {
					railways[railway].push_front(train);
				}
			}
		}
};

int digits(int number) {
	int count = 0;
	while (number > 0) {
		count++;
		number /= 10;
	}
	return count;
}

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