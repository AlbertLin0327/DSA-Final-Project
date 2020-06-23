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
bitset <10010> exist_mail_id;

unordered_map <string, int> route_and_id;

// Length as the key, Message-ID as the value
map <int, set <int> > length_of_mail;

// Date as the key, Set of Message-ID as the value
map <int64_t, set <int> > date;

// Name/Keyword as the key, Set of Message-ID as the value
vector <string> from(10005);
vector <string> to(10005);
unordered_map <string, bitset < 10010 > > keyword;

inline void date_contruction(int &Y, int &M, int &D, int &h, int &m, int &ID){

	// "Y * 100000000 + M * 1000000 + D * 10000 + h * 100 + m" represent YYYY/MM/DD/hh:mm
	date[(int64_t)Y * 100000000 + M * 1000000 + D * 10000 + h * 100 + m].insert(ID);
	return;
}

void parse_and_build_subject(int &ID, FILE* &fp){
	char word[60] = {0};
	
	int idx = 0;
	while((word[idx] = fgetc(fp))){

		if(!isalnum(word[idx])){
			
			if(idx != 0){
				string subject_word(word, idx);
				keyword[subject_word].set(ID);
			}
			
			idx = -1;
		}else
			word[idx] = tolower(word[idx]);

		idx++;

		if(word[idx] == '\n')
			break;
	}
	return;
}

void parse_and_build_content(int &ID, FILE* &fp){
	char word[60] = {0};
	
	int idx = 0, total_length = 0;
	while((word[idx] = fgetc(fp)) != EOF){
		
		if(!isalnum(word[idx])){
					
			if(idx != 0){
				string subject_word(word, idx);
				keyword[subject_word].set(ID);
			}
			
			idx = -1;
		}else{
			word[idx] = tolower(word[idx]);
            total_length++;
		}
		idx++;

		if(feof(fp))
			break;
    }

    length_of_mail[total_length].insert(ID);
	return;
}

void add(string &route){
	// input file route
	FILE* input = fopen(route.c_str(), "rb");
	
	// read words into global buffer and count
	char sender[60], reciever[60], month[15];
	int year, day, time, hour, minute, ID;
	//if(exist_mail_id.find(ID) != exist_mail_id.end())
	// parse the field to put the informations into the tree and skip "Subject: "

	fscanf(input, "From: %s\nDate: %d %s %d at %d:%d\nMessage-ID: %d\nSubject: ", sender, &day, month, &year, &hour, &minute, &ID);
	route_and_id[route] = ID;
	if(exist_mail_id.test(ID)){
		cout << "-\n";
		fclose(input);
		return;
	}
	
	// Get Subject information
	parse_and_build_subject(ID, input);
	
	// Read "To: " and skip "Content: "
	fscanf(input, "To: %s\nContent:\n", reciever);

	// Read and parse content.
	parse_and_build_content(ID, input);

	fclose(input);

	date_contruction(year, months[month], day, hour, minute, ID);

	// Add to search tree
	string message_to;
	message_to.reserve(55);
	string message_from;
	message_from.reserve(55);

	for(int i = 0; i < strlen(reciever); i++)
		message_to += tolower(reciever[i]);

	for(int i = 0; i < strlen(sender); i++)
		message_from += tolower(sender[i]);

	to[ID] = message_to;
	from[ID] = message_from;

	// Update mail list
	total_mail_in_database++;
	exist_mail_id.set(ID);
	//proccessed_mail_id.insert(ID);

	cout << total_mail_in_database << "\n";
	
	return;
}

inline void remove(int &id){

	// remove message-id from exists list but don't erase from processed mail-id list
	if(exist_mail_id.test(id)){
		total_mail_in_database--;
		cout << total_mail_in_database << "\n";
		exist_mail_id.reset(id);
	}else
		cout << "-\n";

	return;
}

inline void longest(){	
	
	for(auto ptr = length_of_mail.rbegin(); ptr != length_of_mail.rend(); ptr++){
        for(auto c : ptr->second)
        	if(exist_mail_id.test(c)){
        		cout << c << " " << ptr->first << "\n";
        		return;
        	}

    }
	cout << "-" << "\n";
	return;
	// return the longest message
}

inline void query_from_to(string &name, bitset <10010> &result, vector <string> &source){
	bitset<10010> record;
	for(int i = result._Find_first(); i <= 10000; i = result._Find_next(i)){
		if(source[i] == name)
			record[i] = 1;
	}

	result = record;
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

void query_date(int64_t &date1, int64_t &date2, bitset<10010> &result){
    bitset<10010> record;
    for(auto i : date){
		//cout << i.first << endl;
        if(i.first > date2)
            break;
        else if(i.first >= date1){
            for(auto c : i.second)
            	if(result[c])
            		record[c] = 1;
        }
    }
    result &= record;
    return ;
}

void query_expression(string &condition, bitset <10010> &result){

	vector < bitset<10010> > numerator;

	vector <string> postfix;
	infix_to_postfix(condition, postfix);

	bool ans = 1;

	for(auto c : postfix){
		//cout << c << " ";
		if(c == "!"){
			bitset <10010> answer = exist_mail_id & (exist_mail_id ^ numerator.back());
			numerator.pop_back();
			numerator.push_back(answer);
		}else if(c == "&"){
			bitset <10010> answer = numerator.back() & numerator[numerator.size() - 2];
			numerator.pop_back();
			numerator.pop_back();
			numerator.push_back(answer);
		}else if(c == "|"){
			bitset <10010> answer = numerator.back() | numerator[numerator.size() - 2];
			numerator.pop_back();
			numerator.pop_back();
			numerator.push_back(answer);
		}else{
			bitset <10010> answer = keyword[c];
			numerator.push_back(answer);
		}
	}

	result &= numerator.back();

	return;
}

void query(string &name){
    bitset<10010> result = exist_mail_id;
	
    for(int i = 0 ; i < name.length(); i ++){
		if(result.none())
			break;
		if(name[i] == '-' && name[i + 1] == 'f'){
            int j = i + 3;
            string from_user;
			from_user.reserve(55);
        	while(name[j] != ' ')
                from_user += tolower(name[j++]);
        	from_user.pop_back();
            query_from_to(from_user, result, from);
            i = j;
        }
        else if(name[i] == '-' && name[i + 1] == 't'){
            int j = i + 3;
            string to_user;
			to_user.reserve(55);
            while(name[j] != ' ')
                to_user += tolower(name[j++]);
        	to_user.pop_back();

            query_from_to(to_user, result, to);
            i = j;            
        }
        else if(name[i] == '-' && name[i + 1] == 'd'){
            int j = i + 2;
            string date_1, date_2;
			date_1.reserve(13);
			date_2.reserve(13);
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
			condition.reserve(1000);
            while(i != name.length())
            	condition += tolower(name[i++]);
            query_expression(condition, result);

            break;
        }
    }

    if(result.none())
    	cout << "-\n";
    else{
        int count = 0;
    	for(int i = 1; i <= 10000; i++){
    		if(result[i]){
                cout << i;
                count++;
                if(count == result.count())
                    cout << "\n";
                else
                    cout << " ";
            }
            
    	}
    }
    return;
}

int main(){

	// fast I/O and also avoid std::cout << std::endl
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	//vector <string> postfix(100);
	total_mail_in_database = 0;
	// reserve space from the data structure

	string mode;
	while(cin >> mode){
		if(mode == "add"){
			string route; cin >> route;
			if(route_and_id.find(route) != route_and_id.end()){
				if(exist_mail_id.test(route_and_id[route]))
					cout << "-" << "\n";
				else{
					total_mail_in_database++;
					cout << total_mail_in_database << "\n";
					exist_mail_id.set(route_and_id[route]);
				}
			}	
			else
				add(route);
		}else if(mode == "longest"){
			if(exist_mail_id.none())
				cout << "-" << "\n";
			else
				longest();
		}else if(mode == "remove"){
			int id; cin >> id;
			remove(id);
		}else{
			string condition; 
			condition.reserve(1000);
			cin.get();
			getline(cin, condition, '\n');

			query(condition);

		}
        cout << flush;
	}


	return 0;
}