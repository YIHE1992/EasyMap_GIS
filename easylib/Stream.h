#if !defined(STREAM_INCLUDED_)
#define STREAM_INCLUDED_

#include "..\\include\\InterfaceStream.h"

#pragma warning(disable: 4786)
#include <string>
using namespace std;

namespace easymap
{

class CStream;
typedef TSmartPtr<CStream> CStreamPtr;
class CFileStream;
typedef TSmartPtr<CFileStream> CFileStreamPtr;

#define STREAM_RW_BT(basetype)\
    inline dword Read(basetype& val){return this->Read(&val, sizeof(basetype));};\
    inline dword Write(const basetype& val){return this->Write(&val, sizeof(basetype));};

//================================================================================
//  ���ļ���
//================================================================================
class CStream : public IStreamX
{
public:
    bool __stdcall Eof() const;
    bool __stdcall Bof() const;

    dword __stdcall ReadData(void* const p, const dword count);
    dword __stdcall WriteData(const void* const p, const dword count);

    virtual dword Read(void* const p, const dword count) = 0;
    virtual dword Write(const void* const p, const dword count) = 0;

    //  �ӵ�ǰ�α�λ�ÿ�ʼ����size���ȵ����ݵ�pDesCStream�ĵ�ǰλ����
    virtual dword CopyDataTo(CStreamPtr const pDesCStream,
        const dword count) = 0;

public:
    //�����򻯻����������͵�read/write����
    //����BT�Ĵ��룬��Ϊ�㲻��template
    //ע�ⲻ��overload bool���ͣ�����ᵼ��ָ�����͵���ʽת��
    STREAM_RW_BT(char)
    STREAM_RW_BT(byte)
    STREAM_RW_BT(short)
    STREAM_RW_BT(word)
    STREAM_RW_BT(long)
    STREAM_RW_BT(dword)
    STREAM_RW_BT(float)
    STREAM_RW_BT(double)
    STREAM_RW_BT(__int64)

    inline dword ReadBool(bool& b){return this->Read(&b, sizeof(bool));};
    inline dword WriteBool(const bool& b){return this->Write(&b, sizeof(bool));};

    dword Read(string& s);
    dword Write(const string& s);
};
//================================================================================

//----------------------------------------
//FileStream�����ļ��İ汾��Ϣ
typedef struct
{
    dword Description[20];
    dword MainVersion;
    dword SubVersion;
    dword Build;
    dword Reserved[30];
}FileStreamHeader;

const char FILEMAPSTREAMMAINVERSION     = 1;
const char FILEMAPSTREAMSUBVERSION      = 1;
const char FILEMAPSTREAMBUILDVERSION    = 4;
static const char* FILEMAPSTREAMIDENTIFY = "YYCat FileMapStream";
const dword FILEMAPHEADERLENGTH = sizeof(FileStreamHeader) + sizeof(dword);

//================================================================================
//  CFileMapStream��CSimpleFileStream�Ļ���
//  ֱ�������ڴ����ļ���Stream
//================================================================================
class CFileStream : public CStream
{
public:
    //  ���ӳ���ļ���
    virtual const char* GetMapFileName() const = 0;

    //  ���ӳ���ļ��Ƿ񱻱�ĳ�����ģ�����ļ����޸�ʱ�䣩
    virtual bool ModifiedByOther() const = 0;

    //  ����ͬ���޸�ʱ��ļ�¼��ModifiedByOther()����
    //  �����������޸�ʱ���Ƿ������¼��һ��������
    virtual void SynchronizeModifyTime() = 0;

    //��������д�ش����ļ�
    virtual void Flush() const = 0;

    //����ӳ�䣬��ͬ�����˸��µ�����
    virtual bool ReMap() = 0;

    //���ļ�������Ϣ�洢��������stream��
    virtual dword LoadFromInfo(CStreamPtr pStream) = 0;
    virtual dword SaveInfo(CStreamPtr pStream) const = 0;

};

}

#endif
