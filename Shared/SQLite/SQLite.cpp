/*------- include files:
-------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "SQLite.h"
#include "Field.h"
#include "Statement.h"

/*-------namespaces:
-------------------------------------------------------------------*/
namespace beesoft {
namespace sqlite {
using namespace std;

const char SQLite::ValidHeader[SQLite::HeaderSize] = {
    0x53, 0x51, 0x4c, 0x69, 0x74, 0x65, 0x20, 0x66,
    0x6f, 0x72, 0x6d, 0x61, 0x74, 0x20, 0x33, 0x00
};

SQLite::SQLite()
    : db(nullptr)
{
    cout << "SQLite ctor" << endl;
    sqlite3_initialize();
}

SQLite::~SQLite() {
    cout << "~SQLite" << endl;
    sqlite3_shutdown();
}

/**
 * SQLite::open
 *
 * Otwarcie bazy danych.
 * Plik bazy danych musi istnieć, musi być i do odczytu, i do zapisu.
 * Dodatkowo plik bazy danych musi mieć odpowiednią sygnaturę
 * (czy 16 pierwszych bajtów pliku musi być takie samo jak 'ValidHeader'.
 *
 * @param fpath - ścieżka do pliku bazy danych.
 * @return true jeśli nie było problemów, false w przeciwnym przypadku.
 */
bool SQLite::open(const std::string& fpath) {
    if (db) return false;

    if (fileExists(fpath)) {
        if(canReadFrom(fpath) && canWriteTo(fpath)) {
            if (isDatabaseFile(fpath)) {
                if (sqlite3_open_v2(fpath.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr) == SQLITE_OK) {
                    cout << "Database opened successfully: " << fpath << endl;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * SQLite::create
 *
 * Utworzenie nowej bazy danych.
 * Po jej utworzeniu wywoływana jest lambda, w której funkcja wywołująca
 * może utworzyć tablice w pustej bazie danych.
 * Jeżeli doszło do wywołania lambdy, to wartość przez nią zwrócona,
 * jest też zwracana jako wynik działania funkcji.
 *
 * @param fpath - ścieżka do bazy danych.
 * @param lambda - lambda wywoływana po utworzeniu bazy danych
 * @return true jeśli nie było problemów, false w przeciwnym przypadku.
 */
bool SQLite::create(const string& fpath, const function<bool(SQLite&)>& lambda, const bool override) {
    if (db) return false;

    // jeśli plik istnieje i jest na to pozwolenie to go usuwamy
    if (fileExists(fpath) && override) {
        if (!removeFile(fpath)) {
            return false;
        }
    }

    // plik nie może istnieć
    if (!fileExists(fpath)) {
        if (sqlite3_open_v2(fpath.c_str(), &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, nullptr) == SQLITE_OK) {
            if (lambda(*this)) {
                cout << "Database created successfully: " << fpath << endl;
                return true;
            }
        }
    }
    cout << sqlite3_errmsg(db) << endl;
    return false;
}

/**
 * SQLite::close
 *
 * Zamknięcie bazy danych.
 * Jest możliwość że nie da się zamknąć bazy danych (SQLITE_BUSY).
 *
 * @see https://www.sqlite.org/c3ref/close.html
 * @return true jeśli nie było problemów, false w przeciwnym przypadku.
 */
bool SQLite::close() {
    if (db) {
        if (sqlite3_close(db) == SQLITE_OK) {
            db = nullptr;
            return true;
        }
        logError();
    }
    return false;
}

bool SQLite::exec(const string& query) {
    lock_guard<mutex> guard(_mutex);

    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
        return true;
    }
    logError();
    return false;
}

int SQLite::insert(const string& name, const vector<Field>& fields) {
    Statement stmt(*this);
    return stmt.insert(name, fields);
}

bool SQLite::update(const string& name, const vector<Field>& fields) {
    Statement stmt(*this);
    return stmt.update(name, fields);
}

vector<vector<Field>> SQLite::select(const string& query) {
    Statement stmt(*this);
    return stmt.select(query);
}

/**
 * SQLite::remove_file
 *
 * @param fpath - plik do usunięcia.
 * @return true/false
 */
bool SQLite::removeFile(const std::string& fpath) const {
    return (remove(fpath.c_str()) == 0);
}

bool SQLite::fileExists(const string& fpath) const {
    return (access(fpath.c_str(), F_OK) == 0);
}

bool SQLite::canReadFrom(const string& fpath) const {
    return (access(fpath.c_str(), R_OK) == 0);
}

bool SQLite::canWriteTo(const string& fpath) const {
    return (access(fpath.c_str(), W_OK) == 0);
}

bool SQLite::isDatabaseFile(const string& fpath) const {
    if (ifstream ifs(fpath, fstream::in); ifs) {
        char buffer[HeaderSize];
        if (ifs.get(buffer, HeaderSize)) {
            for (int i = 0; i < HeaderSize; i++) {
                if (buffer[i] != ValidHeader[i])
                    return false;
            }
            return true;
        }
    }
    return false;
}

void SQLite::logError(const std::string& file, const int n, const std::string& function) {
    cerr << "(" << file << "." << n << ") " << function << ": "
         << sqlite3_errmsg(db)
         << "(" << sqlite3_errcode(db) << ")"
         << endl;
}

}} // namespaces end
