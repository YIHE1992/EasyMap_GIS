#if !defined(GEOMETRYCOLUMNINFO_INCLUDED_)
#define GEOMETRYCOLUMNINFO_INCLUDED_

#include "InterfaceGeometry.h"
#include "MapUnits.h"

namespace easymap
{

typedef struct
{
    //Ϊ�˷��Ż���������Ҫ��Ҫ�ز������ͳһ�ļ�������
    ShapeType ShpType;

    //0 - ��ͨҪ�أ� //1 - ע��
    long FeatureType;

    //���굥λ
    MapUnits MapUnit;

    //ͼ���ԭʼ�����ߣ���1:1��ʾ��ʱ����ʹ�õ���ʾ������
    double BaseScale;

    //ͼ����Χ�����꾫��
    WKSRect DomainXY;
    double ToleranceXY;
    double MaxZ;
    double MinZ;
    double ToleranceZ;

    //��������
    dword SIParam1;
    dword SIParam2;
    double SIParam3;
}GeometryColumnInfo;

}

#endif