#include "uParse.h"
#include <cmath>
#include "uParse_service.h" //Needed!
//Equation class
//Constructor
Equation::Equation():
    parsedOk(false)
{}
//Yet another
Equation::Equation(const std::string& input):
    parsedOk(false)
{
    parse(input);
}

//Calculation
double Equation::calculate() {
    //If not Ok
    if (!parsedOk)
        return 0.0;
    //If nothing parsed
    if (parsed.size() == 0)
        return 0.0;
    //...
    std::stack<double> stk;
    for (unsigned int n = 0; n < parsed.size(); n++) {
        //Nothing. Just nothing.
        if (parsed[n].type == OT_NONE)
            return 0.0;
        //Constant
        if (parsed[n].type == OT_CONST) {
            stk.push(parsed[n].value);
            continue;
        }
        //Variable
        if (parsed[n].type == OT_VAR) {
            stk.push(varMap[parsed[n].name]);
            continue;
        }
        //Operation
        if (stk.size() > 1) {
            double b = stk.top();
            stk.pop();
            double a = stk.top();
            stk.pop();
            double r;
            switch (parsed[n].type) {
            case OT_PLUS:
                r = a + b;
                break;
            case OT_MINUS:
                r = a - b;
                break;
            case OT_MUL:
                r = a * b;
                break;
            case OT_DIV:
                r = a / b;
                break;
            case OT_POW:
                r = pow(a, b);
                break;
            }
            stk.push(r);
        }
    }
    if (stk.size() == 1)
        return stk.top();
    else
        return 0.0;
}
//Parse method
bool Equation::parse(const std::string& input) {
    parsedOk = false;
    original = input;
    parsedOk = convertToPostfix() && scanVars();
    return parsedOk;
}

//Set Variable operator
double& Equation::operator[](std::string varName)
{
    return varMap[varName];
}
//Service...
//->Postfix
bool Equation::convertToPostfix() {
    parsed.clear();
    std::stack<char> stk;
    //!
    unsigned int parsePointer = 0;
    bool ok = true;
    while (parsePointer < original.length() && ok) {
        switch (original[parsePointer]) {
        //Opening brace (parenthesis)
        case '(':
            stk.push('(');
            break;
        //Closing brace, pop all from the stack until met "("
        case ')':
            while (stk.top() != '(' && !stk.empty()) {
                parsed.push_back(constructOperand(stk.top()));
                stk.pop();
            }
            stk.pop();
            break;
        //Houston, we have an operation
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            if (!stk.empty())
                while ((operationPriority(original[parsePointer]) <= operationPriority(stk.top())) && !stk.empty()) {
                    parsed.push_back(constructOperand(stk.top()));
                    stk.pop();
                }
            stk.push(original[parsePointer]);
            break;
        //We have a variable or a constant or an error
        default:
            if (isCypher(original[parsePointer])) { //Constant
                Operand operand = parseNumber(parsePointer, original);
                parsed.push_back(operand);
                parsePointer += operand.operandLength - 1;
            }
            else if(isChar(original[parsePointer])) { //Variable
                Operand operand = parseVariable(parsePointer, original);
                parsed.push_back(operand);
                parsePointer += operand.operandLength - 1;
            }
            else { //Error
                ok = false;
            }
        }
        parsePointer++;
    }
    //Adding the rest of stack to out vector (if OK)
    while (!stk.empty() && ok) {
        parsed.push_back(constructOperand(stk.top()));
        stk.pop();
    }
    //!
    return ok;
}
//Scans variables
bool Equation::scanVars() {
    if (parsed.size() == 0)
        return false;
    varMap.clear();
    for (unsigned int opN = 0; opN < parsed.size(); ++opN) {
        if (parsed[opN].type == OT_VAR) {
            varMap.insert(std::pair<std::string, double>(parsed[opN].name, 0.0));
        }
    }
    return true;
}
