#if !defined(SPATIALINDEX_INCLUDED_)
#define SPATIALINDEX_INCLUDED_

#include "Persist.h"

namespace easymap
{

class CSpatialIndex;
typedef TSmartPtr<CSpatialIndex> CSpatialIndexPtr;

//================================================================================
//  �ռ���������
//================================================================================
typedef dword SIItem;
class CSpatialIndex : public IPersist
{
public:
    //---------  ���������ı��������item�б�  ---------
    class CSISearchResult : public IObj
    {
    protected:
        virtual ~CSISearchResult(){};

    public:
        virtual bool Next(SIItem& item) = 0;
        virtual void First() = 0;
        virtual dword GetCount() const = 0;
        virtual bool IsEof() const = 0;
    };
    typedef TSmartPtr<CSISearchResult> CSISearchResultPtr;
    //--------------------------------------------------


protected:
    virtual ~CSpatialIndex(){};

public:
    //  �õ������ĸ������
    virtual bool GetParameter(const dword index, double& param) = 0;

    //  ���µ�item���������ṹ��
    virtual bool AddItem(const SIItem& item, const WKSRect& mbr) = 0;

    //  ɾ��һ��item�����Ը������item��mbr���Լ���ɾ�������еļ����ٶ�
    virtual bool DeleteItem(const SIItem& item, const WKSRect* const pMBR = NULL) = 0;

    //  XXXX You
    virtual void ClearItems() = 0;

    //  ��ѯ��������������������item�б�
    virtual bool Search(CSISearchResultPtr& pSISearchResult,
        const WKSRect* const pEnvelope) const = 0;

    //  ȡ��item����
    virtual dword GetItemCount() const = 0;
};
//================================================================================

}

#endif
