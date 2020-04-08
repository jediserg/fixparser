//
// Created by serg on 3/31/20.
//

#ifndef FIXPARSER_FIXMESSAGE_H
#define FIXPARSER_FIXMESSAGE_H

#include <istream>
#include <optional>
#include <map>
#include "FixTag.h"
#include "FixField.h"
#include "FixGroup.h"

class FixMessage {
public:
    enum class Type {
        DataRequest = 'V',
        Snapshot = 'W',
        IncrementalRefresh = 'X',

    };
    bool read(std::istream& stream);
    std::string to_string() const;

    const FixField* getField(FixTag tag) const;
    const FixGroup* getGroup(FixTag tag) const;

    //std::optional<const FixField&> getField(FixTag tag) const;

    using FixGroups = std::map<FixTag, FixGroup >;

    std::optional<Type> type() const;
private:
    std::optional<FixField> _readFieldWithTag(std::istream &stream, FixTag tag);

    bool _handleBody(std::istream &stream);

    std::map<FixTag, FixField> _fields;
    FixGroups _groups;
    std::optional<FixTag> _current_group;
    std::optional<Type> _type;
};


#endif //FIXPARSER_FIXMESSAGE_H
