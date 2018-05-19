#if !defined(SIMPLEFILESTREAM_INCLUDED_)
#define SIMPLEFILESTREAM_INCLUDED_

#include "Stream.h"
#include "windows.h"

#pragma warning(disable: 4786)
#include <list>
#include <map>
#include <string>

namespace easymap
{

class CSimpleFileStream;
typedef TSmartPtr<CSimpleFileStream> CSimpleFileStreamPtr;

//================================================================================
//  �����������д�����ļ��ķ����㶨��Stream
//  �����ڴ�С�������ļ�ʵ�ʴ�С�޹أ���˿��Դ򿪺ܴ���ļ��� <2GB ��
//  ���ٶȱ�CFileMapStream���ܶ�
//================================================================================
class CSimpleFileStream : public CFileStream
{
CLASS_NAME(CSimpleFileStream)
NO_EVENTS_DISPATCHER
NO_EVENTS_LISTENER

public:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    bool __stdcall Clone(IObj** ppObj) const;
    bool Clone(IObjPtr& pObj) const;

public:
    CSimpleFileStream(const char* pcFileName, const bool readonly = false);

private:
    CSimpleFileStream();
    ~CSimpleFileStream();

private:
    char m_MapFileName[3000];   //ӳ���ļ�·��ȫ��

    bool m_ReadOnly;            //�Ƿ�ֻ��

    HANDLE m_hMapFile;          //ӳ���ļ����

    dword m_Pos;				//�����α�λ�ã�
                                //ע����Чstream���ļ��е���ʼλ��Ϊsizeof(FileStreamHeader)+4

    FILETIME m_LastModifyTime;  //�ļ�����Stream�����ϴε��޸�ʱ�䣬
                                //����ModifiedByOther()���ж�����

private:
    //  ��filemapstream��������Ϊ��Ч״̬
    void Initial(const char* pcFileName, const bool readonly);

    //  ��filemapstream��������Ϊ��Ч״̬
    void Unload();

    //  ~
    void WhenShitHappen();

    //  ������ʵ�ʳ���д���ļ�ͷ
    void UpdateStreamLengthToFile();

public:
    bool __stdcall ReadOnly() const;
    dword __stdcall MovePos(const long delta, const StreamOffsetOrigin origin = SOFROMBEGINNING);
    dword __stdcall GetPos() const;
    dword Read(void* const p, const dword count);
    dword Write(const void* const p, const dword count);
    bool __stdcall SetSize(const dword size);
    dword __stdcall GetSize() const;
    bool __stdcall SaveToFile(const char* const filename);
    bool __stdcall LoadFromFile(const char* const filename);
    bool Clone(CStreamPtr& pStream) const;
    dword CopyDataTo(CStreamPtr const pDesCStream, const dword count);

public:
    //  ����ļ���
    const char* GetMapFileName() const;

    //  ����ļ��Ƿ񱻱�ĳ�����ģ�����ļ����޸�ʱ�䣩
    bool ModifiedByOther() const;

    //  ����ͬ���޸�ʱ��ļ�¼��ModifiedByOther()����
    //  �����������޸�ʱ���Ƿ������¼��һ��������
    void SynchronizeModifyTime();

    //��������д�ش����ļ�
    void Flush() const;

    bool ReMap();

    dword LoadFromInfo(CStreamPtr pStream);
    dword SaveInfo(CStreamPtr pStream) const;

    //  ���ӳ���ļ������������;
    const HANDLE GetMapFileHandle() const;
};
//================================================================================

}

#endif