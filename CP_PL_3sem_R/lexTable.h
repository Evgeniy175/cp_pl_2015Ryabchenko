#pragma once

#include "errors.h"

#define LEXEMA_FIXSIZE  1			// фиксировнный размер лексемы
#define LT_MAXSIZE		4096		// максимальное кол-во строк в таблице лексем
#define LT_TI_NULL_IDX  0xffffffff	// нет эл-та таблицы идентификаторов
#define LEX_TYPE		't'			// лексема для  type
#define LEX_FUNCTION	'f'			// лексема для  function
#define LEX_RETURN		'r'			// лексема для  return
#define LEX_PRINT		'p'			// лексема для  print
#define LEX_BEGIN		'b'			// лексема для	main
#define LEX_COLON		':'			// лексема для  :
#define LEX_SEMICOLON	';'			// лексема для  ;
#define LEX_COMMA		','			// лексема для  ,
#define LEX_LEFTBRACE	'{'			// лексема для  {
#define LEX_RIGHTBRACE	'}'			// лексема для  }
#define LEX_LEFTHESIS	'('			// лексема для  (
#define LEX_RIGHTHESIS	')'			// лексема для  )
#define LEX_SQBRACEOPEN '['			// лексема для  [
#define LEX_SQBRACECLOSE ']'		// лексема для  ]
#define LEX_EQUALLY		'='			// лексема для	=
#define LEX_ACTIONS		'v'			// лексема для  действий
#define LEX_NEWLINE		'@'			// лексема для	@ (новая строка)
#define LEX_CONDITION	'c'			// лексема для	условия
#define LEX_EXTERN		'e'			// лексема для	extern
#define LEX_ID			'i'			// лексема для  идентификатора
#define LEX_LITERAL		'l'			// лексема для	литералов (общая)

namespace LT				// таблица лексем
{
	struct Entry			// строка таблицы лексем
	{
		char lexema_/*[LEXEMA_FIXSIZE]*/;		// лексема
		int lineNumber_;					// номер строки в исх коде
		int TI_Index_;						// индекс в таблице идентификаторов или LT_TI_NULLIDX

		Entry();
	};

	struct LexTable			// экземпляр таблицы лексем
	{
		int maxSize_;					// емкость таблицы лексем < LT_MAXSIZE
		int size_;						// текущий размер таблицы лексем < maxsize
		Entry* table_;					// массив строк таблицы лексем

		LexTable();
		LexTable(int size);
	};

	LexTable* create(	// создать таблицу лексем
		int size						// емкость таблицы лексем < LT_MAXSIZE
		);

	void addElement(		// добавить строку в таблицу лексем
		LexTable* lexTable,				// экземпляр таблицы лексем
		Entry entry						// строка таблицы лексем
		);

	Entry getEntry(			// получить строку таблицы лексем
		LexTable& lexTable,				// экземпляр таблицы лексем
		int number						// номер получаемой строки
		);

	char getLexem(			// возвращает лексему для текущей разобранной цепочки
		int switchCount					// итый номер итерации цикла для свича
		);

	void del(				// удалить таблицу лексем(освободить память)
		LexTable* lexTable				// таблица лексем
		);

	void setEntry(			// заполнить элемент данными
		LT::Entry& entry,				// элемент для заполнения
		int& i,							// итый номер итерации цикла для свича
		int& lineNumber,				// текущий номер строки
		int itIndex						// индекс в таблице идентификаторов
		);
};