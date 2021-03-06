#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "database.h"
#include <memory>
#include <string>

const long kDefaultBlobContent = 'fcs\0';

std::unique_ptr<fcs::database::database> createDatabase()
{
	const std::string createSql = "CREATE TABLE test (first_column INT, second_column INT);";
	auto db = std::make_unique<fcs::database::database>( ":memory:" );
	if (db->execute(createSql).is_error())
		return nullptr;
	if (db->execute("CREATE TABLE string_test (a INT, b TEXT);").is_error())
		return nullptr;
	if (db->execute("CREATE TABLE blob_test (a INT, b BLOB);").is_error())
		return nullptr;
	return std::move(db);
}

bool populateDatabase(const fcs::database::database & db)
{
	fcs::database::result result { db.execute("INSERT INTO test (first_column, second_column) VALUES (1, 2);") };
	if (result.step() != fcs::database::result::step_status::done || result.is_error())
		return false;
	
	result = db.execute("INSERT INTO test (first_column, second_column) VALUES (3, 4);");
	if (result.step() != fcs::database::result::step_status::done || result.is_error())
		return false;
	
	result = db.execute("INSERT INTO string_test(a, b) VALUES (1, \"lorem ipsum\");");
	if (result.step() != fcs::database::result::step_status::done || result.is_error())
		return false;
	
	result = db.execute("INSERT INTO blob_test (a, b) VALUES (1, ?);", fcs::database::data { const_cast<long *>(&kDefaultBlobContent), sizeof(long) });
	if (result.step() != fcs::database::result::step_status::done || result.is_error())
		return false;
	
	return true;
}

TEST_CASE("Basic SELECT", "[SELECT]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto result = db->execute("SELECT first_column, second_column FROM test;");
	result.step();
	auto firstRow = result.current();
	REQUIRE((int)firstRow.get<int>(0) == 1);
	REQUIRE((int)firstRow.get<int>(1) == 2);
	
	result.step();
	auto secondRow = result.current();
	REQUIRE((int)secondRow.get<int>(0) == 3);
	REQUIRE((int)secondRow.get<int>(1) == 4);
}

TEST_CASE("Parameter SELECT", "[SELECT(?)]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto result = db->execute("SELECT second_column FROM test WHERE first_column=?", 1);
	result.step();
	REQUIRE(2 == (int)result.current().get<int>(0));
}

TEST_CASE("Named Parameter SELECT", "[SELECT(:x)]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto result = db->execute("SELECT second_column FROM test WHERE first_column=:x", std::make_pair(":x", 1));
	result.step();
	REQUIRE((int)result.current().get<int>(0) == 2);
}

TEST_CASE("Test NULL", "[NULL]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto insert = db->execute("INSERT INTO test (first_column, second_column) VALUES(?, ?);", fcs::database::null, 42);
	insert.step();
	
	auto result = db->execute("SELECT second_column FROM test WHERE first_column IS NULL;");
	result.step();
	REQUIRE((int)result.current().get<int>(0) == 42);
}

TEST_CASE("Test SELECT NULL", "[NULL]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto insert = db->execute("INSERT INTO test (first_column, second_column) VALUES(?, ?);", fcs::database::null, 42);
	insert.step();
	
	auto result = db->execute("SELECT first_column FROM test WHERE second_column IS 42;");
	result.step();
	fcs::database::nullable<int> value { result.current().get<int>(0) };
	REQUIRE(!value.has_value());
}

TEST_CASE("String Test", "[STRING]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	
	auto insert = db->execute("INSERT INTO string_test(a, b) VALUES (2, \"dolor sit\");");//, 2, "dolor sit"
	REQUIRE(insert.step() == fcs::database::result::step_status::done);
	REQUIRE(!insert.is_error());
	
	auto bValueResult = db->execute("SELECT b FROM string_test WHERE a IS 1;");
	bValueResult.step();
	fcs::database::nullable<std::string> bValue { bValueResult.current().get<std::string>(0) };
	const std::string & stringValue = bValue;
	REQUIRE(stringValue == "lorem ipsum");
	
	auto aValueResult = db->execute("SELECT a FROM string_test WHERE b = ?;", "dolor sit");
	aValueResult.step();
	fcs::database::nullable<int> aValue { aValueResult.current().get<int>(0) };
	REQUIRE(aValue.has_value());
	const int & intValue = aValue;
	REQUIRE(2 == intValue);
}

TEST_CASE("Data test", "[DATA]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	
	auto lookupBlobResult = db->execute("SELECT b FROM blob_test WHERE a = 1;");
	lookupBlobResult.step();
	fcs::database::nullable<fcs::database::data> blobValue { lookupBlobResult.current().get<fcs::database::data>(0) };
	fcs::database::data blob = blobValue;
	long contents = *(static_cast<const long *>(blob.GetPointer()));
	REQUIRE('f' == ((contents >> 24) & 0xFF));
	REQUIRE('c' == ((contents >> 16) & 0xFF));
	REQUIRE('s' == ((contents >> 8) & 0xFF));
	REQUIRE('\0' == (contents & 0xFF));
	
	auto lookupByBlobResult = db->execute("SELECT a FROM blob_test WHERE b = ?", fcs::database::data { &contents, sizeof(long) });
	auto step = lookupByBlobResult.step();
	fcs::database::nullable<int> value = lookupByBlobResult.current().get<int>(0);
	REQUIRE(value.has_value());

	int intValue = value;
	REQUIRE(intValue == 1);
}