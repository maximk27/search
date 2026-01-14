#pragma once
#include <string>

// returns whether this name is namespaced or not
// [a-zA-Z]+\: <- matching this
bool is_namespaced(const std::string_view &name);

// modifies input to control name
// <space> -> "_"
// uppercase -> lowercase
void control_name(std::string &input);
