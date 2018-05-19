#if !defined(PERSIST_INCLUDE_)
#define PERSIST_INCLUDE_

//================================================================================
//  �����Ԫ�����ṩ����־û�����
//  �������д_LoadInstance()��_SaveInstance������ȡʵ�ʶ�������
//  _DumpTo()��_InstantiateFrom()ʵ���˶�����Զ����л���ʵ��������
//  _DumpTo()��д�����id��������Ϣ��Ȼ�����_SaveInstance()д��ʵ�ʶ�������
//  _InstantiateFrom()�ȶ�ȡ����id��������Ϣ���Զ���������Ȼ�����_LoadInstance()
//  ���ö�������
//
//  _persistsaveassist��_persistloadassist���á�����id�����һ��������ظ���������
//  ע�⣺������id����ÿһ�����������л�������ʵ���������б���Ψһ��Լ��������id��Ϊ0
//  �����ö����������Ƕ�����ظ���������
//
//  ����洢������ʽΪ��4������id��+ 2(�����Ƴ���) + n(������) + ʵ�ʶ�����
//
//  ע�⣬CPersist�ಢû�������κ��麯�����Ա��������˿��Ե���IPersist����
//================================================================================

#include "..\\include\\InterfacePersist.h"
#include "Stream.h"
#include <map>

namespace easymap
{

class CPersist;
typedef TSmartPtr<CPersist> CPersistPtr;

class _persistsaveassist;
class _persistloadassist;
class CPersist : public IPersist
{
public:
    //================================================================================
    //  �������ɸ�������ã����տͻ��˲�Ҫ����
    //  �ɸ�����assist���ݵ��Ӷ��󣬽��������ظ���������
    //  �ȶ�ȡ����ID��������Ϣ�����Զ���������Ȼ�����_LoadInstance()������ȡʵ�ʵĶ�������
    //================================================================================
    static dword _InstantiateFrom(CStreamPtr pStream, CPersistPtr& pPersist,
        void* const assist);

    //================================================================================
    //  �������ɿͻ��˵��ã�����һ��������ʵ��������
    //  �������ᴴ��һ��_persistloadassist���ݸ��Ӷ���
    //  �Զ������Ӷ�����ظ���������
    //================================================================================
    static dword Instantiate(CStreamPtr pStream, CPersistPtr& pPersist);
};

//�������������ظ���������
class _persistsaveassist
{
public:
    _persistsaveassist();
    ~_persistsaveassist();

    //  �����ַ < - > idӳ��
    std::map<CPersist*, dword> m_InstanceMap;
    dword m_NextID;

    //  ����0����ʧ�ܣ��ö����Ѿ�added
    dword Add(const CPersistPtr pPersist);

    //  ����0����ö���δadd
    dword GetID(const CPersistPtr pPersist) const;

    void Clear();
};

//�������������ظ���������
class _persistloadassist
{
public:
    _persistloadassist();
    ~_persistloadassist();

    //  id < - > �����ַӳ��
    std::map<dword, CPersist*> m_InstanceMap;

    //  ���id == 0����ö����������Ƕ����ظ���������
    //  ����false�����id�Ѿ�added
    bool Add(const dword id, const CPersistPtr pPersist);

    //  ����null�����id��δadd
    CPersistPtr GetInstance(const dword id) const;

    void Clear();
};


//�������������������У������������д_DumpTo()��Dump()
#define PERSIST_DUMP(classname)\
public:\
    dword __stdcall _DumpTo(IStreamX* pStream, void* const assist) const\
    {\
        dword oldpos = pStream->GetPos();\
        dword objid = 0;\
        if (_valid(assist))\
        {\
            CPersistPtr pPersist;\
            const_cast<classname*>(this)->GotoInterface("CPersist", (void**)pPersist._ref());\
            _persistsaveassist* pa = (_persistsaveassist*)assist;\
            objid = pa->GetID(pPersist);\
            if (0 == objid)\
            {\
                objid = pa->Add(pPersist);\
            }\
            else\
            {\
                pStream->WriteData(&objid, sizeof(dword));\
                return sizeof(dword);\
            }\
        }\
        pStream->WriteData(&objid, sizeof(dword));\
        const char* const pcClassName = this->_GetClassName();\
        long len = strlen(pcClassName);\
        pStream->WriteData(&len, sizeof(long));\
        pStream->WriteData(pcClassName, len*sizeof(char));\
        this->_SaveInstance(pStream, assist);\
        return pStream->GetPos() - oldpos;\
    };\
    dword __stdcall Dump(IStreamX* pStream) const\
    {\
        _persistsaveassist assist;\
        dword r = this->_DumpTo(pStream, &assist);\
        return r;\
    };

}

#endif
