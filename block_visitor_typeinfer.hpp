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

    virtual void runAll(Code *code) {
        (void) code; // TODO
    }

    virtual void run(CodeGet *code) {
        (void) code; // TODO
    }

    virtual void run(CodeWith *code) {
        (void) code; // TODO
    }

    virtual void run(CodeCall *code) {
        (void) code; // TODO
    }

    virtual void run(CodeLiteral<double> *code) {
        (void) code; // TODO
    }

    virtual void run(CodeLiteral<long> *code) {
        (void) code; // TODO
    }

    virtual void run(CodeLiteral<char> *code) {
        (void) code; // TODO
    }

    virtual void run(CodeLabel *code) {
        (void) code; // TODO
    }

    virtual void run(CodeRef *code) {
        (void) code; // TODO
    }

    virtual void run(CodeBlock *code) {
        (void) code; // TODO
    }
};

}

#endif
