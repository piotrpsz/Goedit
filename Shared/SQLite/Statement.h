#ifndef BEESOFT_STATEMENT_H
#define BEESOFT_STATEMENT_H

/*------- include files:
-------------------------------------------------------------------*/
#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include "SQLite.h"

/*------- namespaces:
-------------------------------------------------------------------*/
namespace beesoft {
namespace sqlite {

using Row = std::vector<Field>;
using Result = std::vector<Row>;

class Statement {
    SQLite& _sqlite;
    sqlite3_stmt* _stmt;
public:
    Statement(SQLite& sqlite): _sqlite(sqlite), _stmt(nullptr) {}

    int  insert(const std::string&, const std::vector<Field>&);
    bool update(const std::string&, const std::vector<Field>&);
    std::vector<std::vector<Field>> select(const std::string&);

private:

};


}} // end of namespaces
#endif // BEESOFT_STATEMENT_H
