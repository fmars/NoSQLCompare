#include <Common/Compat.h>
#include <Common/Logger.h>
#include <Common/System.h>

#include <Hypertable/Lib/Key.h>
#include <Hypertable/Lib/KeySpec.h>

#include <ThriftBroker/Client.h>
#include <ThriftBroker/gen-cpp/Client_types.h>
#include <ThriftBroker/gen-cpp/HqlService.h>
#include <ThriftBroker/ThriftHelper.h>
#include <ThriftBroker/SerializedCellsReader.h>
#include <ThriftBroker/SerializedCellsWriter.h>

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace Hypertable;
using namespace Hypertable::ThriftGen;
using namespace std;

vector<string> parseCast(string &value){
	value+=",";
	vector<string>res;
	string tmp;
	int p=0;
	for(int i=0;i<value.size();i++){
		if(value[i]==','){
			tmp=value.substr(p,i-p);
			while(tmp.size()>0 && tmp[0]==' ')
				tmp=tmp.substr(1,tmp.size()-1);
			while(tmp.size()>0 && tmp[tmp.size()-1]==' ')
				tmp=tmp.substr(0,tmp.size()-1);
			p=i+1;
			res.push_back(tmp);
		}
	}
	return res;
}
vector<string> get_test_source(string field){
	vector<string>res;
	ifstream file("parsedData");
	string str;
	if(!file.is_open()){
		cout<<"Open File Failed\n";
		return res;
	}
	getline(file,str);
	while(getline(file,str)){
		if(str!="NEW RECORD"){
			cout<<"parse error\n";
			return res;
		}
		getline(file,str);//MID
		getline(file,str);
		while(str!="END RECORD"){
			string field_name=str;
			getline(file,str);
			string field_value=str;
			if(field_name==field)
				res.push_back(field_value);
			getline(file,str);
		}
	}
	file.close();
	return res;
}

void query_test_title(Thrift::Client *client,ThriftGen::Namespace &ns, const string &table_name){
	cout<<"BEGIN to test on title\n";
	vector<string>titles=get_test_source("title");
	//avoid HQL parse error
	for(int i=0;i<titles.size();i++)
		for(int j=0;j<titles[i].size();j++)
			if(titles[i][j]=='\"')
				titles[i][j]=' ';
	clock_t t1,t2;
	t1=clock();
	int count=0;
	for(int i=0;i<titles.size();i++){
		cout<<i<<"/"<<titles.size()<<endl;
		string sql="select * from "+table_name+" where fast:title=\""+titles[i]+"\"";
		HqlResult result;
		client->hql_query(result,ns,sql.c_str());
	}
	t2=clock();
	cout<<"Total Query Item: "<<titles.size()<<endl;
	cout<<"Total Query Time="<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;
	cout<<"Each Query Time= "<<((float)t2-(float)t1)/CLOCKS_PER_SEC/titles.size()<<endl;
	cout<<"END to test on title\n";
}
void query_test_cast(Thrift::Client *client,ThriftGen::Namespace &ns, const string &table_name){
	cout<<"BEGIN to test on cast\n";
	vector<string>cast_lines=get_test_source("cast");
	vector<string>casts;
	for(int i=0;i<cast_lines.size();i++){
		vector<string>tmp=parseCast(cast_lines[i]);
		casts.insert(casts.begin(),tmp.begin(),tmp.end());
	}
	//avoid HQL parse error
	for(int i=0;i<casts.size();i++)
		for(int j=0;j<casts[i].size();j++)
			if(casts[i][j]=='\"')
				casts[i][j]=' ';
	clock_t t1,t2;
	t1=clock();
	int count=0;
	for(int i=0;i<casts.size();i++){
		cout<<i<<"/"<<casts.size()<<endl;
		string sql="select * from "+table_name+" where fast:cast=\""+casts[i]+"\"";
		HqlResult result;
		client->hql_query(result,ns,sql.c_str());
	}
	t2=clock();
	cout<<"Total Query Item: "<<casts.size()<<endl;
	cout<<"Total Query Time="<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;
	cout<<"Each Query Time= "<<((float)t2-(float)t1)/CLOCKS_PER_SEC/casts.size()<<endl;
	cout<<"END to test on cast\n";
}
void query_test_genre(Thrift::Client *client,ThriftGen::Namespace &ns, const string &table_name){
	cout<<"BEGIN to test on genre\n";
	vector<string>genres;
	genres.push_back("Comedy");
	genres.push_back("Drama");
	genres.push_back("Action");
	genres.push_back("Crime");
	genres.push_back("Fantasy");
	genres.push_back("Romance");
	genres.push_back("Thriller");
	genres.push_back("Musical");
	genres.push_back("Biography");
	genres.push_back("Animation");
	clock_t t1,t2;
	t1=clock();
	int count=0;
	for(int i=0;i<genres.size();i++){
		cout<<i<<"/"<<genres.size()<<endl;
		string sql="select * from "+table_name+" where fast:genre=\""+genres[i]+"\"";
		HqlResult result;
		client->hql_query(result,ns,sql.c_str());
	}
	t2=clock();
	cout<<"Total Query Item: "<<genres.size()<<endl;
	cout<<"Total Query Time="<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;
	cout<<"Each Query Time= "<<((float)t2-(float)t1)/CLOCKS_PER_SEC/genres.size()<<endl;
	cout<<"END to test on genre\n";
}

int main(){
	Thrift::Client *client = nullptr;

	try {
		client = new Thrift::Client("localhost", 15867);

  		if (!client->namespace_exists("test"))
			client->namespace_create("test");

		ThriftGen::Namespace ns = client->namespace_open("test");
		String table_name("tt_d");
		query_test_title(client,ns,table_name);
		query_test_genre(client,ns,table_name);
		query_test_cast(client,ns,table_name);	
			
		client->namespace_close(ns);

	}catch (ClientException &e) {
		cout << e.message << endl;
		exit(1);
	}
}
