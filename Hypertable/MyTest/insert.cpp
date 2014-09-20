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


		ThriftGen::Schema schema;

		  map<string, ThriftGen::AccessGroupSpec> ag_specs;
		  map<string, ThriftGen::ColumnFamilySpec> cf_specs;

		  // Set table defaults
		  {
		    ThriftGen::AccessGroupOptions ag_options;
		    ThriftGen::ColumnFamilyOptions cf_options;
		    ag_options.__set_blocksize(65536);
		    schema.__set_access_group_defaults(ag_options);
		    cf_options.__set_max_versions(1);
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

		  // Column "a"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    cf.__set_name("a");
		    cf.__set_access_group("ag_normal");
		    cf.__set_value_index(true);
		    cf.__set_qualifier_index(true);
		    cf_specs["a"] = cf;
		  }

		  // Column "b"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    ThriftGen::ColumnFamilyOptions cf_options;
		    cf.__set_name("b");
		    cf.__set_access_group("ag_normal");
		    cf_options.__set_max_versions(3);
		    cf.__set_options(cf_options);
		    cf_specs["b"] = cf;
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

		  // Column "c"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    cf.__set_name("c");
		    cf.__set_access_group("ag_fast");
		    cf_specs["c"] = cf;
		  }

		  // Access group "ag_secure"
		  {
		    ThriftGen::AccessGroupSpec ag;
		    ThriftGen::AccessGroupOptions ag_options;
		    ag.__set_name("ag_secure");
		    ag_options.__set_replication(5);
		    ag.__set_options(ag_options);
		    ag_specs["ag_secure"] = ag;
		  }

		  // Column "d"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    cf.__set_name("d");
		    cf.__set_access_group("ag_secure");
		    cf_specs["d"] = cf;
		  }

		  // Access group "ag_counter"
		  {
		    ThriftGen::AccessGroupSpec ag;
		    ThriftGen::ColumnFamilyOptions cf_options;
		    ag.__set_name("ag_counter");
		    cf_options.__set_counter(true);
		    cf_options.__set_max_versions(0);
		    ag.__set_defaults(cf_options);
		    ag_specs["ag_counter"] = ag;
		  }

		  // Column "e"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    cf.__set_name("e");
		    cf.__set_access_group("ag_counter");
		    cf_specs["e"] = cf;
		  }

		  // Column "f"
		  {
		    ThriftGen::ColumnFamilySpec cf;
		    ThriftGen::ColumnFamilyOptions cf_options;
		    cf.__set_name("f");
		    cf.__set_access_group("ag_counter");
		    cf_options.__set_counter(false);
		    cf.__set_options(cf_options);
		    cf_specs["f"] = cf;
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

				  
		  vector<ThriftGen::Cell> cells;


		  FILE*file=fopen("mydb","w");

		for(int i=0;i<insert_size;i++){
			string name=rand_str(5,10);
			string v1=rand_str(60,65),v2=rand_str(60,65),v3=rand_str(60,65);
		  ThriftGen::Cell cell1,cell2,cell3;
		  cell1.key.__set_timestamp(AUTO_ASSIGN);
		  cell1.key.__set_flag(ThriftGen::KeyFlag::INSERT);
		  cell2.key.__set_timestamp(AUTO_ASSIGN);
		  cell2.key.__set_flag(ThriftGen::KeyFlag::INSERT);
		  cell3.key.__set_timestamp(AUTO_ASSIGN);
		  cell3.key.__set_flag(ThriftGen::KeyFlag::INSERT);

		
		  cell1.key.__set_row(name.c_str());
		  cell1.key.__set_column_family("a");
		  cell1.__set_value(v1.c_str());
		  cells.push_back(cell1);

		  cell2.key.__set_row(name.c_str());
		  cell2.key.__set_column_family("b");
		  cell2.key.__set_column_qualifier("b_sub");
		  cell2.__set_value(v2.c_str());
		  cells.push_back(cell2);

		  cell3.key.__set_row(name.c_str());
		  cell3.key.__set_column_family("c");
		  cell3.key.__set_column_qualifier("crunchy");
		  cell3.__set_value(v3.c_str());
		  cells.push_back(cell3);

			fprintf(file,"%s a %s\n",name.c_str(),v1.c_str());
			fprintf(file,"%s b b_sub %s\n",name.c_str(),v2.c_str());
			fprintf(file,"%s c crunchy %s\n",name.c_str(),v3.c_str());
		}
		fclose(file);
		  cout<<"BB\n";
		  ThriftGen::Mutator mutator = client->mutator_open(ns, "tt", 0, 0);
		  client->mutator_set_cells(mutator, cells);
		  client->mutator_flush(mutator);
	    	client->mutator_close(mutator);
		  //client->set_cells(ns, "TestTable", cells);
		  cout<<"EE\n";


		  client->namespace_close(ns);

	}catch (ClientException &e) {
		cout << e.message << endl;
		exit(1);
	}
}
