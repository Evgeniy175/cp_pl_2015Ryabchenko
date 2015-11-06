#pragma once

#include "errors.h"
#include "auxTable.h"

#define LT_MAXSIZE			4096		// ������������ ���-�� ����� � ������� ������
#define LT_NULL_PARM		0xffffffff	// ��������� �������� ���������
#define LT_AUX_NULL_IDX		0xffffffff	// ��� ��-�� ������� ���������������
#define LT_AUX_NULL_LINE	0xffffffff
#define LT_NULL_LEX			'M'
#define LEX_TYPE			't'			// ������� ���  type
#define LEX_FUNCTION		'f'			// ������� ���  function
#define LEX_RETURN			'r'			// ������� ���  return
#define LEX_PRINT			'p'			// ������� ���  print
#define LEX_BEGIN			'b'			// ������� ���	main
#define LEX_SEMICOLON		';'			// ������� ���  ;
#define LEX_COMMA			','			// ������� ���  ,
#define LEX_LEFTBRACE		'{'			// ������� ���  {
#define LEX_RIGHTBRACE		'}'			// ������� ���  }
#define LEX_LEFTHESIS		'('			// ������� ���  (
#define LEX_RIGHTHESIS		')'			// ������� ���  )
#define LEX_SQBRACEOPEN		'['			// ������� ���  [
#define LEX_SQBRACECLOSE	']'			// ������� ���  ]
#define LEX_EQUALLY			'='			// ������� ���	=
#define LEX_OPERATION		'v'			// ������� ���  ��������
#define LEX_NEWLINE			'@'			// ������� ���	@ (����� ������)
#define LEX_CONDITION		'c'			// ������� ���	�������
#define LEX_ID				'i'			// ������� ���  ��������������
#define LEX_LITERAL			'l'			// ������� ���	��������� (�����)

namespace AT{ enum TYPE; enum DATATYPE; class Table; };

namespace LT{				// ������� ������
	class Element{			// ������ ������� ������
	public:
		Element();
		char	getLex();							// return lexeme
		int		getIdx();							// return auxiliary table index
		int		getParmCount();						// return counter for parameters
		int		getLine();							// return line number

		void	setElem();
		void	setElem(const Element& elem);
		void	setElem(int& i, int& lineNumber);
		void	setIdx(int auxIndex);				// set auxiliary table index
		void	setParmCount(int value);

	private:
		char	lexeme_;						// �������
		int		line_;							// line number
		int		atIndex_;						// ������ � ������� ��������������� ��� LT_TI_NULLIDX
		int		parameterCounter_;
	};

	class Table{			// ��������� ������� ������
	public:
		Table();
		Table(int size);

		void		addElem(Element& elem);

		int			getSize();
		Element*	getElem(int i);

		~Table();
	private:
		int maxSize_;					// ������� ������� ������ < LT_MAXSIZE
		int size_;						// ������� ������ ������� ������ < maxsize
		Element* table_;				// ������ ����� ������� ������
		//char lexArr[NUMBER_OF_GRAPHS];
	};

	char getLex(int switchCount);			// ���������� ������� ��� ������� ����������� �������
};