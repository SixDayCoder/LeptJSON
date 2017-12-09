#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#include <stdlib.h>  /* NULL, malloc(), realloc(), free(), strtod() */
#include <string.h>  /* memcpy() */

//#ifndef LEPT_PARSE_STACK_INIT_SIZE
//#define LEPT_PARSE_STACK_INIT_SIZE 256
//#endif
//
//#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
//#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
//#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')
//#define PUTC(c, ch)         do { *(char*)LeptContextPush(c, sizeof(char)) = (ch); } while(0)
//
//typedef struct {
//
//	const char* json;
//	char* stack;
//	size_t size, top;
//
//}LeptContext;
//
//static void* LeptContextPush(LeptContext* c, size_t size) {
//	void* ret;
//	assert(size > 0);
//	if (c->top + size >= c->size) {
//		if (c->size == 0)
//			c->size = LEPT_PARSE_STACK_INIT_SIZE;
//		while (c->top + size >= c->size)
//			c->size += c->size >> 1;  /* c->size * 1.5 */
//		c->stack = (char*)realloc(c->stack, c->size);
//	}
//	ret = c->stack + c->top;
//	c->top += size;
//	return ret;
//}
//
//static void* LeptContextPop(LeptContext* c, size_t size) {
//	assert(c->top >= size);
//	return c->stack + (c->top -= size);
//}
//
//static void LeptParseWhitespace(LeptContext* c) {
//	const char *p = c->json;
//	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
//		p++;
//	c->json = p;
//}
//
//static int LeptParseLiteral(LeptContext* c, LeptValue* v, const char* literal, LeptType type) {
//	size_t i;
//	EXPECT(c, literal[0]);
//	for (i = 0; literal[i + 1]; i++)
//		if (c->json[i] != literal[i + 1])
//			return LeptParse_INVALID_VALUE;
//	c->json += i;
//	v->type = type;
//	return LeptParseSuccess;
//}
//
//static int LeptParseNumber(LeptContext* c, LeptValue* v) {
//	const char* p = c->json;
//
//	/*
//	json数字的构成
//	number = [ "-" ] int [ frac ] [ exp ]
//	其中
//	(0)负号可选
//	(1)int = "0" / digit1-9 *digit
//	(2)frac = "." 1*digit
//	(3)exp = ("e" / "E") ["-" / "+"] 1*digit
//	*/
//
//	/*(0)校验负号*/
//	if (*p == '-') p++;
//
//	/*(1)校验int, 两种情况,*/
//	if (*p == '0') p++;
//	else {
//		if (!ISDIGIT1TO9(*p)) return LeptParse_INVALID_VALUE;
//		for (p++; ISDIGIT(*p); p++);
//	}
//
//	/*(2)校验小数点*/
//	if (*p == '.') {
//		//小数点后至少一位是数字
//		p++;
//		if (!ISDIGIT(*p)) return LeptParse_INVALID_VALUE;
//		for (p++; ISDIGIT(*p); p++);
//	}
//
//	/*(3)校验指数部分*/
//	if (*p == 'e' || *p == 'E') {
//		p++;
//
//		//跳过'e'或者'E'后,后面可以有正负号,检验
//		/*(3.1)校验跳过E或者e后,是否有正负号*/
//		if (*p == '+' || *p == '-') p++;
//		if (!ISDIGIT(*p)) return LeptParse_INVALID_VALUE;
//
//		/*(3.2)校验跳过了正负号后后面是否都为数字*/
//		for (p++; ISDIGIT(*p); p++);
//	}
//
//	//经过上述处理,json指向第一个不为数字的字符
//	errno = 0;
//	v->u.n = strtod(c->json, NULL);
//	if (errno == ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
//		return LeptParseNumber_TOO_BIG;
//	v->type = LEPT_NUMBER;
//	c->json = p;
//	return LeptParseSuccess;
//
//	/*
//	errno :
//	errno is set to zero at program startup, and
//	any function of the standard C library can modify its value to some value different from zero,
//	generally to signal specific categories of error
//
//	设定errno的初始值为0,c的标准库函数在运行时可修改它的值,一般是由一些宏来定义的
//
//	ERANGE :
//	表示一个范围错误，它在输入参数超出数学函数定义的范围时发生，errno 被设置为 ERANGE。
//
//	Macro constant that expands to a positive expression of type double.
//
//	HUGE_VAL:
//
//	A function returns this value when the result of a mathematical operation yields a value
//	that is too large in magnitude to be representable with its return type
//
//	Actually, functions may either return a positive or a negative HUGE_VAL (HUGE_VAL or -HUGE_VAL)
//	to indicate the sign of the result.
//
//
//	*/
//}
//
//static int LeptParseString(LeptContext* c, LeptValue* v) {
//	size_t head = c->top, len;
//	const char* p;
//	EXPECT(c, '\"');
//	p = c->json;
//	while(true) {
//		char ch = *p++;
//		switch (ch) {
//		case '\"':
//			len = c->top - head;
//			LeptSetString(v, (const char*)LeptContextPop(c, len), len);
//			c->json = p;
//			return LeptParseSuccess;
//		case '\\':
//			switch (*p++) {
//			case '\"': PUTC(c, '\"'); break;
//			case '\\': PUTC(c, '\\'); break;
//			case '/':  PUTC(c, '/'); break;
//			case 'b':  PUTC(c, '\b'); break;
//			case 'f':  PUTC(c, '\f'); break;
//			case 'n':  PUTC(c, '\n'); break;
//			case 'r':  PUTC(c, '\r'); break;
//			case 't':  PUTC(c, '\t'); break;
//			default:
//				c->top = head;
//				return LeptParse_INVALID_STRING_ESCAPE;
//			}
//			break;
//		case '\0':
//			c->top = head;
//			return LeptParse_MISS_QUOTATION_MARK;
//		default:
//			if ((unsigned char)ch < 0x20) {
//				c->top = head;
//				return LeptParse_INVALID_STRING_CHAR;
//			}
//			PUTC(c, ch);
//		}
//	}
//}
//
//static int LeptParseValue(LeptContext* c, LeptValue* v) {
//	switch (*c->json) {
//		case 't':  return LeptParseLiteral(c, v, "true", LEPT_TRUE);
//		case 'f':  return LeptParseLiteral(c, v, "false", LEPT_FALSE);
//		case 'n':  return LeptParseLiteral(c, v, "null", LEPT_NULL);
//		default:   return LeptParseNumber(c, v);
//		case '"':  return LeptParseString(c, v);
//		case '\0': return LeptParse_EXPECT_VALUE;
//	}
//}
//
//void LeptInit(LeptValue * v)
//{
//	assert(v != NULL);
//	v->type = LEPT_NULL;
//}
//
//int LeptParse(LeptValue* v, const char* json) {
//
//	LeptContext c;
//	int ret;
//	assert(v != NULL);
//	c.json = json;
//	c.stack = NULL;
//	c.size = c.top = 0;
//	LeptInit(v);
//
//	//去掉json字符串开始的空白符
//	LeptParseWhitespace(&c);
//	if ((ret = LeptParseValue(&c, v)) == LeptParseSuccess) {
//		//去掉解析值后的空格
//		LeptParseWhitespace(&c);
//		//如果去掉空格后字符串没有结束
//		if (*c.json != '\0') {
//			v->type = LEPT_NULL;
//			ret = LeptParse_ROOT_NOT_SINGULAR;
//		}
//	}
//
//	//添加该断言保证free的逻辑正确性
//	assert(c.top == 0);
//	free(c.stack);
//	return ret;
//
//}
//
//void LeptFree(LeptValue* v) {
//
//	assert(v != NULL);
//	if (v->type == LEPT_STRING)
//		free(v->u.s.s);
//	v->type = LEPT_NULL;
//
//}
//
//LeptType LeptGetType(const LeptValue* v) {
//	assert(v != NULL);
//	return v->type;
//}
//
//int LeptGetBoolean(const LeptValue* v) {
//	assert(v != NULL && (v->type == LEPT_TRUE || v->type == LEPT_FALSE));
//	return v->type == LEPT_TRUE;
//}
//
//void LeptSetBoolean(LeptValue* v, int b) {
//	LeptFree(v);
//	v->type = b ? LEPT_TRUE : LEPT_FALSE;
//}
//
//double LeptGetNumber(const LeptValue* v) {
//	assert(v != NULL && v->type == LEPT_NUMBER);
//	return v->u.n;
//}
//
//void LeptSetNumber(LeptValue* v, double n) {
//	LeptFree(v);
//	v->u.n = n;
//	v->type = LEPT_NUMBER;
//}
//
//const char* LeptGetString(const LeptValue* v) {
//	assert(v != NULL && v->type == LEPT_STRING);
//	return v->u.s.s;
//}
//
//size_t LeptGetStringLength(const LeptValue* v) {
//	assert(v != NULL && v->type == LEPT_STRING);
//	return v->u.s.len;
//}
//
//void LeptSetString(LeptValue* v, const char* s, size_t len) {
//	assert(v != NULL && (s != NULL || len == 0));
//	LeptFree(v);
//	v->u.s.s = (char*)malloc(len + 1);
//	memcpy(v->u.s.s, s, len);
//	v->u.s.s[len] = '\0';
//	v->u.s.len = len;
//	v->type = LEPT_STRING;
//}
