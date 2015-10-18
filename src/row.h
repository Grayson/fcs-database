#pragma once

#include "sqlite3.h"
#include "statement.h"
#include "types.h"

namespace fcs
{
namespace database
{

class row
{
public:
	int number_of_columns() const;
	
	template<typename T>
	nullable<T> get(int columnIndex) const;
	
private:
	row(const statement & statement) : m_statement { statement } {};
	
	const statement & m_statement;
	
	friend class result;
};

}
}