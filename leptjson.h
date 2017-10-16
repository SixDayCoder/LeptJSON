
#ifndef  SIXDAY_LEPTJSON_H
#define  SIXDAY_LEPTJSON_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <cstring>


#define ISDIGIT(ch)      ( (ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)  ( (ch) >= '1' && (ch) <= '9')


#ifndef LEPT_PARSE_STACK_INIT_SIZE
#define LEPT_PARSE_STACK_INIT_SIZE 256
#endif // !1


enum LeptType {

	LEPT_NULL,
	LEPT_FALSE,
	LEPT_TRUE,
	LEPT_NUMBER,
	LEPT_STRING,
	LEPT_ARRAY,
	LEPT_STRING,

};

enum ParseState {

	LEPT_PARSE_SUCCESS,
	LEPT_PARSE_EXPECT_VALUE,       //json只含有空白符
	LEPT_PARSE_ROOT_NOT_SINGULAR, //若一个值在空白之后还有其他的值,意味着根节点不唯一
	LEPT_PARSE_NUMBER_TOO_BIG,    //数字越上届
	LEPT_PARSE_INVALID_VALUE     //除上述两种错误之外的错误

};

struct LeptValue {

	union {

		struct{
			char* s;
			size_t len;
		}str;

		double number;

	}u;
	
	LeptType type;

};


struct LeptContext {

	const char* json;

	char* stack;
	size_t size, top;

};

/// 如果类型为数字,获取LeptValue的number
double LeptGetNumber(const LeptValue& v);


/// 构造解析树
/// json(in) : json文本
/// v(out)   : 解析对象
ParseState LeptParse(const char* json, LeptValue& v);


/// 跳过空白符
/// context(in out) : 解析器上下文
void LeptParseWhitespace(LeptContext& context);


/// 解析字面值
/// context(in out) : 解析器上下文
/// v(out)      : 解析对象
ParseState LeptParseValue(LeptContext& context, LeptValue& v);


/// 解析NULL
/// context(in out) : 解析器上下文
/// v(out)      : 解析对象
ParseState LeptParseNull(LeptContext& context, LeptValue& v);


/// 解析true值
/// context(in out) : 解析器上下文
/// v(out)      : 解析对象
ParseState LeptParseTrue(LeptContext& context, LeptValue& v);


/// 解析false值
/// context(in out) : 解析器上下文
/// v(out)      : 解析对象
ParseState LeptParseFalse(LeptContext& context, LeptValue& v);

/// 解析数字值
/// context(in out) : 解析器上下文
/// v(out)  : 解析对象
ParseState LeptParseNumber(LeptContext& context, LeptValue& v);


/// 解析字符串
/// context(in out) : 解析器上下文
/// v(out)  : 解析对象
ParseState LeptParseString(LeptContext& context, LeptValue& v);

/// 动态分配字符串
/// s(in)  ： 字符串
/// len(in)： 字符串的长度
/// v(out) ： 解析对象
void LeptSetString(const char* s, size_t len, LeptValue& v);


/// 回收分配的字符串
/// v(in out) 存储字符串
void LeptFreeString(LeptValue& v);


/// 以context建立字符串解析的缓冲区,临时存储字符串
/// c(in out) : 解析器上下文
/// size(in)  : 要压栈的字符串的长度 
void* LeptContextPush(LeptContext& c, size_t size);


/// 以context建立字符串解析的缓冲区,临时存储字符串
/// c(in out) : 解析器上下文
/// size(in)  : 要弹出的字符串的长度 
void* LeptContextPop(LeptContext& c, size_t size);
#endif // ! SIXDAY_LEPTJSON_H

