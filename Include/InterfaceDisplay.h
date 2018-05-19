#if !defined(INTERFACEDISPLAY_INCLUDED_)
#define INTERFACEDISPLAY_INCLUDED_

#include "InterfaceDisplayTransformation.h"
#include "InterfaceGeometry.h"
#include "InterfaceStream.h"

namespace easymap
{

class IDisplay;
class IDisplayCache;
class IScreenBrowser;
class ISymbol;
class IPointSymbol;
class ILineSymbol;
class IFillSymbol;
class ITextSymbol;
typedef TSmartPtr<IDisplay> IDisplayPtr;
typedef TSmartPtr<IDisplayCache> IDisplayCachePtr;
typedef TSmartPtr<IScreenBrowser> IScreenBrowserPtr;
typedef TSmartPtr<ISymbol> ISymbolPtr;
typedef TSmartPtr<IPointSymbol> IPointSymbolPtr;
typedef TSmartPtr<ILineSymbol> ILineSymbolPtr;
typedef TSmartPtr<IFillSymbol> IFillSymbolPtr;
typedef TSmartPtr<ITextSymbol> ITextSymbolPtr;


//��������
typedef long SymbolType;
const SymbolType SYMBOLTYPE_POINT   = 1;
const SymbolType SYMBOLTYPE_LINE    = 2;
const SymbolType SYMBOLTYPE_FILL    = 3;
const SymbolType SYMBOLTYPE_TEXT    = 4;

//----------------------------------------
//layer::draw()�ķ���ֵ
typedef long DrawResult;
const DrawResult LAYERDRAW_NORMAL           = 0;
const DrawResult LAYERDRAW_NOVISIBLE        = 1;
const DrawResult LAYERDRAW_TRACKCANCEL      = 2;
const DrawResult LAYERDRAW_EXCEEDLIMIT      = 3;
const DrawResult LAYERDRAW_NOREADY          = 4;
const DrawResult LAYERDRAW_DISPLAYNOREADY   = 5;
const DrawResult LAYERDRAW_NOCACHE          = 6;
const DrawResult LAYERDRAW_NOSUPPORT        = 7;
const DrawResult LAYERDRAW_NOENOUGHMEM      = 8;
const DrawResult LAYERDRAW_UNEXPECTED       = 9;
//----------------------------------------

class ISymbol;

//================================================================================
//  ������װ���Ʋ�������displaytransformation��dc�ȣ�
//  ��һ���豸�ṩͳһ�Ļ��ƹ���ƽ̨
//  ���ṩ���Ƽ��ζ�����ע
//================================================================================
class IDisplay : public IPersist
{
public:
    //����任
    virtual bool __stdcall SetDisplayTransformation(const IDisplayTransformation* pTrans) = 0;
    virtual bool __stdcall GetDisplayTransformation(IDisplayTransformation** ppTrans) const = 0;

    //DC
    virtual bool __stdcall SetDC(const HDC dc) = 0;
    virtual bool __stdcall GetDC(HDC& dc) const = 0;

    //�����С
    virtual bool __stdcall SetRect(const RECT& rect) = 0;
    virtual bool __stdcall GetRect(RECT& rect) const = 0;

    //����ɫ
    virtual bool __stdcall SetBackgroundColor(const COLORREF color) = 0;
    virtual COLORREF __stdcall GetBackgroundColor() const = 0;

    //  StartSraw��FinishDraw����ͳһ����
    //  ĳһ��Display�ϵ����л��Ʋ���
    virtual bool __stdcall StartDraw() = 0;
    virtual bool __stdcall FinishDraw() = 0;
    virtual bool __stdcall IsDrawing() const = 0;

    //  ���ڻ��Ƶĵ㡢�ߡ������
    virtual bool __stdcall SetSymbol(const ISymbol* pSymbol) = 0;
    virtual bool __stdcall GetSymbol(const SymbolType symboltype, ISymbol** ppSymbol) const = 0;

    //  ���Ƽ��ζ��󣬿��Ի���Geometry����Ҳ����
    //  ֱ�ӻ���Stream�����ڼӿ��ٶȲ�������
    virtual bool __stdcall DrawGeometry(const IGeometry* pGeometry) const = 0;
    virtual dword __stdcall DrawStream(IStreamX* pStream) const = 0;

    //  ���Ʊ�ע���ı���
    virtual bool __stdcall DrawText(const IGeometry* pGeometry,
        const char* const pcText, RECT& textenvelope) const = 0;
    virtual bool __stdcall DrawTextXY(const double x, const double y,
        const char* const pcText, RECT& textenvelope) const = 0;

    virtual void __stdcall EraseContent(const WKSRect* const pEnvelope = NULL) const = 0;

    //��Display�еķɻ�����destdc�ϣ������Ե��������豸���ڳߴ�
    virtual bool __stdcall RefreshWindow(const HDC destdc, const RECT& destrect,
        dword rop = SRCCOPY) const = 0;

    //
    virtual bool __stdcall RefreshWindow1() const = 0;
};
//================================================================================


//================================================================================
//  ����һ���ڴ�DC����������֧��ѡ�񼯵Ļ��Ƶ�
//================================================================================
class IDisplayCache : public IPersist
{
public:
    //����һ���ڴ�dc��ָ��͸��������
    virtual long __stdcall CreateCache(const COLORREF bgcolor = RGB(255, 255, 255),
        const BYTE alpha = 255, const bool transparent = false,
        const DWORD rop = SRCCOPY) = 0;

    //���е��ڴ�dc����
    virtual long __stdcall GetCacheCount() const = 0;

    //ȡ����index��dc��id��Ȼ����Ը������idɾ����GetCacheDC
    virtual bool __stdcall GetCacheID(const long index, long& cacheid) const = 0;

    //�ɵ�
    virtual bool __stdcall DeleteCache(const long cacheid) = 0;
    virtual void __stdcall ClearAllCaches() = 0;

    //����cache size
    virtual bool __stdcall SetLockCacheSize(const long cacheid, const bool lock) = 0;
    virtual bool __stdcall GetLockCacheSize(const long cacheid, bool& lock) = 0;
    virtual bool __stdcall SetCacheSize(const long cacheid, const dword width, const dword height) = 0;

    //�ҵ�id��dc
    virtual bool __stdcall GetCacheDC(const long cacheid, HDC& dc) const = 0;

    //���ڴ�dc���Ƶ������dc��
    virtual bool __stdcall PostCache(const HDC dc, const long cacheid) const = 0;
    virtual bool __stdcall PostCache1(const HDC dc, const long cacheid, const long delta_x,
        const long delta_y) const = 0;
    virtual bool __stdcall PostCache2(const HDC dc, const long cacheid, const RECT& partial) const = 0;

    //���ڴ�dc���Ƶ�primary buffer��
    virtual bool __stdcall PostCacheToPrimary(const long cacheid) const = 0;
    virtual bool __stdcall PostCacheToPrimary1(const long cacheid, const long delta_x,
        const long delta_y) const = 0;
    virtual bool __stdcall PostCacheToPrimary2(const long cacheid, const RECT& partial) const = 0;

    //�����ڴ�dc�ı���ɫ��symbol
    virtual bool __stdcall SetCacheBGColor(const long cacheid, const COLORREF color) = 0;
    virtual bool __stdcall GetCacheBGColor(const long cacheid, COLORREF& color) const = 0;
    virtual bool __stdcall SetCacheSymbol(const long cacheid, const ISymbol* pSymbol) = 0;
    virtual bool __stdcall GetCacheSymbol(const long cacheid, const SymbolType symboltype, ISymbol** ppSymbol) const = 0;

    //����geometry���ڴ�dc��
    virtual bool __stdcall DrawCacheGeometry(const long cacheid, const IGeometry* pGeometry) const = 0;
    virtual dword __stdcall DrawCacheStream(const long cacheid, IStreamX* pStream) const = 0;
    virtual bool __stdcall DrawCacheText(const long cacheid, const IGeometry* pGeometry,
        const char* const pcText, RECT& textenvelope) const = 0;
    virtual bool __stdcall DrawCacheTextXY(const long cacheid, const double x,
        const double y, const char* const pcText, RECT& textenvelope) const = 0;

    //��primary buffer���Ƶ��ڴ�dc��
    virtual bool __stdcall CopyPrimaryToCache(const long cacheid, const long delta_x,
        const long delta_y) = 0;
    virtual bool __stdcall CopyPrimaryToCache1(const long cacheid, const RECT& partial) = 0;

    //��������һ��
    virtual bool __stdcall CopyCacheToCache(const long fromid, const long toid,
        const long delta_x, const long delta_y) = 0;
    virtual bool __stdcall CopyCacheToCache1(const long fromid, const long toid, const RECT& partial) = 0;

    //���ڴ�dc���Ϊ����ɫ
    virtual bool __stdcall EraseCacheContent(const long cacheid, const WKSRect* const pEnvelope = NULL) const = 0;
    virtual void __stdcall EraseCachesContent(const WKSRect* const pEnvelope = NULL) const = 0;

    //��ָ����cache����window dc��
    virtual bool __stdcall PostCacheToWindow(const long cacheid) const = 0;
};
//================================================================================


//================================================================================
//  ��Ҫ���ڸ㶨һ������Ļ������ܣ�ע����Щ�����������ڻ�����Ϻ����
//  ������Щ����ִ�����֮�����CDisplay::RefreshWindow()��ˢ�´���
//================================================================================
class IScreenBrowser : public IPersist
{
public:
    //  ��Ļ���Σ�����
    virtual bool __stdcall PanStart(const POINT& screenpoint) = 0;
    virtual bool __stdcall PanMoveTo(const POINT& screenpoint) = 0;
    virtual bool __stdcall PanStop() = 0;
    virtual bool __stdcall Paning() = 0;

    //  �ı���ʾ�����ߣ��������ķ���
    virtual bool __stdcall MapScaleAt(const double mapscale) = 0;

    //  ������Ļ���ĵ�
    virtual bool __stdcall MapCenterAt(const POINT& wndpnt) = 0;
    virtual bool __stdcall MapCenterAt1(const WKSPoint& center) = 0;

    //  ���η���
    virtual bool __stdcall VisibleExtentIn(const RECT& RECT) = 0;
    virtual bool __stdcall VisibleExtentOut(const RECT& RECT) = 0;
    virtual bool __stdcall VisibleMapExtentIn(const WKSRect& extent) = 0;
    virtual bool __stdcall VisibleMapExtentOut(const WKSRect& extent) = 0;
};
//================================================================================


//================================================================================
//  ����
//================================================================================
class ISymbol : public IPersist
{
public:
    virtual bool __stdcall SetCode(const long& code) = 0;
    virtual long __stdcall GetCode() const = 0;
    virtual bool __stdcall SetName(const char* const name) = 0;
    virtual const char* __stdcall GetName() const = 0;
    virtual bool __stdcall GetDC(HDC& dc) const = 0;
    virtual bool __stdcall GetDisplayTransformation(IDisplayTransformation** ppTrans) const = 0;
    virtual bool __stdcall GetROP2(long& rop2) const = 0;

    virtual bool __stdcall Prepare(const HDC dc, const IDisplayTransformation* pTrans,
        const long rop2 = R2_COPYPEN) = 0;
    virtual bool __stdcall Draw(const IGeometry* pGeometry) const = 0;
    virtual dword __stdcall DrawStream(const IStreamX* pStream) const = 0;
    virtual bool __stdcall SetColor(const COLORREF color) = 0;
    virtual bool __stdcall GetColor(COLORREF& color) const = 0;
    virtual void __stdcall SetColorLock(const bool lock) = 0;
    virtual void __stdcall GetColorLock(bool& lock) const = 0;
    virtual SymbolType __stdcall GetSymbolType() const = 0;
};
//================================================================================


//================================================================================
//  �����
//================================================================================
class IPointSymbol : public ISymbol
{
public:
    virtual bool __stdcall SetAngle(const double angle) = 0;
    virtual bool __stdcall GetAngle(double& angle) const = 0;
    virtual bool __stdcall SetOffset(const double x, const double y) = 0;
    virtual bool __stdcall GetOffset(double& x, double& y) const = 0;
    virtual bool __stdcall _ParentOffset(const WKSPoint& offset,
        const double angle, const double size) = 0;
};
//================================================================================


//================================================================================
//  �߷���
//================================================================================
class ILineSymbol : public ISymbol
{
public:
    virtual bool __stdcall SetOffset(const double offset) = 0;
    virtual bool __stdcall GetOffset(double& offset) const = 0;
    virtual bool __stdcall _ParentOffset(const double offset, const double size) = 0;
};
//================================================================================


//================================================================================
//  �����
//================================================================================
class IFillSymbol : public ISymbol
{
};
//================================================================================


//================================================================================
//  ע�Ƿ���
//================================================================================
class ITextSymbol : public ISymbol
{
public:
    virtual bool __stdcall Draw(const IGeometry* pGeometry, RECT& textenvelope) const = 0;
    virtual bool __stdcall SetFont(const LOGFONT& font) = 0;
    virtual bool __stdcall GetFont(LOGFONT& font) const = 0;
    virtual bool __stdcall SetWidth(const double width) = 0;
    virtual bool __stdcall GetWidth(double& width) const = 0;
    virtual bool __stdcall SetHeight(const double height) = 0;
    virtual bool __stdcall GetHeight(double& height) const = 0;
    virtual bool __stdcall SetAngle(const double angle) = 0;
    virtual bool __stdcall GetAngle(double& angle) const = 0;
    virtual bool __stdcall SetText(const char* const text) = 0;
    virtual const char* __stdcall GetText() const = 0;
};
//================================================================================

}

#endif