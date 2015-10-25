#pragma once
#include "lexTable.h"
#include "errors.h"
#include <vector>

#define TI_ID_MAXSIZE		10				// максимальное кол-во символов в идентификаторе
#define TI_ID_FULL_MAXSIZE	32				// максимальный размер идентификатора+имени фции
#define TI_MAXSIZE		4096			// максимальное кол-во строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000		// значение по умолчанию для integer
#define TI_STR_DEFAULT	0x00			// значение по умолчанию для string
#define TI_NULLIDX		0xffffffff		// нет эл-та таблицы идентификаторов
#define TI_STR_MAXSIZE	255				// 
#define TI_ARR_MAXSIZE	256
#define TI_FUNC_SEP		'$'

#define TI_TYPES_SIZE	3
#define TI_TYPES		{\
	"num",\
	"line",\
	"wash"\
}

#define TI_ID_TYPES		{\
	IT::IDDATATYPE::NUM, \
	IT::IDDATATYPE::LINE, \
	IT::IDDATATYPE::WASH \
}

namespace IT						// таблица идентификаторов
{
	enum IDDATATYPE	// типы данных идентификаторов
	{
		UNKNOWN = 0,
		NUM = 1,	// integer
		LINE = 2,	// string
		WASH = 3,		// wash
	};

	enum IDTYPE	// типы идентификаторов
	{
		U = 0,  // uknown
		V = 1,	// переменная
		F = 2,	// фция
		P = 3,	// параметр
		L = 4	// литерал
	};

	struct IdDataType				// типы данных идентификаторов
	{
		std::vector<char*>		name_;
		std::vector<IDDATATYPE> type_;
		IdDataType();
	};

	struct Entry					// строка таблицы идентификаторов
	{
		int			idxFirstLE_;					// индекс первой строки в таблице лексем
		char		id_[TI_ID_FULL_MAXSIZE];		// идентификатор (автоматически усекается до ID_MAXSIZE)
		IDTYPE		idType_;						// тип идентификатора
		IDDATATYPE idDataType_;

		union
		{
			int intValue_;							// значение integer

			struct
			{
				char len_;							// количество символов в string
				char str_[TI_STR_MAXSIZE - 1];		// символы string
			} vstr_;			// значение string
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

		bool isIncluded(char* line);		// был ли добавлен идентификатор ранее
	};

	IdTable* create(					// создать таблицу идентификаторов
		int size									//ёмкость таблицы идентификаторов < TI_MAXSIZE
		);

	void addElement(						// добавить строку в таблицу идентификаторов
		IdTable* idTable,							// экземпляр таблицы идентификаторов
		Entry entry									// строка таблицы идентификаторов
		);

	Entry getEntry(					// получить строку таблицы идентификаторов
		IdTable& idTable,							// экземпляр таблицы идентификаторов
		int n										// номер получаемой строки
		);

	int isId(						// возврат: номер строки(если есть), TI_NULLIDX(если нет)
		IdTable& idTable,							// экземпляр таблицы идентификаторов
		char id[TI_ID_MAXSIZE]							// идентификатор
		);

	void del(IdTable* idTable);								// удалить таблицу идентификаторов(освободить память)
	bool isSimilar(char* firstLine, char* secondLine);		// идентичные ли цепочки

	IDDATATYPE getIdDataType(		// возвращает тип данных идентификатора
		LT::LexTable* lexTable,						// таблица лексем
		char** arrOfLines,							// массив цепочек
		int i										// номер текущей цепочки
		);

	IDDATATYPE getType(
		char* line
		);

	IDTYPE getIdType(				// возвращает тип идентификатора
		LT::LexTable* lexTable,						// таблица лексем
		char* line									// текущая цепочка
		);

	char* getIdDataName(IDDATATYPE type);
	
	void setEntry(					// заполнение элемента данными
		IT::Entry& entry,							// элемент для заполнения
		LT::LexTable* lexTable,						// таблица лексем
		int& ltIndex,							// текущая строка
		char** arrOfLines,							// массив цепочек
		int& chainNumber							// номер текущей цепочки
		);

	void addPrefix(					// добавление префикса(имени функции) к идентификатору
		char* id,									// идентификатор
		char* prefix								// имя функции
		);
	
	void createFuncName(char* funcName, char* line);
};