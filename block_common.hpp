#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

class Type {
public:
};

class Code {
public:
};

class Name {
public:
    enum Visibility {
        V_PUBLIC,
        V_PRIVATE
    } visibility;

    enum Mode {
        M_LOCAL,
        M_CONST,
        M_STATIC,
        M_FAST
    } mode;

    Type *type;
};

class Block {
public:
    Block *parent;
    std::vector<Block *> children;

    std::map<std::string, Name *> members;

    Code *code;
    // void (*entry)(void *data);
};

}

#endif
