#pragma once

#include "errors.h"

#define LEXEMA_FIXSIZE  1			// ������������ ������ �������
#define LT_MAXSIZE		4096		// ������������ ���-�� ����� � ������� ������
#define LT_TI_NULL_IDX  0xffffffff	// ��� ��-�� ������� ���������������
#define LEX_TYPE		't'			// ������� ���  type
#define LEX_FUNCTION	'f'			// ������� ���  function
#define LEX_RETURN		'r'			// ������� ���  return
#define LEX_PRINT		'p'			// ������� ���  print
#define LEX_BEGIN		'b'			// ������� ���	main
#define LEX_COLON		':'			// ������� ���  :
#define LEX_SEMICOLON	';'			// ������� ���  ;
#define LEX_COMMA		','			// ������� ���  ,
#define LEX_LEFTBRACE	'{'			// ������� ���  {
#define LEX_RIGHTBRACE	'}'			// ������� ���  }
#define LEX_LEFTHESIS	'('			// ������� ���  (
#define LEX_RIGHTHESIS	')'			// ������� ���  )
#define LEX_SQBRACEOPEN '['			// ������� ���  [
#define LEX_SQBRACECLOSE ']'		// ������� ���  ]
#define LEX_EQUALLY		'='			// ������� ���	=
#define LEX_ACTIONS		'v'			// ������� ���  ��������
#define LEX_NEWLINE		'@'			// ������� ���	@ (����� ������)
#define LEX_CONDITION	'c'			// ������� ���	�������
#define LEX_EXTERN		'e'			// ������� ���	extern
#define LEX_ID			'i'			// ������� ���  ��������������
#define LEX_LITERAL		'l'			// ������� ���	��������� (�����)

namespace LT				// ������� ������
{
	struct Entry			// ������ ������� ������
	{
		char lexema_/*[LEXEMA_FIXSIZE]*/;		// �������
		int lineNumber_;					// ����� ������ � ��� ����
		int TI_Index_;						// ������ � ������� ��������������� ��� LT_TI_NULLIDX

		Entry();
	};

	struct LexTable			// ��������� ������� ������
	{
		int maxSize_;					// ������� ������� ������ < LT_MAXSIZE
		int size_;						// ������� ������ ������� ������ < maxsize
		Entry* table_;					// ������ ����� ������� ������

		LexTable();
		LexTable(int size);
	};

	LexTable* create(	// ������� ������� ������
		int size						// ������� ������� ������ < LT_MAXSIZE
		);

	void addElement(		// �������� ������ � ������� ������
		LexTable* lexTable,				// ��������� ������� ������
		Entry entry						// ������ ������� ������
		);

	Entry getEntry(			// �������� ������ ������� ������
		LexTable& lexTable,				// ��������� ������� ������
		int number						// ����� ���������� ������
		);

	char getLexem(			// ���������� ������� ��� ������� ����������� �������
		int switchCount					// ���� ����� �������� ����� ��� �����
		);

	void del(				// ������� ������� ������(���������� ������)
		LexTable* lexTable				// ������� ������
		);

	void setEntry(			// ��������� ������� �������
		LT::Entry& entry,				// ������� ��� ����������
		int& i,							// ���� ����� �������� ����� ��� �����
		int& lineNumber,				// ������� ����� ������
		int itIndex						// ������ � ������� ���������������
		);
};