#if !defined(SCREENDISPLAY_INCLUDED_)
#define SCREENDISPLAY_INCLUDED_

#include "MiniDisplay.h"
#include "ExtentAPI.h"

namespace easymap
{

typedef TSmartPtr<CScreenDisplay> CScreenDisplayPtr;
//================================================================================
//  ����˫�����Display����ʵ��CDisplayCache�����Թ���һ�ѻ�����
//  ���⻹�㶨����Ļ���ι���
//================================================================================
class CScreenDisplay :  public CDisplay,
                        public CDisplayCache,
                        public CScreenBrowser
{
private:
    //�ڲ�ʹ��
    struct CCacheItem : public IObj
    {
    CLASS_NAME(CCacheItem)
    NO_EVENTS_DISPATCHER
    NO_EVENTS_LISTENER

        HDC             wnddc;
        HDC             dc;
        HBITMAP         bitmapsaved;
        COLORREF        bgcolor;
        DWORD           rop;
        bool            transparent;
        BYTE            alpha;
        IPointSymbolPtr pPointSymbol;
        ILineSymbolPtr  pLineSymbol;
        IFillSymbolPtr  pFillSymbol;
        ITextSymbolPtr  pTextSymbol;
        bool            lock;

        CCacheItem();
        ~CCacheItem();
        bool __stdcall GotoInterface(const char* const interfacename, void** pp);
        bool __stdcall Clone(IObj** ppObj) const;
        void SetupDC(const HDC compatibledc);
        void Resize(const RECT& rect);
        void SaveToStream(CStreamPtr pStream, void* const assist) const;
        void LoadFromStream(CStreamPtr pStream, void* const assist);
    };
    typedef TSmartPtr<CCacheItem> CCacheItemPtr;

CLASS_NAME(CScreenDisplay)
PERSIST_DUMP(CScreenDisplay)
NO_EVENTS_DISPATCHER
NO_EVENTS_LISTENER

public:
    CScreenDisplay();
private:
    ~CScreenDisplay();

private:
    //��һ��CMiniDisplay�㶨�����Ļ��ƣ�ע�⽫����dc��Ϊ���󻺳�����ͼ��ʵ�����ǻ��Ƶ����dc�ϣ�
    //Ȼ���ٰ����dc��������m_WindowDC��
    CMiniDisplayPtr m_pBufferDisplay;

    //�Լ��� dc
    HDC m_WindowDC;

    //���ռ�������m_WindowDC��ȥ�Ķ��������������˸����
    HDC m_CacheDC;

    //���dc������ֻ�������ڵ�ͼ���Σ�pan��������ʱ���������
    HDC m_BlankDC;

    //�Ƿ���ҪDeleteDC
    bool m_DCOK;

    HBITMAP bitmapsaved_bufferdc, bitmapsaved_cachedc, bitmapsaved_blankdc;

    //����ʵ��DisplayCache
    map<long, CCacheItemPtr> m_Caches;
    long m_NextCache;

    //pan�ķɻ�
    bool m_PanStarted;
    POINT m_PanStartPnt;
    POINT m_PanToPnt;

private:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    bool __stdcall Clone(IObj** ppObj) const;

    dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const;
    dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist);

    void ReleaseBuffer();
    void GetDeviceRect(RECT& rect) const;

    //--ʵ��CDisplay-------------
    bool __stdcall SetDisplayTransformation(const IDisplayTransformation* pTrans);
    bool __stdcall GetDisplayTransformation(IDisplayTransformation** ppTrans) const;
    bool SetDisplayTransformation(const CDisplayTransformationPtr pTrans);
    bool GetDisplayTransformation(CDisplayTransformationPtr& pTrans) const;
    bool __stdcall SetDC(const HDC dc);
    bool __stdcall GetDC(HDC& dc) const;
    bool __stdcall SetRect(const RECT& rect);
    bool __stdcall GetRect(RECT& rect) const;
    bool __stdcall SetBackgroundColor(const COLORREF color);
    COLORREF __stdcall GetBackgroundColor() const;
    bool __stdcall StartDraw();
    bool __stdcall FinishDraw();
    bool __stdcall IsDrawing() const;
    bool __stdcall SetSymbol(const ISymbol* pSymbol);
    bool __stdcall GetSymbol(const SymbolType symboltype, ISymbol** ppSymbol) const;
    bool SetSymbol(const ISymbolPtr pSymbol);
    bool GetSymbol(const SymbolType symboltype, ISymbolPtr& pSymbol) const;
    bool __stdcall DrawGeometry(const IGeometry* pGeometry) const;
    bool DrawGeometry(const IGeometryPtr pGeometry) const;
    dword __stdcall DrawStream(IStreamX* pStream) const;
    dword DrawStream(CStreamPtr pStream) const;
    bool __stdcall DrawText(const IGeometry* pGeometry,
        const char* const pcText, RECT& textenvelope) const;
    bool DrawText(const IGeometryPtr pGeometry,
        const char* const pcText, RECT& textenvelope) const;
    bool __stdcall DrawTextXY(const double x, const double y,
        const char* const pcText, RECT& textenvelope) const;
    void __stdcall EraseContent(const WKSRect* const pEnvelope = NULL) const;
    bool __stdcall RefreshWindow(const HDC destdc, const RECT& destrect,
        dword rop = SRCCOPY) const;
    bool __stdcall RefreshWindow1() const;

    //--ʵ��CDisplayCache-------
    long __stdcall CreateCache(
        const COLORREF bgcolor  = RGB(255, 255, 255),
        const BYTE alpha        = 255,
        const bool transparent  = false,
        const DWORD rop         = SRCCOPY
        );
    long __stdcall GetCacheCount() const;
    bool __stdcall GetCacheID(const long index, long& cacheid) const;
    bool __stdcall DeleteCache(const long cacheid);
    void __stdcall ClearAllCaches();
    bool __stdcall SetLockCacheSize(const long cacheid, const bool lock);
    bool __stdcall GetLockCacheSize(const long cacheid, bool& lock);
    bool __stdcall SetCacheSize(const long cacheid, const dword width, const dword height);
    bool __stdcall GetCacheDC(const long cacheid, HDC& dc) const;
    bool __stdcall PostCache(const HDC dc, const long cacheid) const;
    bool __stdcall PostCache1(const HDC dc, const long cacheid, const long delta_x,
        const long delta_y) const;
    bool __stdcall PostCache2(const HDC dc, const long cacheid, const RECT& partial) const;
    bool __stdcall PostCacheToPrimary(const long cacheid) const;
    bool __stdcall PostCacheToPrimary1(const long cacheid, const long delta_x,
        const long delta_y) const;
    bool __stdcall PostCacheToPrimary2(const long cacheid, const RECT& partial) const;
    bool __stdcall SetCacheBGColor(const long cacheid, const COLORREF color);
    bool __stdcall GetCacheBGColor(const long cacheid, COLORREF& color) const;
    bool __stdcall SetCacheSymbol(const long cacheid, const ISymbol* pSymbol);
    bool __stdcall GetCacheSymbol(const long cacheid, const SymbolType symboltype, ISymbol** ppSymbol) const;
    bool SetCacheSymbol(const long cacheid, const ISymbolPtr pSymbol);
    bool GetCacheSymbol(const long cacheid, const SymbolType symboltype, ISymbolPtr& pSymbol) const;
    bool __stdcall DrawCacheGeometry(const long cacheid, const IGeometry* pGeometry) const;
    bool DrawCacheGeometry(const long cacheid, const IGeometryPtr pGeometry) const;
    dword __stdcall DrawCacheStream(const long cacheid, IStreamX* pStream) const;
    dword DrawCacheStream(const long cacheid, CStreamPtr pStream) const;
    bool __stdcall DrawCacheText(const long cacheid, const IGeometry* pGeometry,
        const char* const pcText, RECT& textenvelope) const;
    bool DrawCacheText(const long cacheid, const IGeometryPtr pGeometry,
        const char* const pcText, RECT& textenvelope) const;
    bool __stdcall DrawCacheTextXY(const long cacheid, const double x, const double y,
        const char* const pcText, RECT& textenvelope) const;
    bool __stdcall CopyPrimaryToCache(const long cacheid, const long delta_x,
        const long delta_y);
    bool __stdcall CopyPrimaryToCache1(const long cacheid, const RECT& partial);
    bool __stdcall CopyCacheToCache(const long fromid, const long toid,
        const long delta_x, const long delta_y);
    bool __stdcall CopyCacheToCache1(const long fromid, const long toid, const RECT& partial);
    bool __stdcall EraseCacheContent(const long cacheid, const WKSRect* const pEnvelope = NULL) const;
    void __stdcall EraseCachesContent(const WKSRect* const pEnvelope = NULL) const;
    bool __stdcall PostCacheToWindow(const long cacheid) const;

    //--ʵ��CScreenBrowser-------
    bool __stdcall PanStart(const POINT& screenpoint);
    bool __stdcall PanMoveTo(const POINT& screenpoint);
    bool __stdcall PanStop();
    bool __stdcall Paning();

    bool __stdcall MapScaleAt(const double mapscale);
    bool __stdcall MapCenterAt(const POINT& wndpnt);
    bool __stdcall MapCenterAt1(const WKSPoint& center);
    bool __stdcall VisibleExtentIn(const RECT& RECT);
    bool __stdcall VisibleExtentOut(const RECT& RECT);
    bool __stdcall VisibleMapExtentIn(const WKSRect& extent);
    bool __stdcall VisibleMapExtentOut(const WKSRect& extent);

public:
    bool GetBufferDC(HDC& dc) const;
};
//================================================================================

CLASS_FACTORY(CScreenDisplay)

}

#endif