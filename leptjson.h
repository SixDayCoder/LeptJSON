#ifndef  SIXDAY_LEPTJSON_H
#define  SIXDAY_LEPTJSON_H

#include <cstdio>
#include <cstdlib>
#include <cassert>


#define ISDIGIT(ch)      ( (ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)  ( (ch) >= '1' && (ch) <= '9')


enum LeptType {

	LEPT_NULL,
	LEPT_FALSE,
	LEPT_TRUE,
	LEPT_NUMBER,
	LEPT_STRING,
	LEPT_ARRAY,
	LEPT_OBJECT

} ;

enum ParseState {

	LEPT_PARSE_SUCCESS,
	LEPT_PARSE_EXPECT_VALUE,       //jsonֻ���пհ׷�
	LEPT_PARSE_ROOT_NOT_SINGULAR, //��һ��ֵ�ڿհ�֮����������ֵ,��ζ�Ÿ��ڵ㲻Ψһ
	LEPT_PARSE_NUMBER_TOO_BIG,    //����Խ�Ͻ�
	LEPT_PARSE_INVALID_VALUE     //���������ִ���֮��Ĵ���

};

struct LeptValue{

	double number;
	LeptType type;

};


struct LeptContext {

	const char* json;

};

/// �������Ϊ����,��ȡLeptValue��number
double LeptGetNumber(const LeptValue& v);


/// ���������
/// json(in) : json�ı�
/// v(out)   : ��������
ParseState LeptParse(const char* json, LeptValue& v);


/// �����հ׷�
/// context(in out) : ������������
void LeptParseWhitespace(LeptContext& context);


/// ��������ֵ
/// context(in out) : ������������
/// v(out)      : ��������
ParseState LeptParseValue(LeptContext& context, LeptValue& v);


/// ����NULL
/// context(in out) : ������������
/// v(out)      : ��������
ParseState LeptParseNull(LeptContext& context, LeptValue& v);


/// ����trueֵ
/// context(in out) : ������������
/// v(out)      : ��������
ParseState LeptParseTrue(LeptContext& context, LeptValue& v);


/// ����falseֵ
/// context(in out) : ������������
/// v(out)      : ��������
ParseState LeptParseFalse(LeptContext& context, LeptValue& v);

/// ��������ֵ
/// context(in out) : ������������
/// v(out)  : ��������
ParseState LeptParseNumber(LeptContext& context, LeptValue& v);


#endif // ! SIXDAY_LEPTJSON_H

