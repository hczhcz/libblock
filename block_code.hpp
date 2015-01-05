#ifndef BLOCK_CODE_HPP
#define BLOCK_CODE_HPP

#include "block_common.hpp"

namespace libblock {

class CodeGet;
class CodeWith;
class CodeCall;
template <class T> class CodeLiteral;
class CodeLabel;
class CodeRef;
class CodeBlock;

class CodeVisitor {
public:
    virtual Code *run(CodeGet *code) = 0;
    virtual Code *run(CodeWith *code) = 0;
    virtual Code *run(CodeCall *code) = 0;
    virtual Code *run(CodeLiteral<double> *code) = 0;
    virtual Code *run(CodeLiteral<long> *code) = 0;
    virtual Code *run(CodeLiteral<char> *code) = 0;
    virtual Code *run(CodeLabel *code) = 0;
    virtual Code *run(CodeRef *code) = 0;
    virtual Code *run(CodeBlock *code) = 0;
};

class CodeGet: public Code {
private:
    name_t name;

public:
    inline CodeGet(name_t &&to_name): name(std::move(to_name)) {}

    virtual ~CodeGet() {}

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
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

    virtual ~CodeWith() {
        if (target) {
            delete target;
        }
        if (action) {
            delete action;
        }
    }

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

class CodeCall: public Code {
private:
    Code *target;
    Code *arg;
    bool exec;

public:
    inline CodeCall(
        Code *to_target, Code *to_arg, bool to_exec
    ): target(to_target), arg(to_arg), exec(to_exec) {}

    virtual ~CodeCall() {
        if (target) {
            delete target;
        }
        if (arg) {
            delete arg;
        }
    }

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
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

    virtual ~CodeLiteral() {}

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

class CodeLabel: public Code {
public:
    inline CodeLabel() {}

    virtual ~CodeLabel() {}

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

class CodeRef: public Code {
private:
    Code *code; // notice: weak reference!!!

public:
    inline CodeRef(Code *to_code): code(to_code) {}

    virtual ~CodeRef() {
        // no delete
    }

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

class CodeBlock: public Code {
private:
    Block *block;

public:
    inline CodeBlock(Block *to_block): block(to_block) {}

    virtual ~CodeBlock() {
        delete block;
    }

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

}

#endif
