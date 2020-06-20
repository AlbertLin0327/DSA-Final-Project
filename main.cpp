#include <bits/stdc++.h>
using namespace std;

// Some global variable
char buffer[15000];

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

/*
void add(string &route){
	ifstream input(route, ios::binary | ios::in);
	
	if(!input) cout << route;
	// read file into a string-stream
	string data(static_cast <stringstream const&> (stringstream() << input.rdbuf()).str());
	
	cout << data;
	input.close();
	// processing the data
	return;
}
*/

void add(string &route){
	// input file route
	FILE* input = fopen(route.c_str(), "rb");
	
	// read words into global buffer and count
	int word_counter = 0; 
	while((buffer[word_counter] = fgetc(input)) != EOF)
		word_counter++;

	// cout << buffer;
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