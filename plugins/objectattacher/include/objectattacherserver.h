#ifndef YARPGAZEBO_OBJECTATTACHER
#define YARPGAZEBO_OBJECTATTACHER

//#include <ObjectAttacher.h>

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

#include <ObjectAttacher.h>

namespace gazebo
{
  class ObjectAttacherServer: public GazeboYarpPlugins::ObjectAttacher
  {
  private:  
     gazebo::physics::WorldPtr _world;
     gazebo::physics::ModelPtr _model;
     
   public:
     ObjectAttacherServer();
     ~ObjectAttacherServer();
     
     /**
      * Attach an object to a link of the robot.
      * @param id string that identifies object in gazebo (returned after creation)
      * @param link_name name of a fully scoped link (e.g. MODEL1::link)
      * @return true if success, false otherwise
      */
     virtual bool attachUnscoped(const std::string& object_name, const std::string& object_link_name, const std::string& robot_name, const std::string& robot_link_name);

     /**
     * Detach a previously attached object.
     * @param id string that identifies object in gazebo (returned after creation)
     * @return true if success, false otherwise
     */
     virtual bool detachUnscoped(const std::string& object_name, const std::string& object_link_name);
     
     /**
     * Enable/disables gravity for an object
     * @param id object id
     * @param enable 1 to enable gravity, 0 otherwise
     * @return returns true or false on success failure
     */
     virtual bool enableGravity(const std::string& id, const bool enable);
     
     void attachWorldPointer(gazebo::physics::WorldPtr p)
     {
         _world=p;
    }
    
    void attachModelPointer(gazebo::physics::ModelPtr p)
    {
        _model=p; 
    }

  };
  
}

#endif
