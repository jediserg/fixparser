//
// Created by serg on 4/2/20.
//
#ifndef FIXPARSER_FIXGROUP_H
#define FIXPARSER_FIXGROUP_H

#include "FixTag.h"
#include "FixField.h"
#include <map>

class FixGroup {
public:
    using FixRecord = std::map<FixTag, FixField>;
    using FixRecords = std::vector<FixRecord>;

    explicit FixGroup(FixTag parent);
    bool addField(FixField field);
    const FixRecords& get() const;
    size_t size() const;

    std::string to_string() const;
private:
    FixTag _parent;
    const std::vector<FixTag>& _child_tags;
    FixRecords _records;

    FixTag _first_group_tag;
};


#endif //FIXPARSER_FIXGROUP_H
