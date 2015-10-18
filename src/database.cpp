#include "database.h"

namespace fcs
{
namespace database
{
database::database(const std::string & path)
{
	int result = sqlite3_open(path.c_str(), &m_sqlite);
	(void)result; /* TODO: do something with result */
}

database::~database()
{
	sqlite3_close(m_sqlite);
}

}
}