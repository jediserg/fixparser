//
// Created by serg on 3/31/20.
//

#include <charconv>
#include "FixMessage.h"
#include "FixField.h"
#include "aixlog.hpp"
#include <algorithm>
#include <numeric>
#include <sstream>

std::optional<FixField> FixMessage::_readFieldWithTag(std::istream& stream, FixTag tag){
    FixField field;
    if(!field.read(stream)){
        return {};
    }

    if(field.tag() != tag){
        LOG(ERROR) << "Expected tag:" << fix_tag_to_str(tag) << " but tag:" << fix_tag_to_str(field.tag()) << " was read";
        return {};
    }

    return {std::move(field)};
}

bool FixMessage::_handleBody(std::istream& stream){
    while(!stream.eof()) {
        FixField field;
        if (!field.read(stream))
            return true;

        if(_current_group.has_value()){
            auto it = _groups.find(_current_group.value());
            if(it == std::end(_groups)){
                LOG(ERROR) << "No current group";
                return false;
            }

            FixGroup& group = it->second;
            if(!group.addField(field)){
                _fields.emplace(field.tag(), std::move(field));
                continue;
            }
        } else {
            if(is_group_tag(field.tag())){
                _current_group = field.tag();
                _groups.emplace(_current_group.value(), FixGroup(_current_group.value()));
                continue;
            } else {
                if(field.tag() == FixTag::MsgType){
                    switch(field.value()[0]){
                        case 'W': _type = Type::Snapshot;
                            break;
                        case 'V': _type = Type::DataRequest;
                            break;
                        case 'X': _type = Type::IncrementalRefresh;
                            break;
                        default:
                            ;
                    }
                }
                _fields.emplace(field.tag(), std::move(field));

            }
        }
    }



    return true;
}

bool FixMessage::read(std::istream &stream) {
    if(!_readFieldWithTag(stream, FixTag::BeginString).has_value())
        return false;

    auto length_field = _readFieldWithTag(stream, FixTag::BodyLength);
    if(!length_field.has_value())
        return false;

    int length;
    if(auto [p, err] = std::from_chars(length_field->value().data(), length_field->value().data() + length_field->value().size(), length); err != std::errc()){
        LOG(ERROR) << "Bad body length:" << length_field->value();
        return false;
    }

    std::string body(length, 0);
    stream.read(body.data(), length);

    size_t sum = 0;
    for(const auto& ch:body)
        sum += (size_t)ch;

    sum = sum % 256;

    auto crc_field = _readFieldWithTag(stream, FixTag::CheckSum);
    if(!crc_field.has_value())
        return false;

    int crc_from_message;
    if(auto [p, err] = std::from_chars(crc_field->value().data(), crc_field->value().data() + crc_field->value().size(), crc_from_message); err != std::errc()){
        LOG(DEBUG) << "Couldn't get crc from field:" << crc_field->value();
        return false;
    }
    std::string empty_line;
    if(!stream.eof())
        std::getline(stream, empty_line);
    //std::readstream
    /*if(sum != crc_from_message)
        return false;*/

    std::stringstream  body_stream(body);

    return _handleBody(body_stream);
}

std::string FixMessage::to_string() const {
    std::stringstream ss;

    ss << "[[" << _fields.at(FixTag::MsgType).to_string() << "]]" << std::endl;
    for(const auto& [tag, field]: _fields){
        ss << "\t" << field.to_string() << "\n";
    }

    for(const auto&[tag, group]: _groups)
        ss << "\t" << group.to_string();

    return ss.str();
}

const FixField* FixMessage::getField(FixTag tag) const{
    auto it = _fields.find(tag);
    if(it == std::end(_fields)){
        return nullptr;
    } else {
        return &it->second;
    }
}

const FixGroup* FixMessage::getGroup(FixTag tag) const {
    auto it = _groups.find(tag);
    if(it == std::end(_groups)){
        return nullptr;
    } else {
        return &it->second;
    }
}

std::optional<FixMessage::Type> FixMessage::type() const {
    return _type;
}