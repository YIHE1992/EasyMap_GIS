#if !defined(DISPLAYTRANSFORMATION_INCLUDED_)
#define DISPLAYTRANSFORMATION_INCLUDED_

#include "CommonInclude.h"
#include "..\\include\\InterfaceDisplayTransformation.h"
#include "Plot3D.h"

namespace easymap
{

class CDisplayTransformation;
typedef TSmartPtr<CDisplayTransformation> CDisplayTransformationPtr;

//================================================================================
//  ���������������Ļ�����ʵ������֮���ת������
//================================================================================
class CDisplayTransformation : public IDisplayTransformation
{
CLASS_NAME(CDisplayTransformation)
PERSIST_DUMP(CDisplayTransformation)
NO_EVENTS_DISPATCHER
NO_EVENTS_LISTENER

public:
    CDisplayTransformation();
private:
    ~CDisplayTransformation();

private:
    double      m_InnerScale;       //�����ڲ�����ı����ߣ�
                                    //ע�Ⲣ��ֱ�ӵ�����ʾ�����ߣ�
                                    //�μ�Set/GetMapScale()����

    WKSPoint    m_MapCenter;        //��ǰ��ʾ�������ĵ�ʵ������
    RECT        m_DeviceRect;       //��ʾ�豸�����壩�Ĵ�С
    long        m_LogPixelX;        //��Ļ���غ�Ӣ���ӳ���ϵ��X�ᣩ
    long        m_LogPixelY;        //��Ļ���غ�Ӣ���ӳ���ϵ��Y�ᣩ

    WKSPoint    m_RotateCenter;     //����ʱ��ʵ����ת����

    WKSRect     m_VisibleExtent;    //��ǰ��ʾ��Χ����������Ҫ������������ٵ�
    WKSRect     m_ExtentExt;        //����������ת�ģ�����任ʱ��������

    MapUnits    m_MapUnit;          //��ͼ��λ��M��KM��
    double      m_ReferenceScale;   //�ο������ߣ���Ҫ������ͼ��������
    string      m_SR;

    CPlot3DPtr  m_pPlot3D;          //���ڿ��ƻ���ʱ����ת�ǶȺ͸�����

    DisplayVisiExtChg* m_pVisiExtChg;

private:
    //Ϊ�˼���ʵ����ת���Ķ����
    void finishplanerotate(const double& degree);

    //��Ҫ�ı���ʾ�����ʱ�����������
    void finishtranslation(bool callback = true);

    //  ���ڼ�����ת��ǰ������
    inline double _device2map_x(const double x_dev) const;
    inline double _device2map_y(const double y_dev) const;
    inline double _map2device_x(const double x_map) const;
    inline double _map2device_y(const double y_map) const;

    //������ƽ����ת����û���Ǹ���
    void _device2map_rotated2d(const double dev_x, const double dev_y,
        WKSPoint& point_map) const;

    void ClipPath(const IPathPtr pPath, IGeometryPtr& pClipped) const;
    void ClipRing(const IRingPtr pRing, IGeometryPtr& pClipped) const;
    void ClipPolyline(const IPolylinePtr pPolyline, IGeometryPtr& pClipped) const;
    void ClipPolygon(const IPolygonPtr pPolygon, IGeometryPtr& pClipped) const;
    void ClipMultiPoint(const IMultiPointPtr pMultiPoint, IGeometryPtr& pClipped) const;

    bool _cliprect(WKSRect& rect) const;

public:
    bool __stdcall GotoInterface(const char* const interfacename, void** pp);
    bool __stdcall Clone(IObj** ppObj) const;

private:
    dword __stdcall _SaveInstance(IStreamX* pStream, void* const assist) const;
    dword __stdcall _LoadInstance(IStreamX* pStream, void* const assist);

public:
    //---------------------------------------------------------------------------------
    //  �ڲ�ʹ�ã��������С��1���ص�XX
    void Device2Map(const WKSPoint& point_dev, WKSPoint& point_map) const;
    void Map2Device(const WKSPoint& point_map, WKSPoint& point_dev) const;
    //---------------------------------------------------------------------------------

    void GetPlot3D(CPlot3DPtr& pPlot3D) const;

public:
    //  ��ͼƽ����ת����λ�Ƕ�
    void __stdcall SetPlaneRotate(const double degree);
    void __stdcall GetPlaneRotate(double& degree) const;

    //�����ǣ���λ�Ƕ�
    void __stdcall SetAttitude(const double degree);
    void __stdcall GetAttitude(double& degree) const;

    //��ת���ģ�ע���������ʱ����ת���Ĳ����ǵ�ͼ��ǰ��ʾ���������
    void __stdcall SetRotateCenter(const WKSPoint& rotatecenter);
    void __stdcall GetRotateCenter(WKSPoint& rotatecenter) const;

    //  ����/ȡ����ͼ��λ
    void __stdcall SetMapUnit(const MapUnits mapunit);
    MapUnits __stdcall GetMapUnit() const;

    //  ����/ȡ����ʾ������
    void __stdcall SetMapScale(const double mapscale);
    void __stdcall GetMapScale(double& mapscale) const;

    //  ����/ȡ����ǰ��ʾ�������������
    void __stdcall SetMapCenter(const WKSPoint& center);
    void __stdcall GetMapCenter(WKSPoint& center) const;

    //  ����/ȡ����ǰ���ӷ�Χ
    void __stdcall SetVisibleExtent(const WKSRect& extent);
    void __stdcall GetVisibleExtent(WKSRect& extent) const;

    //  ����/ȡ����ʾ�豸�����壩��С
    void __stdcall SetDeviceRect(const RECT& rect);
    void __stdcall GetDeviceRect(RECT& rect) const;

    //  ����/ȡ������Ļ���غ�Ӣ��Ķ�Ӧ��ϵ��
    void __stdcall SetLogPixel(const long logpixelx, const long logpixely);
    void __stdcall GetLogPixel(long& logpixelx, long& logpixely) const;

    //  ��Ļ���� <==> ʵ������
    void __stdcall Device2MapXY(const long x_dev, const long y_dev, double& x_map,
        double& y_map) const;
    void __stdcall Map2DeviceXY(const double x_map, const double y_map,
        long& x_dev, long& y_dev) const;
    void __stdcall Device2Map(const POINT& point_dev, WKSPoint& point_map) const;
    void __stdcall Map2Device(const WKSPoint& point_map, POINT& point_dev) const;

    void __stdcall Device2Map_Envelope(const tagRECT& envelope_dev, WKSRect& envelope_map) const;

    //  ����/ȡ���ο�������
    void __stdcall SetReferenceScale(const double refscale);
    void __stdcall GetReferenceScale(double& refscale) const;

    //���Ƿ��ڿ���ƽ�淶Χ��
    bool __stdcall PointInPlane(const double x, const double y) const;

    //��visible extent������geometry�����ڻ���֮ǰ�Ĵ���
    void __stdcall ClipGeometry(const IGeometry* pGeometry, IGeometry** ppClipped) const;

    //  �ռ�ο�����
    bool __stdcall SetSpatialReference(const string& sr);
    bool __stdcall GetSpatialReference(string& sr) const;

    //��ǰ��ʾ��Χ�ı�ʱ�����ص�
    bool __stdcall SetVisibleExtentHandle(DisplayVisiExtChg* const pHandle);

    //  ���㴫��Ĳ����͡��ס��Ķ�Ӧ��ϵ
    static double GetMeterQuotiety(const MapUnits unit);
    //  ���㴫��Ĳ����͡�Ӣ�硱�Ķ�Ӧ��ϵ
    static double GetInchQuotiety(const MapUnits unit);
};
//================================================================================

CLASS_FACTORY(CDisplayTransformation)


bool POINTinRECT(const long& x, const long& y, const tagRECT& rect);
void ExtentViewExtentSlight(WKSRect& viewextent);

}

#endif
