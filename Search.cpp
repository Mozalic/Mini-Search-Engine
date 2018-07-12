#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <map>
#include <vector>
#include <string>

using namespace std;

struct item{
	string key; 
	int id,postion;
};

#define Stop_Word_Threshold 20
/*If a word appear more than Stop_Word_Threshold times,
 we regard it as stop world.*/

map <string,vector <item> >dict;
//dict is used as words storer
map <string,bool> stopword; 
//stopword store the list of stop word. 

int id,Docn;
//Docn means the number of Documents.

void insert(string now,string key,int id,int pos){
	//Add elements into inverted file index. 
	dict[now].push_back((item){key,id,pos});
	if (dict[now].size()>Docn*Stop_Word_Threshold) {
		/*If a word appear more than Stop_Word_Threshold times,
		 we regard it as stop world.
		 And remove it from inverted file index*/
		stopword[now]=1;
		dict[now].clear();
		return ;
	}
}

bool isalpha(char c){
	//Check if the char is a letter.
	if ((c>='A'&&c<='Z')||(c>='a'&&c<='z'))return 1;
	else return 0; 
}

string getstem(string now){
	int len=now.length();
	for (int i=0;i<len;i++){
		//Change letter into lowercase.
		if (now[i]>='A'&&now[i]<='Z'){
			now[i]-='A';
			now[i]+='a';
		}
	}
	if (len<4)return now;
	if (now[len-1]=='d'&&now[len-2]=='e'){
		//Check if is a -ed word
		now.erase(len-2);
		if (now[len-3]=='i')now[len-3]='y';
		return now;
	}
	if (now[len-1]=='g'&&
		now[len-2]=='n'&&
		now[len-3]=='i'){
		//Check if is a -ing word
		now.erase(len-3);
		return now;
	}
	if (now[len-1]=='s'&&
		now[len-2]=='e'){
		if (now[len-3]=='x'||now[len-3]=='z'||now[len-3]=='s'||
			(now[len-3]=='h'&&
				(now[len-4]=='c'||now[len-4]=='s')
			)
		)
		//Check if is a -es word
		now.erase(len-2);
		return now;
	}
	if (now[len-1]=='s'&&
		now[len-2]!='s'){
		//Check if is a -s word
		now.erase(len-1);
		return now;	
	}
	return now;
}

void query(string now){
	if (stopword[now]){
		//Check if the word is stop word. 
		printf("The word you ask for is stop word.\n");
		return;
	}
	string stem=getstem(now);
	if (dict[stem].size()==0){
		puts("The word you query for does not found.");
		return;
	}
	for (int i=0;i<dict[stem].size();i++){
		//reply for querys. 
		cout<<"Orign Word= "<<dict[stem][i].key;
		printf(" Documents ID = %d Postion of the word = %d\n",
				dict[stem][i].id,
				dict[stem][i].postion);
	}
}

void Init(){
	char c;
	int len=0,pos=1;
	string now;
	now.clear();
	while (scanf("%c",&c)!=EOF){
		if (!isalpha(c)){
			//if a word is end, add it to inverted file index.
			if (now.empty())continue;
			//get the stemming.
			string stem=getstem(now);
			if (!stopword[stem])
				insert(stem,now,id,pos);
			pos++;
			//Clear word stored in now.
			now.clear();
			continue;
		}
		else {
			now.push_back(c);
		}
	}
}

void read(){
	for (int i=1;i<=Docn;i++){
		string DocName;
		int tmp=i;
		while (tmp){
			DocName.insert((string::size_type)0,1,'0'+tmp%10);
			tmp/=10;
		}
		DocName+=".txt";
		//Create document name
		freopen(DocName.c_str(),"r",stdin);
		id=i;
		Init();
		freopen("CON", "r", stdin);
	}
	//Redirect input into console.
}

int main(){
	cout<<"Let me know the number of files to be searched:";
	cin>>Docn;
	read();
	string que;
	//while (cin>>que)cout<<getstem(que)<<endl;
	while (cin>>que)query(que);
}
