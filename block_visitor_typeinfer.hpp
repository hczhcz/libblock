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

    virtual void run(CodeGet *code) {
        //
    }

    virtual void run(CodeWith *code) {
        //
    }

    virtual void run(CodeCall *code) {
        //
    }

    // template <class T>
    // virtual void run(CodeLiteral *code) {
    //     //
    // }

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
