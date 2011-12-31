/**
 * �����ļ�������
 *
 * ���ߣ�����
 *
 * ���ڣ�2011 08 09
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
		// ���캯��
		CConfigInfo();
		CConfigInfo(const char* cConfigFile,char cSplitChar = '=');
		~CConfigInfo();

		// ����key��ȡvalue
		string GetValue(string strKey);

		// �޸�value
		void SetValue(string strKey, string strValue);

		// ��������
		void SaveConfig();

		inline int GetKeyCount() const {return m_mapConfig.size();}
	private:
		// ���������ļ�
		//
		void LoadConfig(const char* cConfigFile,char cSplitChar = '=');

	private:
		string m_strConfigFile;
		int m_nRecurseCount;//����а����������ļ��������ĵݹ����������10�㣬���η�ֹ��ѭ��
		map<string, string> m_mapConfig;
};
#endif