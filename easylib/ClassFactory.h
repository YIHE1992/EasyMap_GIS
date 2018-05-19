#if !defined(CLASSFACTORY_INCLUDE_)
#define CLASSFACTORY_INCLUDE_

//================================================================================
//  �����Ԫ�����ṩ������Զ��������ܣ�ͨ�������ַ�����������
//      �� ��ͷ�ļ��м���CLASS_FACTORY(class1)
//      �� ��cpp�ļ��м���CLASS_FACTORY_INSTANCE(class1)
//      �� �Ϳ�����class1֧���Զ�����
//      ���������ǣ�_FactoryManager::CreateInstance("class1", pObj);
//================================================================================

#include "..\\include\\InterfaceObj.h"

#pragma warning(disable: 4786)
#include <map>
#include <string>
#include <vector>
using namespace std;

namespace easymap
{

class _FactoryManager;

//�๤���Ļ���
class _ClassFactory
{
protected:
    char* m_ClassName;  //���๤��Ҫ���ɵ�class����
    bool m_Registered;  //�Ƿ��Ѿ�ע�ᵽ_FactoryManager��

public:
    //ȡ�ö�Ӧ�������ƣ��ַ�����
    const char* const _GetClassName() const;

    //���ִ˷�������ɶ���Ĵ���
    virtual void CreateInstance(IObjPtr& pObj) const = 0;

friend class _FactoryManager;
};

//���������ͷ�ļ��У��Զ�����classname���๤��class��������_ClassFactory
//�๤��Ϊ�������ͣ��乹�캯���Զ����Լ�ע�ᵽ_FactoryManager��
#define CLASS_FACTORY(classname)\
class _ClassFactory_##classname : public _ClassFactory\
{\
    static _ClassFactory_##classname classname##instance;\
    _ClassFactory_##classname();\
    void CreateInstance(IObjPtr& pObj) const;\
};

//���������cpp�ļ��У����������๤����ʵ��
#define CLASS_FACTORY_INSTANCE(classname)\
_ClassFactory_##classname::_ClassFactory_##classname()\
{\
    m_Registered = false;\
    m_ClassName = #classname;\
    _FactoryManager& pool = _FactoryManager::GetFactoryPool();\
    m_Registered = pool.RegisterClassFactory(*this);\
}\
void _ClassFactory_##classname::CreateInstance(IObjPtr& pObj) const\
{\
    classname* p = new classname;\
    IObj* pO = NULL;\
    p->GotoInterface("IObj", (void**)&pO);\
    pObj = pO;\
    pObj->_Release();\
}\
_ClassFactory_##classname _ClassFactory_##classname::classname##instance;

//�������ͣ���������������ע����๤�������ṩ�Զ���������ĺ���
class _FactoryManager
{
private:
    _FactoryManager();
public:
    ~_FactoryManager();

private:
    map<string, _ClassFactory*> m_Factorys;

public:
    bool RegisterClassFactory(_ClassFactory& factory);
    long GetRegisteredClassCount() const;
    void GetRegisteredClassNames(vector<string>& classnames) const;
    _ClassFactory* GetObjectFactory(const char* classname) const;

    //�˺�����֤��Ψһʵ��
    static _FactoryManager& GetFactoryPool();
    //����������������
    static void CreateInstance(const char* classname, IObjPtr& pObj);
};

}

#endif
