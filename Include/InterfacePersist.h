#if !defined(INTERFACEPERSIST_INCLUDE_)
#define INTERFACEPERSIST_INCLUDE_

#include "InterfaceStream.h"

namespace easymap
{

class IPersist;
typedef TSmartPtr<IPersist> IPersistPtr;

class IPersist : public IObj
{
protected:
    //  ����������������Ϣ��ֱ��д���������
    virtual dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const = 0;

    //  ����������������Ϣ��ֱ�Ӷ�ȡ��������
    virtual dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist) = 0;

public:
    //================================================================================
    //  �������ɸ�������ã����տͻ��˲�Ҫ���ã�
    //  �ɸ�����pAssist���ݵ��Ӷ��󣬽��������ظ���������
    //  ����stream��д�����ID��������Ϣǰ׺��Ȼ�����_SaveInstance()д��ʵ�ʵĶ�������
    //  ���pAssist == NULL��������󲻿����ظ���������
    //================================================================================
    virtual dword __stdcall _DumpTo(IStreamX* pStream, void* const assist) const = 0;

    //================================================================================
    //  �������ɿͻ��˵��ã�����һ�����������л�����
    //  �������ᴴ��һ��_persistsaveassist���ݸ��Ӷ���
    //  �Զ������Ӷ�����ظ���������
    //================================================================================
    virtual dword __stdcall Dump(IStreamX* pStream) const = 0;
};

}

#endif
