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

struct entry_t {
    enum Mode {
        M_VAR,
        M_STATIC,
        M_EXPR,
        M_FAST
    } mode;

    bool hidden;

    Code *code;

    inline entry_t(
        const Mode to_mode, bool to_hidden, Code *to_code
    ): mode(to_mode), hidden(to_hidden), code(to_code) {}
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

class CodeLabel: public Code {
public:
    inline CodeLabel() {}
};

class CodeLabelRef: public Code {
private:
    CodeLabel *label; // notice: weak reference!!!

public:
    inline CodeLabelRef(CodeLabel *to_label): label(to_label) {}
};

class CodeBlockId: public Code {
private:
    Block *block;

public:
    inline CodeBlockId(Block *to_block): block(to_block) {}
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
    std::map<name_t, entry_t> members;
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
    //     auto iter = members.find(id);
    //     if (iter != members.end()) {
    //         if (
    //             iter->second->visibility == object->visibility
    //             &&
    //             iter->second->mode == object->mode
    //         ) {
    //             CodeCall::pack(iter->second->code, object->code);
    //         } else {
    //             // TODO: error
    //         }
    //     } else {
    //         members.insert({id, object});
    //     }
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
