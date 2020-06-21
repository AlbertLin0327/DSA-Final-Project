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

unordered_map <char, int> precedence_order{
	{'!', 1},
	{'&', 2},
	{'|', 3},
	{'(', 4}
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
			tolower(word[nowlen]);
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

	// find intersection of "from" list and "result"
	vector <int> intersected(10000);
	auto end = set_intersection(result.begin(), result.end(), source[name].begin(), source[name].end(), intersected.begin());

	// resize the "result" vector
	intersected.resize(end - intersected.begin());
	result = intersected;
	return;
}

void infix_to_postfix(string &expression, vector <string> &postfix){
	stack <char> operator_stack;
	int length = expression.length();
	for(int i = 0; i < length; i++){

		if(!isalpha(expression[i])){
			if(expression[i] == '(')
				operator_stack.push('(');
			else if(expression[i] == ')'){
				while(!operator_stack.empty() && operator_stack.top() != '('){
					string str(1, operator_stack.top());
					postfix.push_back(str);
					operator_stack.pop();
				}

				if(!operator_stack.empty() && operator_stack.top() == '(')
					operator_stack.pop(); 
				
			}else{
				while(!operator_stack.empty() && precedence_order[expression[i]] >= precedence_order[operator_stack.top()]){
					string str(1, operator_stack.top());
					postfix.push_back(str);
					operator_stack.pop();
				}
				operator_stack.push(expression[i]);
			}
		}else{
			string str(1, expression[i]);
			postfix.push_back(str);
		}
	}

	while(!operator_stack.empty()){
		string str(1, operator_stack.top());
		postfix.push_back(str);
		operator_stack.pop();
	}

	for(auto c : postfix)
		cout << c;
	
	return;
}


void query_date(int64_t &date1, int64_t &date2, vector<int> &result){
    set<int> record;


    for(auto i : date){
        if(i.first > date2)
            break;
        else if(i.first >= date1){
            for(auto j : date[i.first])
                record.insert(j);
        }
    }
    vector<int> intersected(10000); 
    vector<int> ::iterator t = set_intersection(record.begin(), record.end(), result.begin(), result.end(), intersected.begin());
    intersected.resize(t - intersected.begin());
    result = intersected;
    return ;
}

void query_expression(string name, vector <int> &result){
	cout << name << endl;
	vector <int> answer(10000);
	vector <string> postfix(100);
	infix_to_postfix(name, postfix);

	bool ans = 1;
	int length = postfix.size();
	for(int i = 0; i < length; i++){
		auto first_set = keyword[postfix[i]];
		if(postfix[i + 1] == "!"){
			i++;
			if(ans){
				set_difference(result.begin(),result.end() , first_set.begin(), first_set.end(), answer.begin());
				result.clear(); 
			}else{
				set_difference(answer.begin(),answer.end() , first_set.begin(), first_set.end(), result.begin());
				answer.clear(); 
			}
			ans = !ans;
		}else{
			auto second_set = keyword[postfix[i + 1]];
			if(postfix[i + 2] == "&"){
				if(ans){
					set_intersection(result.begin(),result.end() , first_set.begin(), first_set.end(), answer.begin());
					result.clear(); 
				}else{
					set_intersection(answer.begin(),answer.end() , first_set.begin(), first_set.end(), result.begin());
					answer.clear(); 
				}
				ans = !ans;
			}else{
				if(ans){
					set_union(result.begin(),result.end() , first_set.begin(), first_set.end(), answer.begin());
					result.clear();
				}else{
					set_union(answer.begin(),answer.end() , first_set.begin(), first_set.end(), result.begin());
					answer.clear(); 
				}
				ans = !ans;
			}
			i += 2;
		}
	}
	return;
}

void query(string &name){
    vector<int> result(exist_mail_id.begin(), exist_nail_id.end());
    for(int i = 0 ; i < name.length(); i ++){
        if(name[i] == '-' && name[i + 1] == 'f'){
            int j = i;
            while(name[j] != ' ')
                j++;
            string from_user = name.substr(i + 3, j - i - 4);
            //cout << "f: " << from_user << endl;
            query_from_to(from_user, result, from);
            i = j;
        }
        else if(name[i] == '-' && name[i + 1] == 't'){
            int j = i;
            while(name[j] != ' ')
                j++;
            string to_user = name.substr(i + 3, j - i - 4);
            //cout << "t: " << to_user << endl;
            query_from_to(to_user, result, to);
            i = j;            
        }
        else if(name[i] == '-' && name[i + 1] == 'd'){
            int j = i;
            while(name[j] != '~')
                j++;
            string date_1 = (i + 2 == j - 1) ? "0" : name.substr(i + 2, 12);
            i = j + 1;
            while(name[j] != ' ')
                j++;
            string date_2 = (i == j) ? "999999999999" : name.substr(i, 12);
            int64_t date_int_1 = stoll(date_1);
            int64_t date_int_2 = stoll(date_2);
            //cout << "d: " << date_int_1 << " " << date_int_2 << endl;
            i = j;
            query_date(date_int_1, date_int_2, result); 
        }
        else{
            string ex = name.substr(i, name.length() - i);
            //cout << "ex: " << ex << endl;
            query_expression(ex, result);
            break;
        }

    }
}

int main(){

	// fast I/O and also avoid std::cout << std::endl
	ios::sync_with_stdio(false);
	cin.tie(NULL);


	vector <string> postfix(100);

	// reserve space from the data structure


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
			string condition; 
			condition.reserve(10000);
			getline(cin, condition, '\n');
			query(condition);
		}
	}


	return 0;
}
