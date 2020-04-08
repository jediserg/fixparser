//
// Created by serg on 4/2/20.
//

#include <sstream>
#include "FixGroup.h"

FixGroup::FixGroup(FixTag parent) : _parent(parent), _child_tags(fix_get_child_tags(parent)) {

}

bool FixGroup::addField(FixField field) {
    if(std::find(std::begin(_child_tags), std::end(_child_tags), field.tag()) == std::end(_child_tags))
        return false;

    if(_records.empty())
        _first_group_tag = field.tag();

    if(field.tag() == _first_group_tag){
        _records.emplace_back(FixRecord{});
    }

    FixRecord& current_record = _records[_records.size() - 1];
    current_record.emplace(field.tag(), std::move(field));

    return true;
}

size_t FixGroup::size() const {
    return _records.size();
}

const FixGroup::FixRecords& FixGroup::get() const{
    return _records;
}

std::string FixGroup::to_string() const {
    std::stringstream ss;
    ss << "[" << fix_tag_to_str(_parent) << "(" << static_cast<int>(_parent) << ") = " << _records.size() <<"]" << std::endl;

    for(const auto& record: _records){
        for(const auto&[tag, field]:record){
            ss << "\t\t" << field.to_string() << std::endl;
        }

        ss << std::string(10, '-') << std::endl;
    }

    return ss.str();
}
