#if !defined(INTERFACELABELLAYER_INCLUDED_)
#define INTERFACELABELLAYER_INCLUDED_

#include "InterfaceLayer.h"
#include "InterfaceSupport.h"

namespace easymap
{

class ILabelLayer;
class ILabelLayerManager;

typedef TSmartPtr<ILabelLayer> ILabelLayerPtr;
typedef TSmartPtr<ILabelLayerManager> ILabelLayerManagerPtr;

//================================================================================
//  LabelLayer������ʾSlimLayer��ShapeLayer�����Ա�ע
//================================================================================
class ILabelLayer : public ILayer
{
public:
    //  ������SlimLayer����ShapeLayer
    virtual bool __stdcall SetVectorLayer(const ILayer* pLayer) = 0;
    virtual bool __stdcall GetVectorLayer(ILayer** ppLayer) const = 0;

    //  ��ע�ֶ�����
    virtual bool __stdcall SetFieldIndex(const long fieldindex) = 0;
    virtual long __stdcall GetFieldIndex() = 0;

    //  ע�Ƿ���
    virtual bool __stdcall SetTextSymbol(const ITextSymbol* pTextSymbol) = 0;
    virtual bool __stdcall GetTextSymbol(ITextSymbol** ppTextSymbol) const = 0;

    //  �ο�������
    virtual bool __stdcall SetRefScale(const double scale) = 0;
    virtual double __stdcall GetRefScale() const = 0;

    //  ȡ�����ӷ�Χ�ڵı�ע����
    //      ppTextPositions��2��doubleΪһ�飺
    //          0 - label��x����
    //          1 - label��y����
    //
    //      ppLabelTexts��ÿ��string��ppTextPositions��һ��2��double����
    //
    //      test_dc���ڼ���textenvelope
    virtual DrawResult __stdcall GetLabelText(IDoubleArray** ppTextPositions,
        IStringArray** ppLabelTexts, const IDisplayTransformation* pDT,
        const ITrackCancel* pTrackCancel, const WKSRect& visibleextent) = 0;

    //  ��ջ������ݣ����ں�����ͼ��ͬ��
    virtual void __stdcall ClearCache() = 0;
};

//================================================================================
//  ����һ��LabelLayer����Map����
//================================================================================
class ILabelLayerManager : public IPersist
{
public:
    //  ����һ��LabelLayer
    virtual bool __stdcall AddLabelLayer(const ILabelLayer* pLabelLayer) = 0;
    virtual bool __stdcall RemoveLabelLayer(const dword index) = 0;
    virtual bool __stdcall RemoveLabelLayerEx(ILabelLayer* pLabelLayer) = 0;
    virtual bool __stdcall GetLabelLayer(ILabelLayer** ppLabelLayer, const dword index) const = 0;
    virtual bool __stdcall SetLabelLayerOrder(const ILabelLayer* pLabelLayer, const dword neworder) = 0;
    virtual void __stdcall ClearLabelLayers() = 0;
    virtual dword __stdcall GetLabelLayerCount() const = 0;

    //  �Ƿ����
    virtual void __stdcall SetTextAvoidable(const bool avoidable) = 0;
    virtual bool __stdcall GetTextAvoidable() const = 0;

    //  ���Ʊ�ע
    virtual DrawResult __stdcall DrawLabels(const IDisplay* pDisplay, const WKSRect* const pEnvelope,
        const ITrackCancel* pTrackCancel) const = 0;

    //  �Ƿ���ʾ�Զ���ע
    virtual void __stdcall EnableLabelDraw(bool Enable) = 0;
    virtual bool __stdcall LabelDrawEnabled() const = 0;
};

}

#endif
