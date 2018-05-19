#if !defined(CELLNONUPLETREE_INCLUDED_)
#define CELLNONUPLETREE_INCLUDED_

#include "..\\include\\WKSInclude.h"
#include "ClassFactory.h"
#include "Persist.h"

#pragma warning(disable: 4786)
#include <map>
#include <list>
using namespace std;

namespace easymap
{

class CCellNonupleTree;
typedef TSmartPtr<CCellNonupleTree> CCellNonupleTreePtr;

//================================================================================
//  �Ź���(�Ų���)�����㷨�������������η�Χ�ڵĶ���
//  ����㷨�������������ε������������Ǻ��Ĳ�����ͬ�ĵط�
//================================================================================
class CCellNonupleTree : public IPersist
{
CLASS_NAME(CCellNonupleTree)
PERSIST_DUMP(CCellNonupleTree)
NO_EVENTS_DISPATCHER
NO_EVENTS_LISTENER

public:
    typedef struct
    {
        WKSRect     extent;
        long        level;
    }CNTParams;

    typedef enum
    {
        NODELEAF_CENTER             = 1,
        NODELEAF_UP                 = 2,
        NODELEAF_DOWN               = 3,
        NODELEAF_LEFT               = 4,
        NODELEAF_RIGHT              = 5,
        NODELEAF_LEFTUP             = 6,
        NODELEAF_RIGHTUP            = 7,
        NODELEAF_LEFTDOWN           = 8,
        NODELEAF_RIGHTDOWN          = 9,
    }NodeLeafEnum;

    class CNonupleNode;

    //��ѯ��������巵��ʲô���������������
    class CCellNonupleResult : public IObj
    {
    protected:
        virtual ~CCellNonupleResult(){};
    };
    typedef TSmartPtr<CCellNonupleResult> CCellNonupleResultPtr;

    //��ѯ��������ʵ��Search���������������ѯ����
    //ע��Ҫ����Clone()����
    class CCellNonupleFilter : public IPersist
    {
    protected:
        virtual ~CCellNonupleFilter(){};

    private:
        //  ��CNonupleNode::Search()���ã��жϲ�ѯ���extent�Ƿ�match
        //  ����ֵ  true��  ��extent�У�����ת��ͬ������һ�ڵ�
        //          false:  ����extent�У�Ӧ����ͬ������һ�ڵ�
        virtual bool CheckExtent(const WKSRect& extent, const long level) const = 0;

        //  �ж��ڸ�Cell���Ƿ��ѯ�ɹ�����CNonupleNode::Search()����
        //  ���ز�ѯ���
        virtual void Search(const double& x, const double& y, const long level,
            CCellNonupleResultPtr& pResult) const = 0;

    friend class CNonupleNode;
    };
    typedef TSmartPtr<CCellNonupleFilter> CCellNonupleFilterPtr;

    //  �Ų����ڵ�
    class CNonupleNode
    {
        typedef struct
        {
            CNonupleNode* pLeaf_Center;
            CNonupleNode* pLeaf_Up;
            CNonupleNode* pLeaf_Down;
            CNonupleNode* pLeaf_Left;
            CNonupleNode* pLeaf_Right;
            CNonupleNode* pLeaf_LeftUp;
            CNonupleNode* pLeaf_RightUp;
            CNonupleNode* pLeaf_LeftDown;
            CNonupleNode* pLeaf_RightDown;

            WKSRect Extent_Center;
            WKSRect Extent_Up;
            WKSRect Extent_Down;
            WKSRect Extent_Left;
            WKSRect Extent_Right;
            WKSRect Extent_LeftUp;
            WKSRect Extent_RightUp;
            WKSRect Extent_LeftDown;
            WKSRect Extent_RightDown;
        }CNodeLeafs;    //�ڵ��µ�9��Ҷ�ڵ�

        CNonupleNode();
        CNonupleNode(const CNonupleNode& nonuplenode);
        CNonupleNode(const long level, const WKSRect& extent, const WKSPoint& region_center);
        ~CNonupleNode();

        //�ڵ������
        long            m_Level;
        WKSRect         m_Extent;
        CNodeLeafs      m_Leafs;
        double          m_Dist_to_center;   //�ýڵ�����������ĵľ���

        //  �����ڵ��m_Extentһ��Ϊ9������9���ӽڵ㡣
        //  ���sublevel>1�����ӽڵ�͸���sublevel - 1��С
        //  ��������ȥ��ע��������ڵ㲻������Ҷ�ڵ㣨�Ѿ���
        //  ���ӽڵ㣩��sublevel<1���򱾺�������ʧ��
        void SplitNode(const long sublevel, const WKSPoint& region_center);

        //  �ñ��ڵ��m_Extent����CCellNonupleFilter::Search()
        //  region_center���������������
        //  pSearchResultΪ�գ���ѯʧ��
        void Search(const CCellNonupleFilterPtr pFilter, CCellNonupleResultPtr& pResult,
            const bool dosearch) const;

    friend class CCellNonupleTree;
    };

private:
    CCellNonupleTree();

public:
    CCellNonupleTree(const CNTParams& paramerters);

private:
    ~CCellNonupleTree();

    CNTParams               m_Parameters;
    CNonupleNode*           m_pRoot;
    CCellNonupleFilterPtr   m_pFilter;

private:
    //  �ɵ����е����ˣ���LoadFromStream����
    void Reset();

    //  �ɹ��캯�����ã������Ų����ڴ�ṹ
    bool Initial(const CNTParams& paramerters);

public:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    bool __stdcall Clone(IObj** ppObj) const;
    bool Clone(IObjPtr& pObj) const;

    dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const;
    dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist);

    void GetParameters(CNTParams& paramerters) const;
    void SetFilter(const CCellNonupleFilterPtr pFilter);
    void Search(CCellNonupleResultPtr& pResult) const;

};
//================================================================================

CLASS_FACTORY(CCellNonupleTree)
}

#endif
