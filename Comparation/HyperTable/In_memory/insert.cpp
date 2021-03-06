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



vector<ThriftGen::Cell> gen_cells(){
	vector<ThriftGen::Cell>res;
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
		ThriftGen::Cell cell;
		cell.key.__set_timestamp(AUTO_ASSIGN);
		cell.key.__set_flag(ThriftGen::KeyFlag::INSERT);
		
		getline(file,str);//MID
		string MID=str;
		cell.key.__set_row(MID);

		getline(file,str);
		while(str!="END RECORD"){
			string field_name=str;
			getline(file,str);
			string field_value=str;
			if(field_name=="genre"){
				vector<string>genres=parseGenre(field_value);
				for(int i=0;i<genres.size();i++){
					cell.key.__set_column_family("fast");
					cell.key.__set_column_qualifier("genre");
					cell.__set_value(genres[i]);
					res.push_back(cell);
				}
			}else if(field_name=="cast"){
				vector<string>casts=parseCast(field_value);
				for(int i=0;i<casts.size();i++){
					cell.key.__set_column_family("fast");
					cell.key.__set_column_qualifier("cast");
					cell.__set_value(casts[i]);
					res.push_back(cell);
				}
			}else if(field_name=="title"){
				cell.key.__set_column_family("fast");
				cell.key.__set_column_qualifier("title");
				cell.__set_value(field_value);
				res.push_back(cell);
			}else{
				cell.key.__set_column_family("normal");
				cell.key.__set_column_qualifier(field_name);
				cell.__set_value(field_value);
				res.push_back(cell);
			}
			getline(file,str);
		}
	}
	file.close();
	return res;
}

int main(){
	Thrift::Client *client = nullptr;

	try {
		client = new Thrift::Client("localhost", 15867);

  		if (!client->namespace_exists("test"))
			client->namespace_create("test");

		ThriftGen::Namespace ns = client->namespace_open("test");


		ThriftGen::Schema schema;

		  map<string, ThriftGen::AccessGroupSpec> ag_specs;
		  map<string, ThriftGen::ColumnFamilySpec> cf_specs;

		  // Set table defaults
		  {
		    ThriftGen::AccessGroupOptions ag_options;
		    ThriftGen::ColumnFamilyOptions cf_options;
		    ag_options.__set_blocksize(65536);
		    schema.__set_access_group_defaults(ag_options);
		    cf_options.__set_max_versions(64);
		    schema.__set_column_family_defaults(cf_options);
		  }

		  // Access group "ag_normal"
		  {
		    ThriftGen::AccessGroupSpec ag;
		    ThriftGen::ColumnFamilyOptions cf_options;
		    cf_options.__set_max_versions(2);
		    ag.__set_name("ag_normal");
		    ag.__set_defaults(cf_options);
		    ag_specs["ag_normal"] = ag;
		  }

		  // Column "normal"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    cf.__set_name("normal");
		    cf.__set_access_group("ag_normal");
		    cf.__set_value_index(true);
		    cf.__set_qualifier_index(true);
		    cf_specs["normal"] = cf;
		  }
		  // Access group "ag_fast"
		  {
		    ThriftGen::AccessGroupSpec ag;
		    ThriftGen::AccessGroupOptions ag_options;
		    ag.__set_name("ag_fast");
		    ag_options.__set_in_memory(true);
		    ag_options.__set_blocksize(131072);
		    ag.__set_options(ag_options);
		    ag_specs["ag_fast"] = ag;
		  }

		  // Column "fast"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    cf.__set_name("fast");
		    cf.__set_access_group("ag_fast");
		    cf.__set_value_index(true);
		    cf_specs["fast"] = cf;
		  }


		  schema.__set_access_groups(ag_specs);
		  schema.__set_column_families(cf_specs);

		  if(client->table_exists(ns,"tt"))
			  client->table_drop(ns,"tt",false);

			  client->table_create(ns, "tt", schema);
/*
		  HqlResult result,r;
		  client->hql_query(result, ns, "SHOW CREATE TABLE TestTable");

		    if (!result.results.empty())
				    cout << result.results[0] << endl;
			cout<<"HHHHHHHHH\n";
*/

				  
		  vector<ThriftGen::Cell> cells=gen_cells();
		
		cout<<"Start Insert\n";
		cout<<"Total cells: "<<cells.size()<<endl;
		  ThriftGen::Mutator mutator = client->mutator_open(ns, "tt", 0, 0);
		  client->mutator_set_cells(mutator, cells);
		  client->mutator_flush(mutator);
    	    	  client->mutator_close(mutator);
		  //client->set_cells(ns, "TestTable", cells);
		cout<<"End Insert\n";
		  
		  client->namespace_close(ns);

	}catch (ClientException &e) {
		cout << e.message << endl;
		exit(1);
	}
}
