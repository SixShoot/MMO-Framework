/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef AdapterMMOMasterH
#define AdapterMMOMasterH

#include "PrototypeMMOMaster.h"
#include <boost/smart_ptr/scoped_ptr.hpp>
#include "DstEvent.h"

class TConverterEvents;
class TWrapperMakerTransport;

class DllExport AdapterMMOMaster : public PrototypeMMOMaster, public TDstEvent
{
  boost::scoped_ptr<TConverterEvents>       mConverter;
  boost::scoped_ptr<TWrapperMakerTransport> mMakerTransport;

  void* mPtrMaster;

  enum{ID_Master=0};
public:
  AdapterMMOMaster();
  virtual ~AdapterMMOMaster();
  // base
  virtual bool Open(PrototypeMMODescOpen* pDesc, int count = 1);
  virtual void DisconnectUp();
  virtual void SendUp(TBreakPacket bp, bool check = true);
  virtual bool IsConnectUp();
  virtual bool IsConnect(unsigned int id);
  virtual void SetLoad(int procent);
  virtual void SetTimeLiveSession(unsigned int time_ms);
  virtual bool GetInfoSession(unsigned int id_session, TIP_Port& ip_port);
  // base_server
  virtual bool IsSessionSecurity(unsigned int id_session, void* crypt, int size_crypt, 
    void* login_password, int size_login_password);
  virtual void DisconnectByKeyClient(unsigned int id_client);
  virtual void SendByClientKey(std::list<unsigned int>& l, TBreakPacket bp);
  virtual void SendDown(unsigned int id_session, TBreakPacket bp, bool check = true);
  virtual int  GetCountDown();
  virtual bool GetDescDown(int index, void* pDesc, int& sizeDesc);
  // active_server
  virtual void ConnectUp(unsigned int ip, unsigned short port, unsigned char subNet = 0);
  // master
  virtual bool TryCreateGroup(std::list<unsigned int>& l_id_client, unsigned int& id_group); 
  virtual void DestroyGroup(unsigned int id_group);
  virtual void LeaveGroup(unsigned int id_client);
  virtual void GetListForGroup(unsigned int id_group, std::list<unsigned int>& l);
  virtual void SetResultLogin(bool res, unsigned int id_session, 
    unsigned int id_client, // ����, �� �� ��������
    void* resForClient, int sizeResClient);
  virtual bool FindSlaveSessionByGroup(unsigned int id_group, unsigned int& id_session);
  // prototype
  virtual PrototypeBaseModule* NewExample();
  virtual void DeleteExample(PrototypeBaseModule* ptr);

  virtual int GetVersionAdapter();
  virtual bool Work();
};

#endif
