
基于Milo Yip《从零开始的JSON库教程》使用C++实现的JSON解析器、生成器。

# 文件组织结构
* src  ：源代码文件夹
* json ：存放解析器使用的json文本和生成器生成的json文本
* vsbuild : vs2015及以上版本的解决方案,在build中引用src的代码和json文件中的txt

# Example

引入LeptJsonParser.h文件夹,如想查看单元测试需引入UnitTest.h.

```c
//1.声明LeptJsonReader的变量
LeptJsonReader reader;
//2.从raw char或者从文件中导入json文本
reader.LoadFromString(json_text);
reader.LoadFromFile(file_path);
//3.使用parse接口解析文本
reader.Parse()；
//4.若解析成功那么ParseState()为success,之后使用ParseResult()返回值,返回值类型为LeptJsonValue
//LeptJsonValue是一个variant,根据value的类型决定访问何种值

//例子如下
LeptJsonReader reader;
reader.LoadFromFile("example1.txt");
reader.Parse();
if (LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS == reader.ParseState()) {
	LeptJsonValue& value = reader.ParseResult();
	std::cout << value << std::endl;
}


LeptJsonWriter writer;
if (LeptJsonParseRet::LEPT_JSON_PARSE_SUCCESS == reader.ParseState()) {
	LeptJsonValue& value = reader.ParseResult();
	if (value.IsHaveKeys()) {
		Object& obj = value.GetObject();
		for (auto it = obj.begin(); it != obj.end(); ++it) {
			const String& key = it->first;
			const LeptJsonValue& val = *it->second;
			writer.Push(key, val);
		}
	}
	else {
		writer.Push(value);
	}
}

writer.Write("output.txt");
```
