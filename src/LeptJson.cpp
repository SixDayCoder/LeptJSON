#include "LeptJson.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>

LeptJsonParser::LeptJsonParser(const LeptContext & context)
{
	m_LeptContext = context;
}

LeptParseRet LeptJsonParser::LeptParseWhiteSpace()
{
	const char *p = m_LeptContext.json;
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
		p++;
	m_LeptContext.json = p;

	return LeptParseRet::LEPT_PARSE_OK;
}

LeptParseRet LeptJsonParser::LeptParseValue()
{
	const char* p = m_LeptContext.json;

	switch (*p){
		case 'n':  return LeptParseLiteral("null");  break;
		case 'f':  return LeptParseLiteral("false"); break;
		case 't':  return LeptParseLiteral("true");  break;
		case '"':  return LeptParseString(); break;
		case '\0': return LeptParseRet::LEPT_PARSE_EXPCET_VALUE; break;
		default:   return LeptParseNumber(); break;
	}
}

LeptParseRet LeptJsonParser::LeptParseLiteral(const char * literal)
{
	const char* p = m_LeptContext.json;

	switch (*p) {

		//parse null;
		case 'n': {
			if (p[1] != 'u' || p[2] != 'l' || p[3] != 'l') {
				return LeptParseRet::LEPT_PARSE_INVALID_VALUE;
			}
			else {
				m_LeptContext.json += 4;
				m_LeptValue.type = LeptType::LEPT_NULL;
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
				m_LeptContext.json += 5;
				m_LeptValue.type = LeptType::LEPT_FALSE;
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
				m_LeptContext.json += 4;
				m_LeptValue.type = LeptType::LEPT_TRUE;
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

LeptParseRet LeptJsonParser::LeptParseNumber()
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

	const char* p = m_LeptContext.json;
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

	m_LeptValue.number = strtod(m_LeptContext.json, NULL);
	

	if (errno == ERANGE && (m_LeptValue.number == HUGE_VAL || m_LeptValue.number == -HUGE_VAL))

		return LeptParseRet::LEPT_PARSE_NUMBER_TOO_BIG;

	m_LeptValue.type = LeptType::LEPT_NUMBRE;

	m_LeptContext.json = p;

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

LeptParseRet LeptJsonParser::LeptParseString()
{
	int32_t head = m_LeptContext.stack.top;
	const char* p = nullptr;

	/*
	assert(*m_LeptContext.json == '"');
	m_LeptContext.json++;
	if (*m_LeptContext.json == '"')
		m_LeptContext.json++;
	*/

	p = m_LeptContext.json;

	while (true) {

		char ch = *p++;

		switch (ch){
			//���������������ַ���
			case '"': {
				//�������ַ����ĳ���
				int32_t len = m_LeptContext.stack.top - head;
				//�������ַ�������ʼ��ַ
				const char* str = m_LeptContext.stack.Pop(len);
				m_LeptValue.SetString(str, len);
				m_LeptContext.json = p;
				return LeptParseRet::LEPT_PARSE_OK;
			}
			break;

			//δ�������ַ����������˽�����־
			case '\0': {
				m_LeptContext.stack.top = head;
				return LeptParseRet::LEPT_PARSE_STRING_MISS_QUOTATION_MARK;
			}
			break;

			//putһ���ַ�
			default:
				m_LeptContext.stack.PutChar(ch);
			break;
		}
	}
}

LeptParseRet LeptJsonParser::Parse()
{
	//��ʼ��Ϊnull����
	m_LeptValue.type = LeptType::LEPT_NULL;

	LeptParseRet ret;

	//json-text = whitespace text whitespace
	//1.ȥ����ͷ���ֿո�whitespace
	LeptParseWhiteSpace();
	//2.�������json��text�ɹ���ôȥ������Ŀո�
	ret = LeptParseValue();
	if (ret == LeptParseRet::LEPT_PARSE_OK) {
		LeptParseWhiteSpace();
		//3.��������ո���Ϻ����ַ����Ľ���,˵��json�Ľ�������������
		if ((*m_LeptContext.json) != '\0') {
			return LeptParseRet::LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
		else {
			return LeptParseRet::LEPT_PARSE_OK;
		}
		
	}
	else {
		return ret;
	}
}
