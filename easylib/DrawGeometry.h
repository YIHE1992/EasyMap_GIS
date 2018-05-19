#if !defined(DRAWGEOMETRY_INCLUDED_)
#define DRAWGEOMETRY_INCLUDED_

//--------------------------------------------------------------------------------
//  �����ṩ�򵥵�Geometry�Ļ��Ʒ���
//--------------------------------------------------------------------------------

#include "CommonInclude.h"
#include "DisplayTransformation.h"
#include "Geometry.h"
#include "..\\include\\InterfaceSymbol.h"

namespace easymap
{

namespace drawgeometry
{

//--------------------------------------------------------------------------------
//  ��ģ��Ļ���
//--------------------------------------------------------------------------------
//draw_template_line�Ļص�����������ʵ�ʵĻ��Ʋ���������ÿһ��mark
//ע��from��toΪ��Ļ���꣬ʹ��double��Ϊ�˱�֤���㾫��
typedef void (callback_draw_template_sector)(const WKSPoint& from, const WKSPoint& to,
    const dword pointcount);
//����ģ������߶Σ�path�е�һ�Σ�ȱʡ�ǻ�����ͨ�ߣ�Ҳ��������drawmark�����Ƶ��ߡ���ϣ��
//ע��from��toΪ��Ļ���꣬ʹ��double��Ϊ�˱�֤���㾫��
void draw_template_line(const HDC dc, double quo, const ILineSimpleTemplatePtr pTemplate,
    long& sectorindex, long& sectorprogress, const WKSPoint& from, const WKSPoint& to,
    callback_draw_template_sector* const drawmark = NULL);
//--------------------------------------------------------------------------------


//********************************************************************************
//  ��һ�麯��������Geometry
//********************************************************************************

//--------------------------------------------------------------------------------
//  ����һ��Geometry����������ʲô���㡢�ߡ��涼���ԣ�
//  ������������Geometry�������Զ�ѡ�����Ļ��ƺ���
//--------------------------------------------------------------------------------
bool DrawGeneralGeometry(
            const IGeometryPtr              pGeometry,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ���Ƶ�
//--------------------------------------------------------------------------------
bool DrawPoint(
            const CPointPtr                 pPoint,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ���Ƶ�Ⱥ
//--------------------------------------------------------------------------------
bool DrawMultiPoint(
            const CMultiPointPtr            pMultiPoint,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ���������ߣ�ע������֧���߷���ģ���ƽ���ߣ�distance��λΪ����
//--------------------------------------------------------------------------------
bool DrawPath(
            const CPathPtr                  pPath,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc,
            const ILineSimpleTemplatePtr    pTemplate = NULL,
            const double&                   distance = 0
            );

//--------------------------------------------------------------------------------
//  ����Ȧ
//--------------------------------------------------------------------------------
bool DrawRing(
            const CRingPtr                  pRing,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ���Ƹ������߶���
//--------------------------------------------------------------------------------
bool DrawPolyline(
            const CPolylinePtr              pPolyline,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ���Ƹ��϶���ζ���
//--------------------------------------------------------------------------------
bool DrawPolygon(
            const CPolygonPtr               pPolygon,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ���ƾ��ζ���
//--------------------------------------------------------------------------------
bool DrawEnvelope(
            const CEnvelopePtr              pEnvelope,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ����circle
//--------------------------------------------------------------------------------
bool DrawCircle(
            const CCirclePtr                pCircle,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ����ellipse
//--------------------------------------------------------------------------------
bool DrawEllipse(
            const CEllipsePtr               pEllipse,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//********************************************************************************
//  ��һ�����
//********************************************************************************


//********************************************************************************
//  �ڶ��麯��������Stream
//********************************************************************************

//--------------------------------------------------------------------------------
//  ����Stream�е�Geoemtry�����ڱ���Geometry���󻯣��ӿ�����ٶ�
//  ������������Geometry�������Զ�ѡ�����Ļ��ƺ���
//  ע�����������Ҫ�����Stream��һ��byte��geometrytypeǰ׺
//--------------------------------------------------------------------------------
long DrawGeneralStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------���º�����block����Ҫgeometrytypeǰ׺-------------------------------------

//--------------------------------------------------------------------------------
//  ��Stream
//--------------------------------------------------------------------------------
long DrawPointStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ��ȺStream
//--------------------------------------------------------------------------------
long DrawMultiPointStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ��������Stream
//--------------------------------------------------------------------------------
long DrawPathStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ����ȦStream
//--------------------------------------------------------------------------------
long DrawRingStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ��������Stream
//--------------------------------------------------------------------------------
long DrawPolylineStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ���϶����Stream
//--------------------------------------------------------------------------------
long DrawPolygonStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ����Stream
//--------------------------------------------------------------------------------
long DrawEnvelopeStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ԲStream
//--------------------------------------------------------------------------------
long DrawCircleStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//--------------------------------------------------------------------------------
//  ellipse Stream
//--------------------------------------------------------------------------------
long DrawEllipseStream(
            CStreamPtr                      pStream,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc
            );

//********************************************************************************
//  �ڶ������
//********************************************************************************


//--------------------------------------------------------------------------------
//  ��label��
//--------------------------------------------------------------------------------
bool GetLabelPoint(
            const IGeometryPtr              pGeometry,
            const CDisplayTransformationPtr pTrans,
            POINT&                          labelpoint
            );

//--------------------------------------------------------------------------------
//  ���Ʊ�ǩ��λ����Geometry����
//--------------------------------------------------------------------------------
bool DrawLabel(
            const IGeometryPtr              pGeometry,
            const char* const               pcLabel,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc,
            RECT&                           textenvelope
            );

//--------------------------------------------------------------------------------
//  ���Ʊ�ǩ��λ����x��y�������
//--------------------------------------------------------------------------------
bool DrawLabelXY(
            const double&                   x,
            const double&                   y,
            const char* const               pcLabel,
            const CDisplayTransformationPtr pTrans,
            HDC                             dc,
            RECT&                           textenvelope
            );

}

}

#endif
