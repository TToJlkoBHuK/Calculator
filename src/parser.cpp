#include "parser.h"
#include <cctype>

void flushVocab(std::string& vocab, std::vector<Vocab>& parser, const Register& registry) {
    if (vocab.empty()) {
        return;
    }

    if (isdigit(vocab[0]) || vocab[0] == '.') {
        parser.emplace_back(Type::NUM, vocab);
    }
    else {
        IOperator* op = registry.getOper(vocab);
        if (!op) {
            throw std::runtime_error("Unknown function '" + vocab + "'");
        }

        if (op->getAr() == 1) {
            parser.emplace_back(Type::UNA_OPR, vocab);
        }
        else {
            parser.emplace_back(Type::BIN_OPR, vocab);
        }
    }
    vocab.clear();
}

bool isUnaryContext(const std::vector<Vocab>& parser) {
    if (parser.empty()) {
        return true;
    }

    const Vocab& last = parser.back();
    if (last.type == Type::BRC && last.value == "(") {
        return true;
    }
    if (last.type == Type::BIN_OPR) {
        return true;
    }
    if (last.type == Type::UNA_OPR) {
        return true;
    }

    return false;
}

void processOperator(char c, std::vector<Vocab>& parser, const Register& registry) {
    std::string opStr(1, c);

    if (opStr == "-" && isUnaryContext(parser)) {
        parser.emplace_back(Type::UNA_OPR, "neg");
        return;
    }

    IOperator* op = registry.getOper(opStr);
    if (!op) {
        throw std::runtime_error("Unknown operator '" + opStr + "'");
    }

    if (op->getAr() == 1) {
        parser.emplace_back(Type::UNA_OPR, opStr);
    }
    else {
        parser.emplace_back(Type::BIN_OPR, opStr);
    }
}

std::vector<Vocab> parse(const std::string& s, const Register& registry) {
    std::vector<Vocab> parser;
    std::string vocab;
    int countBrc = 0;

    for (int i = 0; i < s.length(); ++i) {
        char c = s[i];

        if (isspace(c)) {
            flushVocab(vocab, parser, registry);
            continue;
        }

        if (!vocab.empty()) {
            bool vocabIsNum = isdigit(vocab[0]) || vocab[0] == '.';
            bool vocabIsFunc = isalpha(vocab[0]);
            bool charIsNum = isdigit(c) || c == '.';
            bool charIsFunc = isalpha(c);

            if ((vocabIsNum && !charIsNum) || (vocabIsFunc && !charIsFunc)) {
                flushVocab(vocab, parser, registry);
            }
        }

        if (isalpha(c) || isdigit(c) || c == '.') {
            vocab += c;
        }
        else if (c == '(') {
            flushVocab(vocab, parser, registry);
            countBrc++;
            parser.emplace_back(Type::BRC, "(");
        }
        else if (c == ')') {
            flushVocab(vocab, parser, registry);
            countBrc--;
            if (countBrc < 0) {
                throw std::runtime_error("Check ( )");
            }
            parser.emplace_back(Type::BRC, ")");
        }
        else {
            flushVocab(vocab, parser, registry);
            processOperator(c, parser, registry);
        }
    }

    flushVocab(vocab, parser, registry);

    if (countBrc != 0) {
        throw std::runtime_error("Mismatched parentheses");
    }
    return parser;
}

int Priority(const Vocab& voc, const Register& registry) {
    if (voc.type == Type::NUM || voc.type == Type::BRC) {
        return 0;
    }

    IOperator* op = registry.getOper(voc.value);
    if (!op) {
        throw std::runtime_error("[Priority] Unknown operation '" + voc.value + "'");
    }

    if (voc.value == "+" || voc.value == "-") {
        return 1;
    }
    if (voc.value == "*" || voc.value == "/") {
        return 2;
    }

    if (op->getAr() == 2) {
        return 3;
    }

    if (op->getAr() == 1) {
        return 4;
    }

    return 0;
}

std::vector<Vocab> Postfix(const std::vector<Vocab>& parser, const Register& registry) {
    std::stack<Vocab> stack;
    std::vector<Vocab> postfix;

    for (const Vocab& it : parser) {
        if (it.type == Type::BRC) {
            if (it.value == "(") {
                stack.push(it);
            }
            else if (it.value == ")") {
                while (!stack.empty() && stack.top().value != "(") {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                if (!stack.empty()) {
                    stack.pop();
                }
            }
        }
        else if (it.type == Type::NUM) {
            postfix.push_back(it);
        }
        else {
            if (stack.empty()) {
                stack.push(it);
            }
            else {
                while (!stack.empty() &&
                    Priority(it, registry) <= Priority(stack.top(), registry) &&
                    stack.top().value != "(") {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.push(it);
            }
        }
    }
    while (!stack.empty()) {
        postfix.push_back(stack.top());
        stack.pop();
    }
    return postfix;
}