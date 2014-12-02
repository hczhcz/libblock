#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

const auto name_body = '__body';
const auto name_oncall = '__oncall';

class Type;
class Code; // ASM or IR

class Name {
public:
    enum {
        V_PUBLIC,
        V_PRIVATE
    } visibility;

    enum {
        M_LOCAL,
        M_AST,
        M_STATIC,
        M_FAST
    } mode;

    Type *type;
    Code *value;
};

class Block {
public:
    Block *parent;
    std::vector<Block *> children;

    NameList = std::map<std::string, Name *> members;
};

}

#endif
