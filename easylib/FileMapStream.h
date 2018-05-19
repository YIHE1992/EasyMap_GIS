#if !defined(FILEMAPSTREAM_INCLUDED_)
#define FILEMAPSTREAM_INCLUDED_

#include "Stream.h"
#include "windows.h"

#pragma warning(disable: 4786)
#include <list>
#include <map>
#include <string>

namespace easymap
{

class CFileMapStream;
typedef TSmartPtr<CFileMapStream> CFileMapStreamPtr;

//================================================================================
//  ���ڴ�ӳ���ļ��㶨��stream���������ٴ�ȡ�����ļ����ٶȺܿ�
//  Ϊ�˼򻯱�̣�����һ����ӳ�������ļ����Ƚ������ڴ棬����ļ�����̫��
//  ���Ҫ�������ļ���ʹ��SimpleFileStream
//================================================================================
class CFileMapStream : public CFileStream
{
CLASS_NAME(CFileMapStream)
NO_EVENTS_DISPATCHER
NO_EVENTS_LISTENER

public:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    bool __stdcall Clone(IObj** ppObj) const;
    bool Clone(IObjPtr& pObj) const;

public:
    CFileMapStream(const char* pcFileName, const bool readonly = false);

private:
    CFileMapStream();
    ~CFileMapStream();

private:
    char m_MapFileName[3000];   //ӳ���ļ�·��ȫ��

    bool m_ReadOnly;            //�Ƿ�ֻ��

    dword m_MapFileSize;        //�ļ�ӳ���С(CreateFileMapping()�Ĵ�С)

    dword m_MapFileIncreasSize; //�ļ�ӳ���Զ�������С

    HANDLE m_hMapFile;          //ӳ���ļ����

    HANDLE m_hMappingObject;    //�ļ�ӳ�������

    char* m_pViewBase;          //ӳ�䴰����ʼλ��

    char* m_pStreamBase;        //������ʼλ�ã�
                                //Ϊm_pViewBase+sizeof(FileMapStreamHeader)+4

    char* m_pStreamPos;         //���Ĺ����α�

    dword m_StreamSize;         //������Ч���ȣ�����ӳ���ļ����ȣ�����

    FILETIME m_LastModifyTime;  //�ļ�����Stream�����ϴε��޸�ʱ�䣬
                                //����ModifiedByOther()���ж�����

private:
    //  ��filemapstream��������Ϊ��Ч״̬
    void Initial(const char* pcFileName, const bool readonly);

    //  ��filemapstream��������Ϊ��Ч״̬
    void Unload();

    //  ~
    void WhenShitHappen();

    //  ӳ���ļ���ע����ô˺���֮ǰҪ��֤��
    //      1.�ļ��Ѿ���(��m_hMapFile��Ч)
    //      2.�ļ�û�б�ӳ��(
    //          ��m_hMappingObject��m_pViewBase����Ч)
    bool MappingFile();

    //  ����size֮ǰ�ȼ���ļ�ӳ���С����֤ӳ���С���������ȣ�
    //  ��ע�⡿���������û���޸�m_StreamSize
    bool IncreaseMappingSize(const dword delta);

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
    //  ���ӳ���ļ���
    const char* GetMapFileName() const;

    //  ����ӳ���ļ����Զ�������С����Сֵ��
    bool SetIncreasSize(const dword size);

    //  �õ�ӳ���ļ����Զ�������С����Сֵ��
    dword GetIncreasSize() const;

    //  ���ӳ���ļ��Ƿ񱻱�ĳ�����ģ�����ļ����޸�ʱ�䣩
    bool ModifiedByOther() const;

    //  ����ͬ���޸�ʱ��ļ�¼��ModifiedByOther()����
    //  �����������޸�ʱ���Ƿ������¼��һ��������
    void SynchronizeModifyTime();

    void Flush() const;

    //  ����ӳ��
    bool ReMap();

    dword LoadFromInfo(CStreamPtr pStream);
    dword SaveInfo(CStreamPtr pStream) const;

    //  ���ӳ���ļ������������;
    const HANDLE GetMapFileHandle() const;
};
//================================================================================

}

#endif