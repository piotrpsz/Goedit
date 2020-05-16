/*------- include files:
-------------------------------------------------------------------*/
#include <iostream>
#include <sstream>
#include <iterator>
#include "Field.h"

/*------- namespaces:
-------------------------------------------------------------------*/
namespace beesoft {
namespace sqlite {
using namespace std;


auto deleter = [](void* ptr) {
    delete[] static_cast<char*>(ptr);
};

/********************************************************************
*                                                                   *
*                     C O N S T R U C T O R S                       *
*                                                                   *
********************************************************************/

/**
 * @brief Field::Field(name)
 * Konstruktor pola typu NULL.
 *
 * @param name - nazwa pola/kolumny
 */
Field::Field(const string& name)
    : _name(name)
    , _type(Type::Null)
    , _value(nullptr, deleter)
{}

/**
 * @brief Field::Field(name, i64)
 * Konstruktor pola typu INT.
 *
 * @param name - nazwa pola/kolumny
 * @param v - wartość pola typu i64
 */
Field::Field(const string& name, const i64 v)
    : _name(name)
    , _type(Type::Int)
    , _nbytes(sizeof(i64))
    , _value(new byte[_nbytes], deleter)
{
    memcpy(_value.get(), &v, _nbytes);
}

Field::Field(const string& name, const bool v)
    : Field(name, v ? i64(1) : i64(0))
{}

Field::Field(const string& name, const f64 v)
    : _name(name)
    , _type(Type::Float)
    , _nbytes(sizeof(f64))
    , _value(new byte[_nbytes], deleter)
{
    memcpy(_value.get(), &v, _nbytes);
}

Field::Field(const std::string& name, const text& v)
    : _name(name)
    , _type(Type::Text)
    , _nbytes(v.size())
    , _value(new byte[_nbytes], deleter)
{
    memcpy(_value.get(), v.c_str(), _nbytes);
}

Field::Field(const std::string& name, const void* const ptr, const int nbytes)
    : _name(name)
    , _type(Type::Blob)
    , _nbytes(nbytes)
    , _value(new byte[_nbytes], deleter)
{
    memcpy(_value.get(), ptr, _nbytes);
}

Field::Field(const std::string& name, const std::vector<char>& value)
    : Field(name, value.data(), value.size())
{}

/********************************************************************
*                                                                   *
*                       S E T T E R S                               *
*                                                                   *
********************************************************************/

void Field::value(const i64 v) {
    _type = Type::Int;
    _nbytes = sizeof(i64);
    _value.reset(new byte[_nbytes], deleter);
    memcpy(_value.get(), &v, _nbytes);
}

void Field::value(const bool v) {
    value(v ? i64(1) : i64(0));
}

void Field::value(const f64 v) {
    _type = Type::Float;
    _nbytes = sizeof(f64);
    _value.reset(new byte[_nbytes], deleter);
    memcpy(_value.get(), &v, _nbytes);
}

void Field::value(const text& v) {
    _type = Type::Text;
    _nbytes = v.size();
    _value.reset(new byte[_nbytes], deleter);
    memcpy(_value.get(), v.c_str(), _nbytes);
}

void Field::value(const void* const ptr, const int nbytes) {
    _type = Type::Blob;
    _nbytes = nbytes;
    _value.reset(new byte[_nbytes], deleter);
    memcpy(_value.get(), ptr, _nbytes);
}

void Field::value(const vec& v) {
    value(v.data(), v.size());
}


/********************************************************************
*                                                                   *
*                       G E T T E R S                               *
*                                                                   *
********************************************************************/

i64 Field::as_i64() const {
    if (Type::Int == _type) {
        return *static_cast<i64*>(_value.get());
    }
    const auto errstr = errorString("i64");
    cerr << errstr << endl;
    throw errstr;
}
bool Field::as_bool() const {
    return (as_i64() == 0) ? false : true;
}

f64 Field::as_f64() const {
    if (Type::Float == _type) {
        return *static_cast<f64*>(_value.get());
    }
    const auto errstr = errorString("f64");
    cerr << errstr << endl;
    throw errstr;
}

text Field::as_text() const {
    if (Type::Text == _type) {
        return string(static_cast<char*>(_value.get()), _nbytes);
    }
    const auto errstr = errorString("text");
    cerr << errstr << endl;
    throw errstr;
}

std::vector<char> Field::as_vector() const {
    std::vector<char> data(_nbytes);
    memcpy(&data[0], static_cast<char*>(_value.get()), _nbytes);
    return data;
}

/********************************************************************
*                                                                   *
*                          H E L P E R S                            *
*                                                                   *
********************************************************************/

std::string Field::typeAsString() const {
    switch (_type) {
    case Type::Null:
        return "NULL";
    case Type::Int:
        return "INT";
    case Type::Float:
        return "FLOAT";
    case Type::Text:
        return "TEXT";
    case Type::Blob:
        return "BLOB";
    }
    return "Unknown";
}

std::ostream& operator<<(std::ostream& s, const Field& f) {
    stringstream ss;

    ss << "(";
    ss << "type: " << f.typeAsString() << ", ";
    ss << "name: " << f.name();

    if (f.type() != Type::Null) {
        ss << ", ";
        switch (f.type()) {
        case Type::Int:
            ss << "value: " << f.as_i64();
            break;
        case Type::Float:
            ss << "value: " << f.as_f64();
            break;
        case Type::Text:
            ss << "value: " << f.as_text();
            break;
        default:
            break;
        }
    }
    ss << " [" << f._value.use_count() << "]";
    ss << ")";

    s << ss.str();
    return s;
}

}}
