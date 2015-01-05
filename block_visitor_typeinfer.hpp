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

    virtual Code *run(CodeGet *code) {
        //
    }

    virtual Code *run(CodeWith *code) {
        //
    }

    virtual Code *run(CodeCall *code) {
        //
    }

    virtual Code *run(CodeLiteral<double> *code) {
        //
    }

    virtual Code *run(CodeLiteral<long> *code) {
        //
    }

    virtual Code *run(CodeLiteral<char> *code) {
        //
    }

    virtual Code *run(CodeLabel *code) {
        //
    }

    virtual Code *run(CodeRef *code) {
        //
    }

    virtual Code *run(CodeBlock *code) {
        //
    }
};

}

#endif
