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
