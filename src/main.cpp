#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"


using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace std;

StatementMatcher LoopMatcher =
forStmt(
    hasLoopInit(
        declStmt(
            hasSingleDecl(
                varDecl(
                    hasInitializer(
                        integerLiteral(
                            equals(0)
                        )
                    )
                ).bind("initVarName")
            )
        )
    ),
    hasCondition(
        binaryOperator(
            hasOperatorName("<"),
            hasLHS(
                ignoringParenImpCasts(
                    declRefExpr(
                        to(
                            varDecl(
                                hasType(
                                    isInteger()
                                )
                            ).bind("condVarName")
                        )
                    )
                )
            ),
            hasRHS(
                expr(
                    hasType(
                        isInteger()
                    )
                )
            )
        )
    ),
    hasIncrement(
        unaryOperator(
            hasOperatorName("++"),
            hasUnaryOperand(
                declRefExpr(
                    to(
                        varDecl(
                            hasType(isInteger())
                        ).bind("incVarName")
                    )
                )
            )
        )
    )
).bind("forLoop");

static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
  return First && Second &&
         First->getCanonicalDecl() == Second->getCanonicalDecl();
}

class LoopPrinter : public MatchFinder::MatchCallback {
public :
  virtual void run(const MatchFinder::MatchResult &Result) {
    ASTContext *Context = Result.Context;

    const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoop");
    if (!FS || !Context->getSourceManager().isWrittenInMainFile(FS->getForLoc()))
        return;

    const VarDecl *IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
    const VarDecl *CondVar = Result.Nodes.getNodeAs<VarDecl>("condVarName");
    const VarDecl *InitVar = Result.Nodes.getNodeAs<VarDecl>("initVarName");

    if (!areSameVariable(IncVar, CondVar) || !areSameVariable(IncVar, InitVar))
        return;

    FS->dump();
  }
};

static llvm::cl::OptionCategory MyToolCategory("refactor options");
static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nMore help text...\n");

int main(int argc, const char** argv)
{
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                    OptionsParser.getSourcePathList());
    
    LoopPrinter Printer;
    MatchFinder Finder;
    Finder.addMatcher(LoopMatcher, &Printer);

    return Tool.run(newFrontendActionFactory(&Finder).get());
}