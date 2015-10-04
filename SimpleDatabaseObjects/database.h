#pragma once

#include "result.h"

#include <memory>
#include <string>

#include "sqlite3.h"


namespace fcs
{
namespace database
{	
class database
{
public:
	database(const std::string & path);
	~database();
	database(database &) = delete;
	database operator=(database &) = delete;

	template<typename ...Args>
	result execute(const std::string & command, Args... args) const
	{
		sqlite3_stmt *stmt;
		int status = sqlite3_prepare_v2(m_sqlite, command.c_str(), static_cast<int>(command.length()), &stmt, nullptr);
		(void)status; // TODO: Do something with the status;
		return result { statement { stmt }, args... };
	};
	
private:
	sqlite3 * m_sqlite;
	
};

}
}