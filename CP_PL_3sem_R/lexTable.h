#pragma once

#include "errors.h"
#include "auxTable.h"

#define LT_MAX_SIZE				4096		// max number of lines in lex table
#define LT_NULL_PARM			0xffffffff	// default value for parameter counter
#define LT_AUX_NULL_INDEX		0xffffffff	// default value for index
#define LT_AUX_NULL_LINE		0xffffffff	// default value for line
#define LT_NULL_LEX				'M'			// null lexeme
#define LEX_TYPE				't'			// lexeme for type
#define LEX_FUNCTION			'f'			// lexeme for function
#define LEX_RETURN				'r'			// lexeme for return
#define LEX_BEGIN				'b'			// lexeme for main
#define LEX_SEMICOLON			';'			// lexeme for ;
#define LEX_COMMA				','			// lexeme for ,
#define LEX_OPEN_BRACE			'{'			// lexeme for {
#define LEX_CLOSE_BRACE			'}'			// lexeme for }
#define LEX_OPEN_PARENTHESIS	'('			// lexeme for (
#define LEX_CLOSE_PARENTHESIS	')'			// lexeme for )
#define LEX_OPEN_SQBRACE		'['			// lexeme for [
#define LEX_CLOSE_SQBRACE		']'			// lexeme for ]
#define LEX_COMPARE				'#'			// lexeme for compare (==, !=, <=, >=)
#define LEX_EQUAL				'='			// lexeme for =
#define LEX_OPERATION			'v'			// lexeme for operations
#define LEX_NEW_LINE			'@'			// lexeme for @ (new line)
#define LEX_CONDITION			'c'			// lexeme for conditions
#define LEX_IDENTIFIER			'i'			// lexeme for identificator
#define LEX_LITERAL				'l'			// lexeme for literal

namespace AT{ enum TYPE; enum DATATYPE; class Table; };

namespace LT{		// lexeme table
	class Element{		// element of the lexeme table
	public:
		Element();

		char				getLex();							// return lexeme
		int					getIndex();							// return auxiliary table index
		int					getParmCount();						// return counter for parameters
		int					getLine();							// return line number

		void				setElem();							// set element to default
		void				setElem(const Element& elem);		// set copy elem field's into this element
		void				setElem(int& i, int& lineNumber);	// set element
		void				setIndex(int auxIndex);				// set auxiliary table index
		void				setParmCount(int value);			// set parameter counter

	private:
		char				lexeme_;
		int					line_;								// line number
		int					atIndex_;							// index of this element in auxiliary table
		int					parameterCounter_;					// number of parameters (for function, using in Polish Notation)
	};

	class Table{		// lex table
	public:
		Table();
		Table(int size);
		
		int					getSize();							// return size of lexeme table
		Element*			getElem(int index);					// return element by index

		void				addElem(Element& elem);				// add element to the lexeme table

		~Table();

	private:
		int					maxSize_;							// max size of the lexeme table
		int					size_;								// current size of the lexeme table
		Element*			table_;								// lexeme table that consist elements
	};

	char getLex(int i);											// return lexeme by index
};