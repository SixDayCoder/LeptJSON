#include "LeptJson.h"

#include <errno.h>
#include <math.h>
#include <stdlib.h>

LeptJsonParser::LeptJsonParser(const LeptContext & context)
{
	m_LeptContext.Init();
	m_LeptValue.Init();
	m_LeptContext = context;
}

void LeptJsonParser::SkipWhiteSpace(LeptContext& context)
{
	const char *p = context.json;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	context.json = p;
}

LeptParseRet LeptJsonParser::LeptParseValue(LeptContext& context, LeptValue& value)
{
	
	switch (*context.json){
		case 'n':   return LeptParseLiteral(context, "null", value);  break;
		case 'f':   return LeptParseLiteral(context, "false", value); break;
		case 't':   return LeptParseLiteral(context, "true", value);  break;
		case '\"':  return LeptParseString(context, value); break;
		case '[':   return LeptParseArray(context, value); break;
		case '\0':  return LeptParseRet::LEPT_PARSE_EXPCET_VALUE; break;
		default:    return LeptParseNumber(context, value); break;
	}
}

LeptParseRet LeptJsonParser::LeptParseLiteral(LeptContext& context, const char * literal, LeptValue& value)
{
	const char* p = context.json;

	//--------------------------------------------------------��ע��----------------------------------------------------------//
	//m_LeptValue
	//--------------------------------------------------------��ע��----------------------------------------------------------//

	switch (*p) {

		//parse null;
		case 'n': {
			if (p[1] != 'u' || p[2] != 'l' || p[3] != 'l') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				context.json += 4;
				m_LeptValue.type = LeptType::LEPT_NULL;
				m_LeptValue.constliteral = "null";
				value = m_LeptValue;
				return LeptParseRet::LEPT_PARSE_OK;
			}
		}
		break;

		//parse false
		case 'f': {
			if (p[1] != 'a' || p[2] != 'l' || p[3] != 's' || p[4] != 'e') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				context.json += 5;
				m_LeptValue.type = LeptType::LEPT_FALSE;
				m_LeptValue.constliteral = "false";
				value = m_LeptValue;
				return LeptParseRet::LEPT_PARSE_OK;
			}
		}
		break;

		//parse true
		case 't': {
			if (p[1] != 'r' || p[2] != 'u' || p[3] != 'e') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				context.json += 4;
				m_LeptValue.type = LeptType::LEPT_TRUE;
				m_LeptValue.constliteral = "true";
				value = m_LeptValue;
				return LeptParseRet::LEPT_PARSE_OK;
			}
		}
		break;
		
		//invalid literal
		default:
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
		break;

	}

}

LeptParseRet LeptJsonParser::LeptParseNumber(LeptContext& context, LeptValue& value)
{
	/*
	json���ֵĹ���
	number = [ "-" ] int [ frac ] [ exp ]
	����
	(0)���ſ�ѡ
	(1)int = "0" / digit1-9 *digit
	(2)frac = "." 1*digit
	(3)exp = ("e" / "E") ["-" / "+"] 1*digit
	*/

	const char* p = context.json;
	/*(0)У�鸺��*/
	if (*p == '-') p++;

	/*(1)У��int, �������,*/
	if (*p == '0') p++;

	else {

		if (!IsDigit1To9(*p)) {
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
		}
		else {
			for (p++; IsDigit(*p); p++);
		}
	}

	/*(2)У��С����*/
	if (*p == '.') {

		//С���������һλ������
		p++;

		if (!IsDigit(*p)) {
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
		}
		else{
			for (p++; IsDigit(*p); p++);
		}
	
	}

	/*(3)У��ָ������*/
	if (*p == 'e' || *p == 'E') {

		p++;

		//����'e'����'E'��,���������������,����
		/*(3.1)У������E����e��,�Ƿ���������*/
		if (*p == '+' || *p == '-') p++;

		if (!IsDigit(*p)) { 
			return LeptParseRet::LEPT_PARSE_INVALID_VALUE; 
		}
		else {
			/*(3.2)У�������������ź�����Ƿ�Ϊ����*/
			for (p++; IsDigit(*p); p++);
		}

	}
	//������������,jsonָ���һ����Ϊ���ֵ��ַ�

	errno = 0;

	double number = strtod(m_LeptContext.json, NULL);

	if (errno == ERANGE && (number == HUGE_VAL || number == -HUGE_VAL))
		return LeptParseRet::LEPT_PARSE_NUMBER_TOO_BIG;

	//--------------------------------------------------------��ע��----------------------------------------------------------//
	m_LeptValue.number = number;

	m_LeptValue.type = LeptType::LEPT_NUMBRE;
	//--------------------------------------------------------��ע��----------------------------------------------------------//

	value = m_LeptValue;

	context.json = p;

	return LeptParseRet::LEPT_PARSE_OK;


	/*
	errno :
	errno is set to zero at program startup, and
	any function of the standard C library can modify its value to some value different from zero,
	generally to signal specific categories of error
	�趨errno�ĳ�ʼֵΪ0,c�ı�׼�⺯��������ʱ���޸�����ֵ,һ������һЩ���������

	ERANGE :
	��ʾһ����Χ���������������������ѧ��������ķ�Χʱ������errno ������Ϊ ERANGE��
	Macro constant that expands to a positive expression of type double.
	HUGE_VAL:
	A function returns this value when the result of a mathematical operation yields a value
	that is too large in magnitude to be representable with its return type
	Actually, functions may either return a positive or a negative HUGE_VAL (HUGE_VAL or -HUGE_VAL)
	to indicate the sign of the result.
	*/
}

LeptParseRet LeptJsonParser::LeptParseString(LeptContext& context, LeptValue& value)
{
	//json�ַ����ĸ�ʽ����
	// " text | escape-char ",������˫����,�м������ת���ַ�

	//�ַ�����ͷ������˫����
	assert(*context.json == '\"');
	context.json++;

	int32_t head = context.stack.top;
	const char* p = context.json;

	while (true) {

		char ch = *p++;

		switch (ch){
			//���������������ַ���
			case '\"': {
				//�������ַ����ĳ���
				int32_t len = context.stack.top - head;
				//�������ַ�������ʼ��ַ,�����Ѿ��ɹ��������ַ�
				const char* str = context.stack.Pop(len);
				value.SetString(str, len);
				//--------------------------------------------------------��ע��----------------------------------------------------------//
				m_LeptValue.CleanUp();
				m_LeptValue = value;
				//--------------------------------------------------------��ע��----------------------------------------------------------//
				//value = m_LeptValue;
				context.json = p;
				return LeptParseRet::LEPT_PARSE_OK;
			}
			break;

			//������ת���ַ�
			case '\\': {

				switch (*p++) {
					//�����˫���ŵ���һ����
					case '\"':context.stack.PutChar('\"'); break;
					//�������ת���ַ�
					case '\\':context.stack.PutChar('\\'); break;
					//����Ƿ�б��
					case '/': context.stack.PutChar('/'); break;
					//������˺��
					case 'b': context.stack.PutChar('\b'); break;
					//����ǻ�ҳ��
					case 'f': context.stack.PutChar('\f'); break;
					//����ǻ��з�
					case 'n': context.stack.PutChar('\n'); break;
					//����ǻس���
					case 'r': context.stack.PutChar('\r'); break;
					//������Ʊ��
					case 't': context.stack.PutChar('\t'); break;
					//��������ǲ��Ϸ���ת���ַ�
					default:
						context.stack.top = head;
						value.type = LeptType::LEPT_NULL;
						m_LeptValue.type = LeptType::LEPT_NULL;
						return LeptParseRet::LEPT_PARSE_STRING_INVALID_ESCAPE;
					break;

				}

			}
			break;

			//δ�������ַ����������˽�����־
			case '\0': {
				context.stack.top = head;
				value.type = LeptType::LEPT_NULL;
				m_LeptValue.type = LeptType::LEPT_NULL;
				return LeptParseRet::LEPT_PARSE_STRING_MISS_QUOTATION_MARK;
			}
			break;

			//putһ���ַ�
			default: {
				if ((unsigned char)ch < 0x20) {
					context.stack.top = head;
					value.type = LeptType::LEPT_NULL;
					m_LeptValue.type = LeptType::LEPT_NULL;
					return LeptParseRet::LEPT_PARSE_STRING_INVALID_CHAR;
				}
				else {
					context.stack.PutChar(ch);
				}
			}
			break;
		}
	}
}

LeptParseRet LeptJsonParser::LeptParseArray(LeptContext& context, LeptValue & value)
{
	//�����������[��ͷ
	assert(*context.json == '[');
	context.json++;
	//���鿪ͷ�Ŀո�
	SkipWhiteSpace(context);
	
	LeptParseRet ret;
	//LeptArray arrays;
	LeptValue element;
	element.Init();

	//�������˿�����[]
	if (*context.json == ']') {
		value.Init();
		value.type = LeptType::LEPT_ARRAY;
		//------------------------------------------��ע��--------------------------------------------------//
		m_LeptValue = value;
		//------------------------------------------��ע��--------------------------------------------------//
		context.json++;
		return LeptParseRet::LEPT_PARSE_OK;
	}

	while (true) {

		element.CleanUp();

		ret = LeptParseValue(context, element);
		if (ret != LeptParseRet::LEPT_PARSE_OK) {
			break;
		}
		else {
			context.PushArrayElement(element);
			//ȥ���������Ŀո�
			SkipWhiteSpace(context);

			if (*context.json == ',') {
				context.json++;
				SkipWhiteSpace(context);
			}
			else if (*context.json == ']') {
				context.json++;
				value.type = LeptType::LEPT_ARRAY;
				for (size_t i = 0; i < context.valuebuffer.size(); ++i) {
					value.arrays.PushBack(context.valuebuffer[i]);
				}
				context.CleanArrayBuffer();
				//------------------------------------------��ע��--------------------------------------------------//
				m_LeptValue = value;
				//------------------------------------------��ע��--------------------------------------------------//
				return LeptParseRet::LEPT_PARSE_OK;
			}
			else {
				ret = LeptParseRet::LEPT_PARSE_ARRAY_MISS_COMMA_OR_SQUARE_BRACKET;
				break;
			}

		}

	}

	//ִ�е�����˵������ʧ��
	//1.����ڽ��������в�������ʱ����
	context.CleanArrayBuffer();
	//2.����LeptStackBuffer���ڴ�,���������ڴ�й©
	//TODO
	return ret;
}

LeptParseRet LeptJsonParser::Parse()
{
	//��ʼ��Ϊnull����
	m_LeptValue.type = LeptType::LEPT_NULL;
	LeptValue value;
	value.Init();
	//json-text = whitespace text whitespace
	//1.ȥ����ͷ���ֿո�whitespace
	SkipWhiteSpace(m_LeptContext);
	//2.�������json��text�ɹ���ôȥ������Ŀո�
	LeptParseRet ret = LeptParseValue(m_LeptContext, value);
	if (ret == LeptParseRet::LEPT_PARSE_OK) {
		SkipWhiteSpace(m_LeptContext);
		//3.��������ո���Ϻ����ַ����Ľ���,˵��json�Ľ�������������
		if ((*m_LeptContext.json) != '\0') {
			//����ʧ��
			m_LeptContext.CleanUp();
			return LeptParseRet::LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
		else {
			return LeptParseRet::LEPT_PARSE_OK;
		}
		
	}
	else {
		//����ʧ��
		m_LeptContext.CleanUp();
		return ret;
	}
}
