/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2000, 2014 Oracle and/or its affiliates.  All rights reserved.
 *
 * $Id$
 */

/*
 * Do some regression tests for constructors.
 * Run normally (without arguments) it is a simple regression test.
 * Run with a numeric argument, it repeats the regression a number
 * of times, to try to determine if there are memory leaks.
 */

#include <db_cxx.h>
#include <string>
#include <set>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

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

int num_record=100000;

int main(int argc, char *argv[])
{
	try {
		Db *db = new Db(NULL, 0);
		db->open(NULL, "my.db", NULL, DB_BTREE, DB_CREATE, 0);

		// populate our massive database.
		// all our strings include null for convenience.
		// Note we have to cast for idiomatic
		// usage, since newer gcc requires it.
		set<string> hash;
		rand_init();
		FILE*file=fopen("mydb","w");

		for(int i=0;i<num_record;i++){
			string str_key=rand_str(5,15);
			while(hash.count(str_key)!=0)
				str_key=rand_str(5,15);
			hash.insert(str_key);
			Dbt *keydbt = new Dbt((char *)str_key.c_str(), str_key.size()+1);
			string str_data=rand_str(150,250);
			Dbt *datadbt = new Dbt((char *)str_data.c_str(), str_data.size()+1);
			db->put(NULL, keydbt, datadbt, 0);
			fprintf(file,"%d\n%s\n%s\n",i,str_key.c_str(),str_data.c_str());

		}
		fclose(file);

		// Now, retrieve.  We could use keydbt over again,
		// but that wouldn't be typical in an application.
		//Dbt *goodkeydbt = new Dbt((char *)"key", 4);
		//Dbt *badkeydbt = new Dbt((char *)"badkey", 7);
		set<string>::iterator it=hash.begin();
		string str_query=*it;
		Dbt *searchdbt = new Dbt((char*)str_query.c_str(),str_query.size()+1);
		Dbt *resultdbt = new Dbt();
		resultdbt->set_flags(DB_DBT_MALLOC);
	
		int ret;
		if ((ret = db->get(NULL, searchdbt, resultdbt, 0)) != 0) {
			cout << "get: " << DbEnv::strerror(ret) << "\n";
		}
		else {
			char *result = (char *)resultdbt->get_data();
			cout <<str_query<<endl;
			cout << "got data: " << result << "\n";
		}
		db->close(0);
	}catch (DbException &dbe) {
	cerr << "Db Exception: " << dbe.what();
	}


/*
		int ret;

		if ((ret = db->get(NULL, goodkeydbt, resultdbt, 0)) != 0) {
			cout << "get: " << DbEnv::strerror(ret) << "\n";
		}
		else {
			char *result = (char *)resultdbt->get_data();
			cout << "got data: " << result << "\n";
		}

		if ((ret = db->get(NULL, badkeydbt, resultdbt, 0)) != 0) {
			// We expect this...
			cout << "get using bad key: "
			     << DbEnv::strerror(ret) << "\n";
		}
		else {
			char *result = (char *)resultdbt->get_data();
			cout << "*** got data using bad key!!: "
			     << result << "\n";
		}
		cout << "finished test\n";
	}
	catch (DbException &dbe) {
		cerr << "Db Exception: " << dbe.what();
	}
	*/
	return 0;
}
