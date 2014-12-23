#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

// forward
class Code;
class Block;

struct name_t {
    std::string id;

    inline name_t(std::string &&to_id): id(std::move(to_id)) {}

    inline name_t(const std::string &to_id): id(to_id) {}
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
private:
    Code *next;

public:
    static inline Code *pack(Code *left, Code *right) {
        if (left->next) {
            pack(left->next, right);
        } else {
            left->next = right;
        }

        return left;
    }

    inline Code(): next(nullptr) {}

    inline Code *getNext() const {
        return next;
    }
};

// TODO: use CodeCall("__access") ?
class CodeGet: public Code {
private:
    name_t name;

public:
    inline CodeGet(name_t &&to_name): name(std::move(to_name)) {}
};

class CodeWith: public Code {
private:
    Code *target;
    Code *action;

public:
    inline CodeWith(
        Code *to_target, Code *to_action
    ): target(to_target), action(to_action) {}
};

class CodeCall: public Code {
private:
    Code *target;
    Code *arg;

public:
    inline CodeCall(
        Code *to_target, Code *to_arg
    ): target(to_target), arg(to_arg) {}
};

// TODO: Code -> CodeWithData -> Code???
template <class T>
class CodeLiteral: public Code {
private:
    T value;

public:
    inline CodeLiteral(T &&to_value): value(std::move(to_value)) {}

    // inline CodeLiteral(const T &to_value): value(to_value) {}
};

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

    Code *code;
};

class Function {
private:
    std::vector<argument_t> arguments;
    Code *code;

public:
    inline Function() {
        // TODO
    }
};

class Block {
private:
    Block *parent;
    std::vector<Block *> children;
    std::map<std::string, NameEntry *> members;
    Function *function; // could be null!

public:
    inline Block(
        Block *target
    ): parent(target), function(nullptr) {}

    // virtual ~Block() {}

    inline void putChild(Block *child) {
        children.push_back(child);
    }

    inline void putMember(const std::string &id, NameEntry *object) {
        auto iter = members.find(id);
        if (iter != members.end()) {
            if (
                iter->second->visibility == object->visibility
                &&
                iter->second->mode == object->mode
            ) {
                CodeCall::pack(iter->second->code, object->code);
            } else {
                // TODO: error
            }
        } else {
            members.insert({id, object});
        }
    }

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
