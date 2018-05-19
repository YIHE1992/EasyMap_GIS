#if !defined(DISPLAY_INCLUDED_)
#define DISPLAY_INCLUDED_

#include "..\\include\\InterfaceDisplay.h"
#include "DisplayTransformation.h"
#include "Geometry.h"

namespace easymap
{

class CDisplay;
class CDisplayCache;
class CScreenBrowser;

typedef TSmartPtr<CDisplay> CDisplayPtr;
typedef TSmartPtr<CDisplayCache> CDisplayCachePtr;
typedef TSmartPtr<CScreenBrowser> CScreenBrowserPtr;
typedef TSmartPtr<ISymbol> ISymbolPtr;
typedef TSmartPtr<IPointSymbol> IPointSymbolPtr;
typedef TSmartPtr<ILineSymbol> ILineSymbolPtr;
typedef TSmartPtr<IFillSymbol> IFillSymbolPtr;
typedef TSmartPtr<ITextSymbol> ITextSymbolPtr;

class CDisplay : public IDisplay
{
public:
    //����任
    virtual bool SetDisplayTransformation(const CDisplayTransformationPtr pTrans) = 0;
    virtual bool GetDisplayTransformation(CDisplayTransformationPtr& pTrans) const = 0;

    //  ���ڻ��Ƶĵ㡢�ߡ������
    virtual bool SetSymbol(const ISymbolPtr pSymbol) = 0;
    virtual bool GetSymbol(const SymbolType symboltype, ISymbolPtr& pSymbol) const = 0;

    //  ���Ƽ��ζ��󣬿��Ի���Geometry����Ҳ����
    //  ֱ�ӻ���Stream��Block�����ڼӿ��ٶȲ�������
    virtual bool DrawGeometry(const IGeometryPtr pGeometry) const = 0;
    virtual dword DrawStream(CStreamPtr pStream) const = 0;

    //  ���Ʊ�ע���ı���
    virtual bool DrawText(const IGeometryPtr pGeometry,
        const char* const pcText, RECT& textenvelope) const = 0;
};


class CDisplayCache : public IDisplayCache
{
public:
    virtual bool SetCacheSymbol(const long cacheid, const ISymbolPtr pSymbol) = 0;
    virtual bool GetCacheSymbol(const long cacheid, const SymbolType symboltype,
        ISymbolPtr& pSymbol) const = 0;

    //����geometry���ڴ�dc��
    virtual bool DrawCacheGeometry(const long cacheid, const IGeometryPtr pGeometry) const = 0;
    virtual dword DrawCacheStream(const long cacheid, CStreamPtr pStream) const = 0;
    virtual bool DrawCacheText(const long cacheid, const IGeometryPtr pGeometry,
        const char* const pcText, RECT& textenvelope) const = 0;
};


class CScreenBrowser : public IScreenBrowser
{
};

}

#endif