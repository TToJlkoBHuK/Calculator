#include "Tree.h"

std::unique_ptr<Tree> popNode(std::vector<std::unique_ptr<Tree>>& stack, const std::string& opName) {
    if (stack.empty()) {
        throw std::runtime_error("Invalid expression: missing operand for " + opName);
    }
    std::unique_ptr<Tree> node = std::move(stack.back());
    stack.pop_back();
    return node;
}

void processBinary(const Vocab& v, IOperator* op, std::vector<std::unique_ptr<Tree>>& stack) {
    if (op->getAr() != 2) {
        throw std::runtime_error("Error " + v.value + " is not binary");
    }

    auto node = std::make_unique<Tree>(v, op);

    node->right = popNode(stack, v.value);
    node->left = popNode(stack, v.value);

    stack.push_back(std::move(node));
}

void processUnary(const Vocab& v, IOperator* op, std::vector<std::unique_ptr<Tree>>& stack) {
    if (op->getAr() != 1) {
        throw std::runtime_error("Error " + v.value + " is not unary");
    }

    auto node = std::make_unique<Tree>(v, op);

    node->left = nullptr;
    node->right = popNode(stack, v.value);

    stack.push_back(std::move(node));
}

std::unique_ptr<Tree> build(std::vector<Vocab>& vocabs, const Register& registry) {
    std::vector<std::unique_ptr<Tree>> stack;

    for (auto const& Vocab : vocabs) {
        if (Vocab.type == Type::NUM) {
            stack.push_back(std::make_unique<Tree>(Vocab));
        }
        else if (Vocab.type == Type::BIN_OPR) {
            IOperator* op = registry.getOper(Vocab.value);
            if (!op) throw std::runtime_error("Unknown operation " + Vocab.value);
            processBinary(Vocab, op, stack);
        }
        else if (Vocab.type == Type::UNA_OPR) {
            IOperator* op = registry.getOper(Vocab.value);
            if (!op) throw std::runtime_error("Unknown operation " + Vocab.value);
            processUnary(Vocab, op, stack);
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid expression: unbalanced stack");
    }

    return std::move(stack.back());
}

double evaluate(const std::unique_ptr<Tree> top) {
    if (top->value.type == Type::NUM) {
        try {
            return stod(top->value.value);
        }
        catch (std::invalid_argument e) {
            throw std::runtime_error(e.what());
        }
        catch (std::out_of_range e) {
            throw std::runtime_error(e.what());
        }
    }

    if (top->operation) {
        std::vector<double> args;

        if (top->value.type == Type::BIN_OPR) {
            args.push_back(evaluate(std::move(top->left)));
            args.push_back(evaluate(std::move(top->right)));
        }
        else if (top->value.type == Type::UNA_OPR) {
            args.push_back(evaluate(std::move(top->right)));
        }
        return top->operation->execute(args);
    }

    throw std::runtime_error("Unknown tree node type");
}