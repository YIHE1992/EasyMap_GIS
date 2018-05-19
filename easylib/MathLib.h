#if !defined(MATHLIB_INCLUDED_)
#define MATHLIB_INCLUDED_

//��Ҫ����ƽ�漸�εĻ�����ѧ������

#pragma warning(disable: 4786)
#include <vector>
#include <algorithm>
using namespace std;

#include "..\\include\\WKSInclude.h"
#include "..\\include\\BasicType.h"

namespace easymap
{

namespace mathlib
{

inline bool equal(double x, double y)
{
    unsigned __int64 *px = (unsigned __int64*)&x;
    unsigned __int64 *py = (unsigned __int64*)&y;

    unsigned __int64 d = (px[0] | py[0]);
    d = d>>56;
    if((d & 127) == 0) //x, y��Ϊ����ֵ��С����
    {
        return true;
    }
    unsigned __int64 X = px[0]>>20;
    unsigned __int64 Y = py[0]>>20;
    if(X == Y) return true;
    if(X < Y)
    {
        return X + 1 == Y;
    }
    else
    {
        return Y + 1 == X;
    }
    //���Լ����жϵ�λ�����
//    return true;
}

inline bool dinky(const double a)
{
    return equal(a, 0.0f);
}

inline long Round(const double& a)
{
    long i = (long)a;
    long d = long((a - i)*10);
    if (4 < d)
    {
        i++;
    }
    return i;
}

//����ʵ��һЩ�ߵ��㷨
class geo_path : public std::vector<WKSPoint>
{
public:
    //������˹ѹ��
    void dauglas(double tol2);
    //ȥ����Ч�ĵ㣬��Ч������ٽ����ظ���
    bool remove_invalid(double tol2);
    //��ƽ����
    bool parallel(geo_path& parallel, const double& distance) const;
    //����һ��
    bool neat(const dword depth = 1);
};


//����η���
bool solve(double a, double b, double c, double &t1, double &t2);


//���ﶨ�����������ζ���Ŀռ��ϵ��
typedef short RelationshipType;
const RelationshipType RELATIONTYPE_DISJOINT    = (RelationshipType)0;  //����
const RelationshipType RELATIONTYPE_CROSSES     = (RelationshipType)1;  //��Խ
const RelationshipType RELATIONTYPE_OVERLAPS    = (RelationshipType)2;  //����
const RelationshipType RELATIONTYPE_WITHIN      = (RelationshipType)3;  //��һ���ڵڶ����ڲ�
const RelationshipType RELATIONTYPE_CONTENTS    = (RelationshipType)4;  //��һ�������ڶ���
const RelationshipType RELATIONTYPE_EQUALS      = (RelationshipType)5;  //���
//const RELATIONTYPE_TOUCHES      = (RelationshipType)6;  //�Ӵ�


//������������������������������������������������
//������������������������������������������������

//-----------------------------------------------------------------------------------
//�ж������Ƿ����
//������point1, point2 Ϊ����������
//����ֵ��������������������
//-----------------------------------------------------------------------------------
bool PointEqual(const WKSPoint& point1, const WKSPoint& point2);

//-----------------------------------------------------------------------------------
//ƽ������������ľ���
//������point1, point2 Ϊ����������
//����ֵ������
//-----------------------------------------------------------------------------------
double Distance(const WKSPoint& point1, const WKSPoint& point2);

//-----------------------------------------------------------------------------------
//�������㣬�����Ե�һ��Ϊԭ�㣬���ҵ�ˮƽ����Ϊ�ο��㷽�򣬵ڶ������һ��������ο�����ļн�
//ע���Ƕȷ�Χ0~359
//����ֵ�� -9999.f �д�
//         0~359 ���� 
//-----------------------------------------------------------------------------------
double ComputeAngle(const WKSPoint& PointCen, const WKSPoint& PointMove);

//-----------------------------------------------------------------------------------
//�������㣬�����Ե�һ��Ϊԭ�㣬���ҵ�ˮƽ����Ϊ�ο��㷽�򣬵ڶ������һ��������ο�����ļн�
//ע���Ƕȷ�Χ0~359
//����ֵ�� -9999.f �д�
//         0~359 ���� 
//-----------------------------------------------------------------------------------
double ComputeAngleInverse(const WKSPoint& PointCen, const WKSPoint& PointMove);

//-----------------------------------------------------------------------------------
//��������ֱ�ߵļнǣ�����Ƕȵļ������Խ���Ϊԭ�㣬�Խ��������һ��ֱ�ߵ���һ�����������ΪX��
//�Ƕ���0~360֮��
//-----------------------------------------------------------------------------------
double ComputeTwoLineAngle(const WKSPoint& Line1Str, const WKSPoint& Line1End,
    const WKSPoint& Line2Str, const WKSPoint& Line2End);

//-----------------------------------------------------------------------------------
//���ܣ���ֱ�߷���,��������ֱ�ߵĶ˵㣬��ֱ�߷���
//������slope б��,constant����
//����ֵ�� false ��ʾ��Y��ƽ�У�б��Ϊ����󣬸�ֵΪ-9999.f
//         true ����  
//-----------------------------------------------------------------------------------
bool LineEqution(const WKSPoint& LineFrom, const WKSPoint& LineTo,
    double* const slope, double* const constant);

//-----------------------------------------------------------------------------------
//���ܣ���ֱ��б��
//������LineFrom, LineTo Ϊ����ֱ�ߵ�����
//����ֵ��ƽ����ֱ��б��
//˵����  ���б��Ϊ����󣬷���-9999.0 
//-----------------------------------------------------------------------------------
double LineSlope(const WKSPoint& LineFrom, const WKSPoint& LineTo);

//-----------------------------------------------------------------------------------
//���ܣ������������㼰�����㣬��������㵽ǰ���������ɵ�ֱ�ߵĴ���
//������LineFrom��LineToΪ���������㣬pointΪ�����㣬point1����
//����ֵ����
//ע�� ������XΪ�ο��ƶ�
//-----------------------------------------------------------------------------------
void VerticalLine(const WKSPoint& LineFrom, const WKSPoint& LineTo, const WKSPoint& point,
    WKSPoint& point1);

//-----------------------------------------------------------------------------------
//���ܣ���ö���ε���Ӿ���
//������
//����ֵ���ɹ�true��ʧ��false
//-----------------------------------------------------------------------------------
bool GetPolygonMBR(const vector<WKSPoint>& points, WKSRect& mbr);



//===================================================================================
//�����֮��Ĺ�ϵ
//===================================================================================

//-----------------------------------------------------------------------------------
//���ܣ��㵽ֱ�ߵľ���
//������ LineFrom, LineTo ֱ�ߵ����˵㣬pointΪ�����ĵ�
//����ֵ���㵽ֱ�ߵľ���
//-----------------------------------------------------------------------------------
double PointToLine(const WKSPoint& LineFrom, const WKSPoint& LineTo, const WKSPoint& point);

//-----------------------------------------------------------------------------------
//���ܣ��жϵ��Ƿ���ֱ����
//������ LineFrom, LineTo ֱ�ߵ����˵㣬pointΪ�����ĵ�
//����ֵ������ֱ����ΪTRUE
//        �㲻��ֱ����ΪFALSE 
//-----------------------------------------------------------------------------------
bool IsPointOnLine(const WKSPoint& LineFrom, const WKSPoint& LineTo,
    const WKSPoint& point);

//-----------------------------------------------------------------------------------
//���ܣ��㵽�߶εľ���
//������ LineFrom, LineTo ֱ�ߵ����˵㣬pointΪ�����ĵ�
//����ֵ���㵽�߶εľ���
//-----------------------------------------------------------------------------------
double PointToLineSegment(const WKSPoint& LineFrom, const WKSPoint& LineTo,
    const WKSPoint& point);

//-----------------------------------------------------------------------------------
//���ܣ��жϵ��Ƿ����߶���
//������ LineFrom, LineTo ֱ�ߵ����˵㣬pointΪ�����ĵ�
//����ֵ�������߶���Ϊtrue
//        �㲻���߶���Ϊfalse
//-----------------------------------------------------------------------------------
bool IsPointOnLineSegment(const WKSPoint& LineFrom, const WKSPoint& LineTo,
    const WKSPoint& point);

//-----------------------------------------------------------------------------------
//���ܣ��жϵ��Ƿ���������
//������pPointArray�����ߣ�pointΪ�����ĵ�
//����ֵ�������߶���ΪTRUE
//        �㲻���߶���ΪFALSE 
//-----------------------------------------------------------------------------------
bool IsPointOnLineArray(const vector<WKSPoint>& points, const WKSPoint& point);

//-----------------------------------------------------------------------------------
//���ܣ���ֱ���߶����ĵ�
//������start, end�����ߣ�distanceΪ�㵽�ߵľ���
//      pnt1��pnt2Ϊ��õ������㣬һ��һ��
//����ֵ��always true
//        
//-----------------------------------------------------------------------------------
bool VerticalPointsToLine(const WKSPoint& start, const WKSPoint& end, const double& distance,
    WKSPoint& pnt1, WKSPoint& pnt2);


//===================================================================================
//�ߺ���֮��Ĺ�ϵ
//===================================================================================

//-----------------------------------------------------------------------------------
//���ܣ����ཻ
//������Line1Str,Line1End,Line2Str,Line2End,����ֱ�ߵĶ˵㣬 pointΪ����
//����ֵ��0  û�н���
//        1  �н���  
//-----------------------------------------------------------------------------------
bool LineIntersectPoint(const WKSPoint Line1Str, const WKSPoint Line1End,
    const WKSPoint Line2Str, const WKSPoint Line2End, WKSPoint &point);

//-----------------------------------------------------------------------------------
//���ܣ����ཻ
//������Line1Str,Line1End,ֱ�ߵĶ˵㣬slope,constantΪֱ�ߵ�б��,pointΪ����
//����ֵ��0  û�н���
//        1  �н���  
//-----------------------------------------------------------------------------------
short LineIntersectPoint(const WKSPoint Line1Str, const WKSPoint Line1End,
    const double slope, const double constant, WKSPoint &point);

//-----------------------------------------------------------------------------------
//���ܣ���������ֱ�ߵ�б�ʺͳ������󽻵�
//������slope1Ϊ��һ��ֱ�ߵ�б�ʣ�constant1Ϊ��һ��ֱ�ߵĳ���
//      slope2Ϊ��һ��ֱ�ߵ�б�ʣ�constant2Ϊ��һ��ֱ�ߵĳ���
//����ֵ��false�� �޽���
//        true�� �н���
//-----------------------------------------------------------------------------------
bool LineIntersectPoint(const double slope1, const double constant1, const double slope2,
    const double constant2, double* const x, double* const y);

//-----------------------------------------------------------------------------------
//���ܣ��ж����߶εĹ�ϵ���ཻ�����룬���ཻ���򷵻ؽ���
//������LineFrom1��LineTo1Ϊ��һ��ֱ�ߵ������˵㣬LineFrom2��LineTo2Ϊ�ڶ���ֱ�ߵ������˵㣬
//    IntersectPointΪ���صĽ���
//����ֵ��TRUE: �ཻ�����н��㣬�򽫽��㷵��
//        FALSE�� ����
//-----------------------------------------------------------------------------------
bool GetRelationTwoLine(const WKSPoint LineFrom1, const WKSPoint LineTo1,
    const WKSPoint LineFrom2, const WKSPoint LineTo2, WKSPoint& IntersectPoint,
    bool& parallel);

//-----------------------------------------------------------------------------------
//���ܣ��õ�������Ŀ�꣨�ж���߶���ɣ��Ĺ�ϵ�����ཻ���򷵻ؽ���
//������pLineCoordArray1Ϊ��һ��Ŀ�꣬pLineCoordArray1Ϊ��һ��Ŀ�꣬IntersectPointΪ���صĽ���
//����ֵ��  true: �ཻ�����н��㣬�򽫽��㷵��
//          false�� ����
//-----------------------------------------------------------------------------------
bool GetRelationTwoLineArray(const vector<WKSPoint>& line1, const vector<WKSPoint>& line2,
    vector<WKSPoint>& intersectpoints);


//-----------------------------------------------------------------------------------
//�жϵ��Ƿ���ĳһ���������.
//����ֵ��
//      1   �ڱ߽���
//      >1  ���ڲ�
//      0   ���ⲿ
//      <0  ����
//-----------------------------------------------------------------------------------
long PointInPolygon(const WKSPoint& point, const vector<WKSPoint>& polygon);

//-----------------------------------------------------------------------------------
//  ע�����Ҫ��֤pParallel���㹻�Ŀռ�
//-----------------------------------------------------------------------------------
bool GetParallelLine(const WKSPoint* const line, const dword pointcount, const double& distance,
    WKSPoint* const pParallel);

//��ת2d�㣬����
void RotatePI(WKSPoint &pnt, const WKSPoint &origin, const double& angle);

//��ת2d�㣬����
void RotatePIXY(double &pnt_x, double &pnt_y, const double &origin_x,
    const double &origin_y, const double& angle);

//��ת2d�㣬�Ƕ�
void RotateDegree(WKSPoint &pnt, const WKSPoint &origin, const double& degree);

//˳ʱ���
void RotateDegreeInvert(WKSPoint &pnt, const WKSPoint &origin, const double& degree);

//��envelope����line
//����ֵ��
//      0 - �����߶���extent���棬û�н���
//      1 - from�����棬to�������ұ�����
//      2 - to�����棬from�������ұ�����
//      3 - �����㶼�����棬�����������㣬from��to��������
//      4 - �����㶼�����棬���ò���
long EnvelopeClipLine(const WKSRect& extent, double& x1, double& y1, double& x2, double& y2);

//���õ�ǰ���㾫��
inline void SetMathLibPrecision(const double& precision);
//ȡ�õ�ǰ���㾫��
inline void GetMathLibPrecision(double& precision);


#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

}

}

#endif
