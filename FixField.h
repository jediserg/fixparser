//
// Created by serg on 3/31/20.
//

#ifndef FIXPARSER_FIXFIELD_H
#define FIXPARSER_FIXFIELD_H

static const char FIX_FIELD_END = '^';

static const char FIX_TAG_VALUE_DELIMITER = '=';

#include <iostream>
#include "FixTag.h"

class FixField {
public:
    FixField() = default;
    FixField(const FixField& other) = default;
    FixField(FixField&& other) = default;
    FixField& operator=(const FixField& other) = default;
    FixField& operator=(FixField&& other) = default;
    bool read(std::istream& stream);
    FixTag tag() const;
    const std::string& value() const;
    std::optional<int> value_int() const;
    std::optional<double> value_double() const;

    std::string to_string() const;
private:
    FixTag _tag;
    std::string _value;
};


#endif //FIXPARSER_FIXFIELD_H
