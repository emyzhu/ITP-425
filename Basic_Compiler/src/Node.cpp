#include "Node.h"

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.emplace_back(statement);
}

void NData::AddDecl(NDecl* decl)
{
	mDecls.emplace_back(decl);
}


void CodeContext::AddInstruction(CodeContext& context, std::string command, std::vector<std::string> param) {
    std::pair<std::string, std::vector<std::string>> i = std::make_pair(command, param);
    context.instructions.emplace_back(i);

}
