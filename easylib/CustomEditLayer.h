#if !defined(CUSTOMEDITLAYER_INCLUDED_)
#define CUSTOMEDITLAYER_INCLUDED_

#include "CommonInclude.h"
#include "..\\include\\InterfaceLayerAgent.h"
#include "Display.h"

namespace easymap
{

class CCustomEditLayer;
typedef TSmartPtr<CCustomEditLayer> CCustomEditLayerPtr;

//================================================================================
//
//================================================================================
class CCustomEditLayer : public ILayer, public IEditLayer
{
protected:
    CCustomEditLayer();
    virtual ~CCustomEditLayer();

protected:
    double              m_MaxVisualScale;       //�����ʾ������
    double              m_MinVisualScale;       //��С��ʾ������
    long                m_Tag;                  //����
    bool                m_Visible;              //�Ƿ����
    byte                m_Alpha;                //͸����
    bool                m_Selectable;           //�Ƿ����ѡ�����
    string              m_Name;                 //ͼ������
    bool                m_MapEditable;          //�Ƿ����Map��ͳһ�༭����

    //  �˶����������PreloadInstance()��PresaveInstance()
    dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const;
    dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist);

private:

    //  �������д�˶�������ʵ�־���Ĵ洢
    virtual dword PresaveInstance(CStreamPtr pStream, void* const assist) const = 0;
    virtual dword PreloadInstance(CStreamPtr pStream, void* const assist) = 0;

    //  �������д�˺��������ƾ���Layer��Data��cache
    virtual DrawResult DrawLayerData(
        const CDisplayCachePtr  pDisplayCache,
        const long              cacheid,
        const WKSRect* const    pEnvelope,
        const ITrackCancelPtr   pTrackCancel
        ) = 0;

    //  �������д�˺��������ƾ���Layer��ѡ��
    virtual DrawResult DrawLayerSelection(
        const CDisplayCachePtr  pDisplayCache,
        const long              cacheid,
        const WKSRect* const    pEnvelope,
        const ITrackCancelPtr   pTrackCancel
        ) = 0;

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

    virtual DrawResult DrawData(
        const CDisplayPtr       pDisplay,
        const long              cacheid,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancelPtr   pTrackCancel    = NULL
        );

    virtual DrawResult DrawData1(
        const CDisplayCachePtr  pDisplayCache,
        const long              cacheid,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancelPtr   pTrackCancel    = NULL
        );

    virtual DrawResult DrawSelection(
        const CDisplayCachePtr  pDisplayCache,
        const long              cacheid,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancelPtr   pTrackCancel    = NULL
        );

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
    void __stdcall SetSelectable(const bool selectable);
    bool __stdcall GetSelectable() const;

    //���ű���д
    bool __stdcall SetUndoPoint() {return false;};
    bool __stdcall EditUndoable() const {return false;};
    bool __stdcall EditRedoable() const {return false;};
    bool __stdcall EditUndo() {return false;};
    bool __stdcall EditRedo() {return false;};
    bool __stdcall EditCancel() {return false;};
    bool __stdcall SaveData() {return false;};
    bool __stdcall IsDirty() const {return false;};

    bool __stdcall SetMapEditable(const bool mapeditable);
    bool __stdcall GetMapEditable() const;

    bool __stdcall Clone(IObj** ppObj) const;
};
//================================================================================

}

#endif
