// first.cpp

/*    Copyright 2009 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * this is a good first example of how to use mongo from c++
 */

// It is the responsibility of the mongo client consumer to ensure that any necessary windows
// headers have already been included before including the driver facade headers.
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include "mongo/client/dbclient.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "insert.h"

using namespace std;
using namespace mongo;
void rand_init(){
	srand(time(NULL));
}
int rand_int(int lo,int hi){
	return rand()%(hi-lo)+lo;
}
string rand_str(int len_lo,int len_hi){
	string res;
	int len=rand()%(len_hi-len_lo)+len_lo;
	for(int i=0;i<len;i++)
		res+=rand()%26+'a';	
	return res;
}

void insert( mongo::DBClientConnection & conn, const string &db_name,const int n){ 
	clock_t t1,t2;
	t1=clock();
	cout<<"Begin to insert records"<<endl;
	rand_init();
	for(int i=0;i<n;i++){
		cout<<"Inserting #"<<i<<"..."<<endl;
		//very weird. 
		//If put this variable declaration ahead of for loop
		//program will crash with seg fault
		//There is no reason. Really strange
    	mongo::BSONObjBuilder obj;

		string field_year("year");
		int value_year=rand_int(1900,2014);;
		obj.append(field_year,value_year);

		string field_name("name");
		string value_name=rand_str(5,15);
		obj.append(field_name,value_name);

		string field_des("description");
		string value_des=rand_str(10,50);
		obj.append(field_des,value_des);
		
		string field_id("id");
		int value_id=rand_int(0,100000);;
		obj.append(field_id,value_id);

		string field_title("title");
		string value_title=rand_str(10,20);
		obj.append(field_title,value_title);
		
		string field_review("review");
		string value_review=rand_str(10,150);
		obj.append(field_review,value_review);
		
    	conn.insert( db_name , obj.obj() );
	}
	t2=clock();
	cout<<"Inserting Time="<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;
    std::string e = conn.getLastError();
   	if( !e.empty() ) {
   		cout << "insert failed: " << e << endl;
      	exit(EXIT_FAILURE);
    }
}

void query_all(mongo::DBClientConnection &conn,const string &fu_name){
	mongo::BSONObjBuilder query;
   	std::auto_ptr<mongo::DBClientCursor> cursor = conn.query( fu_name, query.obj() );
    if (!cursor.get()) {
   		cout << "query failure" << endl;
        return ;
    }

    cout << "Query All Records To Disk"<< endl;
	FILE *f=fopen("db","w");
    while ( cursor->more() ) {
    	mongo::BSONObj obj = cursor->next();
		fprintf(f,"%s\n",obj.jsonString().c_str());
        //cout << "\t" << obj.jsonString() << endl;
    }
	fclose(f);
}

template <class T>
void query_record(mongo::DBClientConnection &conn, const string &fu_name, const string &field_name, const T &field_value){
	clock_t t1,t2;
	cout<<"Single Record Find"<<endl;
	cout<<"Field_Name :"<<field_name<<endl;
	cout<<"Field_Value:"<<field_value<<endl;
	t1=clock();

	mongo::BSONObjBuilder query;
    query.append( field_name, field_value);
   	std::auto_ptr<mongo::DBClientCursor> cursor = conn.query( fu_name, query.obj() );
    if (!cursor.get()) {
   		cout << "query failure" << endl;
        return ;
    }

	int count=0;
    while ( cursor->more() ) {
    	mongo::BSONObj obj = cursor->next();
		count++;
        //cout << "\t" << obj.jsonString() << endl;
    }
	t2=clock();
	cout<<"Count="<<count<<endl;
	cout<<"Searching Time="<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;

}

int main( int argc, const char **argv ) {

    const char *port = "27017";
    if ( argc != 1 ) {
        if ( argc != 3 ) {
            cout << "need to pass port as second param" << endl;
            return EXIT_FAILURE;
        }
        port = argv[ 2 ];
    }

    mongo::Status status = mongo::client::initialize();
    if ( !status.isOK() ) {
        std::cout << "failed to initialize the client driver: " << status.toString() << endl;
        return EXIT_FAILURE;
    }

    mongo::DBClientConnection conn;
    string errmsg;
    if ( ! conn.connect( string( "127.0.0.1:" ) + port , errmsg ) ) {
        cout << "couldn't connect : " << errmsg << endl;
        return EXIT_FAILURE;
    }

	cout<<"MongoDB Connected"<<endl;

	if(clear_previous_data){
        mongo::BSONObjBuilder query;
		conn.remove(fu_name,query.obj());
		cout<<"Current DB Record num: "<<conn.count(fu_name)<<endl;
    }


	if(insert_records){
		cout<<"Insert Records"<<endl;
    	insert( conn , fu_name,insert_record_num);
		cout<<"Insert Completed"<<endl;
		cout<<"Current DB Record num: "<<conn.count(fu_name)<<endl;
	}

	if(print_all_the_record)
		query_all(conn,fu_name);
	if(do_single_query){
		string field_name;
		string value;
		int value2;
		while(1){
		cout<<"Input field name"<<endl;
		cin>>field_name;
		if(field_name=="quit")
			break;
		cout<<"Input value:(int or string, value)"<<endl;
		int option;
		cin>>option;
		if(option==0){
			cin>>value2;
			query_record(conn,fu_name,field_name,value2);
		}else{
			cin>>value;
			query_record(conn,fu_name,field_name,value);
		}
		}
	}

    return EXIT_SUCCESS;
}
