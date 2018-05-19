#include "CommonInclude.h"
#include "CellQuadTree.h"

namespace easymap
{

CLASS_FACTORY_INSTANCE(CSingleQuadTree)
CLASS_FACTORY_INSTANCE(CMultiQuadTree)

//����������˳ʱ��0,1,2,3���envelope
inline bool QuadSplitEnvelope(const long subid, const WKSRect& originenvelope,
    WKSRect& subenvelope)
{
    double centerx = (originenvelope.right + originenvelope.left) / 2;
    double centery = (originenvelope.top + originenvelope.bottom) / 2;

    switch(subid)
    {
    case 0:
        {
            subenvelope = originenvelope;
            subenvelope.right = centerx;
            subenvelope.bottom = centery;
        }
        break;
    case 1:
        {
            subenvelope = originenvelope;
            subenvelope.left = centerx;
            subenvelope.bottom = centery;
        }
        break;
    case 2:
        {
            subenvelope = originenvelope;
            subenvelope.left = centerx;
            subenvelope.top = centery;
        }
        break;
    case 3:
        {
            subenvelope = originenvelope;
            subenvelope.right = centerx;
            subenvelope.top = centery;
        }
        break;
    default:
        return false;
    }
    return true;
}


//-----------------------------------------------------------------------
//  ������CQTreeSearchResult�Ķ���
//-----------------------------------------------------------------------

CCellQuadTree::CQTreeSearchResult::CQTreeSearchResult()
{
    INIT_REFCOUNT
}

bool __stdcall CCellQuadTree::CQTreeSearchResult::GotoInterface(const char* const interfacename, void** pp)
{
    if (_invalid(pp)) return false;
    assert(!*pp);

    if ((0 == strcmp(interfacename, "IObj"))
        || (0 == strcmp(interfacename, "CSpatialIndex"))
        || (0 == strcmp(interfacename, "CQTreeSearchResult")))
    {
        *pp = this;
    }
    else
    {
        *pp = NULL;
        return false;
    }

    static_cast<IObj*>(*pp)->_AddRef();

    return true;
}

bool __stdcall CCellQuadTree::Clone(IObj** ppObj) const
{
    return false;
}

bool CCellQuadTree::CQTreeSearchResult::Next(SIItem& item)
{
    if (m_ItemIDs.size() <= 0) return false;
    if (m_Pos == m_ItemIDs.end()) return false;
    item = *(m_Pos++);
    return true;
}

void CCellQuadTree::CQTreeSearchResult::First()
{
    m_Pos == m_ItemIDs.begin();
}

dword CCellQuadTree::CQTreeSearchResult::GetCount() const
{
    return m_ItemIDs.size();
}

bool CCellQuadTree::CQTreeSearchResult::IsEof() const
{
    if ((m_ItemIDs.size() <= 0) || (m_Pos == m_ItemIDs.end()))
    {
        return true;
    }

    return false;
}

bool __stdcall CCellQuadTree::CQTreeSearchResult::Clone(IObj** ppObj) const
{
    return false;
}


//-----------------------------------------------------------------------
//  ������CQuadNode�Ķ���
//-----------------------------------------------------------------------

CCellQuadTree::CQuadNode::CQuadNode(const WKSRect& extent) : m_Extent(extent)
{
    m_Leafs.pLeaf1 = NULL;
    m_Leafs.pLeaf2 = NULL;
    m_Leafs.pLeaf3 = NULL;
    m_Leafs.pLeaf4 = NULL;
}

CCellQuadTree::CQuadNode::CQuadNode(CStreamPtr pStream)
{
    this->LoadFromStream(pStream);
}

CCellQuadTree::CQuadNode::~CQuadNode()
{
    if (m_Leafs.pLeaf1)
    {
        delete m_Leafs.pLeaf1;
    }

    if (m_Leafs.pLeaf2)
    {
        delete m_Leafs.pLeaf2;
    }

    if (m_Leafs.pLeaf3)
    {
        delete m_Leafs.pLeaf3;
    }

    if (m_Leafs.pLeaf4)
    {
        delete m_Leafs.pLeaf4;
    }
};

void CCellQuadTree::CQuadNode::SplitNode(const long sublevel)
{
    if (m_Leafs.pLeaf1 || (1 >= sublevel))
    {
        return;//��Ҷ�ڵ㣬���ʧ��
    }

    WKSRect subextent;

    //����
    QuadSplitEnvelope(0, m_Extent, subextent);
    m_Leafs.pLeaf1 = new CQuadNode(subextent);
    m_Leafs.pLeaf1->SplitNode(sublevel - 1);
    m_Leafs.Extent1 = subextent;

    //����
    QuadSplitEnvelope(1, m_Extent, subextent);
    m_Leafs.pLeaf2 = new CQuadNode(subextent);
    m_Leafs.pLeaf2->SplitNode(sublevel - 1);
    m_Leafs.Extent2 = subextent;

    //����
    QuadSplitEnvelope(2, m_Extent, subextent);
    m_Leafs.pLeaf3 = new CQuadNode(subextent);
    m_Leafs.pLeaf3->SplitNode(sublevel - 1);
    m_Leafs.Extent3 = subextent;

    //����
    QuadSplitEnvelope(3, m_Extent, subextent);
    m_Leafs.pLeaf4 = new CQuadNode(subextent);
    m_Leafs.pLeaf4->SplitNode(sublevel - 1);
    m_Leafs.Extent4 = subextent;
}

void CCellQuadTree::CQuadNode::AddItem(const SIItem& item, const WKSRect& mbr)
{
    if (m_Leafs.pLeaf1)
    {
        if (EnvelopesContented(m_Leafs.Extent1, mbr))
        {
            //����
            m_Leafs.pLeaf1->AddItem(item, mbr);
            return;
        }

        if (EnvelopesContented(m_Leafs.Extent2, mbr))
        {
            //����
            m_Leafs.pLeaf2->AddItem(item, mbr);
            return;
        }

        if (EnvelopesContented(m_Leafs.Extent3, mbr))
        {
            //����
            m_Leafs.pLeaf3->AddItem(item, mbr);
            return;
        }

        if (EnvelopesContented(m_Leafs.Extent4, mbr))
        {
            //����
            m_Leafs.pLeaf4->AddItem(item, mbr);
            return;
        }
    }

    //4���ӽڵ㶼�޷���ȫ����mbr�����Ѿ��������Ҷ�ڵ��ˣ�
    //���Ծͽ�itemid��������
    m_ItemList.push_back(item);
}

bool CCellQuadTree::CQuadNode::DeleteItem(const SIItem& item, const WKSRect* const pMBR)
{
    //�ڱ��ڵ��ڲ���
    list<SIItem>::iterator it = m_ItemList.begin();
    while (it != m_ItemList.end())
    {
        if (*it == item)
        {
            //����
            m_ItemList.erase(it);
            return true;
        }
        it++;
    }

    if (!m_Leafs.pLeaf1)
    {
        return false; //��Ȼû�У�������
    }

    if (_valid(pMBR))
    {
        if (EnvelopesTouched(m_Leafs.pLeaf1->m_Extent, *pMBR))
        {
            //����
            if (m_Leafs.pLeaf1->DeleteItem(item, pMBR))
            {
                return true;
            }
        }
        else if (EnvelopesTouched(m_Leafs.pLeaf2->m_Extent, *pMBR))
        {
            //����
            if (m_Leafs.pLeaf2->DeleteItem(item, pMBR))
            {
                return true;
            }
        }
        else if (EnvelopesTouched(m_Leafs.pLeaf3->m_Extent, *pMBR))
        {
            //����
            if (m_Leafs.pLeaf3->DeleteItem(item, pMBR))
            {
                return true;
            }
        }
        else if (EnvelopesTouched(m_Leafs.pLeaf4->m_Extent, *pMBR))
        {
            //����
            if (m_Leafs.pLeaf4->DeleteItem(item, pMBR))
            {
                return true;
            }
        }
        else
        {
            return false; //�����mbr������
        }
    }
    else
    {
        //����mbr��ֱ�Ӽ���ÿһ��extent

        //����
        if (m_Leafs.pLeaf1->DeleteItem(item, pMBR))
        {
            return true;
        }

        //����
        if (m_Leafs.pLeaf2->DeleteItem(item, pMBR))
        {
            return true;
        }

        //����
        if (m_Leafs.pLeaf3->DeleteItem(item, pMBR))
        {
            return true;
        }

        //����
        if (m_Leafs.pLeaf4->DeleteItem(item, pMBR))
        {
            return true;
        }
    }

    return false;
}

dword CCellQuadTree::CQuadNode::GetItemCount()
{
    dword count = this->m_ItemList.size();
    if (this->m_Leafs.pLeaf1)
    {
        count += m_Leafs.pLeaf1->GetItemCount();
        count += m_Leafs.pLeaf2->GetItemCount();
        count += m_Leafs.pLeaf3->GetItemCount();
        count += m_Leafs.pLeaf4->GetItemCount();
    }
    return count;
}

void CCellQuadTree::CQuadNode::Search(const WKSRect* pEnvelope,
    CQTreeSearchResultPtr pSearchResult) const
{
    if (m_Leafs.pLeaf1)
    {
        //�����ӽڵ�
        if (pEnvelope)
        {
            WKSRect subextent;
            QuadSplitEnvelope(0, m_Extent, subextent);
            if (EnvelopesTouched(subextent, *pEnvelope))//����
            {
                m_Leafs.pLeaf1->Search(pEnvelope, pSearchResult);
            }

            QuadSplitEnvelope(1, m_Extent, subextent);
            if (EnvelopesTouched(subextent, *pEnvelope))//����
            {
                m_Leafs.pLeaf2->Search(pEnvelope, pSearchResult);
            }

            QuadSplitEnvelope(2, m_Extent, subextent);
            if (EnvelopesTouched(subextent, *pEnvelope))//����
            {
                m_Leafs.pLeaf3->Search(pEnvelope, pSearchResult);
            }

            QuadSplitEnvelope(3, m_Extent, subextent);
            if (EnvelopesTouched(subextent, *pEnvelope))//����
            {
                m_Leafs.pLeaf4->Search(pEnvelope, pSearchResult);
            }
        }
        else
        {
            WKSRect subextent;
            QuadSplitEnvelope(0, m_Extent, subextent);
            m_Leafs.pLeaf1->Search(NULL, pSearchResult);

            QuadSplitEnvelope(1, m_Extent, subextent);
            m_Leafs.pLeaf2->Search(NULL, pSearchResult);

            QuadSplitEnvelope(2, m_Extent, subextent);
            m_Leafs.pLeaf3->Search(NULL, pSearchResult);

            QuadSplitEnvelope(3, m_Extent, subextent);
            m_Leafs.pLeaf4->Search(NULL, pSearchResult);
        }
    }

    //���ر��ڵ�Ķ���
    list<SIItem>::const_iterator it = m_ItemList.begin();
    while (it != m_ItemList.end())
    {
        pSearchResult->m_ItemIDs.push_back(*it);
        it++;
    }
}

dword CCellQuadTree::CQuadNode::SaveToStream(CStreamPtr pStream) const
{
    dword oldpos = pStream->GetPos();

    pStream->Write(&m_Extent, sizeof(WKSRect));

    //�洢���ڵ�Ķ���
    dword itemcount = m_ItemList.size();
    pStream->Write(itemcount);
    list<SIItem>::const_iterator it = m_ItemList.begin();
    while (it != m_ItemList.end())
    {
        pStream->Write(&(*it), sizeof(SIItem));
        it++;
    }

    if (m_Leafs.pLeaf1)
    {
        long flag = 1;
        pStream->Write(flag);

        m_Leafs.pLeaf1->SaveToStream(pStream);
        m_Leafs.pLeaf2->SaveToStream(pStream);
        m_Leafs.pLeaf3->SaveToStream(pStream);
        m_Leafs.pLeaf4->SaveToStream(pStream);
    }
    else
    {
        long flag = 0;//û���ӽڵ�
        pStream->Write(flag);
    }

    return pStream->GetPos() - oldpos;
}

dword CCellQuadTree::CQuadNode::LoadFromStream(CStreamPtr pStream)
{
    dword oldpos = pStream->GetPos();

    pStream->Read(&m_Extent, sizeof(WKSRect));

    //��ԭ���ڵ��е�itemid
    dword itemcount;
    pStream->Read(itemcount);
    for (dword i = 0; i < itemcount; i++)
    {
        SIItem item;
        pStream->Read(&item, sizeof(SIItem));
        m_ItemList.push_back(item);
    }

    long flag;
    pStream->Read(flag);
    if (flag)
    {
        QuadSplitEnvelope(0, m_Extent, m_Leafs.Extent1);
        QuadSplitEnvelope(1, m_Extent, m_Leafs.Extent2);
        QuadSplitEnvelope(2, m_Extent, m_Leafs.Extent3);
        QuadSplitEnvelope(3, m_Extent, m_Leafs.Extent4);

        //��ԭ4���ӽڵ�
        m_Leafs.pLeaf1 = new CQuadNode(pStream);
        m_Leafs.pLeaf2 = new CQuadNode(pStream);
        m_Leafs.pLeaf3 = new CQuadNode(pStream);
        m_Leafs.pLeaf4 = new CQuadNode(pStream);
    }
    else
    {
        //don't forget these suckers
        m_Leafs.pLeaf1 = NULL;
        m_Leafs.pLeaf4 = NULL;
        m_Leafs.pLeaf3 = NULL;
        m_Leafs.pLeaf2 = NULL;
    }

    return pStream->GetPos() - oldpos;
}

//-----------------------------------------------------------------------
//  ������CSingleQuadTree�Ķ���
//-----------------------------------------------------------------------

CSingleQuadTree::CSingleQuadTree()
{
    INIT_REFCOUNT

    m_pRoot = NULL;
    m_Parameters.treelevel = 0;
}

CSingleQuadTree::CSingleQuadTree(const CQTParams& paramerters)
{
    INIT_REFCOUNT

    m_pRoot = NULL; //don't forget!
    m_Parameters.treelevel = 0;
    this->Initial(paramerters);
}

CSingleQuadTree::~CSingleQuadTree()
{
    this->Reset();
}

void CSingleQuadTree::Reset()
{
    if (m_pRoot)
    {
        delete m_pRoot;//���Զ�ѭ��ɾ�������ӽڵ�
        m_pRoot = NULL;
    }
}

void CSingleQuadTree::Search(const WKSRect* pEnvelope,
    CCellQuadTree::CQTreeSearchResultPtr pSearchResult) const
{
    if (pEnvelope && !EnvelopesTouched(*pEnvelope, this->m_MaxExtent))
    {
        return;
    }

    m_pRoot->Search(pEnvelope, pSearchResult);
    pSearchResult->m_Pos = pSearchResult->m_ItemIDs.begin();
}

bool CSingleQuadTree::Initial(const CQTParams& paramerters)
{
    if (0 >= paramerters.treelevel)
    {
        return false;
    }

    m_Parameters = paramerters;
    if (0.0001 > m_Parameters.BoundSize)
    {
        m_Parameters.BoundSize = 0.0001;
    }

    if (m_pRoot)
    {
        delete m_pRoot;
    }

    double halfsize = m_Parameters.BoundSize / 2;
    m_MaxExtent.bottom = m_Parameters.centerpoint.y - halfsize;
    m_MaxExtent.left = m_Parameters.centerpoint.x - halfsize;
    m_MaxExtent.right = m_Parameters.centerpoint.x + halfsize;
    m_MaxExtent.top = m_Parameters.centerpoint.y + halfsize;

    //Լ�����ڵ㼶��Ϊ1
    m_pRoot = new CQuadNode(m_MaxExtent);

    //ѭ������Ĳ����ڵ㣬���������Ĳ���
    m_pRoot->SplitNode(m_Parameters.treelevel);

    return true;
}

bool CSingleQuadTree::GetParameter(const dword index, double& param)
{
    bool r = true;

    switch(index)
    {
    case 0:
        {
            param = m_Parameters.centerpoint.x;
            break;
        }
    case 1:
        {
            param = m_Parameters.centerpoint.y;
            break;
        }
    case 2:
        {
            param = m_Parameters.BoundSize;
            break;
        }
    case 3:
        {
            param = m_Parameters.treelevel;
            break;
        }
    default:
        r = false;
    }

    return r;
}

bool CSingleQuadTree::AddItem(const SIItem& item, const WKSRect& mbr)
{
    //���жϷ�Χ�Ƿ񳬹��������˾Ͳ���
    if (!EnvelopesTouched(m_MaxExtent, mbr))
    {
        return false;
    }

    m_pRoot->AddItem(item, mbr);
    return true;
}

bool CSingleQuadTree::DeleteItem(const SIItem& item, const WKSRect* const pMBR)
{
    return m_pRoot->DeleteItem(item, pMBR);
}

void CSingleQuadTree::ClearItems()
{
    this->Reset();
    this->Initial(m_Parameters);
}

bool CSingleQuadTree::Search(CSpatialIndex::CSISearchResultPtr& pSearchResult,
    const WKSRect* const pEnvelope) const
{
    CQTreeSearchResultPtr pQTreeSearchResult
        = new CQTreeSearchResult;

    //�Զ�����ÿһ���ӽڵ�
    m_pRoot->Search(pEnvelope, pQTreeSearchResult);
    pQTreeSearchResult->m_Pos = pQTreeSearchResult->m_ItemIDs.begin();
    pSearchResult = pQTreeSearchResult._p();
    return true;
}

dword CSingleQuadTree::GetItemCount() const
{
    return m_pRoot->GetItemCount();
}

bool __stdcall CSingleQuadTree::GotoInterface(const char* const interfacename, void** pp)
{
    if (_invalid(pp)) return false;
    assert(!*pp);

    if ((0 == strcmp(interfacename, "IObj"))
        || (0 == strcmp(interfacename, "IPersist"))
        || (0 == strcmp(interfacename, "CPersist"))
        || (0 == strcmp(interfacename, "CSpatialIndex"))
        || (0 == strcmp(interfacename, "CCellQuadTree"))
        || (0 == strcmp(interfacename, "CSingleQuadTree"))
        )
    {
        *pp = this;
    }
    else
    {
        *pp = NULL;
        return false;
    }

    static_cast<IObj*>(*pp)->_AddRef();

    return true;
}

dword __stdcall CSingleQuadTree::_SaveInstance(IStreamX* pStream, void* const assist) const
{
    dword oldpos = pStream->GetPos();

    pStream->WriteData(&m_Parameters, sizeof(CQTParams));
    pStream->WriteData(&m_MaxExtent, sizeof(WKSRect));
    if (m_pRoot)
    {
        //����ڵ���
        long flag = 1;
        pStream->WriteData(&flag, sizeof(long));
        CStreamPtr ps = (CStream*)pStream;
        m_pRoot->SaveToStream(ps);
    }
    else
    {
        long flag = 0;
        pStream->WriteData(&flag, sizeof(long));
    }

    return pStream->GetPos() - oldpos;
}

dword __stdcall CSingleQuadTree::_LoadInstance(IStreamX* pStream, void* const assist)
{
    this->Reset();

    dword oldpos = pStream->GetPos();

    pStream->ReadData(&m_Parameters, sizeof(CQTParams));
    pStream->ReadData(&m_MaxExtent, sizeof(WKSRect));

    long flag;
    pStream->ReadData(&flag, sizeof(long));
    if (flag)
    {
        //�Ӹ��ڵ㿪ʼ�ָ�������
        CStreamPtr ps = (CStream*)pStream;
        m_pRoot = new CQuadNode(ps);
    }

    return pStream->GetPos() - oldpos;
}

void CSingleQuadTree::GetParameters(CQTParams& paramerters) const
{
    paramerters = m_Parameters;
}


//-----------------------------------------------------------------------
//  ������CMultiQuadTree�Ķ���
//-----------------------------------------------------------------------

CMultiQuadTree::CMultiQuadTree()
{
    INIT_REFCOUNT

    m_Parameters.treelevel = 0;
}

CMultiQuadTree::CMultiQuadTree(const CQTParams& paramerters)
{
    INIT_REFCOUNT

    this->Initial(paramerters);
}

CMultiQuadTree::~CMultiQuadTree()
{
    this->Reset();
};

void CMultiQuadTree::Reset()
{
    SQTreeMap_it it = m_QuadTrees.begin();
    m_QuadTrees.clear();
    m_CrossBoundItems.clear();
}

bool CMultiQuadTree::CalcGridID(const WKSRect& mbr, long& gridid) const
{
    short multiplyX, multiplyY;
    double tmp = mbr.left - m_Origin.x;
    multiplyX = (short)(tmp / m_Parameters.BoundSize);
    if (tmp < 0) multiplyX += -1;
    tmp = mbr.bottom - m_Origin.y;
    multiplyY = (short)(tmp / m_Parameters.BoundSize);
    if (tmp < 0) multiplyY += -1;
    //multiplyY����2�ֽ� & multiplyX
    long tmpx = 0x0000FFFF & multiplyX;
    long tmpy = multiplyY << 16;
    gridid = tmpy | tmpx;

    tmp = mbr.right - m_Origin.x;
    multiplyX = (short)(tmp / m_Parameters.BoundSize);
    if (tmp < 0) multiplyX += -1;
    tmp = mbr.top - m_Origin.y;
    multiplyY = (short)(tmp / m_Parameters.BoundSize);
    if (tmp < 0) multiplyY += -1;
    //multiplyY����2�ֽ� & multiplyX
    tmpx = 0x0000FFFF & multiplyX;
    tmpy = multiplyY << 16;
    long gridid1 = tmpy | tmpx;

    //���mbr�������Խ������Ƿ���ͬһ��������
    if (gridid == gridid1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

CCellQuadTree::CQTParams CMultiQuadTree::GetParamsFromGridID(const long gridid) const
{
    CQTParams params = m_Parameters;
    //��gridid��ɸߵ͸������ֽڣ��ֱ����centerpoint.x��centerpoint.y
    short multiplyX = (short)(gridid & 0x0000FFFF);
    short multiplyY = (short)((gridid >> 16));
    params.centerpoint.x = params.centerpoint.x
        + params.BoundSize*multiplyX;
    params.centerpoint.y = params.centerpoint.y
        + params.BoundSize*multiplyY;

    return params;
}

bool CMultiQuadTree::Initial(const CQTParams& paramerters)
{
    if (0 >= paramerters.treelevel)
    {
        return false;
    }

    //�ɵ�m_CrossBoundItems�����е��Ĳ���
    m_QuadTrees.clear();
    m_CrossBoundItems.clear();

    //����
    m_Parameters = paramerters;
    if (0.0001 > m_Parameters.BoundSize)
    {
        m_Parameters.BoundSize = 0.0001;
    }
    double halfboundsize = m_Parameters.BoundSize / 2;
    m_Origin.x = m_Parameters.centerpoint.x - halfboundsize;
    m_Origin.y = m_Parameters.centerpoint.y - halfboundsize;

    //�㶨
    return true;
}

bool CMultiQuadTree::GetParameter(const dword index, double& param)
{
    bool r = true;

    switch(index)
    {
    case 0:
        {
            param = m_Parameters.centerpoint.x;
            break;
        }
    case 1:
        {
            param = m_Parameters.centerpoint.y;
            break;
        }
    case 2:
        {
            param = m_Parameters.BoundSize;
            break;
        }
    case 3:
        {
            param = m_Parameters.treelevel;
            break;
        }
    default:
        r = false;
    }

    return r;
}

bool CMultiQuadTree::AddItem(const SIItem& item, const WKSRect& mbr)
{
    long gridid;
    if (this->CalcGridID(mbr, gridid))
    {
        SQTreePtr pSingleQuadTree;

        //�������ɻ��Ƿ���ڣ����ھʹ���
        SQTreeMap_it it_tree = m_QuadTrees.find(gridid);
        if (it_tree == m_QuadTrees.end())
        {
            CQTParams params = this->GetParamsFromGridID(gridid);
            pSingleQuadTree = new CSingleQuadTree(params);
            m_QuadTrees[gridid] = pSingleQuadTree;
        }
        else
        {
            //�Ĳ����Ѿ�����
            pSingleQuadTree = it_tree->second;
        }
        return pSingleQuadTree->AddItem(item, mbr);
    }
    else
    {
        //����m_CrossBoundItems��
        m_CrossBoundItems[item] = mbr; 
        return true;
    }
}

bool CMultiQuadTree::DeleteItem(const SIItem& item, const WKSRect* const pMBR)
{
    //�ȼ��ÿһ���Ĳ���
    SQTreeMap_it it_tree = m_QuadTrees.begin();
    while (it_tree != m_QuadTrees.end())
    {
        SQTreePtr pSingleQuadTree = it_tree->second;
        if (pSingleQuadTree->DeleteItem(item, pMBR))
        {
            return true;
        }
        it_tree++;
    }

    //Ҳ������m_CrossBoundItems��
    ItemMbrMap_it it_cbmbr = m_CrossBoundItems.find(item);
    if (it_cbmbr != m_CrossBoundItems.end())
    {
        m_CrossBoundItems.erase(it_cbmbr);
        return true;
    }

    //������û�У�
    return false;
}

void CMultiQuadTree::ClearItems()
{
    SQTreeMap_cit it_tree = m_QuadTrees.begin();
    while (it_tree != m_QuadTrees.end())
    {
        SQTreePtr pSingleQuadTree = it_tree->second;
        pSingleQuadTree->ClearItems();
        it_tree++;
    }
}

bool CMultiQuadTree::Search(CSpatialIndex::CSISearchResultPtr& pSearchResult,
    const WKSRect* const pEnvelope) const
{
    CQTreeSearchResultPtr pQTreeSearchResult
        = new CQTreeSearchResult;

    //����Ӧ���Ż�һ�£�ֱ�Ӽ������Ҫsearch���Ĳ���ID��
    //����Ӧ�ñ���ÿһ���Ĳ���
    //��������˭����
    SQTreeMap_cit it_tree = m_QuadTrees.begin();
    while (it_tree != m_QuadTrees.end())
    {
        SQTreePtr pSingleQuadTree = it_tree->second;
        pSingleQuadTree->Search(pEnvelope, pQTreeSearchResult);
        it_tree++;
    }

    //��������Щ
    ItemMbrMap_cit it_mbr = m_CrossBoundItems.begin();
    while (it_mbr != m_CrossBoundItems.end())
    {
        if (!pEnvelope || EnvelopesTouched(*pEnvelope, it_mbr->second))
        {
            pQTreeSearchResult->m_ItemIDs.push_back(it_mbr->first);
        }
        it_mbr++;
    }

    pQTreeSearchResult->m_Pos = pQTreeSearchResult->m_ItemIDs.begin();
    pSearchResult = pQTreeSearchResult._p();
    return true;
}

dword CMultiQuadTree::GetItemCount() const
{
    dword itemcount = 0;
    SQTreeMap_cit it_tree = m_QuadTrees.begin();
    while (it_tree != m_QuadTrees.end())
    {
        SQTreePtr pSingleQuadTree = it_tree->second;
        itemcount += pSingleQuadTree->GetItemCount();
        it_tree++;
    }
    return itemcount + m_CrossBoundItems.size();
}

bool __stdcall CMultiQuadTree::GotoInterface(const char* const interfacename, void** pp)
{
    if (_invalid(pp)) return false;
    assert(!*pp);

    if ((0 == strcmp(interfacename, "IObj"))
        || (0 == strcmp(interfacename, "IPersist"))
        || (0 == strcmp(interfacename, "CPersist"))
        || (0 == strcmp(interfacename, "CSpatialIndex"))
        || (0 == strcmp(interfacename, "CCellQuadTree"))
        || (0 == strcmp(interfacename, "CMultiQuadTree"))
        )
    {
        *pp = this;
    }
    else
    {
        *pp = NULL;
        return false;
    }

    static_cast<IObj*>(*pp)->_AddRef();

    return true;
}

dword __stdcall CMultiQuadTree::_SaveInstance(IStreamX* pStream, void* const assist) const
{
    dword oldpos = pStream->GetPos();
    CStreamPtr ps = (CStream*)pStream;

    //����ԭ������
    ps->Write(m_Origin.x);
    ps->Write(m_Origin.y);

    //�������Ĳ�������
    dword count = m_QuadTrees.size();
    ps->Write(count);

    //����ÿһ��gridid<-->�Ĳ���pair
    SQTreeMap_cit it_tree = m_QuadTrees.begin();
    while (it_tree != m_QuadTrees.end())
    {
        long gridid = it_tree->first;
        ps->Write(gridid);
        SQTreePtr pSingleQuadTree = it_tree->second;
        pSingleQuadTree->_DumpTo(pStream, NULL);
        it_tree++;
    }

    //����m_CrossBoundItemsԪ�ظ���
    count = m_CrossBoundItems.size();
    ps->Write(count);

    //����m_CrossBoundItems�е�ÿһ��id<-->mbr pair
    ItemMbrMap_cit it_mbr = m_CrossBoundItems.begin();
    while (it_mbr != m_CrossBoundItems.end())
    {
        ps->Write(&(it_mbr->first), sizeof(SIItem));
        ps->Write(&(it_mbr->second), sizeof(WKSRect));
        it_mbr++;
    }

    //�����׼����
    ps->Write(&m_Parameters, sizeof(CQTParams));

    //�չ�
    return pStream->GetPos() - oldpos;
}

dword __stdcall CMultiQuadTree::_LoadInstance(IStreamX* pStream, void* const assist)
{
    this->Reset();

    dword oldpos = pStream->GetPos();
    CStreamPtr ps = (CStream*)pStream;

    //�ָ�ԭ������
    ps->Read(m_Origin.x);
    ps->Read(m_Origin.y);

    dword count, i;

    //��ȡ�Ĳ�������
    ps->Read(count);

    //˳�����ÿ��id <--> �Ĳ���
    for (i = 0; i < count; i++)
    {
        long gridid;
        ps->Read(gridid);
        CPersistPtr pPersist;
        CPersist::_InstantiateFrom(ps, pPersist, assist);
        SQTreePtr pSingleQuadTree = (CSingleQuadTree*)pPersist._p();
        m_QuadTrees[gridid] = pSingleQuadTree;
    }

    //��ȡm_CrossBoundItemsԪ�ظ���
    ps->Read(count);

    //˳�����ÿ��id <--> mbr pair
    for (i = 0; i < count; i++)
    {
        SIItem item;
        ps->Read(&item, sizeof(SIItem));
        WKSRect mbr;
        ps->Read(&mbr, sizeof(WKSRect));
        m_CrossBoundItems[item] = mbr;
    }

    //��ȡ��׼����
    ps->Read(&m_Parameters, sizeof(CQTParams));

    //����
    return pStream->GetPos() - oldpos;
}

void CMultiQuadTree::GetParameters(CQTParams& paramerters) const
{
    paramerters = m_Parameters;
};

}
