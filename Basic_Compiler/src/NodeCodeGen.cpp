#include "Node.h"
#include "parser.hpp"
#include <iostream>

void NBlock::CodeGen(CodeContext& context)
{
    for (int i = 0; i < mStatements.size(); ++i) {
        mStatements[i]->CodeGen(context);
    }
}

void NData::CodeGen(CodeContext& context)
{
    for (int i = 0; i < mDecls.size(); ++i) {
        mDecls[i]->CodeGen(context);
    }

}

void NProgram::CodeGen(CodeContext& context)
{
    mData->CodeGen(context);
    mMain->CodeGen(context);
    
    std::vector<std::string> param;
    
    context.AddInstruction(context, "exit", param);
    
}

void NNumeric::CodeGen(CodeContext& context)
{
}

void NVarDecl::CodeGen(CodeContext& context)
{
    std::vector<std::string> param = {"r0"};
    context.AddInstruction(context, "push", param);
    context.indicies.emplace_back(std::make_pair(mName, context.lastRegUsed));
}

void NArrayDecl::CodeGen(CodeContext& context)
{
    std::vector<std::string> param = {"r0"};
    for (int i = 0; i < mSize->GetValue(); ++i) {
        context.AddInstruction(context, "push", param);
        context.indicies.emplace_back(std::make_pair(mName, context.lastRegUsed));
    }
}

void NNumericExpr::CodeGen(CodeContext& context)
{
    mResultRegister = "%" + std::to_string(context.lastRegUsed);
    std::vector<std::string> param = {mResultRegister, std::to_string(mNumeric->GetValue())};
    context.AddInstruction(context, "movi", param);
    context.lastRegUsed += 1;

}

void NVarExpr::CodeGen(CodeContext& context)
{
    mResultRegister = "%" + std::to_string(context.lastRegUsed);
       
    int idx;
    for (auto it = context.indicies.begin(); it != context.indicies.end(); ++it) {
        if (it->first == mName) {
            idx = std::distance(context.indicies.begin(), it);
        }
    }
    
//    std::string n = mName;
//    auto itr = std::find(context.indicies.begin(), context.indicies.end(),
//        [&n](const std::pair<std::string, int>& element){
//        return element.first == n;
//    });
//    auto idx = std::distance(context.indicies.begin(), itr);

    std::vector<std::string> param = {mResultRegister, std::to_string(idx)};
    context.AddInstruction(context, "loadi", param);
    context.lastRegUsed += 1;
}

void NBinaryExpr::CodeGen(CodeContext& context)
{
    mLhs->CodeGen(context);
    mRhs->CodeGen(context);
    
    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed), mLhs->GetResultRegister(), mRhs->GetResultRegister()};
    
    std::string binaryOp;
    
    mResultRegister = "%" + std::to_string(context.lastRegUsed);
    
    switch(mType) {
        case 270:
            binaryOp = "add";
        break;
        case 271:
            binaryOp = "sub";
        break;
        case 272:
            binaryOp = "mul";
        break;
        case 273:
            binaryOp = "div";
        break;
    }
    
    context.lastRegUsed += 1;
    
    context.AddInstruction(context, binaryOp, param);
}

void NArrayExpr::CodeGen(CodeContext& context)
{
    mSubscript->CodeGen(context);
    
    int base = -1;
    for (auto it = context.indicies.begin(); it != context.indicies.end(); ++it) {
        if (it->first == mName && base == -1) {
            base = std::distance(context.indicies.begin(), it);
        }
    }
    
//    std::string n = mName;
//    auto itr = std::find_if(context.indicies.begin(), context.indicies.end(),
//        [&n](const std::pair<std::string, int>& element){
//        return element.first == n;
//    });
//    auto base = std::distance(context.indicies.begin(), itr);

    
        
    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed), std::to_string(base)};
    context.AddInstruction(context, "movi", param);
    
    context.lastRegUsed += 1;
    
    std::vector<std::string> param2 = {"%" + std::to_string(context.lastRegUsed), "%" + std::to_string(context.lastRegUsed - 1), mSubscript->GetResultRegister()};
    context.AddInstruction(context, "add", param2);
    
    context.lastRegUsed += 1;
    
    std::vector<std::string> param3 = {"%" + std::to_string(context.lastRegUsed), "%" + std::to_string(context.lastRegUsed - 1)};
    context.AddInstruction(context, "load", param3);
    
    mResultRegister = "%" + std::to_string(context.lastRegUsed);
    
    context.lastRegUsed += 1;
}

void NAssignVarStmt::CodeGen(CodeContext& context)
{
    mRhs->CodeGen(context);
    
    int idx;
    
    for (int i = 0; i < context.indicies.size(); ++i) {
        if (context.indicies[i].first == mName) {
            context.indicies[i].second = context.lastRegUsed;
            idx = i;
        }
    }
    
    std::vector<std::string> param = {std::to_string(idx), "%" + std::to_string(context.lastRegUsed - 1)};
    context.AddInstruction(context, "storei", param);
    
}

void NAssignArrayStmt::CodeGen(CodeContext& context)
{
    mRhs->CodeGen(context);
    mSubscript->CodeGen(context);
    
    int base = -1;
    for (auto it = context.indicies.begin(); it != context.indicies.end(); ++it) {
        if (it->first == mName && base == -1) {
            base = std::distance(context.indicies.begin(), it);
        }
    }
    
//    std::string n = mName;
//    auto itr = std::find_if(context.indicies.begin(), context.indicies.end(),
//        [&n](const std::pair<std::string, int>& element){
//        return element.first == n;
//    });
//    auto base = std::distance(context.indicies.begin(), itr);
    
    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed), std::to_string(base)};
    context.AddInstruction(context, "movi", param);
    
    context.lastRegUsed += 1;
    
    std::vector<std::string> param2 = {"%" + std::to_string(context.lastRegUsed), "%" + std::to_string(context.lastRegUsed - 1), mSubscript->GetResultRegister()};
    context.AddInstruction(context, "add", param2);
    
    
    std::vector<std::string> param3 = {"%" + std::to_string(context.lastRegUsed),  mRhs->GetResultRegister()};
    context.AddInstruction(context, "store", param3);
    
    context.lastRegUsed += 1;
}

void NIncStmt::CodeGen(CodeContext& context)
{
    int idx;
    
    for (int i = 0; i < context.indicies.size(); ++i) {
        if (context.indicies[i].first == mName) {
            idx = i;
        }
    }
    
    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed), std::to_string(idx)};
    context.AddInstruction(context, "loadi", param);
    std::vector<std::string> param2 = {"%" + std::to_string(context.lastRegUsed)};
    context.AddInstruction(context, "inc", param2);
    std::vector<std::string> param3 = {std::to_string(idx), "%" + std::to_string(context.lastRegUsed)};
    context.AddInstruction(context, "storei", param3);
    
    context.lastRegUsed += 1;
    
    
}

void NDecStmt::CodeGen(CodeContext& context)
{
    int idx;
    
    for (int i = 0; i < context.indicies.size(); ++i) {
        if (context.indicies[i].first == mName) {
            idx = i;
        }
    }
    
    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed), std::to_string(idx)};
    context.AddInstruction(context, "loadi", param);
    std::vector<std::string> param2 = {"%" + std::to_string(context.lastRegUsed)};
    context.AddInstruction(context, "dec", param2);
    std::vector<std::string> param3 = {std::to_string(idx), "%" + std::to_string(context.lastRegUsed)};
    context.AddInstruction(context, "storei", param3);
    
    context.lastRegUsed += 1;
}

void NComparison::CodeGen(CodeContext& context)
{
    mLhs->CodeGen(context);
    mRhs->CodeGen(context);
    
    std::string flag;
    
    switch(mType) {
        case 275:
            flag = "cmpeq";
        break;
        case 274:
            flag = "cmplt";
        break;
    }
    
    std::vector<std::string> param = {mLhs->GetResultRegister(), mRhs->GetResultRegister()};
    context.AddInstruction(context, flag, param);
}

void NIfStmt::CodeGen(CodeContext& context)
{

    mComp->CodeGen(context);
    
    
    std::vector<std::string> param3 = {"%" + std::to_string(context.lastRegUsed)};
    int placeholderLoc = context.instructions.size();
    context.AddInstruction(context, "movi", param3);
    

    std::vector<std::string> param4 = {"%" + std::to_string(context.lastRegUsed)};
    context.AddInstruction(context, "jnt", param4);
    context.lastRegUsed += 1;
    
    mIfBlock->CodeGen(context);
    

    if (mElseBlock != nullptr) {
        std::vector<std::string> param5 = {"%" + std::to_string(context.lastRegUsed)};
        int placeholderLoc2 = context.instructions.size();
        context.AddInstruction(context, "movi", param5);
        
        std::vector<std::string> param6 = {"%" + std::to_string(context.lastRegUsed)};
        context.AddInstruction(context, "jmp", param6);
        context.lastRegUsed += 1;
        
        context.instructions[placeholderLoc].second.emplace_back(std::to_string(context.instructions.size()));

        mElseBlock->CodeGen(context);

        context.instructions[placeholderLoc2].second.emplace_back(std::to_string(context.instructions.size()));
    }
    else {
        context.instructions[placeholderLoc].second.emplace_back(std::to_string(context.instructions.size()));
    }
}

void NWhileStmt::CodeGen(CodeContext& context)
{
    int placeholderLoc2 = context.instructions.size();
    
    mComp->CodeGen(context);
    
    std::vector<std::string> param3 = {"%" + std::to_string(context.lastRegUsed)};
    int placeholderLoc = context.instructions.size();
    context.AddInstruction(context, "movi", param3);

    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed)};
    context.AddInstruction(context, "jnt", param);
    context.lastRegUsed += 1;

    mBlock->CodeGen(context);

    std::vector<std::string> param5 = {"%" + std::to_string(context.lastRegUsed),  std::to_string(placeholderLoc2)};
    context.AddInstruction(context, "movi", param5);

    std::vector<std::string> param6 = {"%" + std::to_string(context.lastRegUsed)};
    context.AddInstruction(context, "jmp", param6);
    context.lastRegUsed += 1;
    
    context.instructions[placeholderLoc].second.emplace_back(std::to_string(context.instructions.size()));
}

void NPenUpStmt::CodeGen(CodeContext& context)
{
    std::vector<std::string> param = {};
    context.AddInstruction(context, "penup", param);
}

void NPenDownStmt::CodeGen(CodeContext& context)
{
    std::vector<std::string> param = {};
    context.AddInstruction(context, "pendown", param);
}

void NSetPosStmt::CodeGen(CodeContext& context)
{
    mXExpr->CodeGen(context);
    mYExpr->CodeGen(context);
    
    std::vector<std::string> param = {"tx", "%" + std::to_string(context.lastRegUsed - 2)};
    context.AddInstruction(context, "mov", param);
    
    std::vector<std::string> param2 = {"ty", "%" + std::to_string(context.lastRegUsed - 1)};
    context.AddInstruction(context, "mov", param2);

}

void NSetColorStmt::CodeGen(CodeContext& context)
{
    mColor->CodeGen(context);
    
    std::vector<std::string> param = {"tc", "%" + std::to_string(context.lastRegUsed - 1)};
    context.AddInstruction(context, "mov", param);
}

void NFwdStmt::CodeGen(CodeContext& context)
{
    mParam->CodeGen(context);
    
    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed - 1)};
    context.AddInstruction(context, "fwd", param);
}

void NBackStmt::CodeGen(CodeContext& context)
{
    mParam->CodeGen(context);
    
    std::vector<std::string> param = {"%" + std::to_string(context.lastRegUsed - 1)};
    context.AddInstruction(context, "back", param);
}

void NRotStmt::CodeGen(CodeContext& context)
{
    mParam->CodeGen(context);
    
    std::vector<std::string> param = {"tr", "tr", "%" + std::to_string(context.lastRegUsed - 1)};
    context.AddInstruction(context, "add", param);
}
