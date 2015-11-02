#pragma once
#include "lexAnalyser.h"
#include "errors.h"
#include <vector>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 0 как дефолтное значение инт

#define AUX_NAME_MAXSIZE	12					// максимальное число символов в имени переменной
#define AUX_MAXSIZE			4096				// максимальное кол-во строк в таблице идентификаторов
#define AUX_NUM_DEFAULT		0x00000000			// значение по умолчанию дл€ num
#define AUX_LINE_DEFAULT	0x00				// значение по умолчанию дл€ line
#define AUX_NULLIDX			0xffffffff			// нет эл-та таблицы идентификаторов
#define AUX_LINE_MAXSIZE	255					// 
#define AUX_ARR_MAXSIZE		256					//
#define AUX_LITERAL_PREFIX	"L"

#define AUX_DATA_SIZE	5				// размер массива типов
#define AUX_DATA_NAMES { "num", "line", "wash", "bool", "nil" }
#define AUX_DATA_TYPES	{ AUX::DATATYPE::NUM, AUX::DATATYPE::LINE, AUX::DATATYPE::WASH, AUX::DATATYPE::BOOL, AUX::DATATYPE::NIL }

#define AUX_DATASTRUCT_SIZE	5			// размер массива типов дл€ структуры
#define AUX_DATASTRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define AUX_DATASTRUCT_TYPES { AUX::DATATYPE::LINE, AUX::DATATYPE::NUM, AUX::DATATYPE::NUM, AUX::DATATYPE::NUM, AUX::DATATYPE::NUM }

namespace LA { class LexAnalyser; };

namespace AUX{		// дополнительна€ таблица

	enum TYPE{			// типы
		U = 0,				// unknown
		V = 1,				// переменна€
		F = 2,				// фци€
		P = 3,				// параметр
		L = 4,				// литерал
		E = 5,				// extern function
		S = 6				// элемент структуры
	};

	enum DATATYPE{		// типы данных
		UNKNOWN = 0,		// unknown
		NUM  = 1,			// integer
		LINE = 2,			// string
		WASH = 3,			// wash
		BOOL = 4,			// bool
		NIL  = 5			// аналог void в C++
	};

	class DataStruct{		// типы данных идентификаторов
	public:
		DataStruct();

		std::vector<char*>&		getName();
		std::vector<char*>&		getStructName();
		std::vector<DATATYPE>&	getType();
		std::vector<DATATYPE>&	getStructType();

	private:
		std::vector<char*>		name_;			// дл€ обычных типов
		std::vector<char*>		structName_;	// дл€ структуры
		std::vector<DATATYPE>	type_;			// дл€ обычных типов
		std::vector<DATATYPE>	structType_;	// дл€ структуры
	};

	class Element{					// строка дополнительной таблицы
	public:
		Element();

		void reset();

		int			getIdx();				// get lexTable index
		int			getIntVal();
		char*		getName();
		char*		getFuncName();			// get name of function
		char*		getStrVal();
		TYPE		getType();
		DATATYPE	getDataType();

		void		setIdx(int value);		// set lexTable index
		void		setIntVal(int value);
		void		setName(char* name);
		void		setFuncName(char* name);
		void		setStrVal(char* value);
		void		setValue(char lexeme, char* line = NULL_STR);
		void		setElem(						// заполнение элемента дл€ идентификатора
						LA::LexAnalyser* la,				// таблица лексем
						char*			funcName,				// им€ функции
						char**			arrOfLines,				// массив цепочек
						int&			i,						// номер текущей цепочки
						TYPE			idType = TYPE::U,		// тип
						int				literalCounter = -1		// счетчик литералов
					);

	private:
		int			ltIndex_;					// индекс первой строки в таблице лексем
		char		name_[AUX_NAME_MAXSIZE];		// им€ (автоматически усекаетс€ до ID_MAXSIZE)
		char		funcName_[AUX_NAME_MAXSIZE];
		TYPE		type_;						// тип идентификатора
		DATATYPE	dataType_;					// тип данных
		struct{
			int		intValue_;
			char	strValue_[AUX_ARR_MAXSIZE];
		} value_;
	};

	class Table{					// экземпл€р таблицы идентификаторов
	public:
		Table();
		Table(int size);

		Element*	getElem(int i);
		int			getSize();
		DataStruct*	getDataStruct();

		void		addElem(Element elem);

		bool		isIncluded(char* line, char* funcName);
		int			getIdx(char* name, char* funcName);			// get index [i] in table_ for current name&funcName

		~Table();

	private:
		int maxSize_;								// максимальный размер таблицы идентификаторов < TI_MAXSIZE
		int size_;									// текущий размер таблицы идентификаторов < maxsize
		Element* table_;							// массив строк таблицы идентификаторов
		DataStruct*	dataStruct_;
	};



	void addPrefix(char* dest, char* prefix);
	void createFuncName(char* funcName, char* line);
	bool isFunction(char* first, char* second);
	char* createStrVal(char* line);
};