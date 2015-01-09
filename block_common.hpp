#ifndef BLOCK_COMMON_HPP
#define BLOCK_COMMON_HPP

#include <string>
#include <vector>
#include <map>

namespace libblock {

struct error_t {
    std::string info;

    inline error_t(std::string &&to_info): info(std::move(to_info)) {}
};

// forward
class CodeVisitor;
class Block;

class Code {
private:
    Code *next;

public:
    Block *__type;

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

    virtual void runVisit(CodeVisitor *visitor) = 0;
};

struct name_t {
    std::string id;

    inline name_t(std::string &&to_id): id(std::move(to_id)) {}

    inline name_t(const std::string &to_id): id(to_id) {}
};

struct argument_t {
    enum Mode {
        M_TYPE,
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

    Block *__type;

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

class BlockInstance {
private:
    Block *block;
    std::vector<BlockInstance *> arguments;

public:
    inline BlockInstance(Block *to_block): block(to_block) {}

    inline void putArgument(BlockInstance *value) {
        arguments.push_back(value);
    }

    inline const std::vector<BlockInstance *> &getArguments() const {
        return arguments;
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

    std::vector<argument_t> arguments;
    std::vector<BlockInstance *> instances; // TODO

public:
    using QueryIter =
        std::multimap<std::string, field_t *>::const_iterator;

    using QueryPair =
        std::pair<QueryIter, QueryIter>;

    inline Block() {}

    virtual ~Block() {
        for (BlockInstance *iter: instances) {
            delete iter;
        }
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
            throw libblock::error_t("internal error");
        }

        memberAll.insert({ptr->name.id, ptr});
        if (!ptr->hidden) {
            memberPublic.insert({ptr->name.id, ptr});
        }
        if (ptr->import) {
            memberImport.push_back(ptr);
        }
    }

    inline void addArgument(argument_t &&value) {
        arguments.push_back(value);
    }

    inline void finish() {
        // TODO
    }

    const std::vector<field_t> &getMemberType() const {
        return memberType;
    }

    const std::vector<field_t> &getMemberExpr() const {
        return memberExpr;
    }

    const std::vector<field_t> &getMemberVar() const {
        return memberVar;
    }

    const std::vector<field_t> &getMemberStatic() const {
        return memberStatic;
    }

    const std::vector<field_t> &getMemberFast() const {
        return memberFast;
    }

    const std::multimap<std::string, field_t *> &getMemberAll() const {
        return memberAll;
    }

    const std::multimap<std::string, field_t *> &getMemberPublic() const {
        return memberPublic;
    }

    const std::vector<field_t *> &getMemberImport() const {
        return memberImport;
    }

    inline QueryPair queryAll(const std::string &key) const {
        return memberAll.equal_range(key);
    }

    inline QueryPair queryPublic(const std::string &key) const {
        return memberPublic.equal_range(key);
    }

    inline const std::vector<argument_t> &getArguments() const {
        return arguments;
    }
};

}

#endif
