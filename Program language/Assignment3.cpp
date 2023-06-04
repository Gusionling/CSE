#include <iostream>
#include <cstddef>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <sstream>
#include <map>

using namespace std;

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
//���� flag
bool iserror = false;
//Enter ���� ó�� 
int Enter = 0;

//���ڿ� ������ �о����� 
size_t i = 0;
size_t len;
//������ ���ڿ�
string str;
//������ 
vector<string> op;
//�ǿ����� 
vector<int> operand;

//������ ������ �ڷᱸ��
map<string, int> var;

/* Character classes */
#define DIGIT 1
#define LETTER 2
#define ERROR 3
#define EQL 4
//������ +, - ó�� 
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
//�����̴�. 
#define IDENT 11
#define START 12
#define TYPE 13
#define LOOP 14
#define RELOP 15

#define ASSIGN_OP 20

#define ADD_OP 21
#define SUB_OP 22
#define ASS_OP 23
#define FIN_OP 24
#define RE_EQL 25
#define RE_NOT 26
#define RE_RIG 27
#define RE_LEF 28
#define RIG 29
#define LEF 30
#define NOT 31
#define RIGHT_PAREN 32
#define LEFT_PAREN 33
#define RIGHT_BPAREN 34
#define LEFT_BPAREN 35

void addChar();
void getChar();
int lex();
int calculate(int a, int b, char oper);

void program();
void declaration();
void statement();
bool bexpr();
bool bexpr1(int a, int b, string op);
void aexpr();
void term();
void Var();



void show() {
	for (int i = 0; i < operand.size(); i++) {
		cout << "operand: " << operand[i] << " ";
	}
	cout << "\n";
	for (int i = 0; i < op.size(); i++) {
		cout << " op: " << op[i] << " ";
	}
	cout << "\n";
}

int lookup(char ch) {
	switch (ch) {
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '=':
		addChar();
		nextToken = ASS_OP;
		break;

	case ';':
		addChar();
		nextToken = FIN_OP;
		break;

	case '!':
		addChar();
		nextToken = NOT;
		break;

	case '<':
		addChar();
		nextToken = RIG;
		break;

	case '>':
		addChar();
		nextToken = LEF;
		break;

	case '{':
		addChar();
		nextToken = RIGHT_BPAREN;
		break;

	case '}':
		addChar();
		nextToken = LEFT_BPAREN;
		break;

	case '(':
		addChar();
		nextToken = RIGHT_PAREN;
		break;

	case ')':
		addChar();
		nextToken = LEFT_PAREN;
		break;

	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}

void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		printf("Error - lexeme is too long \n");
}

void getChar() {
	//lex()�� �� �� str�� �ϳ� �� �������Ѽ� �������� ���� ���� ������ ������ 
	if (i != len) {
		nextChar = str[i++];
		//�����̰ų� print�� ��� 
		if (isalpha(nextChar)) {
			//�����̸� �ϳ��� ������ �ȴ�. 
			charClass = LETTER;
		}
		else if (isdigit(nextChar)) {
			charClass = DIGIT;
		}
		else {
			if (nextChar == '+' || nextChar == '-' || nextChar == '=' || nextChar == ';' || nextChar == ' ' || nextChar == '!' || nextChar == '<' || nextChar == '>' || nextChar == '{' || nextChar == '}' || nextChar == '(' || nextChar == ')') {
				charClass = UNKNOWN;
			}
			else {
				//������ �̿��� ���� ������ syntax error�� ����ϱ� ���ؼ� 
				iserror = true;
				charClass = UNKNOWN;
			}
		}
	}
	else
		charClass = EOF;
}


int lex() {
	lexLen = 0;

	//�Լ��� getNonBlank�����ϸ� cout�� ��ȣ�ϴ� ���ͼ� �ڵ�� ����
	while (isspace(nextChar)) {
		getChar();
	}
	/*getNonBlank();*/
	switch (charClass) {
		/* Parse integer literals */
	case LETTER:
		addChar();
		getChar();
		//print�� ������ֱ� ���ؼ� ��� �޴´�.
		//int�� 
		while (charClass == LETTER) {
			addChar();
			getChar();
		}

		nextToken = IDENT;
		break;

	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}

		nextToken = INT_LIT;
		break;

		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		while (isspace(nextChar)) {
			getChar();
		}
		//�񱳿����ڸ� ����� ����
		while (charClass == UNKNOWN && (nextToken == ASSIGN_OP || nextToken == NOT || nextToken == RIG || nextToken == LEF)) {
			addChar();
			getChar();
		}


		break;
		//EOF
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	}
	//print�� ���� lexeme�����ϱ� 
	//�ϴ� ��� �ް��ϰ� lex()�� ���� �� nextToken�� IDENT�ε� 
	//�� ���ڰ� �ƴϰų� print�� �ƴϸ� syntax error�� ������ �ڵ带 �߰������� 
	string lexemStr(lexeme);
	if (nextToken == IDENT) {
		if (lexemStr == "print" || lexLen == 1) {
			cout << "Next lexeme is: " << lexeme << "\n";
			return nextToken;
		}
		else if (lexemStr == "int") {
			nextToken = TYPE;
			cout << "Next lexeme is: " << lexeme << "\n";
			return nextToken;
		}
		else if (lexemStr == "while") {
			nextToken = LOOP;
			cout << "Next lexeme is: " << lexeme << "\n";
			return nextToken;
		}
		else {
			iserror = true;
			nextToken = ERROR;
			return nextToken;
		}
	}
	else if (lexemStr == "==" || lexemStr == "!=" || lexemStr == "<=" || lexemStr == ">=" || lexemStr == "<" || lexemStr == ">") {
		nextToken = RELOP;
		return nextToken;
	}

	cout << "Next lexeme is: " << lexeme << "\n";
	return nextToken;
}

//�Ľ�

void program() {
	while (nextToken == TYPE && nextToken != EOF && nextToken != ERROR) {
		cout << "now nextToken is " << nextToken << "\n";
		declaration();
	}
	while (nextToken != EOF && nextToken != ERROR) {
		if (nextToken == TYPE) {
			nextToken = ERROR;
			iserror = true;
			break;
		}
		else {
			cout << "now nextToken is " << nextToken << "\n";
			statement();
		}
	}
	cout << "exit program" << "\n";
}

void declaration() {
	if (nextToken == TYPE) {
		lex();
		Var();
		if (nextToken != ERROR) {
			lex();
			if (nextToken == FIN_OP) {
				lex();
			}
			else {
				iserror = true;
				nextToken = ERROR;
			}
		}
	}
	else {
		iserror = true;
		nextToken = ERROR;
	}

}

void Var() {
	string variable(lexeme);
	if (variable.length() != 1) {
		iserror = true;
		nextToken = ERROR;
	}
	//���ڸ��ΰ�� 
	else {
		char ch = variable[0];
		if (ch >= 'a' && ch <= 'z') {
			int k = var[variable];
			operand.push_back(k);
		}
		else {
			iserror = true;
			nextToken = ERROR;
		}
	}
}

void statement() {
	if (nextToken == IDENT) {
		string variable(lexeme);
		if (variable == "print") {
			//aexpr�ް� �̺κ� print ��� ���� �� ����ϰ� �ϱ� 
			lex();
			aexpr();
			//;������ ��� ����ϱ� 
			if (nextToken == FIN_OP) {
				int pr = operand.back();
				cout << pr << " ";
				while (!operand.empty()) {
					operand.pop_back();
				}
				show();
				lex();

			}
			else {
				iserror = true;
				nextToken = EOF;
			}
		}
		//�ѱ��� ������ �� 
		else {
			lex();
			if (nextToken == ASS_OP) {
				lex();
				aexpr();
				if (nextToken == FIN_OP) {
					if (iserror == false) {
						int result = operand.back();
						//������ �� ����
						var[variable] = result;
						//�� ���� Ȯ��
						cout << variable << ": " << var[variable] << "\n";
						while (!operand.empty()) {
							operand.pop_back();
						}
						show();
						lex();
					}
				}
				//;�� ������ ���� ���
				else {
					iserror = true;
					nextToken = EOF;
				}
			}
			//�������� =�� ���� ���� ��� 
			else {
				iserror = true;
				nextToken = EOF;
			}
		}
	}
	//while ��
	//�񱳿����� �������� ���� �������� ���ڸ��� ��츸 �����ϰ� ����
	else if (nextToken == LOOP) {
		size_t flag = i;
		//while���� ������ �� ó��
		size_t finflag = 1;
		//while�� ��� ���Ҵ���
		// ó������ while�� ���ǹ��� false�� ��츦 ���
		int count = 0;
		//while���� ������ �ٽ� ���ư� �� nextChar���� �����������
		char store = nextChar;
		int nextClass = charClass;
		//while���� ������ �� nextChar���
		char finstore = nextChar;
		cout << flag << "\n";
		while (true) {
			lex();
			//while�� ���ǽ��� true�� ��� 
			cout << "while�� �� lexeme: " << lexeme << " nextToken: " << nextToken << "i��" << i << "\n";
			if (nextToken == RIGHT_PAREN) {
				lex();
				//lvalue�� �����ΰ��� 0, ������ ���� 1
				//while���� ���ǹ��� �����صα� ���� �������̴�. 
				int leftstatus;
				int leftNum = 0;
				string leftB(lexeme);
				if (nextToken == INT_LIT) {
					leftstatus = 0;
				}
				else {
					leftstatus = 1;
				}
				//bexpr() �κ� 
				aexpr();
				cout << "lexeme: " << lexeme << "nextChar: " << nextChar << "nextToken: " << nextToken << "\n";
				int result1 = operand.back();
				while (!operand.empty()) {
					operand.pop_back();
				}
				if (nextToken == RELOP) {
					string lexemeStr(lexeme);
					lex();
					int rightstatus;
					int rightNum = 0;
					string rightB(lexeme);
					if (nextToken == INT_LIT) {
						rightstatus = 0;
					}
					else {
						rightstatus = 1;
					}
					aexpr();
					int result2 = operand.back();
					while (!operand.empty()) {
						operand.pop_back();
					}
					bool test = bexpr1(result1, result2, lexemeStr);
					//while�� ���ǽ��� ���ΰ�� �״�� ���� 
					if (test == true) {
						if (nextToken == LEFT_PAREN) {
							lex();
							if (nextToken == RIGHT_BPAREN) {
								//while���ǹ� ó���������
								//bexpr1�� ���� true��� while�� ����
								lex();
								while (nextToken != LEFT_BPAREN && nextToken != EOF && nextToken != ERROR) {
									statement();
								}

								if (nextToken == LEFT_BPAREN) {
									//syntax�� ������ ���� �� while�� ó������ ���ư��� �Ѵ�. 
									finflag = i;
									finstore = nextChar;
									i = flag;
									nextChar = store;
									count++;
									charClass = UNKNOWN;
									cout << "nextToken: " << nextToken << " nextChar: " << nextChar << "charClass: " << charClass << "\n";

								}
								//}���� ��� 
								else {
									iserror = true;
									nextToken = ERROR;
								}
							}
							//{ ���� ���
							else {
								iserror = true;
								nextToken = ERROR;
							}
						}
						// ) ���� ��� 
						else {
							iserror = true;
							nextToken = ERROR;
						}

					}
					//���ǽ��� �����ΰ�� 
					else {
						if (count == 0) {
							while (nextToken == LEFT_BPAREN || nextToken == EOF || nextToken == ERROR) {
								lex();
							}
						}
						i = finflag - 1;
						getChar();
						cout << "nextToken: " << nextToken << " nextChar: " << nextChar << "charClass: " << charClass << "\n";
						break;
						count = 0;
					}

				}
				// �񱳿����ڰ� ���� ���� ���
				else {
					iserror = true;
					nextToken = ERROR;
					break;
				}
			}
			//{ �� ���� ���� ��� 
			else {
				iserror = true;
				nextToken = ERROR;
				break;
			}
		}
		lex();
		cout << "nextToken: " << nextToken << " nextChar: " << nextChar << "charClass: " << charClass << "\n";

	}
	cout << "Exit Statement" << "\n";
}

//�ϳ��� lexeme�� �޾ƿͼ� ����
bool bexpr() {
	aexpr();
	int result1 = operand.back();
	while (!operand.empty()) {
		operand.pop_back();
	}
	if (nextToken == RELOP) {
		string lexemeStr(lexeme);
		lex();
		aexpr();
		int result2 = operand.back();
		while (!operand.empty()) {
			operand.pop_back();
		}
		bool test = bexpr1(result1, result2, lexemeStr);
		return test;
	}
	// �񱳿����ڰ� ���� ���� ���
	else {
		iserror = true;
		//return false�� �ص� �ǳ�?
		return false;
	}
}

bool bexpr1(int a, int b, string op) {
	if (op == "==") {
		return a == b;
	}
	else if (op == "!=") {
		return a != b;
	}
	else if (op == "<") {
		return a < b;
	}
	else if (op == ">") {
		return a > b;
	}
	else if (op == "<=") {
		return a <= b;
	}
	else if (op == ">=") {
		return a >= b;
	}
	else {
		cout << "�Ķ���͸� Ȯ�����ּ���!(bexpr)" << "\n";
		return false;
	}
}


void aexpr() {
	cout << "Enter aexpr" << "\n";
	term();
	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		lex();
		term();
	}
	if (!op.empty() && (op.back() == "+" || op.back() == "-")) {
		int b = operand.back();
		operand.pop_back();
		int a = operand.back();
		operand.pop_back();
		char o = op.back()[0];
		op.pop_back();
		int result = calculate(a, b, o);
		operand.push_back(result);

	}
	cout << "Exit aexpr" << "\n";
	show();
}


void term() {
	cout << "Enter term" << "\n";
	if (nextToken == INT_LIT) {
		stringstream ss;
		int k;
		ss << lexeme;
		ss >> k;
		operand.push_back(k);
		lex();
	}
	else if (nextToken == IDENT) {
		string variable(lexeme);
		int k = var[variable];
		operand.push_back(k);
		lex();
	}
	if (nextToken == ADD_OP || nextToken == SUB_OP) {
		op.push_back(lexeme);
	}
	//���� ���� ����� ���⼭ �ϸ� �ȴ�. 
	if (op.size() > 1 && (nextToken == ADD_OP || nextToken == SUB_OP)) {
		int size = op.size();
		//�̷��� aexpr()���� �� ���� ���� ���갡���ϰ� �ؾ���
		if (op[size - 2] == "+" || op[size - 2] == "-") {
			int b = operand.back();
			operand.pop_back();
			int a = operand.back();
			operand.pop_back();
			string st = op[size - 2];
			char o = st[0];
			string save = op.back();
			op.pop_back();
			op.pop_back();
			op.push_back(save);
			int result = calculate(a, b, o);
			operand.push_back(result);
		}
	}

	cout << "Exit term" << "\n";
	show();
}

int calculate(int a, int b, char oper) {
	switch (oper) {
	case '+': return a + b;
	case '-': return a - b;
	}
}

int main() {
	//�� ���� �Է� �޴� while��
	while (Enter < 2) {
		cout << ">> ";
		getline(cin, str);
		if (str == "") {
			Enter++;
		}
		else {
			Enter = 0;
			len = strlen(str.c_str());
			getChar();
			while (true) {
				//���ڿ��� ��ū�� �� �޾��� �� break
				//break�� �ٱ��� while�� �����ؼ� �ٽ� ���ο� ������ �Է� ����

				//�����̳� print�� ;�� ������ �ϳ��� ������ ������ �� 
				if (nextToken == FIN_OP) {
					op.clear();
					operand.clear();
				}


				//;�̰ſ� ���ؼ��� ��� �� ���ΰ� 
				if (nextToken == EOF || nextToken == ERROR) {
					//�Ʒ� �ΰ��� �ʱ�ȭ �ϴ� ���� EOF�� ���� �´�. 
					len = 0;
					i = 0;
					//nextToken�� ���¸� �ʱ�ȭ
					nextToken = START;

					//str�� �ʱ�ȭ ���� �ʿ䰡 ���°� 
					//while���� �� ������ str�� �Է¹��� 
					if (iserror == true) {
						cout << "syntax error!!" << "\n";
					}
					op.clear();
					operand.clear();
					iserror = false;
					break;

				}

				lex();
				//statement���� program����
				program();
				//�� �� �����θ� ������ ���� ����
				var.clear();
			}
		}
	}
	cout << "���α׷� ����";
}