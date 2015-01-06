#ifndef BLOCK_VISITOR_INFER_HPP
#define BLOCK_VISITOR_INFER_HPP

#include "block_code.hpp"

namespace libblock {

class CodeVisitorInfer: public CodeVisitor {
private:
    Block *env;

    bool ok;

public:
    CodeVisitorInfer(
        Block *to_env
    ): env(to_env) {
        //
    }

    virtual void runAll(Code *code) {
        if (code) {
            code->runVisit(this);
            runAll(code->getNext());
        }
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

    virtual void run(CodeBlock *code) {
        (void) code; // TODO
    }
};

}

#endif
