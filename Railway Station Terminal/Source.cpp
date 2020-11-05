#include <iostream>
#include <list>
#include <vector>
#include "Source.h"

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
char** command_tokenizer(char *sentence, int *word_counter, const char *tokens) {
	char aux[255];
	char aux2[255];
	strcpy(aux, sentence);
	strcpy(aux2, sentence);
	int count = 0;
	char* p = strtok(aux, tokens);
	while (p) {
		count++;
		p = strtok(NULL, tokens);
	}
	*word_counter = count;
	char** words = new char*[count];
	int counter = 0;
	p = strtok(aux2, tokens);
	while (p) {
		words[counter] = new char[strlen(p) + 1];
		strcpy(words[counter++], p);
		p = strtok(NULL, tokens);
	}
	return words;
}

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
		cout << "Railway nr: " << railway_nr + 1<< " ";
		int lines = getTotalDigits();
		for (int i = 0; i < lines; i++) {
			cout << "-";
		}
		for (Train current : trains) {
			cout<< current.getAbreviation() << current.getNumber() << "---";
		}
		cout << endl;
	}
	void pop_train() {
		trains.pop_back();
	}
	int getSize() {
		return trains.size();
	}
	int getTotalDigits() {
		int count = 0;
		for (Train t : trains) {
			count += digits(t.getNumber()) + t.getAbreviation().size() + 3;
		}
		return 81 - count;
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
		cout << "------------" << name << "------------" << endl;
		cout << "----- Nr of railways: " << nr_railways << " -----" << endl;
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
		if (railway_nr < nr_railways && railway_nr >= 0) {
			railways[railway_nr].print_railway();
		}
		else {
			cout << "The given railway doesn't exist!(" << railway_nr + 1 << ")" << endl;
		}
	}
	void add_train(int train_number, int railway_number) {
		if (train_number >= 200 && train_number < 17000) {
			if (railway_number >= nr_railways || railway_number < 0) {
				cout << "The given railway doesn't exist!(" << railway_number + 1 << ")" << endl;
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
	void pop_train(int railway_nr) {
		railways[railway_nr].pop_train();
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
		int words = 0;
		char **commands = command_tokenizer(command, &words, " ");

		if (!strcmp(commands[0], "pr")) {
			if (words > 1) {
				int railway = atoi(command + 3);
				station.print_railway(railway - 1);
			}
			else {
				station.print_railways();
			}
		}
		else if (!strcmp(commands[0], "ps")) {
			station.print_station();
		}
		else if (!strcmp(commands[0], "add")) {
			if (words == 3) {
				station.add_train(atoi(commands[1]), atoi(commands[2]) - 1);
			}
			else {
				cout << "Error: Not enough operators given" << endl;
			}
		}
		else if (!strcmp(commands[0], "-help")) {
			cout << "help" << endl;
		}
		else if (!strcmp(commands[0], "remove") || !strcmp(commands[0], "rm")) {
			// TODO fix unexisting railway
			station.pop_train(atoi(commands[1]) - 1);
		}
		else {
			cout << "Command " << command << "not found, use -help for more details" << endl;
		}
		fgets(command, 255, stdin);
		command[strlen(command) - 1] = '\0';
	}
	fclose(in_file);
	return 0;
}