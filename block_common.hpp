#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

class Block; // forward

class CodeTree {
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

    Block *type;
    CodeTree *code;
};

class Argument {
public:
    enum Mode {
        M_IN,
        M_OUT,
        M_VAR
    } mode;

    Name *name;
};

class Interface {
public:
    std::vector<Argument *> arguments;
    CodeTree *code;
};

class Block {
private:
    Block *parent;
    std::vector<Block *> children;
    std::multimap<std::string, Name *> members;
    Interface *interface;

public:
    inline Block(
        Block *target
    ): parent(target), interface(nullptr) {}

    // virtual ~Block() {}

    inline void putChild(Block *child) {
        children.push_back(child);
    }

    inline void putMember(const std::string &string, Name *object) {
        members.insert({string, object});
    }

    inline void setInterface(Interface *object) {
        interface = object;
    }

    inline Block *getParent() {
        return parent;
    }
};

}

#endif
