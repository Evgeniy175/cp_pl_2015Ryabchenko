#pragma once
#include "lexTable.h"
#include "errors.h"
#include <vector>

#define TI_ID_MAXSIZE		12				// максимальное число символов в имени переменной
#define TI_MAXSIZE		4096				// максимальное кол-во строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000			// значение по умолчанию для integer
#define TI_STR_DEFAULT	0x00				// значение по умолчанию для string
#define TI_NULLIDX		0xffffffff			// нет эл-та таблицы идентификаторов
#define TI_STR_MAXSIZE	255					// 
#define TI_ARR_MAXSIZE	256					// 
#define TI_FUNC_SEP		'$'					// сепаратор, добавляемый между функцией и именем переменной
#define TI_LIT_PREFIX	"L"

#define TI_TYPES_SIZE	5				// размер массива типов
#define TI_TYPES { "num", "line", "wash", "bool", "nil" }
#define TI_ID_TYPES	{ IT::DATATYPE::NUM, IT::DATATYPE::LINE, IT::DATATYPE::WASH, IT::DATATYPE::BOOL, IT::DATATYPE::NIL }

#define TI_STRUCT_TYPES_SIZE	5		// размер массива типов для структуры
#define TI_STRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define TI_STRUCT_ID_TYPES { IT::DATATYPE::LINE, IT::DATATYPE::NUM, IT::DATATYPE::NUM, IT::DATATYPE::NUM, IT::DATATYPE::NUM, }

namespace IT		// дополнительная таблица
{
	enum TYPE{			// типы
		U = 0,				// unknown
		V = 1,				// переменная
		F = 2,				// фция
		P = 3,				// параметр
		L = 4,				// литерал
		E = 5,				// extern function
		S = 6				// элемент структуры
	};

	enum DATATYPE{		// типы данных
		NOTDEFINED = 0,		// unknown
		NUM  = 1,			// integer
		LINE = 2,			// string
		WASH = 3,			// wash
		BOOL = 4,			// bool
		NIL  = 5			// аналог void в C++
	};

	struct IdDataType			// типы данных идентификаторов
	{
		std::vector<char*>		name_;			// для обычных типов
		std::vector<char*>		structName_;	// для структуры
		std::vector<DATATYPE>	type_;			// для обычных типов
		std::vector<DATATYPE>	structType_;	// для структуры

		IdDataType();
	};

	class Element					// строка дополнительной таблицы
	{
	public:
		Element();

		int			getIdx();
		int			getIntVal();
		char*		getName();
		char*		getFuncName();
		char*		getStrVal();
		TYPE		getType();
		DATATYPE	getDataType();

		void		setIdx(int value);
		void		setIntVal(int value);
		void		setName(char* name);
		void		setFuncName(char* name);
		void		setStrVal(char* value);
		void		setType(TYPE type);
		void		setDataType(DATATYPE dataType);

	private:
		int			ltIndex_;					// индекс первой строки в таблице лексем
		char		name_[TI_ID_MAXSIZE];		// имя (автоматически усекается до ID_MAXSIZE)
		char		funcName_[TI_ID_MAXSIZE];
		TYPE		type_;						// тип идентификатора
		DATATYPE	dataType_;					// тип данных
		struct{
			int intValue_;
			char strValue_[TI_ARR_MAXSIZE];
		} value_;
	};

	struct IdTable					// экземпляр таблицы идентификаторов
	{
		int maxSize_;								// ёмкость таблицы идентификаторов < TI_MAXSIZE
		int size_;									// текущий размер таблицы идентификаторов < maxsize
		Element* table_;							// массив строк таблицы идентификаторов

		IdTable();
		IdTable(int size);

		bool isIncluded(char* line, char* funcName);				// был ли добавлен идентификатор ранее
		int getIndex(char* name, char* funcName);
	};

	IdTable* create(				// создать таблицу идентификаторов
		int size									//ёмкость таблицы идентификаторов < TI_MAXSIZE
		);

	void addElement(				// добавить строку в таблицу идентификаторов
		IdTable* idTable,							// экземпляр таблицы идентификаторов
		Element elem									// строка таблицы идентификаторов
		);

	Element getEntry(					// получить строку таблицы идентификаторов
		IdTable& idTable,							// экземпляр таблицы идентификаторов
		int n										// номер получаемой строки
		);

	int isId(						// возврат: номер строки(если есть), TI_NULLIDX(если нет)
		IdTable& idTable,							// экземпляр таблицы идентификаторов
		char id[TI_ID_MAXSIZE]						// идентификатор
		);

	void del(IdTable* idTable);		// удалить таблицу идентификаторов(освободить память)

	bool isSimilar(					// идентичные ли цепочки
		char* firstLine,
		char* secondLine
		);

	DATATYPE getDataType(			// возвращает тип данных идентификатора
		LT::LexTable* lexTable,						// таблица лексем
		char** arrOfLines,							// массив цепочек
		int i										// номер текущей цепочки
		);

	TYPE getType(					// возвращает тип идентификатора
		LT::LexTable* lexTable						// таблица лексем
		);

	char* getDataName(IT::DATATYPE type);

	void setEntry(					// заполнение элемента для идентификатора
		IT::Element& elem,							// элемент для заполнения
		LT::LexTable* lexTable,						// таблица лексем
		char* funcName,
		char** arrOfLines,							// массив цепочек
		int& i,										// номер текущей цепочки
		TYPE idType = TYPE::U,						// тип
		int literalCounter = -1						// счетчик литералов
		);

	void setValue(
		IT::Element& elem,							// элемент
		char lexeme,
		char* line = NULL_STR
		);

	void addPrefix(					// добавление префикса(имени функции) к идентификатору
		char* dest,									// destination
		char* prefix								// prefix
		);

	void createFuncName(char* funcName, char* line);
	void reset(IT::Element& elem);
	bool isFunction(char* first, char* second);
	char* createStrVal(char* line);
};