//
// Created by serg on 3/31/20.
//
#include <map>
#include "FixTag.h"
//#define MAGIC_ENUM_RANGE_MAX 110000
//#include "magic_enum.hpp"

static std::map<FixTag, std::vector<FixTag>> g_group_tags {
        {FixTag::NoMdEntryTypes, {FixTag::MDEntryType}}, {FixTag::NoRelatedSym, {FixTag::Symbol}}, {
            FixTag::NoMDEntries, {
                FixTag::MDUpdateAction,
                FixTag::MDEntryType,
                FixTag::MDEntryPx,
                FixTag::MDEntrySize,
                FixTag::MDEntryDate,
                FixTag::DeribitTradeId,
                FixTag::Side,
                FixTag::IndexPrice,
                FixTag::Text,
                FixTag::OrderId,
                FixTag::SecondaryOrderId,
                FixTag::OrdStatus,
                FixTag::DeribitLabel,
                FixTag::DeribitLiquidation
            }
        }
};

bool is_group_tag(FixTag tag) {
    return g_group_tags.find(tag) != std::end(g_group_tags);
}

bool is_child_tag(FixTag parent, FixTag child) {
    auto it = g_group_tags.find(parent);
    if(it == std::end(g_group_tags))
        return false;

    return std::find(std::begin(it->second), std::end(it->second), child) != std::end(it->second);
}

#ifdef XMACRO
#undef XMACRO
#endif
std::optional<FixTag> fix_tag_from_int(int tag) {
#define XMACRO(NAME, ID) case ID: return static_cast<FixTag>(ID);
    switch(tag){
#include "tag_list.h"
        default:return {};
    }
    //return std::optional<FixTag>();
}

#undef XMACRO
std::string fix_tag_to_str(FixTag tag) {
#define XMACRO(NAME, ID) case FixTag::NAME: return #NAME;
    switch(tag){
#include "tag_list.h"
    }

    return "";
}

std::optional<FixTag> fix_get_first_tag_for_group(FixTag parent) {
    auto it = g_group_tags.find(parent);
    if(it == g_group_tags.end())
        return {};

    return it->second[0];
}

const std::vector<FixTag> &fix_get_child_tags(FixTag parent) {
    static std::vector<FixTag> empty;

    auto it = g_group_tags.find(parent);
    if(it == g_group_tags.end())
        return empty;

    return it->second;
}

int fix_tag_to_int(FixTag tag) {
    return static_cast<int>(tag);
}

