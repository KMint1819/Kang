#include "ast.hpp"

namespace kang
{
NumberNode::NumberNode(double val): value(val) {}


VariableNode::VariableNode(const std::string& name): varName(name) {}

CallNode::CallNode(const std::string& callee, std::vector<std::unique_ptr<ExprNode>> args)
    : callee(callee), args(std::move(args))
{
}


SignatureNode ::SignatureNode(const std::string& funcName, std::vector<std::string> args)
    : funcName(funcName), args(std::move(args))
{
}

FunctionNode::FunctionNode(std::unique_ptr<SignatureNode> signature, std::unique_ptr<ExprNode> body)
    : signature(std::move(signature)), body(std::move(body))
{
}


BinaryOpNode::BinaryOpNode(char op, std::unique_ptr<ExprNode> lhs, std::unique_ptr<ExprNode> rhs)
    : op(op), lhs(std::move(lhs)), rhs(std::move(rhs))
{
}

} // namespace kang