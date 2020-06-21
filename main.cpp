#include <bits/stdc++.h>

using namespace std;

// Some global variable
char buffer[15000];

void add(string &route){

	stringstream mail;
	ifstream input(route, ios::binary);
	
	// read file into a string-stream
	mail << input.rdbuf();
	input.close();
	//cout << mail.str();
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

	for(int i = 1; i <= 10000; i++){
		string s = "./test_data/mail";
		s += to_string(i);
		add(s);
	}
	string q; /*q for query to avoid using same variable name */
	while(getline(cin, q)){
		/* split string and get the query */
		string delimiter = " ";
		size_t pos = 0;
		string token;
		pos=q.find(delimiter);
		token=q.substr(0, pos);
		q.erase(0, pos + delimiter.length());
		if(token=="add"){
			add(q);
		}else if(token=="remove"){
			int file_id;
			file_id=stoi(q);
			remove(file_id);
		}else if(token=="longest"){
			longest();
		}else{
			stringstream expression;
			expression<<q;
			query(expression);
		}
		
	}
	return 0;
}
