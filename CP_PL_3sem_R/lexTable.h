#pragma once

#include "errors.h"
#include "auxTable.h"

#define LT_MAXSIZE			4096		// максимальное кол-во строк в таблице лексем
#define LT_NULL_PARM		0xffffffff	// дефолтное значение параметра
#define LT_TI_NULL_IDX		0xffffffff	// нет эл-та таблицы идентификаторов
#define LT_TI_NULL_LINE		0xffffffff
#define LT_NULL_LEX			'M'
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
#define LEX_OPERATION		'v'			// лексема для  действий
#define LEX_NEWLINE			'@'			// лексема для	@ (новая строка)
#define LEX_CONDITION		'c'			// лексема для	условия
#define LEX_EXTERN			'e'			// лексема для	extern
#define LEX_ID				'i'			// лексема для  идентификатора
#define LEX_LITERAL			'l'			// лексема для	литералов (общая)

namespace AT{ enum TYPE; enum DATATYPE; class Table; };

namespace LT{				// таблица лексем
	class Element{			// строка таблицы лексем
	public:
		Element();
		char	getLex();
		int		getIdx();
		int		getParmCount();
		int		getLineNumber();

		void	setIdx(int tiIndex);
		void	setParmCount(int value);
		void	setElem(int& i, int& lineNumber);
		void	setElem(const Element& elem);
		void	setElem();

	private:
		char lexeme_;						// лексема
		int	 lineNumber_;					// номер строки в исх коде
		int	 auxIndex_;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
		int	 parameterCounter;
	};

	class Table{			// экземпляр таблицы лексем
	public:
		Table();
		Table(int size);

		void		addElem(Element& elem);

		int			getSize();
		Element*	getElem(int i);
		AT::TYPE	getType();	// возвращает тип идентификатора

		~Table();
	private:
		int maxSize_;					// емкость таблицы лексем < LT_MAXSIZE
		int size_;						// текущий размер таблицы лексем < maxsize
		Element* table_;				// массив строк таблицы лексем
		//char lexArr[NUMBER_OF_GRAPHS];
	};

	char getLex(int switchCount);			// возвращает лексему для текущей разобранной цепочки
};