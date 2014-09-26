#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include "mongo/client/dbclient.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "insert.h"

using namespace std;
using namespace mongo;

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
    	mongo::BSONObjBuilder obj;
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
template <class T>
int query_record(mongo::DBClientConnection &conn, const string &fu_name, const string &field_name, const T &field_value){

	mongo::BSONObjBuilder query;
    query.append( field_name, field_value);
   	std::auto_ptr<mongo::DBClientCursor> cursor = conn.query( fu_name, query.obj() );
    if (!cursor.get()) {
   		cout << "query failure" << endl;
        return 0;
    }

	int count=0;
    while ( cursor->more() ) {
    	mongo::BSONObj obj = cursor->next();
		count++;
        //cout << "\t" << obj.jsonString() << endl;
    }
	return count;

}
template <class T>
int query_record_regex(mongo::DBClientConnection &conn, const string &fu_name, const string &field_name, const T &field_value){

	mongo::BSONObjBuilder query;
	string regex=".*"+field_value+".*";
	query.appendRegex(field_name,regex);
   	std::auto_ptr<mongo::DBClientCursor> cursor = conn.query( fu_name, query.obj() );
    if (!cursor.get()) {
   		cout << "query failure" << endl;
        return 0;
    }

	int count=0;
    while ( cursor->more() ) {
    	mongo::BSONObj obj = cursor->next();
		count++;
        //cout << "\t" << obj.jsonString() << endl;
    }
	return count;

}
void query_test_title(mongo::DBClientConnection &conn, const string &fu_name){
	cout<<"BEGIN to test on title\n";
	vector<string>titles=get_test_source("title");
	clock_t t1,t2;
	t1=clock();
	int count=0;
	for(int i=0;i<titles.size();i++){
		cout<<i<<"/"<<titles.size()<<endl;
		int c=query_record(conn,fu_name,"title",titles[i]);
		cout<<titles[i]<<":"<<c<<endl;
		count+=c;
	}
	t2=clock();
	cout<<"Total Query Item: "<<titles.size()<<endl;
	cout<<"Total Result Ttem:"<<count<<endl;
	cout<<"Total Query Time="<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;
	cout<<"Each Query Time= "<<((float)t2-(float)t1)/CLOCKS_PER_SEC/titles.size()<<endl;
	cout<<"END to test on title\n";
}

void query_test_genre(mongo::DBClientConnection &conn, const string &fu_name){
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
		int c=query_record_regex(conn,fu_name,"genre",genres[i]);
		cout<<genres[i]<<":"<<c<<endl;
		count+=c;
	}
	t2=clock();
	cout<<"Total Query Item: "<<genres.size()<<endl;
	cout<<"Total Result Ttem:"<<count<<endl;
	cout<<"Total Query Time="<<((float)t2-(float)t1)/CLOCKS_PER_SEC<<endl;
	cout<<"Each Query Time= "<<((float)t2-(float)t1)/CLOCKS_PER_SEC/genres.size()<<endl;
	cout<<"END to test on genre\n";
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

	query_test_title(conn,fu_name);
	query_test_genre(conn,fu_name);


    return EXIT_SUCCESS;
}
