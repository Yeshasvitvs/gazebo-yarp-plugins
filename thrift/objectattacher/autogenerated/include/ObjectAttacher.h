// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_ObjectAttacher
#define YARP_THRIFT_GENERATOR_ObjectAttacher

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

namespace GazeboYarpPlugins {
  class ObjectAttacher;
}


class GazeboYarpPlugins::ObjectAttacher : public yarp::os::Wire {
public:
  ObjectAttacher();
  /**
   * Enable/disables gravity for an object
   * @param id object id
   * @param enable 1 to enable gravity, 0 otherwise
   * @return returns true or false on success failure
   */
  virtual bool enableGravity(const std::string& id, const bool enable);
  /**
   * Attach an object to a link of the robot - takes unscoped names as arguments
   * @param object_name string that identifies object in gazebo (returned after creation or spawnning)
   * @param object_link_name name of the link of the object
   * @param robot_name name of the robot
   * @param robot_link_name name of the link to which the object_link_name has to be attached
   * @return true if success, false otherwise
   */
  virtual bool attachUnscoped(const std::string& object_name, const std::string& object_link_name, const std::string& robot_name, const std::string& robot_link_name);
  /**
   * Detach a previously attached object.
   * @param object_name string that identifies object in gazebo (returned after creation or spawnning)
   * @param object_link_name name of the link from which to detach the joint created using attachUnscoped
   * @return true if success, false otherwise
   */
  virtual bool detachUnscoped(const std::string& object_name, const std::string& object_link_name);
  virtual bool read(yarp::os::ConnectionReader& connection) override;
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif
