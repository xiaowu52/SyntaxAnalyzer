#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

//�궨��ؼ��֡���ʶ��������
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

//�궨�������
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

//�궨����
#define $SEMICOLON 29	//�ֺ�
#define $COMMA 30		//����
#define $LEFT_PAREN 31	//������
#define $RIGHT_PAREN	32	//������

using namespace std;

//�����ֱ�
const string keyWord[15] = { "program","const","var","procedure","begin","end",
"if","then","else","while","do","call","read","write","odd" };
//���ű�
vector<string> Id;
int IdIndex = 0;
//������
vector<string> Const;
int ConstIndex = 0;





//void GetChar(char ch);		//����һ�����ַ����뵽ch�У�����ָʾ��ǰ��һ�ַ�λ��
//void GetBC(char ch);			//���ch�е��ַ��Ƿ�Ϊ�հס����ǣ������Getcharֱ��ch�н���һ���ǿհ��ַ�
//void Concat(char ch, string& strToken);//��ch�е��ַ����ӵ�strToken֮��
bool IsLetter(char ch);			//�ж�ch�Ƿ�Ϊ��ĸ
bool IsDigit(char ch);			//�ж�ch�Ƿ�Ϊ����
int Reserve(string& strToken);	//�ж�strToken�еı�ʶ���Ƿ�Ϊ�ؼ��֣�����Ƿ��ص����ֱ���룬���򷵻�0��0��Ϊ�����ֱ���룩
void Retract(fstream& f1, char& ch);			//������ָʾ���ص�һ���ַ�λ�ã���ch��Ϊ�հ��ַ�
//void InsertID();				//��strToken�еı�ʶ��������ű����ط��ű�ָ��
//void InsertConst();				//��strToken�еĳ������볣�������س�����ָ��


struct Token {
	string symbol;	//��������
	int line;		//�к�
};

vector<Token> LexicalAnalyzer()
{
	vector<Token> lex;//�����з��ż����lex�����ظ��﷨��������


	char ch;				//�ַ�������������¶�����Դ�����ַ�
	string strToken = "";	//�ַ����飬��Ź��ɵ��ʷ��ŵ��ַ���
	int lines = 1;			//���������ڱ����������λ��
	int code;				//�����ֱ����

	fstream f1;
	//f1.open("program.txt", ios::in);
	f1.open("program2.txt",ios::in);
	//f1.open("program3.txt",ios::in);
	if (f1.fail()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
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
				int flag = 0;//�ж��Ƿ��Ѿ������ڷ��ű�
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
			else {//��(�����ֱ�,�������Եķ���ֵ���޶���)���
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
				cout << "��ʶ�����������ֿ�ͷ:";
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
				cout << "δ������š�:��,lines:" << lines << endl;
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
			cout << "δ������š�" << ch << "��,lines:" << lines << endl;
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
