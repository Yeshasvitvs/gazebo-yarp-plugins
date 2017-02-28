#include <objectattacherserver.h>

using namespace gazebo;
using namespace std;
using namespace yarp::os;

ObjectAttacherServer::ObjectAttacherServer()
{}

ObjectAttacherServer::~ObjectAttacherServer()
{}

bool ObjectAttacherServer::attach(const string& object_name, const std::string& object_link_name, const std::string& robot_name, const string& robot_link_name)
{
    gazebo::physics::ModelPtr object_model = _world->GetModel(object_name);
    if(!object_model)
    {
        yError() << "Object " << object_name << " does not exist in gazebo";
        return false;
    }
    else yInfo() << "Object " << object_model->GetName() << " found";
    
    gazebo::physics::LinkPtr object_link = object_model->GetLink(object_link_name);
    if(!object_link)
    {
        yError() << "Object link " << object_link_name << " is not found";
    }
    else yInfo() << "Object link " << object_link->GetName() << " found"; 
    
    gazebo::physics::ModelPtr robot_model = _world->GetModel(robot_name);
    if(!robot_model)
    {
        yError() << "Robot model " << robot_name << " does not exist in gazebo";
        return false;
    }
    else yInfo() << "Robot model " << robot_model->GetName() << " found";
    
    
    
    gazebo::physics::LinkPtr robot_link = robot_model->GetLink(robot_link_name);
    if(!robot_link)
    {
        yError() << "Robot link " << robot_link_name << " is not found";
        return false;
    }
    else yInfo() << "Robot link " << robot_link->GetName() << " found";
    
    //This is joint creation
    gazebo::physics::JointPtr joint;
    joint = _world->GetPhysicsEngine()->CreateJoint("revolute",object_model);
    if(!joint)
    {
        yError() << "Unable to create joint";
        return false;
    }
    
    if(!robot_link)   
    {
        yError() << "Unable to get robot link: " << robot_link_name;
        return false;
    }
    
    if(!object_link)
    {
        yError() << "Unable to get object link: " << object_name;
        return false;
    }
    
    joint->SetName("magnet_joint");
    joint->SetModel(object_model);
    joint->Load(object_link,robot_link,gazebo::math::Pose());
    joint->Attach(object_link,robot_link);
    joint->SetHighStop(0,0);
    joint->SetLowStop(0,0);
    
    return true;
}

//Takes object model name and object link name
bool ObjectAttacherServer::detach(const string& id)
{
    gazebo::physics::ModelPtr object_model = _world->GetModel(id);
    if(!object_model)
    {
        yError() << "Object " << id << " does not exist in gazebo";
        return false;
    }
    
    yError() << "^^" << object_model->GetJointCount();
    
    gazebo::physics::LinkPtr object_link = object_model->GetLink("link");
    //gazebo::physics::JointPtr j = object_link->GetJoint("magnet_joint");
    
    gazebo::physics::JointPtr joint = object_model->GetJoint("magnet_joint");
    if(!joint)
    {
        yError() << "Joint not found";
       return false;
    }
    
    return true;

}


bool ObjectAttacherServer::enableGravity(const std::string& id, const bool enable)
{
  physics::ModelPtr model=_world->GetModel(id);
    if (!model)
    {
      yError() <<"Object " << id << " does not exist in gazebo";
      return false;
    }

  model->SetGravityMode(enable);
  if (enable==true) yInfo("Gravity enabled for model %s", id.c_str());
  else yInfo("Gravity disabled for model %s", id.c_str());
    
  return true;
}