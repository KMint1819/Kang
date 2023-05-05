#pragma once

#include <string>
#include <vector>
#include <memory>

namespace kang
{
class ExprNode
{
public:
    virtual ~ExprNode() = default;
};

class NumberNode : public ExprNode
{
public:
    NumberNode(double val);

private:
    double value;
};

class VariableNode : public ExprNode
{
public:
    VariableNode(const std::string& name);

private:
    const std::string varName;
};

class CallNode : public ExprNode
{
public:
    CallNode(const std::string& callee, std::vector<std::unique_ptr<ExprNode>> args);

private:
    const std::string callee;
    const std::vector<std::unique_ptr<ExprNode>> args;
};

class SignatureNode : public ExprNode
{
public:
    SignatureNode(const std::string& funcName, std::vector<std::string> args);

private:
    const std::string funcName;
    const std::vector<std::string> args;
};

class FunctionNode : public ExprNode
{
public:
    FunctionNode(std::unique_ptr<SignatureNode> signature, std::unique_ptr<ExprNode> body);

private:
    std::unique_ptr<SignatureNode> signature;
    std::unique_ptr<ExprNode> body;
};

class BinaryOpNode : public ExprNode
{
public:
    BinaryOpNode(char op, std::unique_ptr<ExprNode> lhs, std::unique_ptr<ExprNode> rhs);

private:
    char op;
    const std::unique_ptr<ExprNode> lhs, rhs;
};

} // namespace kang