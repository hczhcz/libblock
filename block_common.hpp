#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

class Block; // forward

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

    Block *type;
    Code *code; // TODO: or auto append?
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

class Block {
private:
    Block *parent;
    std::vector<Block *> children;

    std::multimap<std::string, Name *> members;
    //std::vector<std::vector<Argument *>, Code *> call; //?

public:
    inline Block(Block *target): parent(target) {}

    // virtual ~Block() {}

    inline void putChild(Block *child) {
        children.push_back(child);
    }

    inline Block *getParent() {
        return parent;
    }
};

}

#endif
