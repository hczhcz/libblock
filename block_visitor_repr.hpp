#ifndef BLOCK_VISITOR_REPR_HPP
#define BLOCK_VISITOR_REPR_HPP

#include "block_code.hpp"

namespace libblock {

class CodeVisitorRepr: public CodeVisitor {
private:
    Block *env;
    std::ostream &out;
    size_t indent;

    inline void putIndent() {
        out << '\n';
        for (size_t i = 0; i < indent; ++i) {
            out << "    ";
        }
    }

    inline void putField(const field_t &field) {
        putIndent();
        out << field.name.id;

        if (field.import) {
            out << " import";
        }

        if (field.hidden) {
            out << " hidden";
        }

        out << ' ' << (void *) field.code << " := ";
        runAll(field.code);
    }

    inline void putFields(const std::vector<field_t> &fields) {
        ++indent;
        for (const field_t &i: fields) {
            putField(i);
        }
        --indent;
    }

public:
    CodeVisitorRepr(
        Block *to_env, std::ostream &to_out, size_t to_indent = 0
    ): env(to_env), out(to_out), indent(to_indent) {
        out << '{';
        ++indent;

        if (env->proto) {
            putIndent();
            out << "arguments:";

            ++indent;
            for (const argument_t &i: env->proto->getArguments()) {
                putIndent();

                switch (i.mode) {
                case argument_t::Mode::M_IN:
                    out << "in ";
                    break;
                case argument_t::Mode::M_OUT:
                    out << "out ";
                    break;
                case argument_t::Mode::M_VAR:
                    out << "var ";
                    break;
                default:
                    // never reach
                    break;
                }

                out << i.name.id;
            }
            --indent;
        }

        putIndent();
        out << "type:";
        putFields(env->memberType);

        putIndent();
        out << "expr:";
        putFields(env->memberExpr);

        putIndent();
        out << "var:";
        putFields(env->memberVar);

        putIndent();
        out << "static:";
        putFields(env->memberStatic);

        putIndent();
        out << "fast:";
        putFields(env->memberFast);

        --indent;
        putIndent();
        out << '}';
    }

    virtual Code *runAll(Code *code) {
        if (code) {
            if (Code *next = code->getNext()) {
                out << '(';

                ++indent;
                putIndent();
                code->runVisit(this);

                while (next) {
                    out << ',';

                    putIndent();
                    next->runVisit(this);
                    next = next->getNext();
                }

                out << ')';
                --indent;
            } else {
                code->runVisit(this);
            }
        } else {
            out << '(' << ')';
        }

        return nullptr;
    }

    virtual Code *run(CodeGet *code) {
        out << code->getName().id;

        return nullptr;
    }

    virtual Code *run(CodeWith *code) {
        runAll(code->getTarget());
        out << '.';
        runAll(code->getAction());

        return nullptr;
    }

    virtual Code *run(CodeCall *code) {
        out << '<';
        runAll(code->getTarget());
        out << (code->isExec() ? " " : " of ");
        runAll(code->getArgument());
        out << '>';

        return nullptr;
    }

    virtual Code *run(CodeLiteral<double> *code) {
        out << code->getValue() << 'd';

        return nullptr;
    }

    virtual Code *run(CodeLiteral<long> *code) {
        out << code->getValue();

        return nullptr;
    }

    virtual Code *run(CodeLiteral<char> *code) {
        out << '\'' << code->getValue() << '\'';

        return nullptr;
    }

    virtual Code *run(CodeLabel *code) {
        out << '@' << (void *) code;

        return nullptr;
    }

    virtual Code *run(CodeRef *code) {
        out << '&' << (void *) code->getCode();

        return nullptr;
    }

    virtual Code *run(CodeBlock *code) {
        (void) CodeVisitorRepr(code->getBlock(), out, indent);

        return nullptr;
    }
};

}

#endif
