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
    virtual Code *runAll(Code *code) = 0;
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

    inline const name_t &getName() const {
        return name;
    }

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

    inline Code *getTarget() const {
        return target;
    }

    inline Code *getAction() const {
        return action;
    }

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

class CodeCall: public Code {
private:
    Code *target;
    Code *argument;
    bool exec;

public:
    inline CodeCall(
        Code *to_target, Code *to_argument, bool to_exec
    ): target(to_target), argument(to_argument), exec(to_exec) {}

    virtual ~CodeCall() {
        if (target) {
            delete target;
        }
        if (argument) {
            delete argument;
        }
    }

    inline Code *getTarget() const {
        return target;
    }

    inline Code *getArgument() const {
        return argument;
    }

    inline bool isExec() const {
        return exec;
    }

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

template <class T> // T must be a simple type (integer, float, ...)
class CodeLiteral: public Code {
private:
    T value;

public:
    inline CodeLiteral(T &&to_value): value(std::move(to_value)) {}

    // inline CodeLiteral(const T &to_value): value(to_value) {}

    virtual ~CodeLiteral() {}

    inline T getValue() const {
        return value;
    }

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

    inline Code *getCode() const {
        return code;
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

    inline Block *getBlock() const {
        return block;
    }

    virtual Code *runVisit(CodeVisitor *visitor) {
        return visitor->run(this);
    }
};

}

#endif
