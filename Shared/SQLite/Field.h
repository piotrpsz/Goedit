#ifndef BEESOFT_SQLITE_FIELD_H
#define BEESOFT_SQLITE_FIELD_H

/*------- include files:
-------------------------------------------------------------------*/
#include <cstdint>
#include <string>
#include <vector>
#include <any>
#include <string.h>
#include <memory>

/*------- namespaces:
-------------------------------------------------------------------*/
namespace beesoft {
namespace sqlite {

/*------- types:
-------------------------------------------------------------------*/
class Field;
using i64  = int64_t;
using f64  = double;
using text = std::string;
using vec  = std::vector<char>;

using Row = std::vector<Field>;
using Result = std::vector<Row>;

enum class Type {
    Null = 0,
    Int,
    Float,
    Text,
    Blob
};

class Field{
    std::string _name;
    Type _type;
    int _nbytes;
    std::shared_ptr<void> _value;

public:
    ~Field() = default;

    explicit Field(const std::string&);
    explicit Field(const std::string&, const i64);
    explicit Field(const std::string&, const bool);
    explicit Field(const std::string&, const f64);
    explicit Field(const std::string&, const text&);
    explicit Field(const std::string&, const void* const, const int);
    explicit Field(const std::string&, const vec&);

    Type type() const {
        return _type;
    }
    std::string name() const {
        return _name;
    }
    int size() const {
        return _nbytes;
    }
    std::string bindName() const {
        return std::string(":") + _name;
    }
    void name(const std::string& name) {
        _name = name;
    }

    // Setters
    void value(const i64);
    void value(const bool);
    void value(const f64);
    void value(const text&);
    void value(const void* const, const int);
    void value(const vec&);
    // Getters
    i64  as_i64() const;
    bool as_bool() const;
    f64  as_f64() const;
    text as_text() const;
    vec  as_vector() const;

private:
    std::string typeAsString() const;

    std::string errorString(const std::string& marker) const {
        return std::string("Error: SQLite field value conversion to '")
                + marker
                + "' impossible";
    }

    // friends
    friend std::ostream& operator<<(std::ostream&, const Field&);
};

}} // namespace end
#endif // BEESOFT_SQLITE_FIELD_H
