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
    size_t tot_array;

    std::vector<llvm::Value *> value_tuple;
    llvm::Value *value;
    std::string value_name;

    inline void runField(const field_t &field) {
        type.clear();
        type_array.clear();
        type_array.push_back(1);
        runAll(field.code);

        if (type_array.size() == 1) {
            tot_array = 0;
        } else {
            tot_array = 1;
            for (size_t i: type_array) {
                tot_array *= i;
            }
        }
    }

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
        func = gen.func("block_" + std::to_string((size_t) env), v, "int");

        // !!!
        code_current = nullptr;

        // (fake) compile-time execution
        comptime = true;
        for (const field_t &i: env->getMemberVar()) {
            runField(i);
            std::cout << "var " << i.name.id << " [" << tot_array << "]\n";
            gen.identifier1(i.name.id, tot_array);
        }
        for (const field_t &i: env->getMemberFast()) {
            runField(i);
            std::cout << "fast " << i.name.id << " [" << tot_array << "]\n";
            gen.identifier1(i.name.id, tot_array);
        }
        for (const field_t &i: env->getMemberStatic()) {
            runField(i);
            std::cout << "static " << i.name.id << " [" << tot_array << "]\n";
            gen.globalIdentifier1(i.name.id, tot_array);
        }
        for (const field_t &i: env->getMemberType()) {
            runField(i);
        }
        // for (const field_t &i: env->getMemberExpr()) {
        //     runField(i);
        // }

        // TODO: M_VAR, M_OUT and M_TYPE arguments?

        // code generation
        comptime = false;
        runAll(env->queryAll("__code").first->second->code);
    }

    inline void runAll(Code *code) {
        value_tuple.clear();
        while (code) {
            value = nullptr;
            code->runVisit(this);
            if (value) {
                value_tuple.push_back(value);
            }
            code = code->getNext();
        }
    }

    virtual void run(CodeGet *code) {
        if (comptime) {
            type.push_back(code->getName().id);
        } else {
            value_name = code->getName().id;
            if (gen.nvt.find(value_name) != gen.nvt.end()) {
                value = gen.nvt[value_name];
            } else {
                value = nullptr;
            }
        }
    }

    virtual void run(CodeWith *code) {
        (void) code; // TODO
        throw error_t("not implemented");
    }

    virtual void run(CodeCall *code) {
        if (comptime) {
            if (code->isExec()) {
                type_array.push_back(
                    (
                        (CodeLiteral<long> *) code->getTarget()
                    )->getValue()
                );
                runAll(code->getArgument());
            } else {
                runAll(code->getTarget());
                runAll(code->getArgument());
            }
        } else {
            runAll(code->getTarget());
            if (value) {
                // TODO
            } else {
                std::string name = value_name;

                if (name == "__locate") {
                    runAll(code->getArgument());
                    gen.setInsertBlock(code_body[label_id]);
                } else if (name == "__goto") {
                    runAll(code->getArgument());
                    value = gen.unConditionJump(code_body[label_id]);
                } else if (name == "__branch") {
                    auto nextbr = gen.createBlock(
                        "tag__" + std::to_string((size_t) code),
                        func
                    );

                    runAll(code->getArgument());
                    value = gen.conditionJump(
                        value,
                        nextbr,
                        code_body[label_id]
                    );
                    gen.setInsertBlock(nextbr);
                } else if (name == "__assign") {
                    runAll(code->getArgument());
                    if (value_tuple.size() == 2) {
                        value = gen.setValue(value_tuple[0], value_tuple[1]);
                    }
                } else if (name == "__add") {
                    runAll(code->getArgument());
                    if (value_tuple.size() == 2) {
                        value = gen.expression('+', value_tuple[0], value_tuple[1]);
                    }
                } else if (name == "__sub") {
                    runAll(code->getArgument());
                    if (value_tuple.size() == 2) {
                        value = gen.expression('-', value_tuple[0], value_tuple[1]);
                    }
                } else if (name == "__less") {
                    runAll(code->getArgument());
                    if (value_tuple.size() == 2) {
                        value = gen.expression('<', value_tuple[0], value_tuple[1]);
                    }
                } else {
                    std::cout << "func: " << name << '\n';
                    runAll(code->getArgument());
                    value = gen.integerNum(233333); // place holder
                    // auto iter = env->queryAll(value_name);
                    // if (iter.first != iter.second) {
                    //     runAll(code->getArgument());

                    //     auto fx = gen.getFuncName("block_" + std::to_string(
                    //         (size_t) iter.first->second)
                    //     );

                    //     std::cout << fx;
                    //     gen.call(fx, value_tuple);
                    // }
                }

            }
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
                    "label__" + std::to_string(code_body.size()),
                    func
                )
            );
        }

        if (comptime) {
            label_id = code->getId();
            value = nullptr;
        } else {
            label_id = code->getId();
            value = nullptr;
            // // notice: this design is reserved (for runtime jump & continuation etc)
            // value = gen.integerNum((size_t) code->getId());
        }
    }

    virtual void run(CodeBlock *code) {
        if (comptime) {
            CodeVisitorTranslateLLVM(code->getBlock());
            // for inline anonymous class
            if (code_current) {
                gen.setInsertBlock(code_current);
            }
        } else {
            // CodeVisitorTranslateLLVM(code->getBlock());
            // // for inline anonymous class
            // if (code_current) {
            //     gen.setInsertBlock(code_current);
            // }
        }
    }
};

}

#endif
