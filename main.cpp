#include <bits/stdc++.h>
using namespace std;

// Some global variable
char buffer[150000];

// Define Conversion of month and 
unordered_map <string, int> months{
	{"January", 1},
	{"February", 2},
	{"March", 2},
	{"April", 4},
    {"May", 5},
    {"June", 6},
    {"July", 7},
    {"August", 8},
    {"September", 9},
    {"October", 10},
    {"November", 11},
    {"December", 12}
};

unordered_map <string, set < int > > from;
unordered_map <string, set < int > > to;
unordered_map <string, set < int > > keyword;

class DATE{
public:
	inline int get_date(){
		return date_value;
	}

	DATE(int &y, int &m, int &d, int &t){
		year = y;
		month = m;
		day = d;
		time = t;
		date_value = y * 10000000 + m * 1000000 + d * 10000 + t; 
	}

private:
	int year, month, day, time;
	long long int date_value;
};

class MAIL{
public:
	
private:
	int id;
	string from, to;
	DATE senttime;
		
};

void add(string &route){
	// input file route
	FILE* input = fopen(route.c_str(), "rb");
	
	// read words into global buffer and count
	char from[60], to[60], month[15];
	int year, day, time, hour, second, ID;

	char word[60];
	int shift = 0;

	// parse the field to get the imformation
	fscanf(input, "From: %s\nDate: %d %s %d at %d:%d\nMessage-ID: %d\n", from, &year, month, &day, &hour, &second, &ID);

	// Get Subject information
	fgets(buffer, 100000, input);

	// Skip "Subject: " and start parsing from first argument of subject
	char* ptr = strtok(buffer, " ");
	ptr = strtok(NULL, " ");

	while (ptr){

		/***************************** TODO *****************************\
		Get rid of punctuation marks
		\****************************************************************/

		// add key word to request tree
		string subject_word(ptr);
		keyword[subject_word].insert(ID);

		ptr = strtok(NULL, " ");
	} 

	fscanf(input, "To: %s\nContent:\n", to);

	fclose(input);

	// variable needed for key to the mail parsing by string-scan
	
	return;
}

void remove(int &id){

	// remove message-id = id from all data structures
	return;
}

void longest(){

	// return the longest message
}

void query(stringstream &conditions){

	// parse the condition and find the target mail
	return;
}

int main(){

	// fast I/O and also avoid std::cout << std::endl
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	for(int i = 1; i < 10000; i++){
		string s = "./test_data/mail";
		s += to_string(i);
		add(s);
		
	}


	return 0;
}