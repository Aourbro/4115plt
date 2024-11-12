#include <iostream>
#include <memory>
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{
	int ret = -1;

	Lexer lexer;
	Parser parser;

	std::vector<std::pair<TokenClass, std::string> > tokenStream;
	std::unique_ptr<BaseAST> ast;

	std::string inputString;
	std::string inputFile;
	std::string outputFile;

	std::ifstream iFile;

	for (int i = 1; i < argc; ) {
		if (std::string(argv[i]).compare("--help") == 0) {
			goto HELP;
		}
		if (std::string(argv[i]).compare("-s") == 0) {
			++i;
			inputString = argv[i];
			++i;
		} else if (std::string(argv[i]).compare("-f") == 0) {
			++i;
			inputFile = argv[i];
			++i;
		} else if (std::string(argv[i]).compare("-o") == 0) {
			++i;
			outputFile = argv[i];
			++i;
		} else {
			goto HELP;
		}
	}
	
	if (!inputFile.empty()) {
		iFile.open(inputFile, std::ios::in);
		ret = lexer.tokenize(iFile);
	} else if (!inputString.empty()) {
		ret = lexer.tokenize(inputString);
	} else {
		goto HELP;
	}
	if (ret != Error::Success) {
		printf("Error: %s\n", dumpError(Error(ret)).c_str());
		goto HELP;
	}
	
	lexer.printTokens();
	tokenStream = lexer.getStream();
	tokenStream.push_back(std::make_pair(TokenClass::EOS, "$"));

	ast = parser.parse(tokenStream);
	if (parser.getSuccess()) {
		ast->Dump("");
	} else {
		goto HELP;
	}
	return 0;

HELP:
	std::cout << "Usage: ./main [options]\n" <<
		   		 "Options:\n" <<
				 "  --help                 Display this information.\n" <<
			 	 "  -s <string>            Take the <string> as input LaTeX expression.\n" <<
				 "  -f <file>              Take content in the <file> as input.\n" <<
				 "  -o <file>              Place the output into <file>.\n" <<
				 "Option -f has higher priority than -s\n";
	return 0;
}
