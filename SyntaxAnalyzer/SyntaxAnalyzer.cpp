#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "LexicalAnalyzer.h"


using namespace std;

// 定义非终结符及其 FIRST 集
unordered_map<string, set<string>> firstSets = {
	{"<prog>", {"program"}},
	{"<block>", {"const", "var", "procedure", "begin"}},
	{"<condecl>", {"const"}},
	{"<const>", {"ID"}},
	{"<vardecl>", {"var"}},
	{"<proc>", {"procedure"}},
	{"<body>", {"begin"}},
	{"<statement>", {"ID", "if", "while", "call", "begin", "read", "write"}},
	{"<lexp>", {"odd", "+", "-", "ID", "int", "("}},
	{"<exp>", {"+", "-", "ID", "int", "("}},
	{"<term>", {"ID", "int", "("}},
	{"<factor>", {"ID", "int", "("}},
	{"<lop>", {"=", "<>", "<", "<=", ">", ">="}},
	{"<aop>", {"+", "-"}},
	{"<mop>", {"*", "/"}}
};


unordered_map<string, set<string>> syncSet = {
	{"<prog>", {"#"}},
	{"<block>", {"#", ";", "begin"}},
	{"<condecl>", {"var","procedure","begin",";"}},
	{"<const>", {",",";"}},
	{"<vardecl>", {"procedure","begin",";"}},
	{"<proc>", {"begin"}},
	{"<body>", {"#",";","begin","end","else"}},
	{"<statement>", {";","end","else","begin"}},
	{"<lexp>", {"then","do"}},
	{"<exp>", {";","end","else",",",":=","<>","<","<=",">",">=","then","do",")"}},
	{"<term>", {"+","-",";","end","else",",",":=","<>","<","<=",">",">=","then","do",")"}},
	{"<factor>", {"+","-","*","/",",","end","else",",",": = ","<>","<"," <= ",">"," >= ","then","do",")"}},
	{"<lop>", {"+","-","ID","int","("}},
	{"<aop>", {"ID","int","("}},
	{"<mop>", {"ID","int","("}}
};

void initializeSyncSet() {
	syncSet["<program>"] = { "." };
	syncSet["<block>"] = { ".", "procedure", "begin" };
	syncSet["<body>"] = { ".", "end" };
	syncSet["<statement>"] = { ";", "end", "if", "while", "call", "read", "write" };
}

bool isID();
bool isConst();
bool isKeyWord();
bool isEnd();
void prog();
void block();
void condecl();
void ConsT();
void vardecl();
void proc();
void body();
void statement();
void lexp();
void exp();
void term();
void factor();
void lop();
void aop();
void mop();

vector<Token> lex;//词法分析器给出的所有终结符
int index = 0;//索引

int main()
{
	lex = LexicalAnalyzer();
	prog();
	return 0;
}

void prog()
{
	if (lex[index].symbol == "program") {
		index++;
	}
	else {
		cout << "缺少program,lines:"<<lex[index].line << endl;
		while (!isID() && !isKeyWord()) {
			cout << "跳过的终结符"<<lex[index].symbol<<",lines:" << lex[index].line << endl;
			index++;
		}
			
	}

	if (isID()) {//是ID
		index++;
	}
	else if (isKeyWord()) {//ID使用保留字表
		cout << "标识符不能为保留字,lines:" << lex[index].line << endl;
		index++;
	}
	else {//不是ID
		cout << "缺少标识符,lines:" << lex[index].line << endl;
		index++;
	}

	if (lex[index].symbol == ";") {
		index++;
	}
	else {//FIRST集合作为同步符号
		while (lex[index].symbol != ";" && lex[index].symbol != "const" && lex[index].symbol != "var" && lex[index].symbol != "procedure" && lex[index].symbol != "begin" && !isEnd()) {
			cout << "跳过的终结符" << lex[index].symbol << ",lines:" << lex[index].line << endl;
			index++;
		}
			
		if (lex[index].symbol == ";") {
			index++;
		}
		else {
			cout << "缺少“;”lines:" << lex[index].line << endl;
		}
	}
	block();
	if (lex[index].symbol == "#") {
		cout << "程序结束" << endl;
	}
}

void block()
{

	if (lex[index].symbol == "const") {
		condecl();
	}
	if (lex[index].symbol == "var") {
		vardecl();
	}
	if (lex[index].symbol == "procedure") {
		proc();
	}
	body();
}

void condecl()
{
	if (lex[index].symbol != "const") {
		cout << "缺少const,lines:" << lex[index].line << endl;
	}
	if (lex[index].symbol == "const") {
		index++;
	}
	ConsT();
	while (lex[index].symbol == ",") {
		index++;
		ConsT();
	}
	if (lex[index].symbol == ";") {
		index++;
	}
	else {
		cout << "缺少“;”lines:" << lex[index].line << endl;
	}
}

void ConsT()
{
	if (isID()) {//是ID
		index++;
	}
	else {
		cout << "缺少标识符,lines:" << lex[index].line << endl;
		while (lex[index].symbol != ":=" && lex[index].symbol != "," && lex[index].symbol != ";" && !isID() && !isEnd()) {
			cout << "跳过的终结符,lines:" << lex[index].line << endl;
			index++;
		}
	}

	if (lex[index].symbol == ":=") {
		index++;
	}
	else {
		cout << "缺少“:=”lines:" << lex[index].line << endl;
	}
	
	if (isConst()) {//是常数
		index++;
	}
	else {
		cout << "缺少常数,lines:" << lex[index].line << endl;
	}
}

void vardecl()
{
	if (lex[index].symbol == "var") {
		index++;
	}
	else {
		cout << "缺少var,lines:" << lex[index].line << endl;
		while (lex[index].symbol != "," && lex[index].symbol != ";" && !isID() && lex[index].symbol != "begin" && lex[index].symbol != "procedure") {
			cout << "跳过的终结符"<<lex[index].symbol<<",lines:" << lex[index].line << endl;
			index++;
		}
	}
		if (isID()) {//是ID
			index++;
			while (lex[index].symbol == ",") {//,
				index++;
				if (isID()) {
					index++;
				}
			}
		}
		else {
			cout << "缺少标识符,lines:" << lex[index].line << endl;
		}
		if (lex[index].symbol == ";") {
			index++;
		}
		else {
			cout << "缺少分号,lines:" << lex[index].line << endl;
		}
	
}

void proc()
{
	if (lex[index].symbol == "procedure") {
		index++;
	}
	else {
		cout << "缺少procedure,lines:" << lex[index].line << endl;
		while (lex[index].symbol != "," && lex[index].symbol != ";" && lex[index].symbol != "const" 
			&& lex[index].symbol != "begin" && lex[index].symbol != "var" && lex[index].symbol != "procedure"&&!isID() && lex[index].symbol != "(" && lex[index].symbol != ")") {
			cout << "跳过的终结符"<<lex[index].symbol<<",lines:" << lex[index].line << endl;
			index++;
		}
	}
	if (isID()) {//是ID
		index++;
	}
	else {
		cout << "缺少标识符,lines:" << lex[index].line << endl;
	}
	if (lex[index].symbol == "(") {
		index++;
	}
	else {
		cout << "缺少左括号,lines:" << lex[index].line << endl;
	}
	if (isID()) {//是ID
		index++;
		while (lex[index].symbol == ",") {//,
			index++;
			if (isID()) {
				index++;
			}
			else {
				cout << "缺少标识符,lines:" << lex[index].line << endl;
			}
		}
	}
	if (lex[index].symbol == ")") {
		index++;
	}
	else {
		cout << "缺少右括号,lines:" << lex[index].line << endl;
	}
	if (lex[index].symbol == ";") {
		index++;
	}
	else {
		cout << "缺少分号,lines" << lex[index].line << endl;
	}
	block();
	while (!isEnd() && lex[index].symbol == ";") {
		index++;
		proc();
	}
}

void body()
{
	if (lex[index].symbol == "begin") {
		index++;
	}
	else {
		cout << "缺少begin,lines:" << lex[index].line << endl;
	}
	statement();
	while (lex[index].symbol == ";") {
		index++;
		statement();
	}
	if (lex[index].symbol == "end") {
		index++;
	}
	else {
		cout << "缺少end,lines:" << lex[index].line << endl;
	}
}

void statement()
{
	if (isID()) {//是ID
		index++;
		if (lex[index].symbol == ":=") {
			index++;
			exp();
		}
	}
	else if(lex[index].symbol == "if") {
		index++;
		lexp();
		if (lex[index].symbol == "then") {
			index++;
		}
		else {
			cout << "缺少then,lines:" << lex[index].line << endl;
			while (lex[index].symbol != "if" && lex[index].symbol != "while" && lex[index].symbol != "call"
				&& lex[index].symbol != "begin" && lex[index].symbol != "read" && lex[index].symbol != "write" && !isID() ) {
				cout << "跳过的终结符" << lex[index].symbol << ",lines:" << lex[index].line << endl;
				index++;
			}
		}
		statement();
		if (lex[index].symbol == "else") {
			index++;
			statement();
		}
	}
	else if (lex[index].symbol == "while") {
		index++;
		lexp();
		if (lex[index].symbol == "do") {
			index++;
		}
		else {
			cout << "缺少do,lines:" << lex[index].line << endl;
			while (lex[index].symbol != "if" && lex[index].symbol != "while" && lex[index].symbol != "call"
				&& lex[index].symbol != "begin" && lex[index].symbol != "read" && lex[index].symbol != "write" && !isID()) {
				cout << "跳过的终结符" << lex[index].symbol << ",lines:" << lex[index].line << endl;
				index++;
			}
		}
			statement();
	}
	else if (lex[index].symbol == "call") {
		index++;
		if (isID()) {//是ID
			index++;
			if (lex[index].symbol == "(") {
				index++;
				if (lex[index].symbol == "+" || lex[index].symbol == "-" || lex[index].symbol == "(" || isID() || isConst()) {
					exp();
					while (lex[index].symbol == ",") {
						index++;
						exp();
					}
				}
				if (lex[index].symbol == ")") {
					index++;
				}
				else {
					cout << "缺少右括号,lines" << lex[index].line << endl;
				}
			}
		}
	}
	else if (lex[index].symbol == "begin") {//<body>的首符集
		body();
	}
	else if (lex[index].symbol == "read") {
		index++;
		if (lex[index].symbol == "(") {
			index++;
			if (isID()) {//是ID
				index++;
				while (lex[index].symbol == ",") {
					index++;
					if (isID()) {//是ID
						index++;
					}
				}
			}
			if (lex[index].symbol == ")") {
				index++;
			}
			else {
				cout << "缺少右括号,lines" << lex[index].line << endl;
			}
		}
		
	}
	else if (lex[index].symbol == "write") {
		index++;
		if (lex[index].symbol == "(") {
			index++;
			if (lex[index].symbol == "+" || lex[index].symbol == "-" || isID() || isConst() || lex[index].symbol == "(") {//"+", "-", "ID", "int", "("
				exp();
				while (lex[index].symbol == ",") {
					index++;
					exp();
				}
			}
			if (lex[index].symbol == ")") {
				index++;
			}
			else {
				cout << "缺少右括号,lines:" << lex[index].line << endl;
			}
		}
	}
	else {
		while (lex[index].symbol != ";" && lex[index].symbol != "end" && lex[index].symbol != "else") {
			cout << "跳过的终结符"<<lex[index].symbol<<",lines:" << lex[index].line << endl;
			index++;
		}
	}
}

void lexp()
{
	if (lex[index].symbol == "odd") {
		index++;
		exp();
	}
	else if(lex[index].symbol == "+" || lex[index].symbol == "-" || isID() || isConst() || lex[index].symbol == "("){
		exp();
		lop();
		exp();
	}
	else {
		cout << "缺少条件语句,lines:" << lex[index].line << endl;
		while (lex[index].symbol != "then" && lex[index].symbol != "do") {//弹出非终结符
			cout << "跳过的终结符"<<lex[index].symbol<<",lines:" << lex[index].line << endl;
			index++;
		}
	}
}

void exp()
{
	if (lex[index].symbol == "+" || lex[index].symbol == "-") {
		index++;
	}
	term();
	while(lex[index].symbol == "+" || lex[index].symbol == "-") {
		index++;
		if (lex[index].symbol == "(" || isID() || isConst()) {
			term();
		}
	}
}

void term()
{
	factor();
	while (lex[index].symbol == "*" || lex[index].symbol == "/") {
		mop();
		factor();
	}
}

void factor()
{
	if (isID()||isConst()) {
		index++;
	}
	else if (lex[index].symbol == "(") {
		index++;
		exp();
		if (lex[index].symbol == ")") {
			index++;
		}
	}
	else {
		while (lex[index].symbol != "*" && lex[index].symbol != "/" && lex[index].symbol != "+" && lex[index].symbol != "-" && lex[index].symbol != ";" && lex[index].symbol != "end" &&
			lex[index].symbol != "else" && lex[index].symbol != ":=" && lex[index].symbol != "<>" && lex[index].symbol != "<" && lex[index].symbol != "<=" &&
			lex[index].symbol != ">" && lex[index].symbol != ">=") {
			cout << "跳过的终结符"<<lex[index].symbol<<",lines:" << lex[index].line << endl;
			index++;
		}
	}
}

void lop()
{
	if (lex[index].symbol == "=" || lex[index].symbol == "<>" || lex[index].symbol == "<"
		|| lex[index].symbol == "<=" || lex[index].symbol == ">" || lex[index].symbol == ">=") {
		index++;
	}
}

void aop()
{
	if (lex[index].symbol == "+" || lex[index].symbol == "-" ) {
		index++;
	}
}

void mop()
{
	if (lex[index].symbol == "*" || lex[index].symbol == "/") {
		index++;
	}
}

bool isID()
{
	for (int i = 0; i < Id.size(); i++) {
		if (Id[i] == lex[index].symbol) {
			return true;
		}
	}
	return false;
}

bool isConst()
{
	for (int i = 0; i <	Const.size(); i++) {
		if (Const[i] == lex[index].symbol) {
			return true;
		}
	}
	return false;
}

bool isKeyWord()
{
	for (int i = 0; i < keyWord->size(); i++) {
		if (lex[index].symbol == keyWord[i]) {
			return true;
		}
	}
	return false;
}

bool isEnd()
{
	if (lex[index].symbol == "#") {
		cout << "遇到#，程序结束" << endl;
		exit(0);
	}
	return false;
}