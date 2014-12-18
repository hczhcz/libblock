#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

// forward
class Block;

struct name_t {
    std::string id;

    inline name_t(
        const std::string &to_id
    ): id(to_id) {}
};

struct argument_t {
    enum Mode {
        M_IN,
        M_OUT,
        M_VAR
    } mode;

    name_t name;

    inline argument_t(
        const Mode to_mode, name_t &&to_name
    ): mode(to_mode), name(std::move(to_name)) {}
};

class Code {
public:
    // context?
    // AST?
};

class CodeApply: public Code {
private:
    Code *func;
    std::vector<Code *> argument;

public:
    // Code *func;
    // arguments
};

class CodeLiteral: public Code {
public:
    // data
};

class CodeAccess: public Code {
private:
    name_t name;
    Code *action;

public:
    inline CodeAccess(const name_t &to_name): name(to_name) {}
};

// TODO
// using NameMap = std::multimap<std::string, NameEntry *>;
// using NameRange = std::pair<NameMap::iterator, NameMap::iterator>; // or const?

class NameEntry {
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

class Function {
public:
    std::vector<argument_t> arguments;
    Code *code;
};

class Block {
private:
    Block *parent;
    std::vector<Block *> children;
    // NameMap members;
    Function *function; // could be null!

public:
    inline Block(
        Block *target
    ): parent(target), function(nullptr) {}

    // virtual ~Block() {}

    inline void putChild(Block *child) {
        children.push_back(child);
    }

    // inline void putMember(const std::string &id, NameEntry *object) {
    //     members.insert({id, object});
    // }

    inline void setFunction(Function *object) {
        function = object;
    }

    inline Block *getParent() {
        return parent;
    }

    // inline NameRange getMember(const std::string &id) {
    //     NameRange result = members.equal_range(id);

    //     if (result.first == result.second) {
    //         // not found
    //         // TODO: if parent == nullptr, not found
    //         return parent->getMember(id);
    //     } else {
    //         return result;
    //     }

    //     // TODO: NameRange with path of Blocks'?
    // }
};

}

#endif
