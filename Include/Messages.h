#if !defined(MESSAGES_INCLUDED_)
#define MESSAGES_INCLUDED_

//�����������õ�����Ϣ����

#include "InterfaceObj.h"

namespace easymap
{

//��Ϣ����
const event_identity    MESSAGE_VECTORLAYER_                = 1000;
const event_identity    MESSAGE_VECTORFEATURE_              = 1100;

//---------------------------------------------------------------------
//  ����SlimLayer��ShapeLayer��Ϊ��������
//---------------------------------------------------------------------
//  ������Ҫ�أ�����������Ҫ�ص�fid
const event_identity    MESSAGE_VECTORLAYER_ADDED           = MESSAGE_VECTORLAYER_ + 1;
//  Ҫ�ر�ɾ����������ɾ��Ҫ�ص�fid
const event_identity    MESSAGE_VECTORLAYER_DELETED         = MESSAGE_VECTORLAYER_ + 2;
//  Ҫ���Ѿ����޸ģ��������޸ĵ�Ҫ�ص�fid
const event_identity    MESSAGE_VECTORLAYER_MODIFIED        = MESSAGE_VECTORLAYER_ + 3;
//  undo
const event_identity    MESSAGE_VECTORLAYER_UNDO            = MESSAGE_VECTORLAYER_ + 4;
//  redo
const event_identity    MESSAGE_VECTORLAYER_REDO            = MESSAGE_VECTORLAYER_ + 5;
//  �༭������
const event_identity    MESSAGE_VECTORLAYER_EDITSAVED       = MESSAGE_VECTORLAYER_ + 6;
//  �༭������
const event_identity    MESSAGE_VECTORLAYER_EDITCANCELED    = MESSAGE_VECTORLAYER_ + 7;
//  ͼ�㱻����û��޸ģ������ѱ�ˢ��
const event_identity    MESSAGE_VECTORLAYER_INDEXRELOADED   = MESSAGE_VECTORLAYER_ + 8;
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//  ����VectorFeature��Ϊ��������
//---------------------------------------------------------------------
//  ֪ͨ�����ߣ�Feature::Update()�Ѿ������ã��´�����Feature
const event_identity    MESSAGE_VECTORFEATURE_ADDED         = MESSAGE_VECTORFEATURE_ + 1;

//  ֪ͨ�����ߣ�Feature::Update()�Ѿ������ã��޸ĵ�Feature
const event_identity    MESSAGE_VECTORFEATURE_MODIFIED      = MESSAGE_VECTORFEATURE_ + 2;
//---------------------------------------------------------------------


}

#endif
