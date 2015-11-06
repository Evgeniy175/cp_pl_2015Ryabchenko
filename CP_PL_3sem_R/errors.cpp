#include "stdafx.h"
#include "errors.h"

// ���������� ��� ����

namespace ERROR{
	Error* a = ERROR_ENTRY(0, "������������ ��� ������");

	Error* errors[ERROR_MAX_ENTRY] = {
		ERROR_ENTRY(0, "������������ ��� ������"),
		ERROR_ENTRY(1, "��������� ����"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5), ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "�������� -in ������ ���� �����"),
		ERROR_ENTRY(101, "��������� ����� �������� ���������"),
		ERROR_ENTRY(102, "������ ��� �������� ����� � �������� �����(-in)"),
		ERROR_ENTRY(103, "������������ ������ � �������� �����(-in)"),
		ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "�������� ������������ ������ ������� ������"),
		ERROR_ENTRY(201, "�������� ������������ ������ ������� ���������������"),
		ERROR_ENTRY(202, " !������� �� ����������:\t"),
		ERROR_ENTRY(203, "������������ ������������� ���������� "),
		ERROR_ENTRY_NODEF(204), ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY_NODEF(300), ERROR_ENTRY_NODEF(301), ERROR_ENTRY_NODEF(302), ERROR_ENTRY_NODEF(303), ERROR_ENTRY_NODEF(304), ERROR_ENTRY_NODEF(305), ERROR_ENTRY_NODEF(306), ERROR_ENTRY_NODEF(307), ERROR_ENTRY_NODEF(308), ERROR_ENTRY_NODEF(309),
		ERROR_ENTRY_NODEF10(310), ERROR_ENTRY_NODEF10(320), ERROR_ENTRY_NODEF10(330), ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350), ERROR_ENTRY_NODEF10(360), ERROR_ENTRY_NODEF10(370), ERROR_ENTRY_NODEF10(380), ERROR_ENTRY_NODEF10(390),
	};

	Position::Position(){
		this->line_ = -1;
		this->position_ = -1;
	};

	Position::Position(int line, int position){
		this->line_ = line;
		this->position_ = position;
	};

	short Position::getLine(){
		return this->line_;
	};

	short Position::getLinePosition(){
		return this->position_;
	};

	Error::Error(int id, char* message, ERROR::Position* position){
		this->id_ = id;
		strncpy_s(this->message_, message, ERROR_MESSAGE_MAXSIZE - 1);
		this->position_ = position;
	};

	int Error::getId(){
		return this->id_;
	};

	char* Error::getMessage(){
		return this->message_;
	};

	Position* Error::getPosition(){
		return this->position_;
	};

	void Error::setMessage(char* message){
		strcpy_s(this->message_, message_);
	};

	void Error::setPosition(Position* position){
		this->position_ = position;
	};

	Error::~Error(){
		delete this->position_;
	}

	Error* getError(int id, char* str, int line, int position){
		Error* rc = errors[0];

		if (0 < id && id < ERROR_MAX_ENTRY)
			rc = errors[id];

		if (line != NULL_STR)
			rc->setMessage(str);

		if (line >= NULL && position >= NULL)
			rc->setPosition(new Position(line, position));

		return rc;
	};

	Error* getErrorIn(int id, int line = -1, int col = -1){
		Error* rc = getError(0);

		if (0 < id && id < ERROR_MAX_ENTRY){
			rc = getError(id);
			rc->setPosition(new ERROR::Position(line, col));
		};
		return rc;
	};
};