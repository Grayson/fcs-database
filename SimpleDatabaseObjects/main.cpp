//
//  main.cpp
//  SimpleDatabaseObjects
//
//  Created by Grayson Hansard on 9/19/15.
//  Copyright © 2015 From Concentrate Software. All rights reserved.
//

#include <iostream>
#include "Database.h"

int main(int argc, const char * argv[])
{
	const std::string createSql = "CREATE TABLE test (first_column INT, second_column INT);";
	const std::string insertSql = "INSERT INTO test (first_column, second_column) VALUES (1, 2);";
	const std::string selectSql = "SELECT first_column, second_column FROM test;";
	
	fcs::database::database db { ":memory:" };
	if (db.execute(createSql).is_error())
		return -1;
	
	fcs::database::result ir { db.execute(insertSql) };
	if (ir.step() != fcs::database::result::step_status::done || ir.is_error())
		return -1;
	
	db.execute("INSERT INTO test (first_column, second_column) VALUES (3, 4);");
	
	fcs::database::result r { db.execute(selectSql) };
	if (r.is_error())
		return -1;
	
	while (r.step() == fcs::database::result::step_status::more)
	{
		fcs::database::row row { r.current() };
		int first { row[0] };
		int second { row[1] };
		
		std::cout << "Values: " << first << "; " << second << std::endl;
	}
	
	fcs::database::result x { db.execute("SELECT second_column FROM test WHERE first_column=?;", 3) };
	while (x.step() == fcs::database::result::step_status::more) {
		fcs::database::row row = x.current();
		std::cout << "Found: " << row[0] << std::endl;
	}
	
    return 0;
}
