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
        yError() << "Attach --> Object " << object_name << " does not exist in gazebo";
        return false;
    }
    else yInfo() << "Attach --> Object " << object_model->GetName() << " found";
    
    gazebo::physics::LinkPtr object_link = object_model->GetLink(object_link_name);
    if(!object_link)
    {
        yError() << "Attach --> Object link " << object_link_name << " is not found";
    }
    else yInfo() << "Attach --> Object link " << object_link->GetName() << " found"; 
    
    gazebo::physics::ModelPtr robot_model = _world->GetModel(robot_name);
    if(!robot_model)
    {
        yError() << "Attach --> Robot model " << robot_name << " does not exist in gazebo";
        return false;
    }
    else yInfo() << "Attach --> Robot model " << robot_model->GetName() << " found";
    
    
    
    gazebo::physics::LinkPtr robot_link = robot_model->GetLink(robot_link_name);
    if(!robot_link)
    {
        yError() << "Attach --> Robot link " << robot_link_name << " is not found";
        return false;
    }
    else yInfo() << "Attach --> Robot link " << robot_link->GetName() << " found";
    
    //This is joint creation
    gazebo::physics::JointPtr joint;
    joint = _world->GetPhysicsEngine()->CreateJoint("fixed",object_model);
    if(!joint)
    {
        yError() << "Attach --> Unable to create joint";
        return false;
    }
    
    if(!robot_link)   
    {
        yError() << "Attach --> Unable to get robot link: " << robot_link_name;
        return false;
    }
    
    if(!object_link)
    {
        yError() << "Attach --> Unable to get object link: " << object_name;
        return false;
    }
    
    std::string joint_name = object_link_name + "_magnet_joint";
    joint->SetName(joint_name);
    yInfo() << "Attach --> Magnet joint : " << joint->GetName() << " created";
    
    joint->SetModel(object_model);
    joint->Load(object_link,robot_link,gazebo::math::Pose());
    //Attach(prent_link,child_link)
    joint->Attach(object_link,robot_link);
    
    //Used in case of revolute joint
    //joint->SetHighStop(0,0);
    //joint->SetLowStop(0,0);
    //joint->SetLowerLimit(0,0);
    
    return true;
}

//Takes object model name and object link name
bool ObjectAttacherServer::detach(const string& object_name, const std::string& object_link_name)
{
    gazebo::physics::ModelPtr object_model = _world->GetModel(object_name);
    if(!object_model)
    {
        yError() << "Detach --> Object " << object_name << " does not exist in gazebo";
        return false;
    }
    else yInfo() << "Detach --> Object " << object_model->GetName() << " found";
    
    //yError() << "^^" << object_model->GetJointCount();
    
    gazebo::physics::LinkPtr object_link = object_model->GetLink(object_link_name);
    if(!object_link)
    {
        yError() << "Detach --> Object link " << object_link_name << " is not found";
        return false;
    }
    else yInfo() << "Detach --> Object link " << object_link->GetName() << " found";
    
    std::string joint_name = object_name + "::" + object_link_name + "_magnet_joint";
       
    //Get all the joints at the object link
    gazebo::physics::Joint_V joints_v = object_link->GetChildJoints();
    
    for(int i=0; i < joints_v.size(); i++)
    {
        std::string candidate_joint_name = joints_v[i]->GetScopedName();
        yInfo() << "Detach --> Child joint " << candidate_joint_name << " found";
        if(candidate_joint_name == joint_name)
        {
            gazebo::physics::JointPtr joint = joints_v[i];
            
            if(!joint)
            {
                yError() << "Detach --> Joint not found";
                return false;
            }
            else
            {
                joint->Detach();
                yInfo() << "Detach --> Found joint : " << joint->GetName() << " detached"; 
            }
            
        }
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