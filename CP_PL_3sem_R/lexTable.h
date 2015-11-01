#pragma once

#include "errors.h"

#define LEXEMA_FIXSIZE		1			// фиксировнный размер лексемы
#define LT_MAXSIZE			4096		// максимальное кол-во строк в таблице лексем
#define LT_TI_NULL_IDX		0xffffffff	// нет эл-та таблицы идентификаторов
#define LEX_TYPE			't'			// лексема для  type
#define LEX_FUNCTION		'f'			// лексема для  function
#define LEX_RETURN			'r'			// лексема для  return
#define LEX_PRINT			'p'			// лексема для  print
#define LEX_BEGIN			'b'			// лексема для	main
#define LEX_COLON			':'			// лексема для  :
#define LEX_SEMICOLON		';'			// лексема для  ;
#define LEX_COMMA			','			// лексема для  ,
#define LEX_LEFTBRACE		'{'			// лексема для  {
#define LEX_RIGHTBRACE		'}'			// лексема для  }
#define LEX_LEFTHESIS		'('			// лексема для  (
#define LEX_RIGHTHESIS		')'			// лексема для  )
#define LEX_SQBRACEOPEN		'['			// лексема для  [
#define LEX_SQBRACECLOSE	']'			// лексема для  ]
#define LEX_EQUALLY			'='			// лексема для	=
#define LEX_ACTIONS			'v'			// лексема для  действий
#define LEX_NEWLINE			'@'			// лексема для	@ (новая строка)
#define LEX_CONDITION		'c'			// лексема для	условия
#define LEX_EXTERN			'e'			// лексема для	extern
#define LEX_ID				'i'			// лексема для  идентификатора
#define LEX_LITERAL			'l'			// лексема для	литералов (общая)

namespace LT				// таблица лексем
{
	class Element			// строка таблицы лексем
	{
	public:
		Element();
		char getLex();
		int	 getIdx();
		int  getLineNumber();
		void setLex(char lexeme);
		void setLineNumber(int lineNumber);
		void setIdx(int tiIndex);
		void setElem(int& i, int& lineNumber, int itIndex);

	private:
		char lexeme_;						// лексема
		int	 lineNumber_;					// номер строки в исх коде
		int	 itIndex_;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
	};

	class LexTable			// экземпляр таблицы лексем
	{
	public:
		LexTable();
		LexTable(int size);
		void setMaxSize(int maxSize);
		void addElem(Element elem);
		int getSize();
		Element* getTable();
		Element* getElem(int i);

	private:
		int maxSize_;					// емкость таблицы лексем < LT_MAXSIZE
		int size_;						// текущий размер таблицы лексем < maxsize
		Element* table_;					// массив строк таблицы лексем
	};

	LexTable* create(	// создать таблицу лексем
		int size						// емкость таблицы лексем < LT_MAXSIZE
		);

	char getLexem(			// возвращает лексему для текущей разобранной цепочки
		int switchCount					// итый номер итерации цикла для свича
		);

	void del(				// удалить таблицу лексем(освободить память)
		LexTable* lexTable				// таблица лексем
		);
};