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
	{'(', 0},
	{'!', 1},
	{'&', 2},
	{'|', 3}
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
	vector <int> intersected(10000);
	auto end = set_intersection(result.begin(), result.end(), source[name].begin(), source[name].end(), intersected.begin());
	intersected.resize(end - intersected.begin());
	result = intersected;
	return;
}

string infix_to_postfix(string &expression, vector <string> &postfix){
	stack <char> operator_stack;
	int length = expression.length();
	for(int i = 0; i < length; i++){

		if(!isalpha(expression[i])){
			if(expression[i] == '(')
				operator_stack.push('(');
			else if(expression[i] == ')'){
				while(!operator_stack.empty() && operator_stack.top() != '('){
					postfix.push_back(operator_stack.top());
					operator_stack.pop();
				}

				if(!operator_stack.empty() && operator_stack.top() == '(')
					operator_stack.pop(); 
				
			}else{
				while(!operator_stack.empty() && precedence_order[expression[i]] >= precedence_order[operator_stack.top()]){
					postfix.push_back(operator_stack.top());
					operator_stack.pop();
				}
				operator_stack.push(expression[i]);
			}
		}else
			postfix.push_back(expression[i]);
	}
	
	return;
}

void query_date(int64_t &date1, int64_t &date2, vector<int> &all_vector){
    set<int> re_set;
    for(auto i : date){
        if(i.first > date2)
            break;
        else if(i.first >= date1){
            for(auto j : date[i.first])
                re_set.insert(j);
        }
    }
    vector<int> out(10000); 
    vector<int> ::iterator t = set_intersection(re_set.begin(), re_set.end(), all_vector.begin(), all_vector.end(), out.begin());
    out.resize(t - out.begin());
    all_vector = out;
    return ;
}

void query_expression(string name, vector <int> &result){
	vector <int> answer(10000);
	vector <string> postfix(100);
	infix_to_postfix(string name, postfix);

	bool ans = 1;
	int length = postfix.size();
	for(int i = 0; i < length; i++){
		auto first_set = keyword[postfix[i]];
		if(postfix[i + 1] == "!"){
			i++;
			if(ans){
				set_difference(result.begin(),result.end() , first_set.begin(), first_set.end(), answer.begin());
				result.clear(); ans = 0;
			}else{
				set_difference(answer.begin(),answer.end() , first_set.begin(), first_set.end(), result.begin());
				answer.clear(); ans = 0;
			}
		}else{
			auto second_set = keyword[postfix[i + 1]];
			if(postfix[i + 2] == "&"){
				if(ans){
					set_intersection(result.begin(),result.end() , first_set.begin(), first_set.end(), answer.begin());
					result.clear(); ans = 0;
				}else{
					set_intersection(answer.begin(),answer.end() , first_set.begin(), first_set.end(), result.begin());
					answer.clear(); ans = 0;
				}
			}else{
				if(ans){
					set_union(result.begin(),result.end() , first_set.begin(), first_set.end(), answer.begin());
					result.clear(); ans = 0;
				}else{
					set_union(answer.begin(),answer.end() , first_set.begin(), first_set.end(), result.begin());
					answer.clear(); ans = 0;
				}
			}
			i += 2;
		}
	}
	
}


void query(string &conditions){
	vector <int> result(exist_mail_id.begin(), exist_mail_id.end());

	size_t pos = 0;
	string token;
	string delimiter=" ";
	set<int> file_id;
	int64_t num1,num2;
	while ((pos = conditions.find(delimiter)) != string::npos) {
		int extra=0; //for deleting the token
    	token = conditions.substr(0, pos);
    	if(token[0]=='-'){ // case -f -t -d
    		if(token[1]=='f'){
    			token.erase(0,2); // erase -f

    			query_from_to(token, result, from);
    			extra=2+token.length();
    		}else if(token[1]=='t'){
    			token.erase(0,2);
    			query_from_to(token, result, to);

    			extra=2+token.length();
    		}else if(token[1]=='d'){
 				token.erase(0,2);
 				extra+=3; 
    			if(token[0]!='~'){ // has starting date
    				string delimiter2="~";
    				size_t pos2=0;
    				pos2=token.find(delimiter2);
    				string token2=token.substr(0, pos2);
    				token.erase(0, pos2+delimiter2.length());
    				num1=stoll(token2);
    				extra+=token2.length();
    				if(token=="")
    					num2=9999;
    				else{
    					extra+=token.length();
    					num2=stoll(token);
    				}
    			}else{
    				token.erase(0,1);
    				num1=0;
    				if(token=="")
    					num2=9999;
    				else{
    					extra+=token.length();
    					num2=stoll(token);
    				}
    			}
    			query_date(num1, num2, result);
    		}
    	}else{
    		extra=token.length();
    		query_expression(token, result);

		}
    	conditions.erase(0, delimiter.length()+ extra);
	}
	if(conditions[0]=='-'){ // case -f -t -d
    	if(conditions[1]=='f'){
    		conditions.erase(0,2); // erase -f

    		query_from_to(conditions, result, from);
    	}else if(conditions[1]=='t'){
    		conditions.erase(0,2);
    		query_from_to(conditions, result, to);

    	}else if(conditions[1]=='d'){
    		int64_t num1,num2;
    		conditions.erase(0,2); // erase -d
    		if(conditions[0]!='~'){ // has starting date
    			string delimiter2="~";
    			size_t pos2=0;
    			pos2=conditions.find(delimiter2);
    			string conditions2=conditions.substr(0, pos2);
    			conditions.erase(0, pos2+delimiter2.length());
    			num1=stoll(conditions2);
    			if(conditions=="")
    				num2=9999;
    			else
    				num2=stoll(conditions);
    		}else{
    			conditions.erase(0,1);
    			num1=0;
    			if(conditions=="")
    				num2=9999;
    			else
    				num2=stoll(conditions);
			}

    		query_date(num1, num2, result);
    	}
    }else{
    	query_expression(conditions, result);
	}

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

/*


	string str = "winter&vacation&is&coming";
	cout << infix_to_postfix(str) << endl;

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


*/

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
			query(q);
		}
		
	}

	return 0;
}
