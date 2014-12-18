#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

// forward
class Name;
class Block;

class Code {
public:
    // context?
    // AST?
};

class CodeApply: public Code {
public:
    // Code *func;
    // arguments
};

class CodeLiteral: public Code {
public:
    // data
};

class CodeAccess: public Code {
public:
    std::string id;
    Code *action;
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
    Code *code;
};

using NameMap = std::multimap<std::string, Name *>;
using NameRange = std::pair<NameMap::iterator, NameMap::iterator>; // or const?

class Argument {
public:
    enum Mode {
        M_IN,
        M_OUT,
        M_VAR
    } mode;

    Name *name;
};

class Function {
public:
    std::vector<Argument *> arguments;
    Code *code;
};

class Block {
private:
    Block *parent;
    std::vector<Block *> children;
    NameMap members;
    Function *function; // could be null!

public:
    inline Block(
        Block *target
    ): parent(target), function(nullptr) {}

    // virtual ~Block() {}

    inline void putChild(Block *child) {
        children.push_back(child);
    }

    inline void putMember(const std::string &id, Name *object) {
        members.insert({id, object});
    }

    inline void setFunction(Function *object) {
        function = object;
    }

    inline Block *getParent() {
        return parent;
    }

    inline NameRange getMember(const std::string &id) {
        NameRange result = members.equal_range(id);

        if (result.first == result.second) {
            // not found
            // TODO: if parent == nullptr, not found
            return parent->getMember(id);
        } else {
            return result;
        }

        // TODO: NameRange with path of Blocks'?
    }
};

}

#endif
