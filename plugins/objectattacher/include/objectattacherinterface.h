#ifndef YARPGAZEBO_OBJECTATTACHERINTERFACE
#define YARPGAZEBO_OBJECTATTACHERINTERFACE


#include <objectattacherserver.h>

namespace gazebo
{
  class ObjectAttacherInterface : public ModelPlugin
  {
  private:
      
     yarp::os::RpcServer *m_rpcport;
     yarp::os::Network *m_network;
     yarp::os::Property m_parameters;
     
     gazebo::ObjectAttacherServer m_oa_server;
     
   public:
     
      ObjectAttacherInterface();
     ~ObjectAttacherInterface();
     
      void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/);
     
  };
  
  //Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ObjectAttacherInterface)
}



#endif
