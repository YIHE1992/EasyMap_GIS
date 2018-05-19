#if !defined(INTERFACEACTIVEVIEW_INCLUDED_)
#define INTERFACEACTIVEVIEW_INCLUDED_

#include "InterfaceDisplay.h"
#include "InterfaceTrackCancel.h"

namespace easymap
{

class IActiveView;
typedef TSmartPtr<IActiveView> IActiveViewPtr;

//IActiveView
class IActiveView : public IPersist
{
public:
    //Display�ǿ��Ա��滻��
    virtual bool __stdcall SetDisplay(IDisplay* pDisplay) = 0;
    virtual bool __stdcall GetDisplay(IDisplay** ppDisplay) const = 0;

    //���봰������������Ļ���ꡢʵ�����꣨�����ߣ�֮��Ķ�Ӧ��ϵ��
    //ͨ���ڴ����С�ı䣨OnSize����ʱ����ã����л��Ʋ���֮ǰ�����ȵ����������
    virtual bool __stdcall GainFocus(const HDC dc, const RECT& rect) = 0;

    //�ж��루GainFocus()���봰����������İ�
    //���������������Ͳ����ٻ����ˣ��������µ���GainFocus()
    virtual bool __stdcall LostFocus() = 0;

    //GeoMap�Ƿ���Խ��л��Ʋ������Ƿ��Ѿ�����ĳ�����壩
    virtual bool __stdcall IsFocused() const = 0;

    //��������
    //ע������������������dc�е�ԭʼ����
    virtual DrawResult __stdcall DrawData(
        IDisplay*               pDisplay        = NULL,
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancel*     pTrackCancel    = NULL
        ) = 0;

    //����ѡ��
    virtual DrawResult __stdcall DrawSelection(
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancel*     pTrackCancel    = NULL
        ) = 0;

    //ע������������������dc�е�ԭʼ����
    virtual DrawResult __stdcall DrawSelectionEx(
        IDisplay*               pDisplay,
        const long              cacheid,      
        const WKSRect* const    pEnvelope       = NULL,
        const ITrackCancel*     pTrackCancel    = NULL
        ) = 0;

    virtual void __stdcall RefreshWindow() = 0;

    //������壨�ñ���ɫ��䣩
    virtual bool __stdcall EraseView() = 0;

    //���ѡ��
    virtual bool __stdcall EraseSelectionView() = 0;

    //��ȡ���ݣ����ƿ�������
    virtual DrawResult __stdcall UpdateData(const ITrackCancel* pTrackCancel = NULL) = 0;

    //��ȡ���ݣ����ƿ�������
    virtual DrawResult __stdcall UpdateSelection(const ITrackCancel* pTrackCancel = NULL) = 0;

    virtual void __stdcall DrawingHint(const bool visible) = 0;
};

}

#endif