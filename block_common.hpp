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
    static inline Code *pack(Code *&left, Code *right) {
        if (!left) {
            left = right;
        } else {
            pack(left->next, right);
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

class CodeRef: public Code {
private:
    Code *code; // notice: weak reference!!!

public:
    inline CodeRef(Code *to_code): code(to_code) {}

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

public:
    inline Proto() {}

    inline void putArgument(argument_t &&value) {
        arguments.push_back(value);
    }
};

class Block {
private:
    std::vector<field_t> memberType;
    std::vector<field_t> memberExpr;
    std::vector<field_t> memberVar;
    std::vector<field_t> memberStatic;
    std::vector<field_t> memberFast;

    std::multimap<std::string, field_t *> memberAll;
    std::multimap<std::string, field_t *> memberPublic;
    std::vector<field_t *> memberImport;

    Proto *proto; // could be null!

public:
    inline Block(): proto(nullptr) {}

    // virtual ~Block() {}

    inline void setProto(Proto *object) {
        proto = object;
    }

    inline void addField(field_t &&field) {
        field_t *ptr;

        switch (field.mode) {
        case field_t::M_TYPE:
            memberType.push_back(std::move(field));
            ptr = &memberType.back();
            break;
        case field_t::M_EXPR:
            memberExpr.push_back(std::move(field));
            ptr = &memberExpr.back();
            break;
        case field_t::M_VAR:
            memberVar.push_back(std::move(field));
            ptr = &memberVar.back();
            break;
        case field_t::M_STATIC:
            memberStatic.push_back(std::move(field));
            ptr = &memberStatic.back();
            break;
        case field_t::M_FAST:
            memberFast.push_back(std::move(field));
            ptr = &memberFast.back();
            break;
        default:
            // never reach
            throw;
        }

        memberAll.insert({ptr->name.id, ptr});
        if (!ptr->hidden) {
            memberPublic.insert({ptr->name.id, ptr});
        }
        if (ptr->import) {
            memberImport.push_back(ptr);
        }
    }

    inline void finish() {
        // TODO
    }
};

}

#endif
