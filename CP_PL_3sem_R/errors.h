#pragma once

#include <iostream>

#define ERROR_THROW(id) ERROR::getError(id);
#define ERROR_THROW_FULL(id, str, lineNumber, position) ERROR::getError(id, str, lineNumber, position);
#define ERROR_THROW_IN(id, l, c) Error::getErrorIn(id, l, c);
#define ERROR_ENTRY(id, message) new ERROR::Error(id, message, new ERROR::Position())
#define ERROR_MESSAGE_MAXSIZE 200
#define ERROR_ENTRY_NODEF(id) ERROR_ENTRY(-id, "Неопределенная ошибка")
#define ERROR_ENTRY_NODEF10(id) ERROR_ENTRY_NODEF(id+0), ERROR_ENTRY_NODEF(id+1), ERROR_ENTRY_NODEF(id+2), ERROR_ENTRY_NODEF(id+3), ERROR_ENTRY_NODEF(id+4), ERROR_ENTRY_NODEF(id+5), ERROR_ENTRY_NODEF(id+6), ERROR_ENTRY_NODEF(id+7), ERROR_ENTRY_NODEF(id+8), ERROR_ENTRY_NODEF(id+9)
#define ERROR_ENTRY_NODEF100(id) ERROR_ENTRY_NODEF10(id+0), ERROR_ENTRY_NODEF10(id+10), ERROR_ENTRY_NODEF10(id+20), ERROR_ENTRY_NODEF10(id+30), ERROR_ENTRY_NODEF10(id+40), ERROR_ENTRY_NODEF10(id+50), ERROR_ENTRY_NODEF10(id+60), ERROR_ENTRY_NODEF10(id+70), ERROR_ENTRY_NODEF10(id+80), ERROR_ENTRY_NODEF10(id+90)
#define ERROR_MAX_ENTRY 1000

namespace ERROR{
	class Position{
	public:
		Position();
		Position(int line, int position);

		short getLine();														// return line
		short getLinePosition();												// return position in line

	private:
		short line_;
		short position_;
	};

	class Error{
	public:
		Error(int id, char* message, ERROR::Position* position);

		int getId();															// return error id
		char* getMessage();
		Position* getPosition();												// return position that includes line&position in line

		void setMessage(char* message);
		void setPosition(Position* position);

		~Error();

	private:
		int id_;
		char message_[ERROR_MESSAGE_MAXSIZE];
		Position* position_;
	};

	Error* getError(int id, char* str = NULL_STR, int line = -1, int position = -1);
	Error* getErrorIn(int id, int line, int position);
};