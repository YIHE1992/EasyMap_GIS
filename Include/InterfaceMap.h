#if !defined(INTERFACEMAP_INCLUDED_)
#define INTERFACEMAP_INCLUDED_

#include "InterfaceLabelLayer.h"
#include "InterfaceDisplay.h"

namespace easymap
{

typedef long BookMarkStyle;
const BookMarkStyle BOOKMARKSTYLE_RECT          = 0;
const BookMarkStyle BOOKMARKSTYLE_ROUNDRECT     = 1;
const BookMarkStyle BOOKMARKSTYLE_ELLIPSE       = 2;

typedef long BookMarkVisible;
const BookMarkVisible BOOKMARKVISIBLE_NONE      = 0;
const BookMarkVisible BOOKMARKVISIBLE_ALL       = 1;
const BookMarkVisible BOOKMARKVISIBLE_CURRENT   = 2;

class IMap;
typedef TSmartPtr<IMap> IMapPtr;
class IRapidDraw;
typedef TSmartPtr<IRapidDraw> IRapidDrawPtr;
class IPlaceBookmark;
typedef TSmartPtr<IPlaceBookmark> IPlaceBookmarkPtr;

//����һ��ͼ�㣬��������ߡ���ʾ��Χ
//����ͳһ��������ͼ��ı༭�ع�����
class IMap : public IPersist
{
public:
    //����һ��ͼ�㵽GeoMap��
    virtual bool __stdcall AddLayer(const ILayer* pLayer) = 0;

    //ж��ĳ��ͼ��
    virtual bool __stdcall DeleteLayer(const dword index) = 0;

    //ж��ĳ��ͼ�㣬������grouplayer�µ�ͼ��
    virtual bool __stdcall DeleteLayerEx(ILayer* pLayer) = 0;

    //�õ�ĳһ��ͼ��
    virtual bool __stdcall GetLayer(ILayer** ppLayer, const dword index) const = 0;

    //����ͼ��˳��
    virtual bool __stdcall SetLayerOrder(const ILayer* pLayer, const dword neworder) = 0;

    //ж�����е�ͼ��
    virtual void __stdcall ClearLayers() = 0;

    //�Ѿ����ص�ͼ��������ע�ⲻ����GroupLayer�������ͼ��
    virtual dword __stdcall GetLayerCount() const = 0;

    //ȫ����ͼ������������GroupLayer�������ͼ��
    virtual dword __stdcall GetAllCount() const = 0;

    //ͨ��ͼ�����Ʋ���ĳһ��ͼ�㣬����grouplayer֮�µ���ͼ�㣬ע��ֻ�������������ĵ�һ��ͼ��
    //classtype����ΪNULL��ָ��ͼ������
    virtual bool __stdcall FindLayer(ILayer** ppLayer, const char* const layername,
        const char* const classtype) const = 0;

    //ж�����еķɻ�
    virtual void __stdcall ClearAllData() = 0;

    virtual bool __stdcall GetReferenceScale(double& refscale) const = 0;
    virtual const char* __stdcall GetSpatialReference() const = 0;

    virtual bool __stdcall GetMapScale(double& scale) const = 0;
    virtual bool __stdcall GetVisibleExtent(WKSRect& extent) const = 0;

    //ȡ�õ�ǰ�����ص����ݵ����Χ
    virtual bool __stdcall GetFullExtent(WKSRect& fullext) const = 0;

    virtual bool __stdcall SetSelectSymbol(const ISymbol* pSymbol) = 0;
    virtual bool __stdcall GetSelectSymbol(const SymbolType symboltype,
        ISymbol** ppSymbol) const = 0;

    //����Щ
    virtual dword __stdcall SelectByPoint(const WKSPoint& point, const bool append) = 0;
    virtual dword __stdcall Select(const WKSRect& envelope, const bool partialselect,
        const bool append) = 0;
    virtual dword __stdcall DeselectByPoint(const WKSPoint& point) = 0;
    virtual dword __stdcall Deselect(const WKSRect& envelope, const bool partialselect) = 0;
    virtual void __stdcall ClearSelection() = 0;

    virtual void __stdcall SetName(const char* const mapname) = 0;
    virtual const char* __stdcall GetName() const = 0;

    //ͼ���б��ͳһ�༭����
    virtual bool __stdcall SetUndoPoint(const char* const desc) = 0;
    virtual bool __stdcall EditUndoable() const = 0;
    virtual bool __stdcall EditRedoable() const = 0;
    virtual bool __stdcall EditUndo() = 0;
    virtual bool __stdcall EditRedo() = 0;
    virtual bool __stdcall EditCancel() = 0;
    virtual bool __stdcall SaveData() = 0;
    virtual bool __stdcall IsDirty() const = 0;
};


//����һ��ͼ�㣬���п�����Ļ����
//������GPS�źŵ�ʵʱ������ʾ
class IRapidDraw : public IPersist
{
public:
    //����һ��ͼ�㣬�������ػ��ʱ���draw
    virtual bool __stdcall RD_AddLayer(const ILayer* pLayer) = 0;
    virtual bool __stdcall RD_RemoveLayer(const dword index) = 0;
    virtual bool __stdcall RD_RemoveLayerEx(ILayer* pLayer) = 0;
    virtual bool __stdcall RD_GetLayer(ILayer** ppLayer, const dword index) const = 0;
    virtual bool __stdcall RD_SetLayerOrder(const ILayer* pLayer, const dword neworder) = 0;
    virtual void __stdcall RD_ClearLayers() = 0;
    virtual dword __stdcall RD_GetLayerCount() const = 0;

    //�Ƿ�ʵʱ����
    virtual void __stdcall EnableRapidDraw(bool Enable) = 0;
    virtual bool __stdcall RapidDrawEnabled() const = 0;
};


//λ����ǩ����
class IPlaceBookmark : public IPersist
{
public:
    //������bookmark����ǰ��ת������Ϊbookmark��
    //����bookmark id��Ψһ��ʶ
    //����<0����ʧ��
    virtual long __stdcall AddBookmark(const char* const text) = 0;

    //ָ����Χ
    //����bookmark id��Ψһ��ʶ
    //����<0����ʧ��
    virtual long __stdcall AddBookmarkEx(const WKSRect& extent, const char* const text) = 0;

    //�޸�bookmark
    virtual bool __stdcall ModifyBookmark(const long id, const WKSRect& extent,
        const char* const text) = 0;

    //ͨ��id�õ�bookmark
    virtual bool __stdcall GetBookmarkByID(const long id, WKSRect& extent,
        IAnsiString** ppText) const = 0;

    //index <--> bookmark id
    //����<0����ʧ��
    virtual long __stdcall GetBookmarkIDByIndex(const long index) const = 0;
    virtual long __stdcall GetBookmarkIndexByID(const long id) const = 0;

    //�õ���ǰbookmark
    virtual long __stdcall GetCurrentBookmarkID() const = 0;
    virtual bool __stdcall GetCurrentBookmark(WKSRect& extent, IAnsiString** ppText) const = 0;

    //����ʾ��Χ���óɵ�ǰ��bookmark��ע������ת����
    virtual bool __stdcall SetViewToCurrentBookmark() = 0;

    //���õ�ǰbookmarkΪ��һ��bookmark
    //����bookmark id < 0 ��ʾ�Ѿ���ͷ
    virtual long __stdcall NextBookmark() = 0;

    //���õ�ǰbookmarkΪ��һ��bookmark
    //����bookmark id < 0 ��ʾ�Ѿ���ͷ
    virtual long __stdcall PreviousBookmark() = 0;

    //ɾ��ָ��id��bookmark
    virtual bool __stdcall DeleteBookmark(const long id) = 0;

    //ɾ������bookmark
    virtual void __stdcall ClearBookmarks() = 0;

    //����
    virtual dword __stdcall GetBookmarkCount() const = 0;

    //�Ƿ���ʾbookmark
    virtual void __stdcall SetBookmarksVisible(const BookMarkVisible visible) = 0;
    virtual BookMarkVisible __stdcall GetBookmarksVisible() const = 0;

    //���塢��״�͵�ɫ
    virtual bool __stdcall SetBookmarkSymbol(const ITextSymbol* pTextSymbol) = 0;
    virtual bool __stdcall GetBookmarkSymbol(ITextSymbol** ppTextSymbol) const = 0;
    virtual void __stdcall SetBookmarkStyle(const BookMarkStyle style) = 0;
    virtual BookMarkStyle __stdcall GetBookmarkStyle() const = 0;
    virtual void __stdcall SetBookmarkColor(const COLORREF noactive, const COLORREF active) = 0;
    virtual void __stdcall GetBookmarkColor(COLORREF& noactive, COLORREF& active) const = 0;

    //��ͻ����ʾ��ǰbookmark
    virtual void __stdcall DisableActiveBookmarkShow() = 0;

    //�洢
    virtual dword __stdcall SaveBookmarksTo(IStreamX* pStream) = 0;
    virtual dword __stdcall LoadBookmarksFrom(IStreamX* pStream) = 0;
};

}

#endif