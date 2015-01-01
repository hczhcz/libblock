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

struct field_t {
    enum Mode {
        M_TYPE,
        M_EXPR,
        M_VAR,
        M_STATIC,
        M_FAST
    } mode;

    bool import;
    bool hidden;

    name_t name;
    Code *code;

    inline field_t(
        const Mode to_mode, bool to_import, bool to_hidden,
        name_t &&to_name, Code *to_code
    ):
        mode(to_mode), import(to_import), hidden(to_hidden),
        name(to_name), code(to_code) {}
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

    // TODO
    virtual void codeGen() = 0;
};

// TODO: use CodeCall("__access") ?
class CodeGet: public Code {
private:
    name_t name;

public:
    inline CodeGet(name_t &&to_name): name(std::move(to_name)) {}

    virtual void codeGen() {
        //
    }
};

class CodeWith: public Code {
private:
    Code *target;
    Code *action;

public:
    inline CodeWith(
        Code *to_target, Code *to_action
    ): target(to_target), action(to_action) {}

    virtual void codeGen() {
        //
    }
};

class CodeCall: public Code {
private:
    Code *target;
    Code *arg;

public:
    inline CodeCall(
        Code *to_target, Code *to_arg
    ): target(to_target), arg(to_arg) {}

    virtual void codeGen() {
        //
    }
};

// TODO: Code -> CodeWithData -> Code???
template <class T>
class CodeLiteral: public Code {
private:
    T value;

public:
    inline CodeLiteral(T &&to_value): value(std::move(to_value)) {}

    // inline CodeLiteral(const T &to_value): value(to_value) {}

    virtual void codeGen() {
        //
    }
};

class CodeLabel: public Code {
public:
    inline CodeLabel() {}

    virtual void codeGen() {
        //
    }
};

class CodeLabelRef: public Code {
private:
    CodeLabel *label; // notice: weak reference!!!

public:
    inline CodeLabelRef(CodeLabel *to_label): label(to_label) {}

    virtual void codeGen() {
        //
    }
};

class CodeBlock: public Code {
private:
    Block *block;

public:
    inline CodeBlock(Block *to_block): block(to_block) {}

    virtual void codeGen() {
        //
    }
};

class Proto {
private:
    std::vector<argument_t> arguments;

    // TODO
    // not bind with code
    // body code: expr __body is ??? default <code>

public:
    inline Proto() {}

    inline void putArgument(argument_t &&value) {
        arguments.push_back(value);
    }
};

class Block {
private:
    std::vector<field_t> memberType;
    std::vector<field_t> memberVar;
    std::vector<field_t> memberStatic;
    std::vector<field_t> memberExpr;
    std::vector<field_t> memberFast;

    std::multimap<name_t, field_t *> memberAll;
    std::multimap<name_t, field_t *> memberPublic;
    std::vector<field_t *> memberImport;

    Proto *proto; // could be null!

public:
    inline Block(): proto(nullptr) {}

    // virtual ~Block() {}

    inline void setProto(Proto *object) {
        proto = object;
    }

    inline void addField(field_t &&field) {
        //
    }
};

}

#endif
