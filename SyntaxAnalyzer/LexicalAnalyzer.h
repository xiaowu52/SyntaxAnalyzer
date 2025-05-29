#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

//宏定义关键字、标识符、常数
#define $PROGRAM 1
#define $CONST 2
#define $VAR 3
#define $PROCEDURE 4
#define $BEGIN 5
#define $END 6
#define $IF 7
#define $THEN 8
#define $ELSE 9
#define $WHILE 10
#define $DO 11
#define $CALL 12
#define $READ 13
#define $WRITE 14
#define $ODD 15
#define $ID 16
#define $INT 17

//宏定义运算符
#define $ASSIGN 18
#define $EQUAL 19
#define $PLUS 20
#define $MINUS 21
#define $STAR 22
#define $DIVIDE 23
#define $NOTEQUAL 24
#define $LESS 25
#define $LESS_EQUAL 26
#define $GREATER 27
#define $GREATER_EQUAL 28

//宏定义界符
#define $SEMICOLON 29	//分号
#define $COMMA 30		//逗号
#define $LEFT_PAREN 31	//左括号
#define $RIGHT_PAREN	32	//右括号

using namespace std;

//保留字表
const string keyWord[15] = { "program","const","var","procedure","begin","end",
"if","then","else","while","do","call","read","write","odd" };
//符号表
vector<string> Id;
int IdIndex = 0;
//常数表
vector<string> Const;
int ConstIndex = 0;





//void GetChar(char ch);		//将下一输入字符读入到ch中，搜索指示器前移一字符位置
//void GetBC(char ch);			//检查ch中的字符是否为空白。若是，则调用Getchar直至ch中进入一个非空白字符
//void Concat(char ch, string& strToken);//将ch中的字符连接到strToken之后
bool IsLetter(char ch);			//判断ch是否为字母
bool IsDigit(char ch);			//判断ch是否为数字
int Reserve(string& strToken);	//判断strToken中的标识符是否为关键字，如果是返回单词种别编码，否则返回0（0不为单词种别编码）
void Retract(fstream& f1, char& ch);			//将搜索指示器回调一个字符位置，将ch置为空白字符
//void InsertID();				//将strToken中的标识符插入符号表，返回符号表指针
//void InsertConst();				//将strToken中的常数插入常数表，返回常数表指针


struct Token {
	string symbol;	//符号名称
	int line;		//行号
};

vector<Token> LexicalAnalyzer()
{
	vector<Token> lex;//将所有符号加入进lex并返回给语法分析器。


	char ch;				//字符变量，存放最新读进的源程序字符
	string strToken = "";	//字符数组，存放构成单词符号的字符串
	int lines = 1;			//行数，用于报告错误所在位置
	int code;				//单词种别编码

	fstream f1;
	//f1.open("program.txt", ios::in);
	f1.open("program2.txt",ios::in);
	//f1.open("program3.txt",ios::in);
	if (f1.fail()) {
		cout << "文件打开失败！" << endl;
		exit(0);
	}
	while (!f1.eof()) {
		f1 >> noskipws >> ch;
		while (!f1.eof() && (ch == ' ' || ch == '\n')) {
			if (ch == '\n') {
				lines++;
			}
			f1 >> noskipws >> ch;
		}

		if (f1.eof())
			break;

		if (IsLetter(ch)) {
			while (IsLetter(ch) || IsDigit(ch)) {
				strToken += ch;
				if (!f1.eof())
					f1 >> noskipws >> ch;
			}
			Retract(f1, ch);
			code = Reserve(strToken);
			if (code == 0) {
				int flag = 0;//判断是否已经存在于符号表
				for (int i = 0; i < Id.size(); i++) {
					if (Id[i] == strToken) {
						lex.push_back({ strToken,lines });
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					Id.push_back(strToken);
					lex.push_back({ strToken,lines });
					IdIndex++;
				}
				strToken = "";
			}
			else {//按(单词种别,单词属性的符号值或无定义)输出
				lex.push_back({ strToken,lines });
				strToken = "";
			}
		}
		else if (IsDigit(ch)) {
			while (IsDigit(ch)) {
				strToken += ch;
				if (!f1.eof())
					f1 >> noskipws >> ch;
			}
			if (IsLetter(ch)) {
				cout << "标识符不能以数字开头:";
				while (IsDigit(ch) || IsLetter(ch)) {
					strToken += ch;
					if (!f1.eof())
						f1 >> noskipws >> ch;
				}
				cout << strToken << ",lines:" << lines << endl;
				strToken = "";
				continue;
			}
			Retract(f1, ch);
			Const.push_back(strToken);
			lex.push_back({ strToken,lines });
			ConstIndex++;
			strToken = "";
		}
		else if (ch == '+') {
			lex.push_back({ string(1, ch), lines });
		}
		else if (ch == '-') {
			lex.push_back({ string(1, ch), lines });
		}
		else if (ch == '*') {
			lex.push_back({ string(1, ch), lines });
		}
		else if (ch == '/') {
			lex.push_back({ string(1, ch), lines });
		}
		else if (ch == '>') {
			strToken += ch;
			if (!f1.eof())
				f1 >> noskipws >> ch;
			if (ch != '=') {
				Retract(f1, ch);
				lex.push_back({ strToken,lines });
				strToken = "";
			}
			else {
				strToken += ch;
				lex.push_back({ string(strToken),lines });
				strToken = "";
			}
		}
		else if (ch == '<') {
			strToken += ch;
			if (!f1.eof())
				f1 >> noskipws >> ch;
			if (ch == '=') {
				strToken += ch;
				lex.push_back({ strToken,lines });
				strToken = "";
			}
			else if (ch == '>') {
				strToken += ch;
				lex.push_back({ strToken,lines });
				strToken = "";
			}
			else {
				Retract(f1, ch);
				lex.push_back({ strToken,lines });
			}
		}
		else if (ch == ':') {
			strToken += ch;
			if (!f1.eof())
				f1 >> noskipws >> ch;
			if (ch == '=') {
				strToken += ch;
				lex.push_back({ strToken,lines });
				strToken = "";
			}
			else {
				cout << "未定义符号“:”,lines:" << lines << endl;
			}
		}
		else if (ch == ';') {
			lex.push_back({ string(1, ch),lines });
		}
		else if (ch == '(') {
			lex.push_back({ string(1, ch),lines });
		}
		else if (ch == ')') {
			lex.push_back({ string(1, ch),lines });
		}
		else if (ch == ',') {
			lex.push_back({ string(1, ch),lines });
		}
		else if (ch == '=') {
			lex.push_back({ string(1, ch),lines });
		}
		else if (ch == '#') {
			lex.push_back({ string(1, ch),lines });
			break;
		}
		
		else {
			cout << "未定义符号“" << ch << "”,lines:" << lines << endl;
			continue;
		}
	}

	f1.close();

	return lex;
}

//void GetChar(char ch)
//{
//
//}
//
//void GetBC(char ch)
//{
//}

//void Concat(char ch, string& strToken)
//{
//}

bool IsLetter(char ch)
{
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
		return true;
	else
		return false;
}

bool IsDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}

int Reserve(string& strToken)
{
	for (int i = 0; i < 15; i++) {
		if (strToken == keyWord[i]) {
			return i + 1;
		}
	}
	return 0;
}

void Retract(fstream& f1, char& ch)
{
	f1.seekg(-1L, ios::cur);
	ch = ' ';
}

//void InsertID()
//{
//
//}
//
//void InsertConst()
//{
//}
