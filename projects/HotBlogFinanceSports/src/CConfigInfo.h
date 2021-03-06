/**
 * 配置文件解析类
 *
 * 作者：开江
 *
 * 日期：2011 08 09
 *
 */

#ifndef CCONFIGINFO_H
#define CCONFIGINFO_H
#include <map>
#include <string>

using namespace std;

class CConfigInfo
{
	public:
		// 构造函数
		CConfigInfo();
		CConfigInfo(const char* cConfigFile,char cSplitChar = '=');
		~CConfigInfo();

		// 根据key获取value
		string GetValue(string strKey);

		// 修改value
		void SetValue(string strKey, string strValue);

		// 保存配置
		void SaveConfig();

		inline int GetKeyCount() const {return m_mapConfig.size();}
	private:
		// 加载配置文件
		//
		void LoadConfig(const char* cConfigFile,char cSplitChar = '=');

	private:
		string m_strConfigFile;
		int m_nRecurseCount;//如果有包含的配置文件，则最大的递归层数不包含10层，依次防止死循环
		map<string, string> m_mapConfig;
};
#endif
