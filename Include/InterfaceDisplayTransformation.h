#if !defined(INTERFACEDISPLAYTRANSFORMATION_INCLUDED_)
#define INTERFACEDISPLAYTRANSFORMATION_INCLUDED_

#include "WKSInclude.h"
#include "InterfacePersist.h"
#include "InterfaceGeometry.h"
#include "MapUnits.h"
#include "windows.h"

namespace easymap
{

class IDisplayTransformation;
typedef TSmartPtr<IDisplayTransformation> IDisplayTransformationPtr;

//��ǰ��ʾ��Χ�ı�ʱ�����ص�
typedef void (_stdcall DisplayVisiExtChg)(const WKSRect&);

class IDisplayTransformation : public IPersist
{
public:
    //  ��ͼ��ת�Ƕȣ���λ�Ƕ�
    virtual void __stdcall SetPlaneRotate(const double degree) = 0;
    virtual void __stdcall GetPlaneRotate(double& degree) const = 0;

    //�����ǣ���λ�Ƕ�
    virtual void __stdcall SetAttitude(const double degree) = 0;
    virtual void __stdcall GetAttitude(double& degree) const = 0;

    //��ת���ģ�ע���������ʱ����ת���Ĳ����ǵ�ͼ��ǰ��ʾ���������
    virtual void __stdcall SetRotateCenter(const WKSPoint& rotatecenter) = 0;
    virtual void __stdcall GetRotateCenter(WKSPoint& rotatecenter) const = 0;

    //  ����/ȡ����ͼ��λ
    virtual void __stdcall SetMapUnit(const MapUnits mapunit) = 0;
    virtual MapUnits __stdcall GetMapUnit() const = 0;

    //  ����/ȡ����ʾ������
    virtual void __stdcall SetMapScale(const double mapscale) = 0;
    virtual void __stdcall GetMapScale(double& mapscale) const = 0;

    //  ����/ȡ����ǰ��ʾ�������������
    virtual void __stdcall SetMapCenter(const WKSPoint& center) = 0;
    virtual void __stdcall GetMapCenter(WKSPoint& center) const = 0;

    //  ����/ȡ����ǰ���ӷ�Χ
    virtual void __stdcall SetVisibleExtent(const WKSRect& extent) = 0;
    virtual void __stdcall GetVisibleExtent(WKSRect& extent) const = 0;

    //  ����/ȡ����ʾ�豸�����壩��С
    virtual void __stdcall SetDeviceRect(const RECT& rect) = 0;
    virtual void __stdcall GetDeviceRect(RECT& rect) const = 0;

    //  ����/ȡ������Ļ���غ�Ӣ��Ķ�Ӧ��ϵ��
    virtual void __stdcall SetLogPixel(const long logpixelx, const long logpixely) = 0;
    virtual void __stdcall GetLogPixel(long& logpixelx, long& logpixely) const = 0;

    //  ��Ļ���� <==> ʵ������
    virtual void __stdcall Device2MapXY(const long x_dev, const long y_dev, double& x_map,
        double& y_map) const = 0;
    virtual void __stdcall Map2DeviceXY(const double x_map, const double y_map,
        long& x_dev, long& y_dev) const = 0;
    virtual void __stdcall Device2Map(const POINT& point_dev, WKSPoint& point_map) const = 0;
    virtual void __stdcall Map2Device(const WKSPoint& point_map, POINT& point_dev) const = 0;

    virtual void __stdcall Device2Map_Envelope(const tagRECT& envelope_dev, WKSRect& envelope_map) const = 0;

    //  ����/ȡ���ο�������
    virtual void __stdcall SetReferenceScale(const double refscale) = 0;
    virtual void __stdcall GetReferenceScale(double& refscale) const = 0;

    //���Ƿ��ڿ���ƽ�淶Χ��
    virtual bool __stdcall PointInPlane(const double x, const double y) const = 0;

    //��visible extent������geometry�����ڻ���֮ǰ�Ĵ���
    //ע��ֻ��������֤Geometry���Ƶ���ȷ��
    virtual void __stdcall ClipGeometry(const IGeometry* pGeometry, IGeometry** ppClipped) const = 0;
    //--------------------------------------------------------------------------

    //��ǰ��ʾ��Χ�ı�ʱ�����ص�
    virtual bool __stdcall SetVisibleExtentHandle(DisplayVisiExtChg* const pHandle) = 0;
};

}

#endif
