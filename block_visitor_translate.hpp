#ifndef BLOCK_VISITOR_TRANSLATE_HPP
#define BLOCK_VISITOR_TRANSLATE_HPP

#include "block_common.hpp"

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

    virtual void run(CodeGet *code) {
        //
    }

    virtual void run(CodeWith *code) {
        //
    }

    virtual void run(CodeCall *code) {
        //
    }

    template <class T>
    virtual void run(CodeLiteral *code) {
        //
    }

    virtual void run(CodeLabel *code) {
        //
    }

    virtual void run(CodeRef *code) {
        //
    }

    virtual void run(CodeBlock *code) {
        //
    }
};

}

#endif
