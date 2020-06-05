//#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

class date{
	public:
		int getdate()
			return date_value;
		date(int y, int m, int d, int t){
			year = y;
			month = m;
			day = d;
			time = t;
			date_value = y * 10000000 + m * 1000000 + d * 10000 + t; 
		}

	private:
		int year, month, day, time;
		long long int date_value;
}

class mail{
	public:
		
	private:
		int id;
		string from, to;
		date senttime;
		
};

void add(string &route){
	stringstream mail;
	ifstream input(route, ios::binary);
	
	// read file into a string-stream
	mail << input.rdbuf();
	input.close();
	cout << mail.str();
	// processing the data
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
	unordered_map<string, int> months;
	months["January"] = 1;
    months["February"] = 2;
    months["March"] = 3;
    months["April"] = 4;
    months["May"] = 5;
    months["June"] = 6;
    months["July"] = 7;
    months["August"] = 8;
    months["September"] = 9;
    months["October"] = 10;
    months["November"] = 11;
    months["December"] = 12;

	string test = "test_data\\mail1";
	add(test);
	return 0;
}