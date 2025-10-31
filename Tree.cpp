#include "Tree.h"

std::unique_ptr<Tree> build(std::vector<Vocab>& vocabs, const Register& registry) {
	std::vector<std::unique_ptr<Tree>> stack;

	for (auto const& Vocab : vocabs) {
		if (Vocab.type == Type::NUM) {
			stack.push_back(std::make_unique<Tree>(Vocab));
		}

		else if (Vocab.type == Type::BIN_OPR || Vocab.type == Type::UNA_OPR) {

			IOperator* op = registry.getOper(Vocab.value);
			if (!op) {
				throw std::runtime_error("Unknow operation " + Vocab.value);
			}

			if (Vocab.type == Type::BIN_OPR && op->getAr() != 2) {
				throw std::runtime_error("Error " + Vocab.value + " is not binary");
			}
			if (Vocab.type == Type::UNA_OPR && op->getAr() != 1) {
				throw std::runtime_error("Error " + Vocab.value + " is not unary");
			}

			std::unique_ptr<Tree> node = std::make_unique<Tree>(Vocab, op);

			if (Vocab.type == Type::BIN_OPR) {
				std::unique_ptr<Tree> right;
				if (!stack.empty()) {
					right = std::move(stack.back());
					stack.pop_back();
				}
				else { throw std::runtime_error("Invalid expression"); }

				std::unique_ptr<Tree> left;
				if (!stack.empty()) {
					left = std::move(stack.back());
					stack.pop_back();
				}
				else { throw std::runtime_error("Invalid expression"); }

				node->left = std::move(left);
				node->right = std::move(right);
			}
			else if (Vocab.type == Type::UNA_OPR) {
				std::unique_ptr<Tree> right;
				if (!stack.empty()) {
					right = std::move(stack.back());
					stack.pop_back();
				}
				else { throw std::runtime_error("Invalid expression"); }

				node->left = nullptr;
				node->right = std::move(right);
			}
			stack.push_back(std::move(node));
		}
		
	}

	return std::move(stack.back());
}

double evaluate(const std::unique_ptr<Tree> top) {
	if (top->value.type == Type::NUM) {
		try {
			return stod(top->value.value);
		}
		catch (std::invalid_argument e) { throw std::runtime_error(e.what()); }
		catch (std::out_of_range e) { throw std::runtime_error(e.what()); }
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