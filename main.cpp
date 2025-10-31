#include <iostream>
#include <string>
#include "parser.h"
#include "vocabulary.h"
#include "Tree.h"
#include "Register.h"
#include "includes/ErrorMessage.h"
#include "includes/ErrorMessage.cpp"

int main() {

	Register registry;

	registry.loadPlug();

	std::string input;

	while (true) {
		try {
			std::cin >> input;
			if (input == "exit") {
				std::cout << "EXIT" << std::endl;
				return 0;
			}
			std::vector<Vocab> vocabs = parse(input, registry);
			std::vector<Vocab> postvocabs = Postfix(vocabs, registry);

			std::unique_ptr<Tree> top = build(postvocabs, registry);
			double result = evaluate(std::move(top));

			std::cout << result << std::endl;
		}
		catch (std::runtime_error e) {
			std::cerr << e.what() << std::endl;
		}
		catch (ErrorMessage e) {
			std::cerr << e.getMsg() << std::endl;
		}
	}
}