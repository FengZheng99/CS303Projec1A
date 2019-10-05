//  CS303 Project 1A - Feng Zheng
//
//  Created by Feng Zheng on 10/3/19.
//  Copyright © 2019 Feng Zheng. All rights reserved.

// Libraries
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
using namespace std;

// Function Declaration
int prec(string item);
bool is_oper(string item);
bool is_num(string item);
vector<string> InfixToPostfix(string infix, unsigned long size);
int result(vector<string> postfix);
bool is_binoper(string item);
void check_error(string infix);


// Main
int main() {
    
    // Enter the infix expression
    string infix;
    cout << "Enter the infix expression ==> ";
    getline(cin,infix);
    
    // Check error
    check_error(infix);
    
    // Return the postfix of infix expression
    unsigned long size = infix.length();
    vector<string> postfix = InfixToPostfix(infix, size);
    //for (int i = 0; i < postfix.size(); i++)
    //    cout << postfix[i];
    int answer = result(postfix);
    // Result
    cout << "Result : " << answer;
    cout << endl << endl;
    
    return 0;
}



// Return the precedence of operator
int prec(string item){
    
    if(item == "!" || item == "++" || item == "--" || item == "neg") return 8;
    else if(item == "^") return 7;
    else if(item == "*" || item == "/" || item == "%") return 6;
    else if(item == "+" || item == "-") return 5;
    else if(item == ">" || item == ">=" || item == "<" || item == "<=") return 4;
    else if(item == "==" || item == "!=") return 3;
    else if(item == "&&") return 2;
    else if(item == "||") return 1;
    else return 0;
}

// Check if is operator
bool is_oper(string item){
    
    string oper[18] = {"!","++","--","neg","^","*","/","%","+","-",
                       ">",">=","<","<=","==","!=","&&","||"};
    
    for (int i = 0; i < 17; i++){
        if(item == oper[i]){
            return true;
        }
    }
    return false;
}

// Binary operator
bool is_binoper(string item){
    
    string oper[18] = {"!",">",">=","<","<=","==","!=","&&","||"};
    
    for (int i = 0; i < 17; i++){
        if(item == oper[i]){
            return true;
        }
    }
    return false;
}

// Check if is number
bool is_num(string item){
    
    if (item == "0" || item == "1" || item == "2" || item == "3" || item == "4" ||
        item == "5" || item == "6" || item == "7" || item == "8" || item == "9"){
        return true;
    }
    return false;
}

// Convert Infix to Postfix
vector<string> InfixToPostfix(string infix, unsigned long size) {
    
    // Declare Variable
    stack<string> s;
    vector<string> postfix;
    int precedence;
    string str, temp, temp2, index_0;
    
    // Iterate through expression
    for (int i = 0 ;i < size; i++) {
        
        index_0 = infix[0];
        str = infix[i];
        temp = infix[i+1];

        
        // Check for Error
        if (index_0 == ")")
        {cout << "Expression can’t start with a closing parenthesis\n\n";exit(0);}
        else if (index_0 == ">")
        {cout << "Expression can’t start with a binary operator\n\n";exit(0);}
        
        
        // Skip the spaces
        if (str == " "){
            temp2 = infix[i-1];
            if (i>=1 && is_num(temp) && is_num(temp2))
            {cout << "Two operands in a row\n\n";exit(0);}
            continue;
        }
        
        // Check for complex operator
        else if (str == "+"){if (temp == "+"){str += temp;i++;}}
        else if (str == "-"){if (temp == "-"){str += temp;i++;}}
        else if (str == "="){if (temp == "="){str += temp;i++;}}
        else if (str == "!"){if (temp == "="){str += temp;i++;}}
        else if (str == "&"){if (temp == "&"){str += temp;i++;}}
        else if (str == "|"){if (temp == "|"){str += temp;i++;}}
        else if (str == "<"){if (temp == "="){str += temp;i++;}}
        else if (str == ">"){if (temp == "="){str += temp;i++;}}
        if (str == "!"){if (temp == "!"){i++;continue;}}
        
        // Check for negative
        if (str == "-" && is_num(temp) && i == 0){str = "neg";}
        else if ( str == "-" && is_num(temp) && is_oper(temp2)){str = "neg";}
        
        // Check for number greater than 9
        if (is_num(str)){
            while (is_num(temp)){
                str+= temp;
                i+=1;
                temp = infix[i+1];
            }
        }
        
        // Check for Parenthesis
        if (str == "(") {
            s.push(str);
            continue;
        }
        if (str == ")") {
            while (!s.empty() && s.top() != "(") {
                postfix.push_back(s.top());
                s.pop();
            }
            if (!s.empty()) {
                s.pop();
            }
            continue;
        }
        
        // Check for precedence
        precedence = prec(str);
        
        // It is a number so push to the vector
        if (precedence == 0) {postfix.push_back(str);}
        
        // Is is a operator
        else {
            if (s.empty()) {
            s.push(str);
            }
            else {
                while (!s.empty() && s.top() != "(" && precedence < prec(s.top())) {
                    postfix.push_back(s.top());
                    s.pop();
                }
            s.push(str);
            }
            
        }
    }
    
    // Pop out the stack of operators
    while (!s.empty()) {
        postfix.push_back(s.top());
        s.pop();
    }
    
    // Return the postfix expression
    return postfix;
}

// Calculate the postfix expressxion
int result(vector<string> postfix){
    
    //Declare Variables
    stack<int> s;
    int precedence;
    int operand1, operand2, answer;
    
    for (int i = 0; i < postfix.size(); i++){
        
        precedence = prec(postfix[i]);
        // Push the number to stack
        if (precedence == 0){int temp = stoi(postfix[i]);s.push(temp);}
        
        // Calculate
        else if (is_oper(postfix[i])){
            
            // Logical not
            if (postfix[i] == "!"){
                operand1 = s.top();
                s.pop();
                if (operand1 == 1){s.push(0);}
                else if (operand1 == 0){s.push(1);}
                else {cout << "Invalid Expression\n\n"; exit(0);}
            }
            // Increment
            else if (postfix[i] == "++"){
                if (is_binoper(postfix[i+1]))
                {cout << "A unary operand can’t be followed by a binary operator\n\n";exit(0);}
                operand1 = s.top();
                s.pop();
                s.push(++operand1);
            }
            // Decrement
            else if (postfix[i] == "--"){
                if (is_binoper(postfix[i+1]))
                {cout << "A unary operand can’t be followed by a binary operator\n\n";exit(0);}
                operand1 = s.top();
                s.pop();
                s.push(--operand1);
            }
            // Negative
            else if (postfix[i] == "neg"){
                operand1 = s.top();
                s.pop();
                s.push(-1*operand1);
            }
            // Power
            else if (postfix[i] == "^"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                s.push(pow(operand2,operand1));
            }
            // Multiply
            else if (postfix[i] == "*"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                s.push(operand2*operand1);
            }
            // Divide
            else if (postfix[i] == "/"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand1 == 0){cout << "Can't divided by 0\n\n";exit(0);}
                s.push(operand2/operand1);
            }
            // Reminder
            else if (postfix[i] == "%"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                s.push(operand2%operand1);
            }
            // Add
            else if (postfix[i] == "+"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                s.push(operand2+operand1);
            }
            // Subtrat
            else if (postfix[i] == "-"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                s.push(operand2-operand1);
            }
            // Greater than
            else if (postfix[i] == ">"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 > operand1){s.push(1);}
                else {s.push(0);}
            }
            // Greater than or equal to
            else if (postfix[i] == ">="){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 >= operand1){s.push(1);}
                else {s.push(0);}
            }
            // Less than
            else if (postfix[i] == "<"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 < operand1){s.push(1);}
                else {s.push(0);}
            }
            // Less than or equal to
            else if (postfix[i] == "<="){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 <= operand1){s.push(1);}
                else {s.push(0);}
            }
            // Equality
            else if (postfix[i] == "=="){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 == operand1){s.push(1);}
                else {s.push(0);}
            }
            // Not euqal
            else if (postfix[i] == "!="){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 != operand1){s.push(1);}
                else {s.push(0);}
            }
            // Logical And
            else if (postfix[i] == "&&"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 == 1 && operand1 == 1){s.push(1);}
                else {s.push(0);}
            }
            // Logical Or
            else if (postfix[i] == "||"){
                operand1 = s.top();
                s.pop();
                operand2 = s.top();
                s.pop();
                if (operand2 == 1 || operand1 == 1){s.push(1);}
                else {s.push(0);}
            }
        }
        else{
            cout << "Invalid Expression\n\n";
            exit(0);
        }
    }
    
    answer = s.top();
    
    return answer;
}

// Check for error

void check_error(string infix){
    vector<string> s;
    string str, temp;
    string oper[9] = {"!",">",">=","<","<=","==","!=","&&","||"};
    infix.erase(remove(infix.begin(), infix.end(), ' '), infix.end());
    for (int i = 0; i < infix.size(); i++){
        str = infix[i];
        temp = infix[i+1];
        if (str == "+"){if (temp == "+"){str += temp;i++;}}
        else if (str == "-"){if (temp == "-"){str += temp;i++;}}
        else if (str == "="){if (temp == "="){str += temp;i++;}}
        else if (str == "!"){if (temp == "="){str += temp;i++;}}
        else if (str == "&"){if (temp == "&"){str += temp;i++;}}
        else if (str == "|"){if (temp == "|"){str += temp;i++;}}
        else if (str == "<"){if (temp == "="){str += temp;i++;}}
        else if (str == ">"){if (temp == "="){str += temp;i++;}}
        s.push_back(str);
    }
    for (int i = 0; i < s.size(); i++)
        for (int j = 0; j < 9; j++)
            if (s[i] == oper[j])
                for (int z = 0; z < 9; z++)
                    if (s[i+1] == oper[z]){
                        cout << "Two binary operators in a row\n\n";
                        exit(0);
                    }
}
