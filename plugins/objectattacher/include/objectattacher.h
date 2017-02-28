#ifndef YARPGAZEBO_OBJECTATTACHER
#define YARPGAZEBO_OBJECTATTACHER

#include <map>

#include "gazebo/gazebo.hh"
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/World.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/physics/Link.hh>
#include <gazebo/physics/PhysicsEngine.hh>
#include <gazebo/common/Events.hh>

#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Property.h>
#include <yarp/os/Log.h>
#include <yarp/os/LogStream.h>
#include <GazeboYarpPlugins/common.h>
#include <GazeboYarpPlugins/ConfHelpers.hh>

namespace gazebo
{
  class ObjectAttacher : public ModelPlugin
  {
  private:
     event::ConnectionPtr updateConnection;

     yarp::os::RpcServer *m_rpcport;
     yarp::os::Network *m_network;
     yarp::os::Property m_parameters;
     
     gazebo::physics::WorldPtr _world;
     gazebo::physics::ModelPtr _model;
     
   public:
     ObjectAttacher();
     ~ObjectAttacher();
     
     /**
      * Attach an object to a link of the robot.
      * @param id string that identifies object in gazebo (returned after creation)
      * @param link_name name of a fully scoped link (e.g. MODEL1::link)
      * @return true if success, false otherwise
      */
     virtual bool attach(const std::string& id, const std::string& link_name);

     /**
     * Detach a previously attached object.
     * @param id string that identifies object in gazebo (returned after creation)
     * @return true if success, false otherwise
     */
     virtual bool detach(const std::string& id);

     void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/);
     void onUpdate(const gazebo::common::UpdateInfo & /*_info*/);
     
     gazebo::physics::LinkPtr HELPER_getLink(std::string full_scoped_link_name);
  };
  
  //Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ObjectAttacher)
}



#endif
