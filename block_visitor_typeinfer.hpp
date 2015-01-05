#ifndef BLOCK_VISITOR_TYPEINFER_HPP
#define BLOCK_VISITOR_TYPEINFER_HPP

#include "block_code.hpp"

namespace libblock {

class CodeVisitorTypeInfer: public CodeVisitor {
private:
    Block *env;

public:
    CodeVisitorTypeInfer(
        Block *to_env
    ): env(to_env) {
        //
    }

    virtual Code *runAll(Code *code) {
        return code; // TODO
    }

    virtual Code *run(CodeGet *code) {
        return code; // TODO
    }

    virtual Code *run(CodeWith *code) {
        return code; // TODO
    }

    virtual Code *run(CodeCall *code) {
        return code; // TODO
    }

    virtual Code *run(CodeLiteral<double> *code) {
        return code; // TODO
    }

    virtual Code *run(CodeLiteral<long> *code) {
        return code; // TODO
    }

    virtual Code *run(CodeLiteral<char> *code) {
        return code; // TODO
    }

    virtual Code *run(CodeLabel *code) {
        return code; // TODO
    }

    virtual Code *run(CodeRef *code) {
        return code; // TODO
    }

    virtual Code *run(CodeBlock *code) {
        return code; // TODO
    }
};

}

#endif
