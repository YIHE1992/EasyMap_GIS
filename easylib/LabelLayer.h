#if !defined(LabelLayer_INCLUDED_)
#define LabelLayer_INCLUDED_

#include "CommonInclude.h"
#include "..\\include\\InterfaceLabelLayer.h"
#include "SlimLayer.h"

namespace easymap
{

class CLabelLayer;
typedef TSmartPtr<CLabelLayer> CLabelLayerPtr;

class CLabelLayer : public ILabelLayer
{
CLASS_NAME(CLabelLayer)
PERSIST_DUMP(CLabelLayer)
NO_EVENTS_DISPATCHER
EVENTS_LISTENER

    struct LabelText
    {
        string text;
        vector<WKSPoint> labelpoints;
    };

public:
    CLabelLayer();
private:
    ~CLabelLayer();

private:
    double              m_MaxVisualScale;       //  �����ʾ������
    double              m_MinVisualScale;       //  ��С��ʾ������
    double              m_RefScale;             //  �ο�������
    long                m_Tag;                  //  ����
    bool                m_Visible;              //  �Ƿ����
    byte                m_Alpha;                //  ͸����
    string              m_Name;                 //  ͼ������
    long                m_FieldIndex;           //   < 0�����עfid

    CVectorLayerPtr     m_pVL;                  //  ��һ��ʸ��ͼ��
    ITextSymbolPtr      m_pTextSymbol;          //  ���Ʊ�ע�ķ��ţ��϶�����

    map<dword, LabelText> m_LabelCache;         //  ��ע�Ļ���
    vector<dword>       m_FidsToSaved;          //  ʸ��ͼ���б༭�޸ĺ�δ�����fid

private:
    DrawResult DoDrawData( const IDisplayCache* pDisplayCache, const long cacheid,
        const WKSRect* const pEnvelope, const ITrackCancel* pTrackCancel);

    void DrawGeometryLabel(const IDisplayTransformationPtr pDT, const IDisplayCachePtr pDisplayCache,
        const dword cacheid, const dword fid, const IGeometryPtr pGeometry, const string& text);
    void DrawCacheLabel(const IDisplayCachePtr pDisplayCache, const dword cacheid,
        const LabelText& label);

    void GetLabelPoints(const IPointPtr pTempPoint, const IDisplayTransformationPtr pDT,
        const dword fid, const IGeometryPtr pGeometry, const string& text, vector<WKSPoint>& textlabelpoints);

    //ά��������������
    void RemoveCacheItemForEdit(dword fid);
    void PostCacheItemsForEdit();
    void RollbackCacheItemsForEdit();
    void UndoRedoCacheItemsForEdit();

public:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    bool __stdcall Clone(IObj** ppObj) const;

    dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const;
    dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist);

    bool __stdcall DispatchMessage(const event_identity ei, const IObj* pMessage,
        const easy_variant& tag, const char* const message_description);

public:
    DrawResult __stdcall DrawData(
        const IDisplay*         pDisplay,
        const long              cacheid,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancel*     pTrackCancel    = NULL
        );

    DrawResult __stdcall DrawSelection(
        const IDisplay*         pDisplay,
        const long              cacheid,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancel*     pTrackCancel    = NULL
        );

public:
    bool __stdcall GetExtent(WKSRect& fullext) const;
    MapUnits __stdcall GetMapUnit() const;
    bool __stdcall GetBaseScale(double& scale) const;
    void __stdcall SetName(const char* const name);
    const char* __stdcall GetName() const;
    void __stdcall SetVisible(const bool visible);
    bool __stdcall GetVisible() const;
    void __stdcall SetAlpha(const byte alpha);
    byte __stdcall GetAlpha() const;
    void __stdcall SetScaleLimit(const double maxscale, const double minscale);
    void __stdcall GetScaleLimit(double& maxscale, double& minscale) const;
    void __stdcall SetTag(const long tag);
    long __stdcall GetTag() const;

    const char* __stdcall GetSpatialReference() const;

    void __stdcall SetSelectable(const bool selectable);
    bool __stdcall GetSelectable() const;
    dword __stdcall Select(const WKSRect& envelope, const bool partialselect = true,
        const bool append = true);
    dword Deselect(const WKSPoint& point);
    dword __stdcall Deselect(const WKSRect& envelope, const bool partialselect = true);
    dword __stdcall GetSelectCount() const;
    void __stdcall ClearSelection();
    dword Select(const WKSPoint& point, const bool append = true);

    bool __stdcall SetVectorLayer(const ILayer* pLayer);
    bool __stdcall GetVectorLayer(ILayer** ppLayer) const;
    bool __stdcall SetFieldIndex(const long fieldindex);
    long __stdcall GetFieldIndex();
    bool __stdcall SetTextSymbol(const ITextSymbol* pTextSymbol);
    bool __stdcall GetTextSymbol(ITextSymbol** ppTextSymbol) const;
    bool __stdcall SetRefScale(const double scale);
    double __stdcall GetRefScale() const;
    DrawResult __stdcall GetLabelText(IDoubleArray** ppTextPositions,
        IStringArray** ppLabelTexts, const IDisplayTransformation* pDT,
        const ITrackCancel* pTrackCancel, const WKSRect& visibleextent);

    void __stdcall ClearCache();

friend class CGeoMap;
};
//================================================================================

CLASS_FACTORY(CLabelLayer)

}

#endif
