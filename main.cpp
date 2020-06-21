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

void query_from_to(string name, vector<int> allID, string test){
	//cout<<name<<" "<<test<<endl;
	return;
}
void query_date(int64_t &date1, int64_t &date2, vector<int> allID){
	//cout<<date1<<" "<<date2<<endl;
	return;
}
void query_expression(string& expressions, vector<int> allID){
	//cout<<expressions<<endl;
	return;
}
void query(string &conditions){
	vector<int> allID;
	string from="0", to="1";
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
    			query_from_to(token, allID, from);
    			extra=2+token.length();
    		}else if(token[1]=='t'){
    			token.erase(0,2);
    			query_from_to(token, allID, to);
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
    			query_date(num1, num2, allID);
    		}
    	}else{
    		extra=token.length();
    		query_expression(token, allID);
		}
    	conditions.erase(0, delimiter.length()+ extra);
	}
	if(conditions[0]=='-'){ // case -f -t -d
    	if(conditions[1]=='f'){
    		conditions.erase(0,2); // erase -f
    		query_from_to(conditions, allID, from);
    	}else if(conditions[1]=='t'){
    		conditions.erase(0,2);
    		query_from_to(conditions, allID, to);
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
    		query_date(num1, num2, allID);
    	}
    }else{
    	query_expression(conditions, allID);
	}
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
			query(q);
		}
		
	}
	return 0;
}
