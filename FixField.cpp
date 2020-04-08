//
// Created by serg on 3/31/20.
//

#include <string>
#include <charconv>
#include <optional>
#include "FixField.h"
#include "aixlog.hpp"
#include "FixTag.h"

bool FixField::read(std::istream &stream) {
    std::string line;
    std::getline(stream, line, FIX_FIELD_END);

    if(line.empty())
        return false;

    auto pos = line.find(FIX_TAG_VALUE_DELIMITER);

    if(pos == std::string::npos || pos == line.length() - 1) {
        LOG(ERROR) << "Couldn't find '" << FIX_TAG_VALUE_DELIMITER << "' in '" << line << "'";
        return false;
    }

    int tag_num;
    if(auto [p, ec] = std::from_chars(line.data(), line.data() + pos, tag_num);
            ec != std::errc()){
        LOG(ERROR) << "Bad tag in line:'" << line << "'";
        return false;
    }

    auto tag = fix_tag_from_int(tag_num);
    if(!tag.has_value()){
        LOG(ERROR) << "Unknown tag:" << tag_num;
        return false;
    }

    _tag = tag.value();
    _value = line.substr(pos + 1);

    return true;
}

FixTag FixField::tag() const {
    return _tag;
}

const std::string &FixField::value() const {
    return _value;
}

std::optional<int> FixField::value_int() const {
    int result;
    if(auto [p, err] = std::from_chars(_value.data(), _value.data() + _value.size(), result); err != std::errc()){
        LOG(DEBUG) << "Couldn't convert field:" << fix_tag_to_str(tag()) << ", value:" << value();
        return {};
    }

    return result;
}

std::string FixField::to_string() const {
    std::stringstream ss;
    ss << fix_tag_to_str(tag()) << "(" << fix_tag_to_int(tag()) << ") = " << value();
    return ss.str();
}

std::optional<double> FixField::value_double() const {
    std::size_t pos = _value.find('.');
    if(pos == std::string::npos || pos == 0 || pos == _value.length() - 1)
        return {};

    int int_part;
    int float_part;

    if(auto [p, err] = std::from_chars(_value.data(), _value.data() + pos, int_part); err != std::errc()){
        LOG(DEBUG) << "Couldn't convert field:" << fix_tag_to_str(tag()) << ", value:" << value();
        return {};
    }

    if(auto [p, err] = std::from_chars(_value.data() + pos + 1, _value.data() + _value.size(), float_part); err != std::errc()){
        LOG(DEBUG) << "Couldn't convert field:" << fix_tag_to_str(tag()) << ", value:" << value();
        return {};
    }

    return int_part + float_part / (_value.length() - pos - 1);
}
