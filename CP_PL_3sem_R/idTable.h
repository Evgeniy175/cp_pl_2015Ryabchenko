#pragma once
#include "lexTable.h"
#include "errors.h"
#include <vector>

#define TI_ID_MAXSIZE		12				// максимальное кол-во символов в идентификаторе
#define TI_ID_FULL_MAXSIZE	16				// максимальный размер идентификатора+имени фции
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
#define TI_ID_TYPES	{ IT::IDDATATYPE::NUM, IT::IDDATATYPE::LINE, IT::IDDATATYPE::WASH, IT::IDDATATYPE::BOOL, IT::IDDATATYPE::NIL }

#define TI_STRUCT_TYPES_SIZE	5		// размер массива типов для структуры
#define TI_STRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define TI_STRUCT_ID_TYPES { IT::IDDATATYPE::LINE, IT::IDDATATYPE::NUM, IT::IDDATATYPE::NUM, IT::IDDATATYPE::NUM, IT::IDDATATYPE::NUM, }

namespace IT		// таблица идентификаторов
{
	enum IDDATATYPE	// типы данных идентификаторов
	{
		NOTDEFINED = 0,	// uknown
		NUM	 = 1,		// integer
		LINE = 2,		// string
		WASH = 3,		// wash
		BOOL = 4,		// bool
		NIL  = 5		// аналог void в C++
	};

	enum IDTYPE		// типы идентификаторов
	{
		U = 0,			// unknown
		V = 1,			// переменная
		F = 2,			// фция
		P = 3,			// параметр
		L = 4,			// литерал
		E = 5,			// extern function
		S = 6			// элемент структуры
	};

	enum ELEMENTTYPE
	{
		UNKNOWN = 0,	// unknown
		ID = 1,			// идентификатор
		LITERAL = 2,	// литерал
		ACTION = 3		// арифметическое действие
	};

	struct IdDataType			// типы данных идентификаторов
	{
		std::vector<char*>		name_;			// для обычных типов
		std::vector<char*>		structName_;	// для структуры
		std::vector<IDDATATYPE> type_;			// для обычных типов
		std::vector<IDDATATYPE>	structType_;	// для структуры
		
		IdDataType();
	};

	struct Entry					// строка таблицы идентификаторов
	{
		int			idxFirstLE_;					// индекс первой строки в таблице лексем
		char		id_[TI_ID_FULL_MAXSIZE];		// имя (автоматически усекается до ID_MAXSIZE)
		IDTYPE		idType_;						// тип идентификатора
		IDDATATYPE	idDataType_;					// тип данных
		ELEMENTTYPE elementType_;					// тип элемента

		union
		{
			int intValue_;							// значение integer

			struct
			{
				int len_;							// количество символов в string
				char str_[TI_STR_MAXSIZE];			// символы string
			} vstr_;								// значение string
		} value_;									// значение идентификатора

		Entry();
	};

	struct IdTable					// экземпляр таблицы идентификаторов
	{
		int maxSize_;								// ёмкость таблицы идентификаторов < TI_MAXSIZE
		int size_;									// текущий размер таблицы идентификаторов < maxsize
		Entry* table_;								// массив строк таблицы идентификаторов

		IdTable();
		IdTable(int size);

		bool isIncluded(char* line);				// был ли добавлен идентификатор ранее
		int getIndex(char* line);
	};

	IdTable* create(				// создать таблицу идентификаторов
		int size									//ёмкость таблицы идентификаторов < TI_MAXSIZE
		);

	void addElement(				// добавить строку в таблицу идентификаторов
		IdTable* idTable,							// экземпляр таблицы идентификаторов
		Entry entry									// строка таблицы идентификаторов
		);

	Entry getEntry(					// получить строку таблицы идентификаторов
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

	IDDATATYPE getDataType(			// возвращает тип данных идентификатора
		LT::LexTable* lexTable,						// таблица лексем
		char** arrOfLines,							// массив цепочек
		int i										// номер текущей цепочки
		);

	IDTYPE getType(					// возвращает тип идентификатора
		LT::LexTable* lexTable						// таблица лексем
		);

	char* getDataName(IT::IDDATATYPE type);
	
	void setEntry(					// заполнение элемента для идентификатора
		IT::Entry& entry,							// элемент для заполнения
		LT::LexTable* lexTable,						// таблица лексем
		char** arrOfLines,							// массив цепочек
		int& chainNumber,							// номер текущей цепочки
		IDTYPE idType = IDTYPE::U,					// тип
		int literalCounter = -1						// счетчик литералов
		);

	void setValue(
		IT::Entry& entry,							// элемент
		char* line = NULL_STR
		);

	void addPrefix(					// добавление префикса(имени функции) к идентификатору
		char* dest,									// destination
		char* prefix								// prefix
		);
	
	void createFuncName(char* funcName, char* line);
	void reset(IT::Entry& entry);
	bool isFunction(char* first, char* second);
};