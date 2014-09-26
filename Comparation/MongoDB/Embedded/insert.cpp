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

vector<string> parseGenre(string &value){
	value+="/";
	vector<string>res;
	string tmp;
	int p=0;
	for(int i=0;i<value.size();i++){
		if(value[i]=='/'){
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


void insert( mongo::DBClientConnection & conn, const string &db_name){ 
	cout<<"Begin to insert records"<<endl;
	ifstream file("parsedData");
	string str;
	if(!file.is_open()){
		cout<<"Open File Failed\n";
		return;
	}
	getline(file,str);
	while(getline(file,str)){
    	mongo::BSONObjBuilder obj;
		if(str!="NEW RECORD"){
			cout<<"parse error\n";
			return;
		}
		getline(file,str);//MID
		getline(file,str);
		while(str!="END RECORD"){
			string field_name=str;
			getline(file,str);
			string field_value=str;
			if(field_name=="genre"){
				vector<string>genres=parseGenre(field_value);
				obj.append(field_name,genres);
			}else if(field_name=="cast"){
				vector<string>casts=parseCast(field_value);
				obj.append(field_name,casts);
			}else{
				obj.append(field_name,field_value);
			}
			getline(file,str);
		}
    	conn.insert( db_name , obj.obj() );
	}
	file.close();
    std::string e = conn.getLastError();
   	if( !e.empty() ) {
   		cout << "insert failed: " << e << endl;
      	exit(EXIT_FAILURE);
    }
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
    	insert( conn , fu_name );
		cout<<"Insert Completed"<<endl;
		cout<<"Current DB Record num: "<<conn.count(fu_name)<<endl;
	}

    return EXIT_SUCCESS;
}
