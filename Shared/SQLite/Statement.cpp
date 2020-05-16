/*
 *  BSD 2-Clause License
 *
 *  Copyright (c) 2020, Piotr Pszczółkowski
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*------- include files:
-------------------------------------------------------------------*/
#include <iostream>
#include <sstream>
#include "Statement.h"
#include "Field.h"

/*------- namespaces:
-------------------------------------------------------------------*/
namespace beesoft {
namespace sqlite {
using namespace std;

/**
 * @brief Statement::select
 * Execute SELECT query and fetch data from database.
 *
 * @param query - query with SELECT to execute.
 * @return vector of rows, where row is vector of fields (type Field).
 */
vector<vector<Field>> Statement::select(const string& query) {
    lock_guard<mutex> guard(_sqlite._mutex);
    Result result;

    if (sqlite3_prepare_v2(_sqlite.db, query.c_str(), query.size(), &_stmt, nullptr) == SQLITE_OK) {
        if (const int column_count = sqlite3_column_count(_stmt); column_count > 0) {
            while (SQLITE_ROW == sqlite3_step(_stmt)) {
                Row row;
                for (int i = 0; i < column_count; i++) {
                    const string column_name = sqlite3_column_name(_stmt, i);
                    switch (sqlite3_column_type(_stmt, i)) {
                    case SQLITE_INTEGER:
                        row.emplace_back(column_name,
                                         i64(sqlite3_column_int64(_stmt, i)));
                        break;
                    case SQLITE_FLOAT:
                        row.emplace_back(column_name,
                                         f64(sqlite3_column_double(_stmt, i)));
                        break;
                    case SQLITE3_TEXT:
                        row.emplace_back(column_name,
                                         text(reinterpret_cast<const char*>(sqlite3_column_text(_stmt, i)),
                                              sqlite3_column_bytes(_stmt, i)));

                        break;
                    case SQLITE_BLOB:
                        row.emplace_back(column_name,
                                         sqlite3_column_blob(_stmt, i),
                                         sqlite3_column_bytes(_stmt, i));
                        break;
                    case SQLITE_NULL:
                        row.emplace_back(column_name);
                        break;
                    }
                }
                if (row.size()) {
                    result.push_back(row);
                }
            }
        }
        if (SQLITE_OK == sqlite3_finalize(_stmt)) {
            return result;
        }
    }
    _sqlite.logError();
    return Result();
}


/**
 * @brief Statement::update
 * Execute UPDATE query.
 * Updated are all fields except for the field with index 0.
 * The field with index 0 is used in clause WHERE to locate the row.
 *
 * @param table - name of the table
 * @param fields - information about content of row's fields.
 * @return true when OK, false otherwise.
 */
bool Statement::update(const string& table, const vector<Field>& fields) {
    lock_guard<mutex> guard(_sqlite._mutex);

    if (const int n = fields.size(); n > 1) { // co najmniej 2 pola: id + coś
        const int last = fields.size() - 1;

        stringstream ss_query;
        ss_query << "UPDATE " << table << " SET ";
        for (int i = 1; i < last; i++) {
            ss_query << fields[i].name() << "=" << fields[i].bindName() << ",";
        }
        ss_query << fields[last].name() << "=" << fields[last].bindName()
                 << " WHERE " << fields[0].name() << "=" << fields[0].as_i64();
        const string query = ss_query.str();

        if (sqlite3_prepare_v2(_sqlite.db, query.c_str(), query.size(), &_stmt, nullptr) == SQLITE_OK) {
            for (int i = 0; i < n; i++) {
                const auto& f = fields[i];
                if (const int idx = sqlite3_bind_parameter_index(_stmt, f.bindName().c_str()); idx != 0) {
                    switch (f.type()) {
                    case Type::Null:
                        sqlite3_bind_null(_stmt, idx);
                        break;
                    case Type::Int:
                        sqlite3_bind_int64(_stmt, idx, f.as_i64());
                        break;
                    case Type::Float:
                        sqlite3_bind_double(_stmt, idx, f.as_f64());
                        break;
                    case Type::Text: {
                        const auto text = f.as_text();
                        sqlite3_bind_text(_stmt, idx, text.c_str(), text.size(), SQLITE_TRANSIENT); }
                        break;
                    case Type::Blob: {
                        const auto vector = f.as_vector();
                        sqlite3_bind_blob(_stmt, idx, vector.data(), vector.size(), nullptr); }
                        break;
                    }
                }
            }
            if (sqlite3_step(_stmt) == SQLITE_DONE) {
                if (sqlite3_finalize(_stmt) == SQLITE_OK) {
                    return true;
                }
            }
            _sqlite.logError();
            return false;
 }
    }
    return false;
}

/**
 * @brief Statement::insert
 * Execute INSERT query.
 *
 * @param table - name of the table
 * @param fields - information about content of row's fields.
 * @return last inserted rowid when OK, -1 othewise.
 */
int Statement::insert(const string& table, const vector<Field>& fields) {
    lock_guard<mutex> guard(_sqlite._mutex);

    if (const int n = fields.size() - 1; n > 0) {
        string names, binds;
        for (int i = 0; i < n; i++) {
            const Field& f = fields[i];
            names += (f.name() + ",");
            binds += (f.bindName() + ",");
        }
        const Field& f = fields[n];
        names += (f.name());
        binds += (f.bindName());

        string query("INSERT INTO ");
        query.append(table);
        query.append(" (");
        query.append(names);
        query.append(") VALUES (");
        query.append(binds);
        query.append(")");

        if (sqlite3_prepare_v2(_sqlite.db, query.c_str(), query.size(), &_stmt, nullptr) == SQLITE_OK) {
            for (auto i = 0; i < int(fields.size()); i++) {
                const auto& f = fields[i];
                if (const int idx = sqlite3_bind_parameter_index(_stmt, f.bindName().c_str()); idx != 0) {
                    switch (f.type()) {
                    case Type::Null:
                        sqlite3_bind_null(_stmt, idx);
                        break;
                    case Type::Int:
                        sqlite3_bind_int64(_stmt, idx, f.as_i64());
                        break;
                    case Type::Float:
                        sqlite3_bind_double(_stmt, idx, f.as_f64());
                        break;
                    case Type::Text: {
                        const auto text = f.as_text();
                        sqlite3_bind_text(_stmt, idx, text.c_str(), text.size(), SQLITE_TRANSIENT); }
                        break;
                    case Type::Blob: {
                        const auto vector = f.as_vector();
                        sqlite3_bind_blob(_stmt, idx, vector.data(), vector.size(), nullptr); }
                        break;
                    }
                }
            }
            if (sqlite3_step(_stmt) == SQLITE_DONE) {
                if (sqlite3_finalize(_stmt) == SQLITE_OK) {
                    return sqlite3_last_insert_rowid(_sqlite.db);
                }
            }
            _sqlite.logError();
            return -1;
        }
    }
    return -1;
}

}} // namespaces end
