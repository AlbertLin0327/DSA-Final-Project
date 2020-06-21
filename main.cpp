#include <bits/stdc++.h>
using namespace std;

// Some global variable

// Define Conversion of monvth and its value
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

// Message-ID as the key
set <int> exist_mail_id;
unordered_set <int> proccessed_mail_id;

// Length as the key, Message-ID as the value
map <int, int> length_of_mail;

// Date as the key, Set of Message-ID as the value
map <int64_t, set < int > > date;

// Name/Keyword as the key, Set of Message-ID as the value
unordered_map <string, set < int > > from(10000);
unordered_map <string, set < int > > to(10000);
unordered_map <string, set < int > > keyword(400000);

inline void date_contruction(int &Y, int &M, int &D, int &h, int &m, int &ID){

	// "Y * 100000000 + M * 1000000 + D * 10000 + h * 100 + m" represent YYYY/MM/DD/hh:mm
	date[(int64_t)Y * 100000000 + M * 1000000 + D * 10000 + h * 100 + m].insert(ID);
	return;
}

void parse_and_build(int &ID, FILE* &fp){
	char word[1000];
	while(fscanf(fp, "%s", word) != EOF){

		// two pointer method to record the start and end of a string
		int nowlen = 0, lastlen = -1;
		char cutted_word[1000];

		// Sequential search and cut every non alphabet and number character
		int length = strlen(word);
		while(nowlen <= length){
		    if(nowlen == length || !isalnum(word[nowlen])){

				if(lastlen < nowlen - 1){

					// copy trimmed string 
				    strncpy(cutted_word, word + lastlen + 1, nowlen - lastlen - 1);
				    cutted_word[nowlen - lastlen - 1] = '\0';

				    // construct the tree
				    string subject_word = cutted_word;
					keyword[subject_word].insert(ID);
				} 

				// update pointer
				lastlen = nowlen;    
		    }
		    nowlen++;
		}
	} 
	return;
}

void add(string &route){
	// input file route
	FILE* input = fopen(route.c_str(), "rb");
	
	// read words into global buffer and count
	char sender[60], reciever[60], month[15];
	int year, day, time, hour, minute, ID;

	char word[60];
	int shift = 0;

	// parse the field to put the informations into the tree and skip "Subject: "
	fscanf(input, "From: %s\nDate: %d %s %d at %d:%d\nMessage-ID: %d\nSubject: ", sender, &day, month, &year, &hour, &minute, &ID);
	
	// Escape if this mail is already in the exists mail list
	if(exist_mail_id.find(ID) != exist_mail_id.end()){
		cout << "-" << "\n";
		fclose(input);
		return;
	}

	date_contruction(year, day, months[month], hour, minute, ID);

	// Get Subject information
	parse_and_build(ID, input);
	
	// Read "To: " and skip "Content: "
	fscanf(input, "To: %s\nContent:\n", reciever);

	// Read and parse content.
	parse_and_build(ID, input);

	// Add to search tree
	string message_to = reciever;
	string message_from = sender;
	to[message_to].insert(ID);
	from[message_from].insert(ID);
	length_of_mail[ftell(input)] = ID;

	// Update mail list
	exist_mail_id.insert(ID);
	proccessed_mail_id.insert(ID);

	cout << ID << "\n";
	fclose(input);
	
	return;
}

inline void remove(int &id){

	// remove message-id from exists list but don't erase from processed mail-id list
	if(exist_mail_id.find(id) != exist_mail_id.end())
		cout << id << "\n";
	else
		cout << "-" << "\n";

	exist_mail_id.erase(id);
	return;
}

void longest(){
	auto p1 = exist_mail_id.begin();
	auto p2 = length_of_mail.rbegin();
	while(p1 != exist_mail_id.end() && p2 !=length_of_mail.rend()){
		if(*p1 == p2->second){
			cout << *p1 << "\n";
			return;
		}else if(*p1 > p2->second)
			p2++;
		else
			p1++;
	}

	cout << "-" << "\n";
	// return the longest message
}

inline void query_from_to(string name, vector <int> &result, unordered_map <string, set < int > >& source){
	vector <int> intersected(10000);
	set_intersection(result.begin(), result.end(), source[name].begin(), source[name].end(), intersected.begin());
	result = intersected;
	return;
}

void query(string &conditions){
	cout << "-" << "\n";
	// parse the condition and find the target mail
	return;
}

int main(){

	// fast I/O and also avoid std::cout << std::endl
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// reserve space from the data structure
	/*
	string s; s.reserve(100);
	for(int i = 1; i < 10000; i++){
		exist_mail_id.insert(i);
		proccessed_mail_id.insert(i);
		s = "./test_data/mail";
		s += to_string(i);
		add(s);
	}
	*/

	string mode;
	while(cin >> mode){
		if(mode == "add"){
			string route; cin >> route;
			add(route);
		}else if(mode == "longest"){
			longest();
		}else if(mode == "remove"){
			int id; cin >> id;
			remove(id);
		}else{
			string condition; condition.reserve(10000);
			getline(cin, condition, '\n');
			query(condition);
		}
	}
	
	return 0;
}