/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ControlLight.h"
#include "BL_Debug.h"
#include "PrototypeMOG.h"
#include "MathTools.h"
#include "IBaseObjectGeneral.h"
#include <valarray>
#include "DevTool_Share.h"
#include "PrototypeGraphicEngine.h"

using namespace nsMathTools;


#define ASPECT_LIGHT_X 0.01f
#define ASPECT_LIGHT_Y 0.01f


TControlLight::TControlLight()
{
  isDrag = false;
}
//----------------------------------------------------------------------------------
TControlLight::~TControlLight()
{
  Done();
}
//----------------------------------------------------------------------------------
void TControlLight::Done()
{
  mVecObjLight.clear();
}
//----------------------------------------------------------------------------------
void TControlLight::Init()
{
  pGE  = TDevTool_Share::Singleton()->GetComponent()->mGraphicEngine;
  pMOC = TDevTool_Share::Singleton()->GetComponent()->mMOG;
  // �������� ��������
  int cnt = GetCountAllLight(); 
  for(int i = 0 ; i < cnt ; i++ )
  {
    IBaseObjectGeneral* pSphere = pMOC->CreateObject(3);// 
    pGE->AddObject((IBaseObjectGE*)pSphere);
    mVecObjLight.push_back(pSphere);
  }
  Update();
}
//----------------------------------------------------------------------------------
void TControlLight::SetLightEnable(int index, bool v)
{
  BL_ASSERT(pGE);
  pGE->SetLightEnable(index, v);
  mVecObjLight[index]->SetShow(v);
}
//----------------------------------------------------------------------------------
bool TControlLight::GetLightEnable(int index)
{
  BL_ASSERT(pGE);
  return pGE->GetLightEnable(index);
}
//----------------------------------------------------------------------------------
int TControlLight::GetCountAllLight()
{
  BL_ASSERT(pGE);
  return pGE->GetCountAllLight();
}
//----------------------------------------------------------------------------------
const TVector3* TControlLight::GetLightPosition(int index)	
{
  BL_ASSERT(pGE);
  return pGE->GetLightPosition(index);
}
//----------------------------------------------------------------------------------
void TControlLight::SetLightPosition(int index, TVector3* m3)
{
  BL_ASSERT(pGE);
  pGE->SetLightPosition(index, m3);

  SetObjectPos(m3, index);
}
//----------------------------------------------------------------------------------
const TVector3* TControlLight::GetLightColor(int index)
{
  BL_ASSERT(pGE);
  return pGE->GetLightColor(index);
}
//----------------------------------------------------------------------------------
void TControlLight::SetLightColor(int index, TVector3* color)
{
  BL_ASSERT(pGE);
  pGE->SetLightColor(index, color);
}
//----------------------------------------------------------------------------------
void TControlLight::BeginDrag(int x, int y)// �������� ���������� ����
{
  isDrag = true;
  mOldX = x;
  mOldY = y;
}
//----------------------------------------------------------------------------------
void TControlLight::EndDrag()
{
  isDrag = false;
}
//----------------------------------------------------------------------------------
void TControlLight::Drag(int iCurLight,// ������� ��������
                         const TMatrix16* view, 
                         int x, int y)// �������� ���������� ����
{
  if(isDrag==false) return;

  TMatrix16 mInv;
  SetMatrixInverse(&mInv,NULL,view);
  TVector3 right = TVector3(mInv.s._11, mInv.s._12, mInv.s._13);
  TVector3 up    = TVector3(mInv.s._21, mInv.s._22, mInv.s._23);

  TVector3 posL = *GetLightPosition(iCurLight);
  TVector3 vecL = posL - mPos;
    
  float lenVecL_old = vecL.lenght();

  float angleV = ASPECT_LIGHT_Y*float(mOldY - y);
  float angleH = ASPECT_LIGHT_X*float(mOldX - x);
  float angle = 0;// 

  // ������� ������ �������������� ���
  TQuaternion axis;
  SetQuaternionRotationAxis(&axis,&right,angleV);// ������ ���� ��� ����� ������������ ��������
  //-----------------------------------------
  TQuaternion rot = TQuaternion(vecL.x,vecL.y,vecL.z,0);// ���� ������ ����� ���������
  SetQuaternionMultiply(&rot,&rot,&axis);// �������
  SetQuaternionToAxisAngle(&rot,&vecL,&angle);// �������� ����� ������ �, ��� �������� �������, ����

  // ������� ������ ������������ ���
  SetQuaternionRotationAxis(&axis,&up,angleH);// ������ ���� ��� ����� ������������ ��������
  //-----------------------------------------
  rot = TQuaternion(vecL.x,vecL.y,vecL.z,0);// ���� ������ ����� ���������
  SetQuaternionMultiply(&rot,&rot,&axis);// �������
  SetQuaternionToAxisAngle(&rot,&vecL,&angle);// �������� ����� ������ �, ��� �������� �������, ����
  
  float lenVecL_new = vecL.lenght();
  vecL *= lenVecL_old/lenVecL_new;
  float lenVecL = vecL.lenght();

  posL = vecL + mPos;

  mOldX = x;
  mOldY = y;

  SetLightPosition(iCurLight, &posL);
}
//----------------------------------------------------------------------------------
void TControlLight::SetPosition(TVector3* pPos)// ����� ��������
{
  mPos = *pPos;
}
//----------------------------------------------------------------------------------
void TControlLight::Update()
{
  int cnt = mVecObjLight.size();
  for(int i = 0 ; i < cnt ; i++)
  {
    const TVector3* pV = GetLightPosition(i);
    SetObjectPos((TVector3*)pV,i);
    mVecObjLight[i]->SetShow(GetLightEnable(i));
  }
}
//----------------------------------------------------------------------------------
void TControlLight::SetObjectPos(TVector3* m3, int i)
{
  IBaseObjectGeneral* pObj = mVecObjLight[i];
  TMatrix16 world = *pObj->GetWorld();
  world.s._41 = m3->x;
  world.s._42 = m3->y;
  world.s._43 = m3->z;
  pObj->SetWorld(&world);
}
//----------------------------------------------------------------------------------
void TControlLight::SetViewObject(bool v)
{
  if(v==false)
  {
    int cnt = mVecObjLight.size();
    for(int i = 0 ; i < cnt ; i++)
      mVecObjLight[i]->SetShow(false);
  }
  else
    Update();
}
//----------------------------------------------------------------------------------