#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

//Character classes
#define DIGIT 1
#define UNKNOWN 2
#define ERROR 3

//Token codes
#define INT_LIT 10

#define IDENT 11
#define START 12

//operator
#define ADD_OP 20
#define SUB_OP 21

//iserror
#define FALSE 0
#define TRUE 1

#define ALPHA_REVERSE 449
#define OPER_ADD 450
#define OPER_SUB 451

char input[100];
char ret[100];
long operand[100];
char reversed[100];
char transfer[100];
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
unsigned int i;
int iserror = FALSE;
long unit;
int len;
int is_reverse = FALSE;
long result = 9999;


void getChar();
void addChar();
void getNonBlank();
int lex();
void expr();
void oper();

int lookup(char ch) {

    switch (ch)
    {
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;

    case '-':
        addChar();
        nextToken = SUB_OP;
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
        printf("Error - lexeme is too long");
}

void getChar() {
    //printf("현재 커서 위치 %d, nextchar = %c\n", i, nextChar);
    if (i != len) {
        nextChar = input[i++];
        if (isdigit(nextChar)) {
            charClass = DIGIT;
        }
        else {
            if (nextChar == ' ' || nextChar == '+' || nextChar == '-') {
                charClass = UNKNOWN;
            }
            else {
                iserror = TRUE;
                charClass = UNKNOWN;
            }
        }
    }
    else
        charClass = EOF;
}


int lex() {
    lexLen = 0;

    //printf("lex의 현재 커서 위치 %d, nextchar = %c\n", i, nextChar);

    while (isspace(nextChar)) {
        getChar();
    }
    switch (charClass)
    {
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }
    return nextToken;
}

void expr() {
    if (nextToken == INT_LIT) {
        operand[0] = strtol(lexeme, NULL, 10);
        strcpy(reversed, lexeme);
        syscall(ALPHA_REVERSE, reversed);
        
        lex();
        if (nextToken == EOF) {
            is_reverse = TRUE;
            return;
        }
        else if (nextToken == ADD_OP) {
            lex();
            if (nextToken == INT_LIT) {
                operand[1] = strtol(lexeme, NULL, 10);
            }
            lex();
            if (nextToken != EOF) {
                iserror = TRUE;
            }
            else {
                syscall(OPER_SUB, operand[0], operand[1], &result);

            }
        }
        else if (nextToken == SUB_OP) {
            lex();
            if (nextToken == INT_LIT) {
                operand[1] = strtol(lexeme, NULL, 10);
            }
            lex();
            if (nextToken != EOF) {
                iserror = TRUE;
            }
            else {
                syscall(OPER_ADD, operand[0], operand[1], &result);
            }
        }
    }
    else {
        iserror = TRUE;
    }
}

//operand비워주기 필요
int main(int argc, char* argv)
{

    while (1) {
        printf("input: ");
        fgets(input, sizeof(input), stdin);  // 사용자로부터 문자열 입력을 받습니다.

        len = strlen(input) - 1; //"\n"을 전체길이에서 빼주기위해서 위해서 -1
        // 문자열의 길이를 확인하여 사용자가 아무 문자열도 입력하지 않으면 루프를 종료합니다.
        if (len == 0 && input[0] == '\n') {
            break;
        }
        // 입력받은 문자열을 출력합니다.

        getChar();
        while (1) {
            //문자열의 토큰을 다 받았을 때 break
            //break후 바깥쪽 while문 실행해서 다시 새로운 입력을 받음

            if (nextToken == EOF) {
                len = 0;
                i = 0;
                //nextToken의 상태를 초기화

                nextToken = START;
                if (iserror == TRUE) {
                    printf("Wrong input!\n");
                }
                else if (result != 9999) {
                    printf("output : %ld\n", result);
                    result = 9999;
                }
                else if (is_reverse == TRUE)
                {
                    printf("output : %s\n", reversed);
                    is_reverse = FALSE;
                }

                memset(operand, '\0', 100);
                memset(reversed, '\0', 100);
                iserror = FALSE;
                break;
            }

            lex();
            expr();
        }
    }

    return 0;
}

