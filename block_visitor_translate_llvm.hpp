#ifndef BLOCK_VISITOR_TRANSLATE_LLVM_HPP
#define BLOCK_VISITOR_TRANSLATE_LLVM_HPP

#include "block_code.hpp"
#include "../LLVM-Simple-Wrapper/llvmgenerator.h"

namespace libblock {

/*

temporary ver for SJTU SE302 homework

this is a prototype version of the code generator
it have:
    a simple type system (POD only, no casting, no templates)
    LLVM code generator
    support of var/fast/static variables
    built-in operators

we will rewrite the class "CodeVisitorTranslateLLVM" in the future with:
    a JIT-exec-driven type checking/inference system
    support of templates (via "BlockInstance") and function overloading
    actual "of arg" semantic (create a frame) and "yield" semantic

*/

class CodeVisitorTranslateLLVM: public CodeVisitor {
private:
    Block *env;
    llvmgenerator::LLVMGenerator &gen;

    bool comptime;

    llvm::Function *func;

    llvm::BasicBlock *code_current;
    std::vector<llvm::BasicBlock *> code_body;
    size_t label_id;

    // notice: as JIT-based compile time execution (class template) is not implemented,
    //         represent the type in a naive way
    std::vector<std::string> type;
    std::vector<size_t> type_array;

    std::vector<llvm::Value *> value_tuple;
    llvm::Value *value;

public:
    CodeVisitorTranslateLLVM(
        Block *to_env
    ): env(to_env), gen(llvmgenerator::instance()), comptime(false) {
        std::vector<std::string> v;

        for (const argument_t &i: env->getArguments()) {
            v.push_back(i.name.id);
        }
        // implemented integer-like argument only
        // pass by value ("var" and "out" semantics are not implemented)
        func = gen.func("Block_" + std::to_string((size_t) env), v, "int");

        // (fake) compile-time execution
        comptime = true;
        for (const field_t &i: env->getMemberVar()) {
            gen.identifier(i.name.id);
        }
        for (const field_t &i: env->getMemberFast()) {
            gen.identifier(i.name.id);
        }
        for (const field_t &i: env->getMemberStatic()) {
            gen.identifier(i.name.id);
        }
        // TODO: M_TYPE and M_EXPR fields? M_VAR, M_OUT and M_TYPE arguments?

        // code generation
        comptime = false;
        runAll(env->queryAll("__code").first->second->code);
    }

    inline void runAll(Code *code) {
        if (code) {
            code->runVisit(this);
            value_tuple.push_back(value);
            runAll(code->getNext());
        }
    }

    virtual void run(CodeGet *code) {
        if (comptime) {
            type.push_back(code->getName().id);
        } else {
            value = gen.nvt[code->getName().id];
        }
    }

    virtual void run(CodeWith *code) {
        (void) code; // TODO
        throw error_t("not implemented");
    }

    virtual void run(CodeCall *code) {
        if (comptime) {
            if (code->isExec()) {
                runAll(code->getTarget());
                runAll(code->getArgument());
            } else {
                type_array.push_back(
                    (
                        (CodeLiteral<long> *) code->getTarget()
                    )->getValue()
                );
                runAll(code->getArgument());
            }
        } else {
            runAll(code->getTarget());
            runAll(code->getArgument());
        }
        // llvm::BasicBlock *createBlock(const std::string &name, llvm::Function *func);
        // void setInsertBlock(llvm::BasicBlock *block);// need to set the block to insert the code

        // (void) code; // TODO
    }

    virtual void run(CodeLiteral<double> *code) {
        if (comptime) {
            throw error_t("bad type");
        } else {
            value = gen.doubleNum(code->getValue());
        }
    }

    virtual void run(CodeLiteral<long> *code) {
        if (comptime) {
            throw error_t("size should be before typename");
        } else {
            value = gen.integerNum(code->getValue());
        }
    }

    virtual void run(CodeLiteral<char> *code) {
        if (comptime) {
            throw error_t("bad type");
        } else {
            value = gen.integerNum(code->getValue()); // ???
        }
    }

    virtual void run(CodeLabel *code) {
        // allocate blocks
        while (code_body.size() <= code->getId()) {
            code_body.push_back(
                gen.createBlock(
                    "SubBlock__" + std::to_string(code_body.size()),
                    func
                )
            );
        }

        if (comptime) {
            label_id = code->getId();
            value = nullptr;
        } else {
            // notice: this design is reserved (for runtime jump & continuation etc)
            value = gen.integerNum((size_t) code->getId());
        }
    }

    virtual void run(CodeBlock *code) {
        if (comptime) {
            CodeVisitorTranslateLLVM(code->getBlock());
            gen.setInsertBlock(code_current);
        } else {
            // notice: this design is reserved (for RTTI etc)
            value = gen.integerNum((size_t) code->getBlock());
        }
    }
};

}

#endif
