#ifndef SYNTAXMODULE_UPARSE_H
#define SYNTAXMODULE_UPARSE_H

#include <string>
#include <stack>
#include <vector>
#include <map>

//Operand type
enum OperandType {OT_NONE, OT_CONST, OT_VAR, OT_PLUS, OT_MINUS, OT_MUL, OT_DIV, OT_POW};
//Operand structure
struct Operand {
    OperandType type;
    double value;
    std::string name;
    unsigned int operandLength;
};
//Equation class
class Equation {
public:

    Equation();
    Equation(const std::string& input);
    double calculate();
    bool parse(const std::string& input);
    //std::vector varArray();
    double& operator[](std::string varName);

private:
    std::string original;
    bool parsedOk;
    std::vector<Operand> parsed;
    std::map<std::string, double> varMap;
    bool convertToPostfix();
    bool scanVars();
};


#endif //SYNTAXMODULE_UPARSE_H
