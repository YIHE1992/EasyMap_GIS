#if !defined(STRINGFUNCS_INCLUDE_)
#define STRINGFUNCS_INCLUDE_

#include "CommonInclude.h"

namespace easymap
{

class Strings
{
public:
    Strings();
    Strings(const string& text);
    ~Strings();

private:
    vector<string> m_lines;

public:
    void SetText(const string& text);
    string GetText() const;
    dword GetLineCount() const;
    bool GetLine(const dword index, string& line) const;
    void AppendLine(const string& line);
    bool InsertLine(const dword index, const string& line);
    bool SetLine(const dword index, const string& line);
    bool DeleteLine(const dword index);
};

//--------------------------------------------------------------------------------
//  ȥ����ߵĿո�
//--------------------------------------------------------------------------------
string TrimLeft(const string& s);

//--------------------------------------------------------------------------------
//  ȥ���ұߵĿո�
//--------------------------------------------------------------------------------
string TrimRight(const string& s);

//--------------------------------------------------------------------------------
//  ȥ�����˵Ŀո�
//--------------------------------------------------------------------------------
string Trim(const string& s);

//--------------------------------------------------------------------------------
//  ��ɴ�д
//--------------------------------------------------------------------------------
string UpperString(const string& s);

//--------------------------------------------------------------------------------
//  ���Сд
//--------------------------------------------------------------------------------
string LowerString(const string& s);

//--------------------------------------------------------------------------------
//  ����    ->  �ַ���
//--------------------------------------------------------------------------------
string IntToStr(const long i);

//--------------------------------------------------------------------------------
//  �ַ���  ->  ����
//--------------------------------------------------------------------------------
long StrToInt(const string& s);

//--------------------------------------------------------------------------------
//  ������  ->  �ַ���
//--------------------------------------------------------------------------------
string FloatToStr(const double f);

//--------------------------------------------------------------------------------
//  �ַ���  ->  ������
//--------------------------------------------------------------------------------
double StrToFloat(const string& s);

//--------------------------------------------------------------------------------
//  �ҵ���ǰһ��'find'�ַ�
//--------------------------------------------------------------------------------
long FindFirstChar(const char* pc, const char find);

//--------------------------------------------------------------------------------
//  �ҵ����һ��'find'�ַ�
//--------------------------------------------------------------------------------
long FindLastChar(const char* pc, const char find);

//--------------------------------------------------------------------------------
//  ȡ��·�����֣�ȥ�������ļ�����
//--------------------------------------------------------------------------------
string GetDirectoryPart(const string& pathfilename);

//--------------------------------------------------------------------------------
//  ȡ�������ļ�����ȥ��·�����֣�
//--------------------------------------------------------------------------------
string RemoveDirectoryPart(const string& pathfilename);

//--------------------------------------------------------------------------------
//  ȡ����չ��
//--------------------------------------------------------------------------------
string GetExtNamePart(const string& filename);

//--------------------------------------------------------------------------------
//  ȥ����չ��
//--------------------------------------------------------------------------------
string RemoveExtNamePart(const string& filename);

//--------------------------------------------------------------------------------
//  �����ı��ļ�
//--------------------------------------------------------------------------------
bool File2String(const string& filename, string& text);

//--------------------------------------------------------------------------------
//  д���ı��ļ�
//--------------------------------------------------------------------------------
bool String2File(const string& text, const string& filename);

//--------------------------------------------------------------------------------
//  ����ini
//--------------------------------------------------------------------------------
bool ini_setkeyvalue(string& text, const string& key, const string& value);
bool ini_findkeyvalue(const string& text, const string& key, string& value);
bool ini_deletekey(string& text, const string& key);

}

#endif