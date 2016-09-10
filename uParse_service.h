//Cypher check
bool isCypher(char chr) {
    if (chr >= '0' && chr <= '9')
        return true;
    else
        return false;
}
//Character check
bool isChar(char chr) {
    if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'))
        return true;
    else
        return false;
}
//Number parser
Operand parseNumber(const unsigned int& position, const std::string& source) {
    if (!(position < source.length() && isCypher(source[position])))
        return {OT_NONE, 0.0, "", 0};
    double intPart = 0.0;
    unsigned int length = 0, pos = position;
    while (isCypher(source[pos]) && pos < source.length()) {
        intPart *= 10.0;
        intPart += source[pos++] - 0x30;
        length++;
    }
    double fracPart = 0.0, div = 10.0;
    if (source[pos] == '.') {
        length++;
        pos++;
        while (isCypher(source[pos]) && pos < source.length())
        {
            fracPart += (source[pos++] - 0x30) / div;
            div *= 10.0;
            length++;
        }
    }
    return {OT_CONST, intPart + fracPart, "", length};
}
//Variable parser
Operand parseVariable(const unsigned int& position, const std::string& source) {
    if (!(position < source.length() && isChar(source[position])))
        return {OT_NONE, 0.0, "", 0};
    unsigned int pos = position, length = 0;
    std::string varName = "";
    while (pos < source.length() && (isChar(source[pos]) || isCypher(source[pos]) || source[pos] == '.')) {
        varName += source[pos++];
        length++;
    }
    return {OT_VAR, 0.0, varName, length};
}
//Returns priority of operation
int operationPriority(const char& chr) {
    switch (chr) {
    case '+':
    case '-':
        return 0;
    case '*':
    case '/':
        return 1;
    case '^':
        return 2;
    default:
        return -1;
    }
}
//Constructs an operand
Operand constructOperand(const char& chr) {
    Operand opr{OT_NONE, 0.0, "", 0};
    switch (chr) {
    case '+':
        opr.type = OT_PLUS;
        break;
    case '-':
        opr.type = OT_MINUS;
        break;
    case '*':
        opr.type = OT_MUL;
        break;
    case '/':
        opr.type = OT_DIV;
        break;
    case '^':
        opr.type = OT_POW;
        break;
    }
    return opr;
}
//...
Operand constructOperand(const std::string& str) {
    return {OT_VAR, 0.0, str, str.length()};
}
//...
Operand constructOperand(const double& value) {
    return {OT_CONST, value, "", 0};
}
