#if !defined(CELLQUADTREE_INCLUDED_)
#define CELLQUADTREE_INCLUDED_

//CSingleQuadTree�ǻ��ڹ̶���Χ/�̶��������ֵ��Ĳ����ռ�����
//CMultiQuadTree�ǵ������������ÿ��������һ��CSingleQuadTree���

#include "SpatialIndex.h"

#pragma warning(disable: 4786)
#include <map>
#include <list>
using namespace std;

namespace easymap
{

class CCellQuadTree;
class CSingleQuadTree;
class CMultiQuadTree;

typedef TSmartPtr<CCellQuadTree> CCellQuadTreePtr;
typedef TSmartPtr<CSingleQuadTree> CSingleQuadTreePtr;
typedef TSmartPtr<CMultiQuadTree> CMultiQuadTreePtr;

//================================================================================
//  �Ĳ�������
//================================================================================
class CCellQuadTree : public CSpatialIndex
{
public:
    //�Ĳ����ռ������Ĳ���
    typedef struct
    {
        WKSPoint    centerpoint;    //����������Χ�����ĵ�
        double      BoundSize;      //��������Ŀ��
        long        treelevel;      //�����ļ���
    }CQTParams;

protected:
    //--------------------------------------------------
    //  �Ĳ����Ĳ�ѯ���
    class CQTreeSearchResult : public CSpatialIndex::CSISearchResult
    {
    CLASS_NAME(CQTreeSearchResult)
    NO_EVENTS_DISPATCHER
    NO_EVENTS_LISTENER

    public:
        CQTreeSearchResult();
        ~CQTreeSearchResult(){};

        bool __stdcall GotoInterface(const char* const interfacename, void** pp);
        bool __stdcall Clone(IObj** ppObj) const;

        list<SIItem> m_ItemIDs;
        list<SIItem>::const_iterator m_Pos;

        bool Next(SIItem& item);
        void First();
        dword GetCount() const;
        bool IsEof() const;

    friend class CQuadNode;
    friend class CSingleQuadTree;
    friend class CMultiQuadTree;
    };
    typedef TSmartPtr<CQTreeSearchResult> CQTreeSearchResultPtr;
    //--------------------------------------------------

    //--------------------------------------------------
    //  �Ĳ����ڵ㣬�Ĳ������/�����Ȼ����㷨�����������ʵ��
    class CQuadNode
    {
        //�ĸ�ͬ�����ӽڵ�
        typedef struct
        {
            CQuadNode* pLeaf1;  //����
            CQuadNode* pLeaf2;  //����
            CQuadNode* pLeaf3;  //����
            CQuadNode* pLeaf4;  //����

            WKSRect Extent1;                    //�ڱ��ڵ��ϼ�¼�ӽڵ�
            WKSRect Extent2;                    //�ķ�Χ��Ϊ�˼ӿ����
            WKSRect Extent3;                    //�ٶȣ�����Ҫͨ������
            WKSRect Extent4;                    //��������ӽڵ�ķ�Χ
        }CQuadNodeLeafs;

        //----------------- disabled -----------------
        CQuadNode();
        CQuadNode(const CQuadNode& quadnode);
        //----------------- disabled -----------------

        CQuadNode(const WKSRect& extent);
        CQuadNode(CStreamPtr pStream);
        ~CQuadNode();//���Զ��ɵ��������ӽڵ�

        WKSRect         m_Extent;       //���ڵ�ķ�Χ
        CQuadNodeLeafs  m_Leafs;        //���ڵ��¼���4��Ҷ�ڵ�
        list<SIItem>    m_ItemList;     //���ڱ��ڵ��ϵ�itemID

        //  �����ڵ��extentһ��Ϊ�ģ������ĸ��ӽڵ㡣
        //  ���sublevel>1�����ӽڵ�͸���sublevel - 1��С
        //  ��������ȥ��ע��������ڵ㲻������Ҷ�ڵ㣨�Ѿ���
        //  ���ӽڵ㣩��sublevel<1���򱾺�������ʧ��
        void SplitNode(const long sublevel);

        //  ���mbr��ĳһ���ӽڵ���ȫ�������͵��ø��ӽڵ�
        //  ��additem����˵ݹ棬ֱ������Ҷ�ڵ��û���ӽ�
        //  ������ȫ����mbrΪֹ����itemid�����Ǹ��ڵ���
        void AddItem(const SIItem& item, const WKSRect& mbr);

        //  ����additem�����ҵ��ڵ������ڵ��е�itemid�б�
        //  ���û���ҵ������������
        bool DeleteItem(const SIItem& item, const WKSRect* const pMBR);

        //  ��ñ��ڵ㼰�������ӽڵ��oid����
        dword GetItemCount();

        //  ������pEnvelope ��touch�� ����ÿһ���ڵ㣬ȡ�ù�
        //  ����Щ�ڵ��ϵ�itemid
        void Search(const WKSRect* pEnvelope,
            CQTreeSearchResultPtr pSearchResult) const;

        //  �������ֻ�ᱻ���캯������
        dword SaveToStream(CStreamPtr pStream) const;

        //  �������ֻ�ᱻ���캯������
        dword LoadFromStream(CStreamPtr pStream);

    friend class CSingleQuadTree;
    };
    //--------------------------------------------------

protected:
    virtual ~CCellQuadTree(){};

protected:
    //�Ĳ����Ĳ����������MultiQuadTree������Grid IDΪ0���Ǹ�X@#%%$&*��
    CQTParams m_Parameters;

public:
    //  ��������Χ�Ȳ���
    virtual void GetParameters(CQTParams& paramerters) const = 0;

    bool __stdcall Clone(IObj** ppObj) const;

friend class CQuadNode;
};
//================================================================================


//================================================================================
//  ���ڹ̶��������ֵļ��Ĳ����㷨
//================================================================================
class CSingleQuadTree : public CCellQuadTree
{
CLASS_NAME(CSingleQuadTree)
PERSIST_DUMP(CSingleQuadTree)
NO_EVENTS_DISPATCHER
NO_EVENTS_LISTENER

private:
    CSingleQuadTree();

public:
    CSingleQuadTree(const CQTParams& paramerters);

private:
    ~CSingleQuadTree();

private:
    //���Χ����ʵ���Ǹ��ڵ�ķ�Χ
    WKSRect m_MaxExtent;

    //���ڵ�
    CQuadNode* m_pRoot;

private:
    //  �ɵ����е����ˣ���LoadFromStream����
    void Reset();

    //  ��CMultiQuadTreeʹ��
    void Search(const WKSRect* pEnvelope,
        CCellQuadTree::CQTreeSearchResultPtr pSearchResult) const;

    //  �ɹ��캯�����ã������Ĳ����ڴ�ṹ
    bool Initial(const CQTParams& paramerters);

private:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const;
    dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist);

public:
    bool GetParameter(const dword index, double& param);
    bool AddItem(const SIItem& item, const WKSRect& mbr);
    bool DeleteItem(const SIItem& item, const WKSRect* const pMBR = NULL);
    void ClearItems();
    bool Search(CSpatialIndex::CSISearchResultPtr& pSearchResult,
        const WKSRect* const pEnvelope) const;
    dword GetItemCount() const;

    void GetParameters(CQTParams& paramerters) const;

friend class CMultiQuadTree;
};
//================================================================================


//================================================================================
//  ���������һ��������ɣ�ÿ��������һ���Ĳ������������������Զ���չ
//  Ŀ����Ϊ�˽�������Ĳ����ķ�Χ��������
//  p.s.�ѵ����Ĳ�������������Χ��Ϊͼ����С�ȽϺ���
//================================================================================
class CMultiQuadTree : public CCellQuadTree
{
CLASS_NAME(CMultiQuadTree)
PERSIST_DUMP(CMultiQuadTree)
NO_EVENTS_DISPATCHER
NO_EVENTS_LISTENER

private:
    CMultiQuadTree();

public:
    CMultiQuadTree(const CQTParams& paramerters);

private:
    ~CMultiQuadTree();

private:
    //��һ�����������½����꣬����
    WKSPoint m_Origin;

    typedef CSingleQuadTreePtr SQTreePtr;
    typedef map<long, SQTreePtr> SQTreeMap;
    typedef SQTreeMap::iterator SQTreeMap_it;
    typedef SQTreeMap::const_iterator SQTreeMap_cit;
    //�Ѿ��������Ĳ�����key���ɸߵ�16λX/Y�����ɵ�32λ����ID
    SQTreeMap m_QuadTrees;

    typedef map<SIItem, WKSRect> ItemMbrMap;
    typedef ItemMbrMap::iterator ItemMbrMap_it;
    typedef ItemMbrMap::const_iterator ItemMbrMap_cit;
    //���Ĳ����߽��item���޷��õ����Ĳ���������ģ�ȫ����������
    //ÿ�β�ѯ�����������б�����Ĳ�����Χ����ú����б��е�Ԫ�ؾͲ�Ӧ����̫��
    ItemMbrMap m_CrossBoundItems;

private:
    //  ��LoadFromStream���ã��ɵ����е�����
    void Reset();

    //  ��AddItem()�������ã����ڼ���MBR���ڸ�����ID��
    //  �������false��ʾMBR������߽磬��Ҫ�����m_CrossBoundItems��
    bool CalcGridID(const WKSRect& mbr, long& gridid) const;

    //  ����Ҫ����һ�����Ĳ�����ʱ��ͨ��gridid������������
    CCellQuadTree::CQTParams GetParamsFromGridID(const long gridid) const;

    //  �ɹ��캯�����ã������Ĳ����ڴ�ṹ
    bool Initial(const CQTParams& paramerters);

private:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const;
    dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist);

public:
    bool GetParameter(const dword index, double& param);
    bool AddItem(const SIItem& item, const WKSRect& mbr);
    bool DeleteItem(const SIItem& item, const WKSRect* const pMBR = NULL);
    void ClearItems();
    bool Search(CSpatialIndex::CSISearchResultPtr& pSearchResult,
        const WKSRect* const pEnvelope) const;
    dword GetItemCount() const;

    //  ע���������ǵ�һ���Ĳ�����IDΪ0���Ĳ�������Ϊ���������Ĳ�����
    void GetParameters(CQTParams& paramerters) const;
};
//================================================================================

CLASS_FACTORY(CSingleQuadTree)
CLASS_FACTORY(CMultiQuadTree)

}

#endif