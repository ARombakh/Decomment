/*------------------------------------------------*/
/* Author: Artyom Rombakh                         */
/*------------------------------------------------*/

#include <stdio.h>
#include <stdbool.h>

enum Statetype {USUAL_TEXT, PRECOMM, COMM, POSTCOMM, LIT1, LIT2, ESC1, ESC2, ACCEPT, REJECT};
int pc; /*declaring saved previous character*/
int line;
int wrong_line;

/*Implement the USUAL_TEXT state of the DFA. */

enum Statetype handleUsualText(int c)
{
	enum Statetype state;
	switch (c) {
		case '/':
			pc = c;
			state = PRECOMM;
			break;
		case '\'':
			putchar(c);
			state = LIT1;
			break;
		case '\"':
			putchar(c);
			state = LIT2;
			break;
		case EOF:
			state = ACCEPT;
			break;
		default:
			putchar(c);		/*If everything but /, ", ' then print getchar*/
			state = USUAL_TEXT;
	}
	return state;
}

/*Implement the PRECOMM state of the DFA*/

enum Statetype handlePreComm(int c)
{
	enum Statetype state;
	switch (c) {
		case '/':
			putchar(pc);
			pc = c;
			state = PRECOMM;
			break;
		case '*':
			putchar(' '); /*check for possibility of prohibiting variable usage*/
			wrong_line = line;
			state = COMM;
			break;
		case EOF:
			putchar(pc);
			state = ACCEPT;
			break;
		case '\'':
			putchar(pc);
			putchar(c);
			state = LIT1;
			break;
		case '\"':
			putchar(pc);
			putchar(c);
			state = LIT2;
			break;
		default:
			putchar(pc);
			putchar(c);
			state = USUAL_TEXT;
	}
	return state;
}

/*Implement the COMM state of the DFA*/

enum Statetype handleComm(int c)
{
	enum Statetype state;
	switch (c) {
		case '\n':
			putchar(c);
			state = COMM;
			break;
		case '*':
			state = POSTCOMM;
			break;
		case EOF:
			state = REJECT;
			break;
		default:
			state = COMM;
	}
	return state;
}

/*Implement the POSTCOMM state of the DFA*/

enum Statetype handlePostComm(int c)
{
	enum Statetype state;
	switch (c) {
		case '\n':
			putchar(c);
			state = COMM;
			break;
		case '*':
			state = POSTCOMM;
			break;
		case EOF:
			state = REJECT;
			break;
		case '/':
			state = USUAL_TEXT;
			break;
		default:
			state = COMM;
	}
	return state;
}

/*Implement the LIT1 state of the DFA*/

enum Statetype handleLit1(int c)
{
	enum Statetype state;
	switch (c) {
		case '\'':
			putchar(c);
			state = USUAL_TEXT;
			break;
		case '\\':
			putchar(c);
			state = ESC1;
			break;
		case EOF:
			state = ACCEPT;
			break;
		default:
			putchar(c);
			state = LIT1;
	}
	return state;
}

/*Implement the LIT2 state of the DFA*/

enum Statetype handleLit2(int c)
{
	enum Statetype state;
	switch (c) {
		case '\"':
			putchar(c);
			state = USUAL_TEXT;
			break;
		case '\\':
			putchar(c);
			state = ESC2;
			break;
		case EOF:
			state = ACCEPT;
			break;
		default:
			putchar(c);
			state = LIT2;
	}
	return state;
}

/*Implement the ESC1 state of the DFA*/

enum Statetype handleEsc1(int c)
{
	enum Statetype state;
	switch (c) {
		case EOF:
			state = ACCEPT;
			break;
		default:
			putchar(c);
			state = LIT1;
	}
	return state;
}

/*Implement the ESC2 state of the DFA*/

enum Statetype handleEsc2(int c)
{
	enum Statetype state;
	switch (c) {
		case EOF:
			state = ACCEPT;
			break;
		default:
			putchar(c);
			state = LIT2;
	}
	return state;
}

int main(void)
{
	int c;
	char err1[13] = "Error: line ";
	char err2[23] = ": unterminated comment";
	line = 1;
	enum Statetype state = USUAL_TEXT;
	do {
		c = getchar();
		if (c == '\n') {
			line++;
		};
		switch (state) {
			case USUAL_TEXT:
				state = handleUsualText(c);
				break;
			case PRECOMM:
				state = handlePreComm(c);
				break;
			case COMM:
				state = handleComm(c);
				break;
			case POSTCOMM:
				state = handlePostComm(c);
				break;
			case LIT1:
				state = handleLit1(c);
				break;
			case LIT2:
				state = handleLit2(c);
				break;
			case ESC1:
				state = handleEsc1(c);
				break;
			case ESC2:
				state = handleEsc2(c);
				break;
		}
	} while (c != EOF);

	if (state == REJECT){
		fprintf(stderr, "%s%d%s\n", err1, wrong_line, err2);
		return 1;
	} else {
		return 0;
	}
}



/*
Solutions to handling EOF:
	Infinite loop with cases of EOF in functions
	Variable of accepting/rejecting state

	Loop terminating when state is in acc or rej


Everything to happen in the first moment happens in Usual text*/
