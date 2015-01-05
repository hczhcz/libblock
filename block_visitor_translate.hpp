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

    virtual Code *runAll(Code *code) {
        //
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
