#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "database.h"
#include <memory>

std::unique_ptr<fcs::database::database> createDatabase()
{
	const std::string createSql = "CREATE TABLE test (first_column INT, second_column INT);";
	auto db = std::make_unique<fcs::database::database>( ":memory:" );
	if (db->execute(createSql).is_error())
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
	
	return true;
}

TEST_CASE("Basic SELECT", "[SELECT]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto result = db->execute("SELECT first_column, second_column FROM test;");
	result.step();
	auto firstRow = result.current();
	REQUIRE(firstRow[0] == 1);
	REQUIRE(firstRow[1] == 2);
	
	result.step();
	auto secondRow = result.current();
	REQUIRE(secondRow[0] == 3);
	REQUIRE(secondRow[1] == 4);
}

TEST_CASE("Parameter SELECT", "[SELECT(?)]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto result = db->execute("SELECT second_column FROM test WHERE first_column=?", 1);
	result.step();
	REQUIRE(result.current()[0] == 2);
}

TEST_CASE("Named Parameter SELECT", "[SELECT(:x)]")
{
	auto db = createDatabase();
	REQUIRE(populateDatabase(*db));
	auto result = db->execute("SELECT second_column FROM test WHERE first_column=:x", std::make_pair(":x", 1));
	result.step();
	REQUIRE(result.current()[0] == 2);
}