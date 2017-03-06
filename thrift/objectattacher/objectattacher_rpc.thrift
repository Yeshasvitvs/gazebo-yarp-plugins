#object attacher RPC server
namespace yarp GazeboYarpPlugins

service ObjectAttacher {

     /** 
    * Enable/disables gravity for an object
    * @param id object id
    * @param enable 1 to enable gravity, 0 otherwise
    * @return returns true or false on success failure
    */
    bool enableGravity (1: string id, 2: bool enable);

    /**
    * Attach an object to a link of the robot - takes unscoped names as arguments
    * @param object_name string that identifies object in gazebo (returned after creation or spawnning)
    * @param object_link_name name of the link of the object
    * @param robot_name name of the robot
    * @param robot_link_name name of the link to which the object_link_name has to be attached
    * @return true if success, false otherwise
    */
    bool attachUnscoped(1:string object_name, 2:string object_link_name, 3:string robot_name, 4:string robot_link_name);
    
    /**
    * Detach a previously attached object.
    * @param object_name string that identifies object in gazebo (returned after creation or spawnning)
    * @param object_link_name name of the link from which to detach the joint created using attachUnscoped
    * @return true if success, false otherwise
    */
    bool detachUnscoped(1:string object_name, 2:string object_link_name);
}
