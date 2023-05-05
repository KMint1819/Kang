#pragma once

#include "lexer.hpp"
#include "ast.hpp"
#include <map>

namespace kang
{

std::unique_ptr<ExprNode> logErr(const std::string& msg)
{
    fprintf(stderr, "Error: %s\n", msg.c_str());
    return nullptr;
}

std::unique_ptr<SignatureNode> logErrSig(const std::string& msg)
{
    logErr(msg);
    return nullptr;
}


std::unique_ptr<ExprNode> parse();
std::unique_ptr<ExprNode> parseNumber()
{
    auto ret = std::make_unique<NumberNode>(numVal);
    eatToken();
    return std::move(ret);
}

std::unique_ptr<ExprNode> parseIdentifier()
{
    std::string id = identifierStr;
    eatToken();

    // Just a variable
    if (curToken != '(')
        return std::make_unique<VariableNode>(id);

    // Function call
    // Eat "("
    eatToken();

    std::vector<std::unique_ptr<ExprNode>> args;
    while (curToken != ')')
    {
        auto expr = parse();
        if (!expr)
            return logErr("Argument error in function call");
        args.push_back(std::move(expr));

        if (curToken == ')')
            break;

        if (curToken != ',')
            return logErr("Expected ')' or ',' after an argument in the function argument list");

        // Eat ","
        eatToken();
    }

    // Eat ")"
    eatToken();
    return std::make_unique<CallNode>(id, std::move(args));
}

std::unique_ptr<ExprNode> parseParenthesis()
{
    // Eat "("
    eatToken();

    auto ret = parse();
    if (!ret)
        return nullptr;

    if (curToken != ')')
        return logErr("Expect \")\"");

    // Eat ")"
    eatToken();
    return ret;
}

// Primary tokens: Number, Identifier, Parenthesis
std::unique_ptr<ExprNode> parsePrimary()
{
    if (curToken == Token::NUMBER)
        return parseNumber();
    else if (curToken == Token::IDENTIFIER)
        return parseIdentifier();
    else if (curToken == '(')
        return parseParenthesis();
    return logErr("Unknown token when parsing primary");
}

std::unique_ptr<SignatureNode> parseSignature()
{
    if (curToken != Token::IDENTIFIER)
        return logErrSig("Expecting function name in signature");

    std::string funcName = identifierStr;
    eatToken(); // eat funcName

    if (curToken != '(')
        return logErrSig("Expecting '(' after function name");

    std::vector<std::string> argNames;

    while (eatToken() == Token::IDENTIFIER)
        argNames.push_back(identifierStr);

    if (curToken != ')')
        return logErrSig("Expecting ')' after argument list");

    eatToken(); // Eat ")"

    return std::make_unique<SignatureNode>(funcName, argNames);
}

std::unique_ptr<FunctionNode> parseFunction()
{
    eatToken(); // eat def
    auto signature = parseSignature();

    if (!signature)
        return nullptr;

    auto body = parse();
    if (!body)
        return nullptr;
    return std::make_unique<FunctionNode>(std::move(signature), std::move(body));
}

std::unique_ptr<SignatureNode> parseExtern()
{
    eatToken(); // Eat extern
    return parseSignature();
}

std::unique_ptr<FunctionNode> parseAnonymousFunction()
{
    if (auto expr = parse())
    {
        auto signature = std::make_unique<SignatureNode>("", std::vector<std::string>());
        return std::make_unique<FunctionNode>(std::move(signature), std::move(expr));
    }
    return nullptr;
}

// Get the priority of curToken. Returns -1 if curToken is not a binary op.
int getTokPriority()
{
    static const std::map<char, int> priorityMap = {
        { '<', 10 },
        { '+', 20 },
        { '-', 20 },
        { '*', 40 },
    };

    if (!isascii(curToken))
        return -1;

    auto it = priorityMap.find(curToken);
    if (it == priorityMap.end())
        return -1;
    return it->second;
}

std::unique_ptr<ExprNode> parseBinaryOp(int prevPriority, std::unique_ptr<ExprNode> lhs)
{
    while (true)
    {
        int curPriority = getTokPriority();
        if (curPriority < prevPriority)
            return lhs;

        int binop = curToken;
        eatToken(); // Eat binop

        auto rhs = parsePrimary();
        if (!rhs)
            return nullptr;

        int nextPriority = getTokPriority();
        if (curPriority < nextPriority)
        {
            // TODO: experiment passing curPriority only.
            rhs = parseBinaryOp(curPriority + 1, std::move(rhs));
            if (!rhs)
                return nullptr;
        }

        lhs = std::make_unique<BinaryOpNode>(binop, std::move(lhs), std::move(rhs));
    }
}

std::unique_ptr<ExprNode> parse()
{
    auto lhs = parsePrimary();
    if (!lhs)
        return nullptr;
    return parseBinaryOp(0, std::move(lhs));
}
} // namespace kang