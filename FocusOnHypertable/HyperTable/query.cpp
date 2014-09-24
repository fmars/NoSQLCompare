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
string rand_str(int len_lo,int len_hi){
	string res;
	int len=rand()%(len_hi-len_lo)+len_lo;
	for(int i=0;i<len;i++)
		res+=rand()%26+'a';	
	return res;
}
int insert_size = 1000000;
int main(){
	Thrift::Client *client = nullptr;

	try {
		client = new Thrift::Client("localhost", 15867);

  		if (!client->namespace_exists("test"))
			client->namespace_create("test");

		ThriftGen::Namespace ns = client->namespace_open("test");
		
		string sql;
		cout<<"INTPUT HQL\n";
		getline(cin,sql);
		while(sql!="quit"){
			HqlResult result;
			clock_t t1,t2;
			t1=clock();
			client->hql_query(result,ns,sql.c_str());
			t2=clock();
			cout<<"TIME"<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;
			cout<<"INTPUT HQL\n";
			getline(cin,sql);
		}


		  client->namespace_close(ns);

	}catch (ClientException &e) {
		cout << e.message << endl;
		exit(1);
	}
}
