#include <iostream>
#include "Lexer.h"
#include "Parser.h"

#include <tclap/CmdLine.h>
#include <fstream>

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

	Thought::Lexer l(text);
	// Parser parser(l);
	l.reset();
	while(l.has_next()) {
		// try
		// {((-3) ^ 6) * (9 + (4 + float(7)))) / 54
			std::cout << to_string(l.next()) << std::endl;
		// } catch(LexerError e) {
		// 	std::cerr << e.what() << std::endl;
		// }
	}
}