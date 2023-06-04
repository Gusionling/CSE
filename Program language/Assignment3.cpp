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
//에러 flag
bool iserror = false;
//Enter 종료 처리 
int Enter = 0;

//문자열 어디까지 읽었는지 
size_t i = 0;
size_t len;
//들어오는 문자열
string str;
//연산자 
vector<string> op;
//피연산자 
vector<int> operand;

//변수를 저장할 자료구조
map<string, int> var;

/* Character classes */
#define DIGIT 1
#define LETTER 2
#define ERROR 3
#define EQL 4
//연산자 +, - 처리 
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
//변수이다. 
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
	//lex()에 들어갈 때 str을 하나 더 증가시켜서 지정되지 않은 범위 참조를 막고자 
	if (i != len) {
		nextChar = str[i++];
		//변수이거나 print인 경우 
		if (isalpha(nextChar)) {
			//변수이면 하나만 받으면 된다. 
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
				//위에꺼 이외의 것이 들어오면 syntax error를 출력하기 위해서 
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

	//함수로 getNonBlank구현하면 cout이 모호하다 나와서 코드로 정리
	while (isspace(nextChar)) {
		getChar();
	}
	/*getNonBlank();*/
	switch (charClass) {
		/* Parse integer literals */
	case LETTER:
		addChar();
		getChar();
		//print를 만들어주기 위해서 계속 받는다.
		//int도 
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
		//비교연산자를 만들기 위함
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
	//print와 변수 lexeme구현하기 
	//일단 계속 받게하고 lex()를 나갈 때 nextToken이 IDENT인데 
	//한 글자가 아니거나 print가 아니면 syntax error가 나오게 코드를 추가해주자 
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

//파싱

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
	//한자리인경우 
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
			//aexpr받고 이부분 print 블록 나갈 때 출력하게 하기 
			lex();
			aexpr();
			//;만나면 결과 출력하기 
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
		//한글자 변수일 때 
		else {
			lex();
			if (nextToken == ASS_OP) {
				lex();
				aexpr();
				if (nextToken == FIN_OP) {
					if (iserror == false) {
						int result = operand.back();
						//변수에 값 대입
						var[variable] = result;
						//잘 들어갔나 확인
						cout << variable << ": " << var[variable] << "\n";
						while (!operand.empty()) {
							operand.pop_back();
						}
						show();
						lex();
					}
				}
				//;로 끝나지 않은 경우
				else {
					iserror = true;
					nextToken = EOF;
				}
			}
			//변수다음 =가 오지 않은 경우 
			else {
				iserror = true;
				nextToken = EOF;
			}
		}
	}
	//while 문
	//비교연산자 기준으로 왼쪽 오른쪽이 한자리인 경우만 가능하게 구현
	else if (nextToken == LOOP) {
		size_t flag = i;
		//while문이 나왔을 때 처리
		size_t finflag = 1;
		//while이 몇번 돌았는지
		// 처음부터 while의 조건문이 false인 경우를 대비
		int count = 0;
		//while문이 나오고 다시 돌아갈 때 nextChar까지 설정해줘야함
		char store = nextChar;
		int nextClass = charClass;
		//while문이 나왔을 때 nextChar기억
		char finstore = nextChar;
		cout << flag << "\n";
		while (true) {
			lex();
			//while의 조건식이 true일 경우 
			cout << "while들어갈 때 lexeme: " << lexeme << " nextToken: " << nextToken << "i는" << i << "\n";
			if (nextToken == RIGHT_PAREN) {
				lex();
				//lvalue가 숫자인경우는 0, 문자인 경우는 1
				//while문의 조건문을 저장해두기 위한 변수들이다. 
				int leftstatus;
				int leftNum = 0;
				string leftB(lexeme);
				if (nextToken == INT_LIT) {
					leftstatus = 0;
				}
				else {
					leftstatus = 1;
				}
				//bexpr() 부분 
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
					//while의 조건식이 참인경우 그대로 진행 
					if (test == true) {
						if (nextToken == LEFT_PAREN) {
							lex();
							if (nextToken == RIGHT_BPAREN) {
								//while조건문 처리해줘야함
								//bexpr1의 값이 true라면 while문 돌기
								lex();
								while (nextToken != LEFT_BPAREN && nextToken != EOF && nextToken != ERROR) {
									statement();
								}

								if (nextToken == LEFT_BPAREN) {
									//syntax적 에러가 없을 떄 while문 처음으로 돌아가게 한다. 
									finflag = i;
									finstore = nextChar;
									i = flag;
									nextChar = store;
									count++;
									charClass = UNKNOWN;
									cout << "nextToken: " << nextToken << " nextChar: " << nextChar << "charClass: " << charClass << "\n";

								}
								//}빠진 경우 
								else {
									iserror = true;
									nextToken = ERROR;
								}
							}
							//{ 빠진 경우
							else {
								iserror = true;
								nextToken = ERROR;
							}
						}
						// ) 빠진 경우 
						else {
							iserror = true;
							nextToken = ERROR;
						}

					}
					//조건식이 거짓인경우 
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
				// 비교연산자가 오지 않은 경우
				else {
					iserror = true;
					nextToken = ERROR;
					break;
				}
			}
			//{ 이 오지 않은 경우 
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

//하나의 lexeme을 받아와서 진행
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
	// 비교연산자가 오지 않은 경우
	else {
		iserror = true;
		//return false로 해도 되나?
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
		cout << "파라미터를 확인해주세요!(bexpr)" << "\n";
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
	//같은 레벨 계산은 여기서 하면 된다. 
	if (op.size() > 1 && (nextToken == ADD_OP || nextToken == SUB_OP)) {
		int size = op.size();
		//이러면 aexpr()나갈 때 덧셈 뺄셈 연산가능하게 해야함
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
	//한 줄을 입력 받는 while문
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
				//문자열의 토큰을 다 받았을 때 break
				//break후 바깥쪽 while문 실행해서 다시 새로운 수식을 입력 받음

				//대입이나 print가 ;를 만나서 하나의 연산이 끝났을 때 
				if (nextToken == FIN_OP) {
					op.clear();
					operand.clear();
				}


				//;이거에 대해서는 어떻게 할 것인가 
				if (nextToken == EOF || nextToken == ERROR) {
					//아래 두개를 초기화 하는 것은 EOF일 때가 맞다. 
					len = 0;
					i = 0;
					//nextToken의 상태를 초기화
					nextToken = START;

					//str은 초기화 해줄 필요가 없는게 
					//while문을 돌 때마다 str을 입력받음 
					if (iserror == true) {
						cout << "syntax error!!" << "\n";
					}
					op.clear();
					operand.clear();
					iserror = false;
					break;

				}

				lex();
				//statement인지 program인지
				program();
				//한 줄 단위로만 변수에 값을 저장
				var.clear();
			}
		}
	}
	cout << "프로그램 종료";
}