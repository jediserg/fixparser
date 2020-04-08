//
// Created by serg on 3/31/20.
//

#ifndef FIXPARSER_FIXTAG_H
#define FIXPARSER_FIXTAG_H

#include <set>
#include <array>
#include <algorithm>

#ifdef XMACRO
#undef XMACRO
#endif
#define XMACRO(NAME, ID) NAME = ID,
enum class FixTag {
#include "tag_list.h"
};

bool is_group_tag(FixTag tag);


bool is_child_tag(FixTag parent, FixTag child);

std::optional<FixTag> fix_tag_from_int(int tag);

std::string fix_tag_to_str(FixTag tag);
int fix_tag_to_int(FixTag);
std::optional<FixTag> fix_get_first_tag_for_group(FixTag parent);

const std::vector<FixTag>& fix_get_child_tags(FixTag parent);
#endif //FIXPARSER_FIXTAG_H
