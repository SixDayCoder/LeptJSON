//#ifndef  LEPTJSONDEFINE_H_
//#define  LEPTJSONDEFINE_H_
//
//#include <assert.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>
//#include <vector>
//
//#define LEPT_PARSE_STACK_INIT_SIZE 128
//
//
//struct LeptString;
//struct LeptValue;
//struct LeptArray;
//struct LeptStackBuffer;
//struct LeptContext;
//
//enum class LeptType
//{
//	LEPT_NULL,
//	LEPT_FALSE,
//	LEPT_TRUE,
//	LEPT_NUMBRE,
//	LEPT_STRING,
//	LEPT_ARRAY,
//	LEPT_OBJECT
//};
//
//
//enum class LeptParseRet
//{
//	LEPT_PARSE_OK,
//	LEPT_PARSE_EXPCET_VALUE,
//	LEPT_PARSE_INVALID_VALUE,
//	LEPT_PARSE_ROOT_NOT_SINGULAR,
//	LEPT_PARSE_STRING_MISS_QUOTATION_MARK,
//	LEPT_PARSE_STRING_INVALID_ESCAPE,//�������˷Ƿ���ת���
//	LEPT_PARSE_STRING_INVALID_CHAR,
//	LEPT_PARSE_ARRAY_MISS_COMMA_OR_SQUARE_BRACKET,
//	LEPT_PARSE_NUMBER_TOO_BIG
//};
//
//
//struct LeptString {
//
//	char* cstr;
//	int32_t len;
//
//	//LeptString(const LeptString& other) {
//	//	if (this == &other) {
//	//		return;
//	//	}
//
//	//	this->CleanUp();
//
//	//	cstr = (char*)malloc(other.len + 1);
//	//	memcpy(cstr, other.cstr, len);
//	//	cstr[len] = '\0';
//	//	this->len = len;
//	//}
//
//	void Init() {
//		cstr = nullptr;
//		len = 0;
//	}
//
//	void SetString(const char* str, int32_t len) {
//
//		assert( ( (str != nullptr) && (len >= 0) ) ||  ( (str == nullptr) && (len == 0) ) );
//		
//		CleanUp();//���ԭֵ
//
//		cstr = (char*)malloc(len + 1);
//		//���ַ���
//		if (str == nullptr && len == 0) {
//			cstr[0] = '\0';
//		}
//		else {
//			memcpy(cstr, str, len);
//			cstr[len] = '\0';//C����ַ���
//		}
//		this->len = len;
//
//	}
//
//	void CleanUp() {
//		if (cstr != nullptr) {
//			free(cstr);
//		}
//		cstr = nullptr;
//		len = 0;
//	}
//};
//
//struct LeptArray {
//
//	std::vector<LeptValue*> values;
//
//	void Init() {
//		for (size_t i = 0; i < values.size(); ++i) {
//			values[i] = nullptr;
//		}
//	}
//
//	void PushBack(LeptValue& value) {
//		values.push_back(&value);
//	}
//
//	void CleanUp() {
//		for (size_t i = 0; i < values.size(); ++i) {
//			if (values[i] != nullptr) {
//				free(values[i]);
//				values[i] = nullptr;
//			}
//		}
//	}
//
//};
//
//struct LeptValue 
//{
//	LeptType type;
//
//	const char* constliteral;
//	double number;
//	LeptString str;
//	LeptArray arrays;
//	
//	//LeptValue(const LeptValue& other) {
//	//	if (this == &other) {
//	//		return;
//	//	}
//
//	//	this->CleanUp();
//
//	//	type = other.type;
//	//	number = other.number;
//	//	str = other.str;
//	//	arrays = other.arrays;
//	//
//	//}
//
//	void Init() {
//		type = LeptType::LEPT_NULL;
//		number = 0.0f;
//		constliteral = nullptr;
//		str.Init();
//		arrays.Init();
//	}
//	
//	void SetNumber(double num) {
//		number = num;
//		type = LeptType::LEPT_NUMBRE;
//	}
//
//	double GetNumber()const {
//		assert(type == LeptType::LEPT_NUMBRE);
//		return number;
//	}
//
//	void SetString(const char* cstr, int32_t len) {
//		str.SetString(cstr, len);
//		type = LeptType::LEPT_STRING;
//	};
//
//	LeptString GetString()const {
//		assert(type == LeptType::LEPT_STRING);
//		return str;
//	}
//
//	const LeptArray& GetArray()const {
//		assert(type == LeptType::LEPT_ARRAY);
//		return arrays;
//	}
//
//	LeptArray& GetArray(){
//		assert(type == LeptType::LEPT_ARRAY);
//		return arrays;
//	}
//
//	void CleanUp(){
//		type = LeptType::LEPT_NULL;
//		number = 0.0f;
//		constliteral = nullptr;
//		str.CleanUp();
//		arrays.CleanUp();
//	}
//
//};
//
//
//struct LeptStackBuffer {
//
//	char* stack;
//	int32_t size;
//	int32_t top;
//
//	void Init() {
//		stack = nullptr;
//		size = top = 0;
//	}
//
//	void CleanUp() {
//		if (stack != nullptr) {
//			free(stack);
//		}
//		stack = nullptr;
//		size = top = 0;
//	}
//
//	void PutChar(char ch) {
//		char* base = Push(sizeof(char));
//		if(base != nullptr)
//			*base = ch;
//	}
//
//	//����pushǰ��������ʼָ��
//	char* Push(int32_t size) {
//		assert(size > 0);
//		//��Ҫ����
//		if (top + size >= this->size) {
//			//���η���size
//			if (this->size == 0)
//				this->size = LEPT_PARSE_STACK_INIT_SIZE;
//			//����size
//			while (top + size >= this->size) 
//				this->size += this->size >> 1;/*size * 1.5,ÿ������1.5��*/
//			//���·����ڴ�
//			stack = (char*)realloc(stack, this->size);
//		}
//
//		char* ret = stack + top;
//		top += size;
//
//		return ret;
//	}
//
//	//����pop���topָ��
//	char* Pop(int32_t size) {
//		assert(top >= size);
//		top -= size;
//		return stack + top;
//	}
//};
//
//struct LeptContext
//{
//	const char* json;
//	LeptStackBuffer stack;
//	std::vector<LeptValue> valuebuffer;
//
//	void Init() {
//		json = nullptr;
//		valuebuffer.clear();
//		stack.Init();
//	}
//
//	void CleanUp() {
//		json = nullptr;
//		valuebuffer.clear();
//		stack.CleanUp();
//	}
//
//	void PushArrayElement(const LeptValue& value) {
//		valuebuffer.push_back(value);
//	}
//
//	LeptValue PopArrayElement() {
//		LeptValue value = valuebuffer.back();
//		valuebuffer.pop_back();
//		return value;
//	}
//
//	void CleanArrayBuffer() {
//		valuebuffer.clear();
//	}
//};
//
//
//#endif // !LEPTJSONDEFINE_H_
