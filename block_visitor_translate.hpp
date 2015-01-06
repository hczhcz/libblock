#ifndef BLOCK_VISITOR_TRANSLATE_HPP
#define BLOCK_VISITOR_TRANSLATE_HPP

#include "block_code.hpp"

namespace libblock {

class CodeVisitorTranslate: public CodeVisitor {
private:
    Block *env;
    std::ostream &out;

public:
    CodeVisitorTranslate(
        Block *to_env, std::ostream &to_out
    ): env(to_env), out(to_out) {
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
        out << code->getId();
    }

    virtual void run(CodeBlock *code) {
        (void) code; // TODO
    }
};

}

#endif
