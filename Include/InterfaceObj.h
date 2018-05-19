#if !defined(INTERFACEOBJ_INCLUDE_)
#define INTERFACEOBJ_INCLUDE_

//================================================================================
//                                                                               *
//  IObjΪ���нӿڵ�ԭ�ͣ����������£�                                           *
//      1> ���ü����Թ���                                                      *
//      2> �Զ����¼��ķ����������                                              *
//                                                                               *
//  Ϊ�˼���Delphi�Ľӿڣ�IObjû�ж����κγ�Ա�������麯����ͨ��ʵ�ִ����ú��   *
//      ��ʽ���嵽����ʵ����Ķ����У�                                           *
//  IObj�ĵ�һ���麯��ֻ��ռλ�ã������壬Ŀ��Ҳ��Ϊ�˼���Delphi�Ľӿ�           *
//                                                                               *
//================================================================================

#include "BasicType.h"
#include <assert.h>

#pragma warning(disable: 4786)

namespace easymap
{

//================================================================================
//  ����ָ��
//================================================================================
template<typename T>
struct TSmartPtr
{
    __stdcall TSmartPtr(){obj_pointed_to = NULL;};

    __stdcall TSmartPtr(T* pObj){obj_pointed_to = pObj; this->_AddRef_();};

    __stdcall TSmartPtr(const TSmartPtr& obj){
        obj_pointed_to = obj.obj_pointed_to;
        this->_AddRef_();};

    __stdcall ~TSmartPtr(){this->_Release_();};

    inline TSmartPtr<T>& __stdcall operator=(const TSmartPtr<T>& rhs){
        if (this == &rhs) return *this;
        this->_Release_();
        obj_pointed_to = rhs.obj_pointed_to;
        this->_AddRef_();
        return *this;};

    inline TSmartPtr<T>& __stdcall operator=(T* pObj){
        if (obj_pointed_to == pObj) return *this;
        this->_Release_();
        obj_pointed_to = pObj;
        this->_AddRef_();
        return *this;};

    inline T* __stdcall operator->() const{return obj_pointed_to;};

    inline T& __stdcall operator*() const{return *obj_pointed_to;};

    inline bool __stdcall operator==(const TSmartPtr<T>& obj) const{
        return (obj_pointed_to == obj.obj_pointed_to) ? true : false;};

    inline bool __stdcall operator==(const T* const p) const{
        return (p == obj_pointed_to) ? true : false;};

    inline bool __stdcall operator!=(const TSmartPtr<T>& obj) const{
        return (obj_pointed_to != obj.obj_pointed_to) ? true : false;};

    inline bool __stdcall operator!=(const T* const p) const{
        return (p != obj_pointed_to) ? true : false;};

    inline void __stdcall Clear(){
        this->_Release_();
        obj_pointed_to = NULL;};

    inline bool __stdcall Assigned() const{return (NULL != obj_pointed_to) ? true : false;};

    inline T* __stdcall _p() const{return obj_pointed_to;};

    inline T** __stdcall _ref(){return &obj_pointed_to;};

    inline bool __stdcall Compare(const TSmartPtr<T>& obj) const{
        return (this->obj_pointed_to == obj.obj_pointed_to) ? true : false;};

private:
    inline dword __stdcall _AddRef_(){
        if (NULL != obj_pointed_to) return obj_pointed_to->_AddRef();
        else return 0;};

    inline dword __stdcall _Release_(){
        if (NULL != obj_pointed_to){
            long r = obj_pointed_to->_Release();
            if (0 == r) obj_pointed_to = NULL;
            return r;}
        else return 0;};

    T* obj_pointed_to;
};
//================================================================================


typedef long event_identity;    //�¼�����Ϣ������
const event_identity EVENT_IDENTITY_NULL   = 0;


class IObj;
typedef TSmartPtr<IObj> IObjPtr;
class IObjArray;
typedef TSmartPtr<IObjArray> IObjArrayPtr;


//================================================================================
//  �����ü��������������������
//  �������֮����Ϣ���ݵ�����
//================================================================================
class IObj
{
protected:
    struct _donottouchme
    {
        dword   Data1;
        word    Data2;
        word    Data3;
        byte    Data4[8];
    };
public:
    //  ռλ�ã�Ϊ�˼���Delphi�Ľӿ�
    virtual long __stdcall _placeholder_queryinterface(const _donottouchme iid, void** pp) = 0;

    //-----------------------------------------------------------------------------------
    //  ���ü�������
    //  һ������²����ֹ����֣�ֻ�轫CLASS_NAME(classname)����ھ����������м���
    //-----------------------------------------------------------------------------------
    virtual dword __stdcall _AddRef() = 0;
    virtual dword __stdcall _Release() = 0;
    virtual dword __stdcall _Debug() = 0;
    //-----------------------------------------------------------------------------------


    //-----------------------------------------------------------------------------------
    //  ���������ƣ��ַ������Ľӿڲ�ѯ
    virtual bool __stdcall GotoInterface(const char* const interfacename, void** pp) = 0;
    //-----------------------------------------------------------------------------------


    //-----------------------------------------------------------------------------------
    //  �õ������ơ��ַ�����
    //  һ������²����ֹ����֣�ֻ�轫CLASS_NAME(classname)����ھ����������м���
    virtual const char* const __stdcall _GetClassName() const = 0;
    //-----------------------------------------------------------------------------------


    //-----------------------------------------------------------------------------------
    //  ��Ϊ[��������]������[������]�б��������������м���EVENTS_DISPATCHER������д��Щ������
    //
    //  ע��[��������]����ÿ��[������]��raw reference��without ref-counting����Ŀ���Ǳ���
    //      ѭ�����ã�
    //
    //  ÿ��[������]�����ж�����һ��[��������]�����ã�with ref-counting����
    //  ÿ��[������]���������������н��Լ���[��������]�ļ����б����Ƴ�
    //-----------------------------------------------------------------------------------

    //  �õ����д��������¼�����
    virtual dword __stdcall GetEventsCount() const
    {
        return 0;           //�ȴ���EVENTS_DISPATCHER��д
    };

    //  �����¼�
    virtual bool __stdcall AddEvent(const event_identity ei)
    {
        return false;       //�ȴ���EVENTS_DISPATCHER��д
    };

    //  ɾ���¼���ע������м����߼������¼����Ͳ���ɾ��
    virtual bool __stdcall RemoveEvent(const event_identity ei)
    {
        return false;           //�ȴ���EVENTS_DISPATCHER��д
    };

    //  �õ����������¼�
    virtual event_identity __stdcall GetEvent(const dword index) const
    {
        return EVENT_IDENTITY_NULL; //�ȴ���EVENTS_DISPATCHER��д
    };

    //  ע���¼��ļ����ߣ��¼�������ei���֡�
    //  �ɼ����߶����RegisterToDispatcher()��������
    virtual bool __stdcall _RegisterListener(const event_identity ei,
        const IObj* const pListener)
    {
        return false;       //�ȴ���EVENTS_DISPATCHER��д
    };

    //  ��ע���¼��ļ����ߣ��¼�������ei���֡�
    //  �ɼ����߶����UnregisterFromDispatcher()��������
    virtual bool __stdcall _UnregisterListener(const event_identity ei,
        const IObj* const pListener)
    {
        return false;       //�ȴ���EVENTS_DISPATCHER��д
    };

    //  �õ�����ĳһ�¼���ȫ��������
    //  ����¼������ڣ�����false
    virtual bool __stdcall _GetListenersByEvent(const event_identity ei,
        IObjArray** ppListeners) const
    {
        return 0;
    }

    //  ���ĳһ�������������������¼�������
    //  �ɼ����ߵ��ã����ھ����Ƿ��ܽ��������ߴӼ����ߵ�m_Dispatchers�б������
    virtual dword __stdcall _GetEventCountByListener(const IObj* const pListener) const
    {
        return 0;           //�ȴ���EVENTS_DISPATCHER��д
    };

    //  �ߵ�ĳһ�����ߵ����м������ɼ�����������������
    virtual bool __stdcall _KickAssListener(const IObj* const pListener)
    {
        return false;       //�ȴ���EVENTS_DISPATCHER��д
    };
    //-----------------------------------------------------------------------------------



    //-----------------------------------------------------------------------------------
    //  ��Ϊ[������]���������������м���EVENTS_LISTENER������д���к�����
    //
    //  ע��[������]������һ��[��������]�б�m_Dispatchers��with ref-counting���� ����ʱ��
    //      ��ÿ��[��������]����kick-my-ass�����һ��ǳ���Ҫ�����뽫KICKASS_LISTENER_FROM_DISPATCHERS
    //      ����ӵ�ÿһ��[������]������������
    //-----------------------------------------------------------------------------------

    //  ����pDispatcher->_RegisterListener()���������߶���ע�ᵽ�������ߵ�ĳһ�¼���
    //  ͬʱ��pDispatcher�����ñ��浽m_Dispatchers�б��У�with ref-counting��
    virtual bool __stdcall RegisterToDispatcher(const event_identity ei, IObj* pDispatcher)
    {
        return false;       //�ȴ���EVENTS_LISTENER��д
    };

    //  ����pDispatcher->_KickListener()���������߶���ӱ������ߵ�ĳһ�¼����Ƴ�
    //  ���pDispatcher�������κ��¼���Ҫ��������pDispatcher�Ƴ�m_Dispatchers�б�
    virtual bool __stdcall UnregisterFromDispatcher(const event_identity ei, IObj* pDispatcher)
    {
        return false;       //�ȴ���EVENTS_LISTENER��д
    };

    //  �������߶���ӱ������ߵ������¼����Ƴ�������pDispatcher�Ƴ�m_Dispatchers�б�
    //  ���ڽ���������뱻�����ߵ����м�����ϵ
    virtual bool __stdcall UnregisterAllFromDispatcher(IObj* pDispatcher)
    {
        return false;       //�ȴ���EVENTS_LISTENER��д
    };

    //  �����������и�д�˺���������[��������]�ĵ���
    //      ei                  -       �����¼����ͣ����ж���
    //      pMessage            -       ��Ϣ����
    //      tag                 -       ���ڴ���������ֵ
    //      message_description -       ��Ϣ���������ڴ����򵥵���Ϣ
    virtual bool __stdcall DispatchMessage(const event_identity ei, const IObj* pMessage,
        const easy_variant& tag, const char* const message_description)
    {
        return false;       //===  �ȴ��û��Լ���д  ====
    };
    //-----------------------------------------------------------------------------------


    //-----------------------------------------------------------------------------------
    //  ��¡�Լ��������֧�־ͷ���false��null����
    virtual bool __stdcall Clone(IObj** ppObj) const = 0;
    //-----------------------------------------------------------------------------------
};
//================================================================================


//================================================================================
//  �������飬�����ڽӿڲ����д���һ�����
//================================================================================
class IObjArray : public IObj
{
public:
    virtual bool __stdcall Add(const IObj* pObj) = 0;
    virtual bool __stdcall SetAt(const dword index, const IObj* const pObj) = 0;
    virtual bool __stdcall GetAt(const dword index, IObj** ppObj) const = 0;
    virtual void __stdcall Clear() = 0;
    virtual bool __stdcall Resize(const dword newsize) = 0;
    virtual dword __stdcall GetSize() const = 0;
};
//================================================================================


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//1.    ��������ÿ�������������У������Զ���д_GetClassName()����
//      ���������೧���ʾ������˽�г�Ա���е�bt
//2.    ʵ�����ü�����ע�⣬�մ���������ü�����0
#define CLASS_NAME(classname) \
protected:\
    dword m_RefCount;\
public:\
    long __stdcall _placeholder_queryinterface(const _donottouchme iid, void** pp)\
    {\
        throw;\
    };\
    const char* const __stdcall _GetClassName() const\
    {\
        return #classname;\
    };\
    dword __stdcall _AddRef()\
    {\
        return ++m_RefCount;\
    };\
    dword __stdcall _Release()\
    {\
        m_RefCount--;\
        if (m_RefCount)\
        {\
            return m_RefCount;\
        }\
        else\
        {\
            delete this;\
            return 0;\
        }\
    };\
    dword __stdcall _Debug()\
    {\
        return m_RefCount;\
    }\
friend class _ClassFactory_##classname;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ע�����Ҫ�����������[ÿ��]���캯����
//ȷ�������ʼ����ʱ�����ü���Ϊ0
#define INIT_REFCOUNT this->m_RefCount = 0;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�����������У������¼������ߣ��������ߣ�����
#define EVENTS_DISPATCHER \
private:\
    std::map<event_identity, std::list<const IObj*> > m_EventListeners;\
public:\
    dword __stdcall GetEventsCount() const\
    {\
        return m_EventListeners.size();\
    };\
    bool __stdcall AddEvent(const event_identity ei)\
    {\
        std::map<event_identity, std::list<const IObj*> >::const_iterator it_events = m_EventListeners.find(ei);\
        if (it_events != m_EventListeners.end()) return false;\
        std::list<const IObj*> objlist;\
        m_EventListeners[ei] = objlist;\
        return true;\
    };\
    bool __stdcall RemoveEvent(const event_identity ei)\
    {\
        std::map<event_identity, std::list<const IObj*> >::iterator it_events = m_EventListeners.find(ei);\
        if (it_events == m_EventListeners.end()) return false;\
        m_EventListeners.erase(it_events);\
        return true;\
    };\
    event_identity __stdcall GetEvent(const dword index) const\
    {\
        if (m_EventListeners.size() <= index) return EVENT_IDENTITY_NULL;\
        std::map<event_identity, std::list<const IObj*> >::const_iterator it_events = m_EventListeners.begin();\
        std::advance(it_events, index);\
        return it_events->first;\
    };\
    bool __stdcall _RegisterListener(const event_identity ei, const IObj* const pListener)\
    {\
        if (!pListener) return false;\
        std::map<event_identity, std::list<const IObj*> >::const_iterator it_events = m_EventListeners.find(ei);\
        if (it_events == m_EventListeners.end()) return false;\
        std::list<const IObj*> objlist = it_events->second;\
        std::list<const IObj*>::const_iterator it_objlist = objlist.begin();\
        while (it_objlist != objlist.end())\
        {\
            if (*it_objlist == pListener)\
            {\
                return false;\
            }\
            it_objlist++;\
        }\
        objlist.push_back(pListener);\
        m_EventListeners[ei] = objlist;\
        return true;\
    };\
    bool __stdcall _UnregisterListener(const event_identity ei, const IObj* const pListener)\
    {\
        if (!pListener) return false;\
        std::map<event_identity, std::list<const IObj*> >::const_iterator it_events = m_EventListeners.find(ei);\
        if (it_events == m_EventListeners.end()) return false;\
        std::list<const IObj*> objlist = it_events->second;\
        std::list<const IObj*>::iterator it_objlist = objlist.begin();\
        while (it_objlist != objlist.end())\
        {\
            if (*it_objlist == pListener)\
            {\
                objlist.erase(it_objlist);\
                m_EventListeners[ei] = objlist;\
                return true;\
            }\
            it_objlist++;\
        }\
        return false;\
    };\
    bool __stdcall _GetListenersByEvent(const event_identity ei, IObjArray** ppListeners) const\
    {\
        if (!ppListeners) return false;\
        *ppListeners = NULL;\
        std::map<event_identity, std::list<const IObj*> >::const_iterator it_events = m_EventListeners.find(ei);\
        if (it_events == m_EventListeners.end()) return false;\
        IObjPtr pObj;\
        _FactoryManager::CreateInstance("CObjArray", pObj);\
        pObj->GotoInterface("IObjArray", (void**)ppListeners);\
        std::list<const IObj*> objlist = it_events->second;\
        std::list<const IObj*>::iterator it_objlist = objlist.begin();\
        while (it_objlist != objlist.end())\
        {\
            (*ppListeners)->Add(*it_objlist);\
            it_objlist++;\
        }\
        return true;\
    };\
    dword __stdcall _GetEventCountByListener(const IObj* const pListener) const\
    {\
        if (!pListener) return 0;\
        dword eventcount = 0;\
        std::map<event_identity, std::list<const IObj*> >::const_iterator it_events = m_EventListeners.begin();\
        while (it_events != m_EventListeners.end())\
        {\
            std::list<const IObj*> objlist = it_events->second;\
            std::list<const IObj*>::iterator it_objlist = objlist.begin();\
            while (it_objlist != objlist.end())\
            {\
                if (*it_objlist == pListener)\
                {\
                    eventcount++;\
                    break;\
                }\
                it_objlist++;\
            }\
            it_events++;\
        }\
        return eventcount;\
    };\
    bool __stdcall _KickAssListener(const IObj* const pListener)\
    {\
        bool r = false;\
        std::map<event_identity, std::list<const IObj*> >::const_iterator it_events = m_EventListeners.begin();\
        while (it_events != m_EventListeners.end())\
        {\
            bool flag = false;\
            std::list<const IObj*> objlist = it_events->second;\
            std::list<const IObj*>::iterator it_objlist = objlist.begin();\
            while (it_objlist != objlist.end())\
            {\
                if (*it_objlist == pListener)\
                {\
                    objlist.erase(it_objlist);\
                    flag = r = true;\
                    break;\
                }\
                it_objlist++;\
            }\
            if (flag) m_EventListeners[it_events->first] = objlist;\
            it_events++;\
        }\
        return r;\
    };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//���������ߡ��������ߣ�����һ�������¼�����
#define ADD_EVENTTYPE(ei) this->AddEvent(ei);

//��������ע��ġ������ߡ�����һ���¼���Ϣ
#define DISPATCH_MESSAGE_TOALL(ei, messageobj, tag, description) \
    {\
        IObjArrayPtr pListeners;\
        this->_GetListenersByEvent(ei, pListeners._ref());\
        assert(pListeners._p());\
        if (pListeners._p())\
        {\
            for (dword i = 0; i < pListeners->GetSize(); i++)\
            {\
                IObjPtr pListener;\
                pListeners->GetAt(i, pListener._ref());\
                pListener->DispatchMessage(ei, messageobj, tag, description);\
            }\
        }\
    }

//long tag
#define DISPATCH_LONG_TOALL(ei, tag_long) \
    {\
        easy_variant vtag;\
        vtag.value_long = tag_long;\
        DISPATCH_MESSAGE_TOALL(ei, NULL, vtag, NULL)\
    }

//double tag
#define DISPATCH_DOUBLE_TOALL(ei, tag_double) \
    {\
        easy_variant vtag;\
        vtag.value_double = tag_double;\
        DISPATCH_MESSAGE_TOALL(ei, NULL, vtag, NULL)\
    }

//�������ö���ӵ���¼�������������������
#define NO_EVENTS_DISPATCHER


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�����������У���������߹��ܣ���ÿ�������߶����б��汻�����ߵ�����ָ���б�
//ע�⣺������������������е�KICKASS_LISTENER_FROM_DISPATCHERS���ʹ��
#define EVENTS_LISTENER \
private:\
    std::list<IObjPtr> m_Dispatchers;\
public:\
    bool __stdcall RegisterToDispatcher(const event_identity ei, IObj* pDispatcher)\
    {\
        if (!ei || !pDispatcher) return false;\
        IObj* pThisObj = NULL;\
        this->GotoInterface("IObj", (void**)&pThisObj);\
        this->_Release();\
        if (!pDispatcher->_RegisterListener(ei, pThisObj)) return false;\
        std::list<IObjPtr>::const_iterator it = m_Dispatchers.begin();\
        while (it != m_Dispatchers.end())\
        {\
            if ((*it)._p() == pDispatcher) return true;\
            it++;\
        }\
        IObjPtr pD = pDispatcher;\
        m_Dispatchers.push_back(pD);\
        return true;\
    };\
    bool __stdcall UnregisterFromDispatcher(const event_identity ei, IObj* pDispatcher)\
    {\
        if (!ei || !pDispatcher) return false;\
        IObj* pThisObj = NULL;\
        this->GotoInterface("IObj", (void**)&pThisObj);\
        this->_Release();\
        pDispatcher->_UnregisterListener(ei, pThisObj);\
        if (pDispatcher->_GetEventCountByListener(pThisObj) > 0) return true;\
        std::list<IObjPtr>::iterator it = m_Dispatchers.begin();\
        while (it != m_Dispatchers.end())\
        {\
            if ((*it)._p() == pDispatcher)\
            {\
                m_Dispatchers.erase(it);\
                it = m_Dispatchers.begin();\
                continue;\
            }\
            it++;\
        }\
        return true;\
    };\
    bool __stdcall UnregisterAllFromDispatcher(IObj* pDispatcher)\
    {\
        for (dword i = 0; i < pDispatcher->GetEventsCount(); i++)\
        {\
            event_identity ei = pDispatcher->GetEvent(i);\
            this->UnregisterFromDispatcher(ei, pDispatcher);\
        }\
        return true;\
    };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//�򷢲���ע�����ĳһ�¼�
#define REGISTER_EVENT_TO_DISPATCHER(ei, dispatch)\
    {\
        IObjPtr pDispatcher;\
        CAST_PTR(dispatch, pDispatcher, IObj)\
        this->RegisterToDispatcher(ei, pDispatcher._p());\
    }

//��ע�ᵽ������ĳһ�¼��ļ���
#define UNREGISTER_EVENT_TO_DISPATCHER(ei, dispatch)\
    {\
        IObjPtr pDispatcher;\
        CAST_PTR(dispatch, pDispatcher, IObj)\
        this->UnregisterFromDispatcher(ei, pDispatcher._p());\
    }

//��ע�ᵽ�����������¼��ļ��������ж�listener��dispatcher֮�����ϵ
#define UNREGISTER_ALLEVENT_TO_DISPATCHER(dispatch)\
    {\
        IObjPtr pDispatcher;\
        CAST_PTR(dispatch, pDispatcher, IObj)\
        this->UnregisterAllFromDispatcher(pDispatcher._p());\
    }

//�������ö���ӵ���¼����գ�����������
#define NO_EVENTS_LISTENER


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//����ÿ�����������������У�ʹ�����߶���������ʱ��ÿ���������߷���kick-my-ass����
#define KICKASS_LISTENER_FROM_DISPATCHERS \
    IObj* pThisObj = NULL;\
    this->GotoInterface("IObj", (void**)&pThisObj);\
    this->m_RefCount--;\
    list<IObjPtr>::const_iterator it = m_Dispatchers.begin();\
    while (it != m_Dispatchers.end())\
    {\
        (*it)->_KickAssListener(pThisObj);\
        it++;\
    }\
    m_Dispatchers.clear();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ͨ���ӿ����Ʋ�ѯ�ӿ�
#define CAST_PTR(from_ptr, to_ptr, interface_name)\
    to_ptr.Clear();\
    from_ptr->GotoInterface(#interface_name, (void**)to_ptr._ref());
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//��¡�������������IObjPtr
#define CLONE_PTR(source_ptr, dest_IObjptr)\
    dest_IObjptr.Clear();\
    {\
        easymap::IObj* _point_to_iobj_ = NULL;\
        source_ptr->Clone(&_point_to_iobj_);\
        if (easymap::_valid(_point_to_iobj_))\
        {\
            dest_IObjptr = _point_to_iobj_;\
            _point_to_iobj_->_Release();\
        }\
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define RELEASE(p) if (_valid(p)) {p->_Release(); p = NULL;}


}

#endif