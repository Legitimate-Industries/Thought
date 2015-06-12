#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include <lib/VM.h>

#include <tclap/CmdLine.h>
#include <fstream>

using namespace Thought;

// #include "nodes/Nodes.h"
int main(int argc, char* argv[]) {
	std::string file_name = "";
	try {
		TCLAP::CmdLine cmd("Dorma Engine", ' ', "0.1");
		TCLAP::ValueArg<std::string> fileArg("f","file","File to read", true, "", "string", cmd);
		cmd.parse(argc, argv);

		file_name = fileArg.getValue();
	} catch (TCLAP::ArgException &e)
	{ std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl; }

	std::string text = "";
	std::string line;
	std::ifstream file(file_name);
	while(file.good()) {
		getline(file, line);
		text += line + "\n";
	}
	file.close();

	Lexer l(text);
	Parser parser(l);
	auto node = parser.parseNode();
	node->print(std::cout);
	std::cout << std::endl;
	l.reset();
	while(l.has_next()) {
		std::cout << to_string(l.next()) << std::endl;
	}

	// VM test from here... (after the parser is finished, handthe resulting Ref<Block> to the VM)
	Ref<Block> testBlock(new Block);
	testBlock->push_inst_ad(OP_PUSHC, 0, 0);
	testBlock->push_inst_ad(OP_RETURN, 0, 0);

	VM vm;
	auto ref1 = vm.createDouble(3);
	ValueHandle ref2;
	{
		ref2 = vm.createString(std::string("Maine").c_str());
		testBlock->add_constant(ref2);
	}
	vm.callBlock(testBlock, 0);
	vm.dump();

	{
		std::cout << ref2.as_string() << std::endl;
	}
}