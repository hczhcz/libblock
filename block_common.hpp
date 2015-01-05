#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

// forward
class CodeVisitor;
class Block;

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

    // destructable (public)
    virtual ~Code() {
        if (next) {
            delete next;
        }
    }

    inline Code *getNext() const {
        return next;
    }

    virtual Code *runVisit(CodeVisitor *visitor) = 0;

    // virtual void repr(std::ostream &stream, size_t indent = 0) const = 0;
};

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

    inline field_t(field_t &&source):
        mode(source.mode), import(source.import), hidden(source.hidden),
        name(source.name), code(source.code) {
            source.code = nullptr;
        }

    // simple destruction
    inline ~field_t() {
        delete code;
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

    virtual ~Block() {
        if (proto) {
            delete proto;
        }
    }

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
