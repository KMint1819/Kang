#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

namespace kang
{
extern std::unique_ptr<llvm::LLVMContext> myContext;
extern std::unique_ptr<llvm::IRBuilder<>> myBuilder;
extern std::unique_ptr<llvm::Module> myModule;
extern std::map<std::string, llvm::Value*> variables;

class ExprNode
{
public:
    virtual ~ExprNode() = default;
    virtual llvm::Value* emit() = 0;
};

class NumberNode : public ExprNode
{
public:
    NumberNode(double val);

private:
    double m_val;
    llvm::Value* emit() final override;
};

class VariableNode : public ExprNode
{
public:
    VariableNode(const std::string& name);
    llvm::Value* emit() final override;

private:
    const std::string m_name;
};

class CallNode : public ExprNode
{
public:
    CallNode(const std::string& calleeName, std::vector<std::unique_ptr<ExprNode>> args);
    llvm::Value* emit() final override;

private:
    const std::string m_calleeName;
    const std::vector<std::unique_ptr<ExprNode>> m_args;
};

class SignatureNode : public ExprNode
{
public:
    SignatureNode(const std::string& funcName, std::vector<std::string> args);
    llvm::Value* emit() final override;

private:
    const std::string m_funcName;
    const std::vector<std::string> m_args;
};

class FunctionNode : public ExprNode
{
public:
    FunctionNode(std::unique_ptr<SignatureNode> signature, std::unique_ptr<ExprNode> body);
    llvm::Value* emit() final override;

private:
    std::unique_ptr<SignatureNode> m_signature;
    std::unique_ptr<ExprNode> m_body;
};

class BinaryOpNode : public ExprNode
{
public:
    BinaryOpNode(char op, std::unique_ptr<ExprNode> lhs, std::unique_ptr<ExprNode> rhs);
    llvm::Value* emit() final override;

private:
    char m_op;
    const std::unique_ptr<ExprNode> m_lhs, m_rhs;
};

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

llvm::Value* logErrV(const std::string& msg)
{
    logErr(msg);
    return nullptr;
}

} // namespace kang