#pragma once

#include "sqlite3.h"
#include "statement.h"

namespace fcs
{
namespace database
{

class row
{
public:
	int number_of_columns() const;
	int operator [](int columnIndex) const;
	
private:
	row(const statement & statement) : m_statement { statement } {};
	
	const statement & m_statement;
	
	friend class result;
};

}
}