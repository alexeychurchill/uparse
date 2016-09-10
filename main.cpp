#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <cmath>

#include "uParse.h"

//using namespace std;

int main() {

    std::string input("(a/b^2-455.34)*(4-d^4/2)");
    Equation equation;
    if (!equation.parse(input))
    {
        std::cout << "Parse error." << std::endl;
        return 0;
    }
    equation["a"] = 2;
    equation["b"] = 3;
    equation["d"] = 4;
    std::cout << "Equation value: " << equation.calculate() << std::endl;
    return 0;
}
