#include <bits/stdc++.h>
using namespace std;

// Some global variable

// Define Conversion of monvth and its value
unordered_map <string, int> months{
	{"January", 1},
	{"February", 2},
	{"March", 3},
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
int total_mail_in_database;
set <int> exist_mail_id;
//unordered_set <int> proccessed_mail_id;

unordered_map <string, int> route_and_id;

// Length as the key, Message-ID as the value
map <int, set < int > > length_of_mail;

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

void parse_and_build_subject(int &ID, FILE* &fp){
	char word[1000];
	char buffer[10000];
	fgets(buffer, 10000, fp);
	int shift = 0, offset;
	while(sscanf(buffer + shift, "%s%n", word, &offset) == 1){
		shift += offset;

		// two pointer method to record the start and end of a string
		int nowlen = 0, lastlen = -1;
		char cutted_word[1000];

		// Sequential search and cut every non alphabet and number character
		int length = strlen(word);
		while(nowlen <= length){
			word[nowlen] = tolower(word[nowlen]);
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

void parse_and_build_content(int &ID, FILE* &fp, int &total_length){
	char word[1000];
	while(fscanf(fp, "%s", word) != EOF){

		// two pointer method to record the start and end of a string
		int nowlen = 0, lastlen = -1;
		char cutted_word[1000];

		// Sequential search and cut every non alphabet and number character
		int length = strlen(word);
		while(nowlen <= length){
			word[nowlen] = tolower(word[nowlen]);
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
		    }else
		    	total_length++;

		    nowlen++;
		}
	} 
	return;
}

void add(string &route){
	int total_mail_length = 0;
	// input file route
	FILE* input = fopen(route.c_str(), "rb");
	
	// read words into global buffer and count
	char sender[60], reciever[60], month[15];
	int year, day, time, hour, minute, ID;
	//if(exist_mail_id.find(ID) != exist_mail_id.end())
	// parse the field to put the informations into the tree and skip "Subject: "
	fscanf(input, "From: %s\nDate: %d %s %d at %d:%d\nMessage-ID: %d\nSubject: ", sender, &day, month, &year, &hour, &minute, &ID);
	if(exist_mail_id.find(ID) != exist_mail_id.end()){
		cout << "-\n";
		fclose(input);
		return;
	}
	route_and_id[route] = ID;
	
	// Get Subject information
	parse_and_build_subject(ID, input);
	
	// Read "To: " and skip "Content: "
	fscanf(input, "To: %s\nContent:\n", reciever);

	// Read and parse content.
	parse_and_build_content(ID, input, total_mail_length);

	fclose(input);

	date_contruction(year, months[month], day, hour, minute, ID);

	//total_mail_length += strlen(sender) + strlen(month) + (day >= 10 ? 2 : 1) + 9 + (int)log10(ID) + 9 + strlen(reciever);

	// Add to search tree
	string message_to;
	string message_from;

	for(int i = 0; i < strlen(reciever); i++)
		message_to += tolower(reciever[i]);

	for(int i = 0; i < strlen(sender); i++)
		message_from += tolower(sender[i]);

	to[message_to].insert(ID);
	from[message_from].insert(ID);

	length_of_mail[total_mail_length].insert(ID);

	// Update mail list
	total_mail_in_database++;
	exist_mail_id.insert(ID);
	//proccessed_mail_id.insert(ID);

	cout << total_mail_in_database << "\n";
	
	
	return;
}

inline void remove(int &id){

	// remove message-id from exists list but don't erase from processed mail-id list
	if(exist_mail_id.find(id) != exist_mail_id.end()){
		total_mail_in_database--;
		cout << total_mail_in_database << "\n";
	}else
		cout << "-" << "\n";

	exist_mail_id.erase(id);
	return;
}

void longest(){
	
	for(auto ptr = length_of_mail.rbegin(); ptr != length_of_mail.rend(); ptr++){
		auto p1 = exist_mail_id.begin();
		auto p2 = ptr->second.begin();

		while(p1 != exist_mail_id.end() && p2 != ptr->second.end()){

			if(*p1 == *p2){
				cout << *p1 << " " << ptr->first << "\n";
				return;
			}else if(*p1 > *p2)
				p2++;
			else
				p1++;
		}
	}

	cout << "-" << "\n";
	// return the longest message
}

inline void query_from_to(string name, vector <int> &result, unordered_map <string, set < int > >& source){

	// find intersection of "from" list and "result"
	/*cout << "result: ";
	for(auto i : result)
		cout << i << " ";
	cout << endl <<  name << ":";
	for(auto i : source[name])
		cout << i << " ";
	cout << endl;*/
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

		if(!isalnum(expression[i])){
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
			while(i + 1 < length && isalnum(expression[i + 1]))
				str += expression[++i];

			postfix.push_back(str);
		}
	}

	while(!operator_stack.empty()){
		string str(1, operator_stack.top());
		postfix.push_back(str);
		operator_stack.pop();
	}
	
	return;
}

void query_date(int64_t &date1, int64_t &date2, vector<int> &result){
    set<int> record;
	//cout << "d1: " << date1 << "d2: " << date2 << endl;
    for(auto i : date){
		//cout << i.first << endl;
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

void query_expression(string condition, vector <int> &result){

	vector < vector <int> > numerator;

	vector <string> postfix;
	infix_to_postfix(condition, postfix);

	bool ans = 1;

	for(auto c : postfix){
		//cout << c << " ";
		if(c == "!"){
			vector <int> answer(10000);
			auto end = set_difference(exist_mail_id.begin(), exist_mail_id.end(), numerator.back().begin(), numerator.back().end(), answer.begin());
			answer.resize(end - answer.begin());
			numerator.pop_back();
			numerator.push_back(answer);
		}else if(c == "&"){
			vector <int> answer(10000);
			auto end = set_intersection(numerator.back().begin(), numerator.back().end(), numerator[numerator.size() - 2].begin(), numerator[numerator.size() - 2].end(), answer.begin());
			answer.resize(end - answer.begin());
			numerator.pop_back();
			numerator.pop_back();
			numerator.push_back(answer);
		}else if(c == "|"){
			vector <int> answer(10000);
			auto end = set_union(numerator.back().begin(), numerator.back().end(), numerator[numerator.size() - 2].begin(), numerator[numerator.size() - 2].end(), answer.begin());
			answer.resize(end - answer.begin());
			numerator.pop_back();
			numerator.pop_back();
			numerator.push_back(answer);
		}else{
			vector <int> answer(keyword[c].begin(), keyword[c].end());
			//for(int i = 0; i < answer.size(); i ++)
			//	cout << answer[i] << " " ;
			numerator.push_back(answer);
		}
	}

	vector <int> answer(10000);
	auto end = set_intersection(numerator.back().begin(), numerator.back().end(), result.begin(), result.end(), answer.begin());
	answer.resize(end - answer.begin());

	result = answer;

	return;
}

void query(string &name){
    vector<int> result(exist_mail_id.begin(), exist_mail_id.end());
	
    for(int i = 0 ; i < name.length(); i ++){
        /*cout << name[i] << endl;
		for(auto i : result)
			cout << i << " ";
		cout << endl;*/
		if(result.empty())
			break;
		if(name[i] == '-' && name[i + 1] == 'f'){
            int j = i + 3;
            string from_user;

        	while(name[j] != ' ')
                from_user += tolower(name[j++]);
        	from_user.pop_back();
			//cout << "name: " << from_user;
            query_from_to(from_user, result, from);
            i = j;
        }
        else if(name[i] == '-' && name[i + 1] == 't'){
            int j = i + 3;
            string to_user;
            while(name[j] != ' ')
                to_user += tolower(name[j++]);
        	to_user.pop_back();

            query_from_to(to_user, result, to);
            i = j;            
        }
        else if(name[i] == '-' && name[i + 1] == 'd'){
            int j = i + 2;
            string date_1, date_2;

            while(name[j] != '~')
                date_1 += name[j++];

            if(date_1 == "")
            	date_1 = "0";

            j++;
            while(name[j] != ' ')
                date_2 += name[j++];

            if(date_2 == "")
            	date_2 = "999999999999";
            int64_t date_int_1 = stoll(date_1);
            int64_t date_int_2 = stoll(date_2);

            i = j;
            query_date(date_int_1, date_int_2, result); 
        }
        else{
            string condition;
            while(i != name.length())
            	condition += tolower(name[i++]);
			//cout << "exp: " <<condition << endl;
            query_expression(condition, result);

            break;
        }
    }

    if(result.empty())
    	cout << "-" << "\n";
    else{
    	for(int i = 0; i < result.size(); i++){
    		cout << result[i];
    		if(i == result.size() - 1)
    			cout << "\n";
    		else
    			cout << " ";
    	}
    	/*for(auto c : result)
    		cout << c << " ";
    	cout << endl;*/
    }
    return;
}

int main(){

	// fast I/O and also avoid std::cout << std::endl
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	vector <string> postfix(100);
	total_mail_in_database = 0;
	// reserve space from the data structure

	string mode;
	while(cin >> mode){
		if(mode == "add"){
			string route; cin >> route;
			//cout <<"r: " <<  route << endl;
			if(route_and_id.find(route) != route_and_id.end()){
				if(exist_mail_id.find(route_and_id[route]) != exist_mail_id.end())
					cout << "-" << "\n";
				else{
					//cout << "hi\n";
					total_mail_in_database++;
					cout << total_mail_in_database << endl;
					exist_mail_id.insert(route_and_id[route]);
				}
			}	
			else
				add(route);
		}else if(mode == "longest"){
			longest();
		}else if(mode == "remove"){
			int id; cin >> id;
			remove(id);
		}else{
			//char c;
			string condition; 
			condition.reserve(10000);
			getline(cin, condition, '\n');
			//cout << condition << endl;
			condition.erase(0, 1);
			//cout << condition << endl;
			query(condition);

		}
	}


	return 0;
}
