#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

 const int MAXN = 100000;
// Some global variable
 char buffer[MAXN];
inline bitset<10010> record;

// Define Conversion of monvth and its value
inline unordered_map <string, int> months{
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

inline unordered_map <char, int> precedence_order{
	{'!', 1},
	{'&', 2},
	{'|', 3},
	{'(', 4}
};

// Message-ID as the key
int total_mail_in_database;
bitset <10010> exist_mail_id;

gp_hash_table <string, int> route_and_id;

// Length as the key, Message-ID as the value
map <int, set <int> > length_of_mail;

// Date as the key, Set of Message-ID as the value
map <int64_t, set <int> > date;

// Name/Keyword as the key, Set of Message-ID as the value
vector <string> from(10005);
vector <string> to(10005);
gp_hash_table <string, bitset < 10010 > > keyword;

// Global Variable to save execution time
inline string word(60, 0);
inline string sender(60, 0);
inline string reciever(60, 0);
inline int64_t timestamp;
inline int ID;
inline vector <string> postfix(60);
inline int postfix_size;
inline vector < bitset<10010> > numerator(100);

void add(string &route){

	// Read file into buffer
	ifstream file(route.c_str(), ios::binary | ios::ate);
	size_t length = file.tellg();
	file.seekg(0, ios::beg);
	file.read(buffer, length);
	buffer[length] = '\0';
    
	
	// Parse From:
	int idx = 0, shift = 6;
	while(buffer[shift + idx] != '\n'){
		sender[idx] = tolower(buffer[shift + idx]);
		idx++;
	}
	sender.resize(idx);
	shift += idx + 7;
	
	// Calculate Time
	if(buffer[shift + 1] == ' '){
		timestamp = 10000 * (buffer[shift] - '0');
		shift += 2;
	}
	else{
		timestamp = 100000 * (buffer[shift] - '0') + 10000 * (buffer[shift + 1] - '0');
		shift += 3;
	}
	idx = 0;
	string month(12, 0);
	while(buffer[shift + idx] != ' '){
		month[idx] = buffer[shift + idx];
		idx++;
	}
	month.resize(idx);
	shift += idx + 1;
	timestamp += 100000000000ll * (int64_t)(buffer[shift] - '0') + 10000000000ll * (buffer[shift + 1] - '0') + 1000000000ll * (buffer[shift + 2] - '0') + 100000000ll * (buffer[shift + 3] - '0') +\
	            1000 * (buffer[shift + 8] - '0') + 100 * (buffer[shift + 9] - '0') + 10 * (buffer[shift + 11] - '0') + (buffer[shift + 12] - '0') + months[month] * 1000000ll;
	shift += 26;

	// Parse ID:
	idx = 0;
	while(buffer[shift + idx] != '\n'){
		ID = ID * 10 + (buffer[shift + idx] - '0');
		idx++;
	}
	shift += idx + 10;

	// Parse Subject:
	idx = 0; 
	while(buffer[shift + idx] != '\n'){
		if(!isalnum(buffer[shift + idx])){
			
			if(idx != 0){
				word.resize(idx);
				keyword[word].set(ID);
				word.resize(60);
			}
			shift += idx + 1;
			idx = -1;
		}else
			word[idx] = tolower(buffer[shift + idx]);

		idx++;
	}
	if(idx != 0){
		word.resize(idx);
		keyword[word].set(ID);
		word.resize(60);
	}
	shift += idx + 5;

	// Find To:
	idx = 0;
	while(buffer[shift + idx] != '\n'){
		reciever[idx] = tolower(buffer[shift + idx]);
		idx++;
	}
	reciever.resize(idx);
	shift += idx + 10;

	// Parse Content:
	idx = 0; int total_length = 0;
	while(buffer[shift + idx] != '\0'){
		if(!isalnum(buffer[shift + idx])){
			
			if(idx != 0){
				word.resize(idx);
				keyword[word].set(ID);
				word.resize(60);
			}
			shift += idx + 1;
			idx = -1;
		}else{
			total_length++;
			word[idx] = tolower(buffer[shift + idx]);
		}

		idx++;
	}
	if(idx != 0){
		word.resize(idx);
		keyword[word].set(ID);
	}
	
	// Update Parsed data and clear the variables
	to[ID] = reciever;
	from[ID] = sender;
	date[timestamp].insert(ID);
	exist_mail_id.set(ID);
	route_and_id[route] = ID;
	total_mail_in_database++;
	length_of_mail[total_length].insert(ID);

	printf("%d\n", total_mail_in_database);
	timestamp = 0;
	ID = 0;
	sender.resize(60);
	reciever.resize(60);
	word.resize(60);

	return;
}

inline void remove(int &id){

	// remove message-id from exists list but don't erase from processed mail-id list
	if(exist_mail_id.test(id)){
		total_mail_in_database--;
		printf("%d\n", total_mail_in_database);
		exist_mail_id.reset(id);
	}else
		puts("-");

	return;
}

inline void longest(){	
	
	// Brute force search for longest mail
	for(auto ptr = length_of_mail.rbegin(); ptr != length_of_mail.rend(); ptr++){
        for(auto c : ptr->second)
        	if(exist_mail_id.test(c)){
        		printf("%d %d\n", c, ptr->first);
        		return;
        	}

    }
	puts("-");
	return;
}

// Source indicate From or To 
inline void query_from_to(string &name, bitset <10010> &result, vector <string> &source){

	// Simply brute force search
	record.reset();
	for(int i = result._Find_first(); i <= 10000; i = result._Find_next(i)){
		if(source[i] == name)
			record.set(i);
	}

	result = record;
	return;
}

// Convert Infix to Postfix order
void infix_to_postfix(string &expression){
	postfix_size = 0;

	// Stack to store operator
	stack <char> operator_stack;
	int length = expression.length();
	for(int i = 0; i < length; i++){

		// Find encounter operator, pop out those with higher precedence
		if(!isalnum(expression[i])){
			if(expression[i] == '(')
				operator_stack.push('(');
			else if(expression[i] == ')'){
				while(!operator_stack.empty() && operator_stack.top() != '('){
					string str(1, operator_stack.top());
					postfix[postfix_size++] = str;
					operator_stack.pop();
				}

				// Pop out left parenthesis
				if(!operator_stack.empty() && operator_stack.top() == '(')
					operator_stack.pop(); 
				
			}else{
				while(!operator_stack.empty() && precedence_order[expression[i]] >= precedence_order[operator_stack.top()]){
					string str(1, operator_stack.top());
					postfix[postfix_size++] = str;
					operator_stack.pop();
				}
				operator_stack.push(expression[i]);
			}
		}else{

			// If encounter string object
			string str(1, expression[i]);
			while(i + 1 < length && isalnum(expression[i + 1]))
				str += expression[++i];

			postfix[postfix_size++] = str;
		}
	}

	// Clear operator stack
	while(!operator_stack.empty()){
		string str(1, operator_stack.top());
		postfix[postfix_size++] = str;
		operator_stack.pop();
	}
	
	return;
}

// Find date between date1 and date2
inline void query_date(int64_t &date1, int64_t &date2, bitset<10010> &result){
    record.reset();
    auto ptr = date.lower_bound(date1);
    while(ptr != date.end() && ptr->first <= date2){
    	for(auto c : ptr->second)
    		record.set(c);
    	ptr++;
    }
    result &= record;
    return;
}

// Bitset operation and finding
void query_expression(string &condition, bitset <10010> &result){

	int total = 0;
	
	infix_to_postfix(condition);

	for(int i = 0; i < postfix_size; i++){
		auto c = postfix[i];
		if(c == "!"){
			numerator[total - 1] = exist_mail_id & (exist_mail_id ^ numerator[total - 1]);
		}else if(c == "&"){
			numerator[total - 2] = numerator[total - 1] & numerator[total - 2];
			total--;
		}else if(c == "|"){
			numerator[total - 2] = numerator[total - 1] | numerator[total - 2];
			total--;
		}else{
			numerator[total] = keyword[c];
			total++;
		}
	}

	result &= numerator[total - 1];
	return;
}

// Parse query [-f"[Name]"] [-t"[Name]"] [-d[time]~[time]] expression
void query(string &name){

	// result bitset to record current answer
    bitset<10010> result = exist_mail_id;
	int length = name.length();
    for(int i = 0 ; i < length; i ++){
		if(result.none())
			break;
		
		if(name[i] == '-' && name[i + 1] == 'f'){

			// Parse -f option
            int j = i + 3;
            string from_user;
			from_user.reserve(55);
        	while(name[j] != ' ')
                from_user += tolower(name[j++]);

        	from_user.pop_back();
            query_from_to(from_user, result, from);
            i = j;
        }else if(name[i] == '-' && name[i + 1] == 't'){

        	// Parse -t option
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

        	// Parse -d option
            int j = i + 2;
            int64_t date_int_1 = 0;
            int64_t date_int_2 = 0;

            if(name[j] == '~'){
            	j++;
            	for(int k = 0; k < 12; k++, j++)
            		date_int_2 = date_int_2 * 10 + (name[j] - '0');
            	date_int_1 = 0;
            }else if(isdigit(name[j + 13])){
            	for(int k = 0; k < 12; k++, j++)
            		date_int_1 = date_int_1 * 10 + (name[j] - '0');
            	j++;
            	for(int k = 0; k < 12; k++, j++)
            		date_int_2 = date_int_2 * 10 + (name[j] - '0');
            }else{
            	for(int k = 0; k < 12; k++, j++)
            		date_int_1 = date_int_1 * 10 + (name[j] - '0');
            	j++;
            	date_int_2 = 999999999999;
            }

            i = j;
            query_date(date_int_1, date_int_2, result); 
        }else{

        	// Parse expression
            string condition;
			condition.reserve(1200);
            while(i != length)
            	condition += tolower(name[i++]);
            query_expression(condition, result);

            break;
        }
    }

    // Output result of query
    if(result.none())
    	puts("-");
    else{
    	for(int i = result._Find_first(); i <= 10000; i = result._Find_next(i))
    		printf("%d ", i);
    	puts("");
    }
    return;
}

int main(){

	//vector <string> postfix(100);
	total_mail_in_database = 0;
	// reserve space from the data structure

	for(int i = 0; i < 100; i++)
		numerator.push_back(bitset <10010> (0));

	// Read instruction and execute
	char mode[15];
	while(scanf("%s", mode) == 1){

		// Only call add when never encounter, otherwise update exist list
		if(!strcmp(mode, "add")){
			string route; cin >> route; 

			// If processed
			if(route_and_id.find(route) != route_and_id.end()){
				if(exist_mail_id.test(route_and_id[route]))
					puts("-");
				else{
					total_mail_in_database++;
					printf("%d\n", total_mail_in_database);
					exist_mail_id.set(route_and_id[route]);
				}
			}	
			// Never Processed
			else add(route);

		}else if(!strcmp(mode, "longest")){
			if(exist_mail_id.none())
				puts("-");
			else
				longest();

		}else if(!strcmp(mode, "remove")){
			int id; scanf("%d", &id);
			remove(id);

		}else{

			// Read the space and parse expression
			getchar();
			string condition(1000, 0);
			int idx = 0;
			while((condition[idx] = getchar()) != '\n'){
				condition[idx] = tolower(condition[idx]);
				idx++;
			}
			condition.resize(idx);

			query(condition);
		}
	}

	return 0;
}
