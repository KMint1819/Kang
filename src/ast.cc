#include "ast.hpp"
#include <fmt/core.h>

namespace kang
{
using llvm::Value;
std::unique_ptr<llvm::LLVMContext> myContext;
std::unique_ptr<llvm::IRBuilder<>> myBuilder;
std::unique_ptr<llvm::Module> myModule;
std::map<std::string, Value *> variables;

NumberNode::NumberNode(double val) : m_val(val)
{
}

Value *NumberNode::emit()
{
    return llvm::ConstantFP::get(*myContext, llvm::APFloat(m_val));
}

VariableNode::VariableNode(const std::string &name) : m_name(name)
{
}
Value *VariableNode::emit()
{
    auto it = variables.find(m_name);
    if (it == variables.end())
    {
        return nullptr;
    }
    return it->second;
}

CallNode::CallNode(const std::string &calleeName, std::vector<std::unique_ptr<ExprNode>> args)
    : m_calleeName(calleeName), m_args(std::move(args))
{
}

Value *CallNode::emit()
{
    llvm::Function *callee = myModule->getFunction(m_calleeName);
    if (!callee)
        return logErrV(fmt::format("Function {} not found!", m_calleeName));

    if (callee->arg_size() != m_args.size())
        return logErrV(fmt::format("Expecting {} for function {}. Getting {} instead.", callee->arg_size(),
                                   m_calleeName, m_args.size()));
    std::vector<Value *> args;
    for (const auto &arg : m_args)
    {
        args.push_back(arg->emit());
    }
    return myBuilder->CreateCall(callee, args, "k_call");
}

SignatureNode ::SignatureNode(const std::string &funcName, std::vector<std::string> args)
    : m_funcName(funcName), m_args(std::move(args))
{
}

llvm::Function *SignatureNode::emit()
{
    std::vector<llvm::Type *> doubles(m_args.size(), llvm::Type::getDoubleTy(*myContext));
    auto *functionType = llvm::FunctionType::get(llvm::Type::getDoubleTy(*myContext), doubles, false);

    auto *f = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_funcName, myModule.get());

    int idx = 0;
    for (auto &arg : f->args())
        arg.setName(m_args[idx++]);
    return f;
}

const std::string &SignatureNode::getName() const
{
    return m_funcName;
}

FunctionNode::FunctionNode(std::unique_ptr<SignatureNode> signature, std::unique_ptr<ExprNode> body)
    : m_signature(std::move(signature)), m_body(std::move(body))
{
}

llvm::Function *FunctionNode::emit()
{
    llvm::Function *f = myModule->getFunction(m_signature->getName());
    if (!f)
    {
        f = m_signature->emit();
    }
    if (!f)
    {
        logErr(fmt::format("Cannot find function '{}'.", m_signature->getName()));
        return nullptr;
    }

    if (!f->empty())
    {
        logErr(fmt::format("Function '' is defined more than once!", m_signature->getName()));
        return nullptr;
    }

    llvm::BasicBlock *bb = llvm::BasicBlock::Create(*myContext, "k_entry", f);
    myBuilder->SetInsertPoint(bb);

    variables.clear();
    for (auto &arg : f->args())
        variables.insert({std::string(arg.getName()), &arg});

    if (Value *ret = m_body->emit())
    {
        myBuilder->CreateRet(ret);

        llvm::verifyFunction(*f);
        return f;
    }
    f->eraseFromParent();
    return nullptr;
}

BinaryOpNode::BinaryOpNode(char op, std::unique_ptr<ExprNode> lhs, std::unique_ptr<ExprNode> rhs)
    : m_op(op), m_lhs(std::move(lhs)), m_rhs(std::move(rhs))
{
}

Value *BinaryOpNode::emit()
{
    Value *lval = m_lhs->emit();
    Value *rval = m_rhs->emit();
    if (lval == nullptr || rval == nullptr)
        return nullptr;

    if (m_op == '+')
        return myBuilder->CreateFAdd(lval, rval, "k_add");
    if (m_op == '-')
        return myBuilder->CreateFSub(lval, rval, "k_sub");
    if (m_op == '*')
        return myBuilder->CreateFMul(lval, rval, "k_mul");
    if (m_op == '<')
    {
        lval = myBuilder->CreateFCmpULT(lval, rval, "k_cmp");
        return myBuilder->CreateUIToFP(lval, llvm::Type::getDoubleTy(*myContext), "k_bool");
    }
    return logErrV(fmt::format("Invalid binary operator '{}' !", m_op));
}

} // namespace kang