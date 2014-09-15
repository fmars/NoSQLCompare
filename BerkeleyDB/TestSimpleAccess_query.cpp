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





int main(int argc, char *argv[])
{
	try {
		Db *db = new Db(NULL, 0);
		db->open(NULL, "my.db", NULL, DB_BTREE, DB_CREATE, 0);

		// populate our massive database.
		// all our strings include null for convenience.
		// Note we have to cast for idiomatic
		// usage, since newer gcc requires 
		string str_query;//="aaaegll";
		cout<<"input query string"<<endl;
		cin>>str_query;
		Dbt *searchdbt = new Dbt((char*)str_query.c_str(),str_query.size()+1);
		Dbt *resultdbt = new Dbt();
		resultdbt->set_flags(DB_DBT_MALLOC);
	
		clock_t t1,t2;
		int ret;
		t1=clock();
		if ((ret = db->get(NULL, searchdbt, resultdbt, 0)) != 0) {
			cout << "get: " << DbEnv::strerror(ret) << "\n";
		}
		else {
			char *result = (char *)resultdbt->get_data();
			cout << "got data: " << result << "\n";
		}
		t2=clock();
		cout<<"TIME:"<<((float)t1-(float)t2)/CLOCKS_PER_SEC<<endl;
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
