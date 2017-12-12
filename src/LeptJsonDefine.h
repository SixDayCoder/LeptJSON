#ifndef  LEPTJSONDEFINE_H_
#define  LEPTJSONDEFINE_H_

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LEPT_PARSE_STACK_INIT_SIZE 128

enum class LeptType
{
	LEPT_NULL,
	LEPT_FALSE,
	LEPT_TRUE,
	LEPT_NUMBRE,
	LEPT_STRING,
	LEPT_ARRAY,
	LEPT_OBJECT
};


enum class LeptParseRet
{
	LEPT_PARSE_OK,
	LEPT_PARSE_EXPCET_VALUE,
	LEPT_PARSE_INVALID_VALUE,
	LEPT_PARSE_ROOT_NOT_SINGULAR,
	LEPT_PARSE_STRING_MISS_QUOTATION_MARK,
	LEPT_PARSE_NUMBER_TOO_BIG
};


struct LeptString {

	char* cstr;
	int32_t len;

	void Init() {
		cstr = nullptr;
		len = 0;
	}

	void SetString(const char* str, int32_t len) {
		assert((str != nullptr) && (len >= 0));

		CleanUp();//���ԭֵ
		cstr = (char*)malloc(len + 1);
		memcpy(cstr, str, len);
		cstr[len] = '\0';//C����ַ���
		this->len = len;

	}

	void CleanUp() {
		if (cstr != nullptr) {
			free(cstr);
		}
		cstr = nullptr;
		len = 0;
	}
};


struct LeptValue 
{
	LeptString str;


	double number;
	LeptType type;
	
	void Init() {
		type = LeptType::LEPT_NULL;
		number = 0.0f;
		str.Init();
	}
	
	void SetNumber(double num) {
		number = num;
		type = LeptType::LEPT_NUMBRE;
	}

	double GetNumber()const {
		assert(type == LeptType::LEPT_NUMBRE);
		return number;
	}

	void SetString(const char* cstr, int32_t len) {
		str.SetString(cstr, len);
		type = LeptType::LEPT_STRING;
	};

	LeptString GetString()const {
		assert(type == LeptType::LEPT_STRING);
		return str;
	}

	void CleanUp() {

		type = LeptType::LEPT_NULL;
		number = 0.0f;
		str.CleanUp();

	}

};


struct LeptBufferStack {

	char* stack;
	int32_t size;
	int32_t top;

	void CleanUp() {
		if (stack != nullptr) {
			free(stack);
		}
		stack = nullptr;
		size = top = 0;
	}

	void PutChar(char ch) {
		*Push(1) = ch;
	}

	//����pushǰ��������ʼָ��
	char* Push(int32_t size) {
		assert(size > 0);
		//��Ҫ����
		if (top + size >= this->size) {
			//���η���size
			if (this->size == 0)
				this->size = LEPT_PARSE_STACK_INIT_SIZE;
			//����size
			while (top + size >= this->size) 
				this->size += this->size >> 1;/*size * 1.5,ÿ������1.5��*/
			//���·����ڴ�
			stack = (char*)realloc(stack, this->size);
		}

		char* ret = stack + top;
		top += size;

		return ret;
	}

	//����pop���topָ��
	char* Pop(int32_t size) {
		assert(top >= size);
		top -= size;
		return stack + top;
	}
};

struct LeptContext
{
	const char* json;
	LeptBufferStack stack;
};


#endif // !LEPTJSONDEFINE_H_
