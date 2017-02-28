#include <objectattacher.h>

using namespace gazebo;
using namespace std;
using namespace yarp::os;

ObjectAttacher::ObjectAttacher() : ModelPlugin(), m_network(0), m_rpcport(0)
{
}

ObjectAttacher::~ObjectAttacher()
{
  if(m_rpcport)
  {
    m_rpcport->close();
    delete m_rpcport;
  }

  if(m_network)
    delete m_network;
}

void ObjectAttacher::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
  if(m_network!=0)
    return;

  m_network = new yarp::os::Network();

  if(!yarp::os::Network::checkNetwork(GazeboYarpPlugins::yarpNetworkInitializationTimeout))
  {
    yError() << "ObjectAttacher::Load error: yarp network does not seem to be available, is the yarpserver running?";
    return;
  }

  bool configuration_loaded = false;

  if( _sdf->HasElement("yarpConfigurationFile") )
  {
    std::string ini_file_name = _sdf->Get<std::string>("yarpConfigurationFile");
    std::string ini_file_path = gazebo::common::SystemPaths::Instance()->FindFileURI(ini_file_name);

    if(ini_file_path != "" && m_parameters.fromConfigFile(ini_file_path.c_str()))
    {
      yInfo() << "Found yarpConfigurationFile: loading from " << ini_file_path;
      configuration_loaded = true;
    }

    if(!configuration_loaded)
    {
      yError() << "ObjectAttacher: Load error, could not load configuration file";
      return;
    }

    //TODO Extend this in case of more parameters
    std::string portname = m_parameters.find("name").asString();

    m_rpcport = new yarp::os::RpcServer();
    m_rpcport->open(portname);
    
  }

}

bool ObjectAttacher::attach(const string& id, const string& link_name)
{
    gazebo::physics::ModelPtr object_model_1 = _world->GetModel(id);
    if(!object_model_1)
    {
        yError() << "Object " << id << " does not exist in gazebo";
        return false;
    }
    
    gazebo::physics::JointPtr joint;
    joint = _world->GetPhysicsEngine()->CreateJoint("revolute",object_model_1);
    if(!joint)
    {
        yError() << "Unable to create joint";
        return false;
    }
    
    gazebo::physics::LinkPtr parent_link = object_model_1->GetLink();
    gazebo::physics::LinkPtr object_link = HELPER_getLink(link_name);
    
    if(!object_link)   
    {
        yError() << "Unable to get object link: " << link_name;
        return false;
    }
    
    if(!parent_link)
    {
        yError() << "Unable to get parent link: " << id;
        return false;
    }
    
    joint->SetName("magnet_joint");
    joint->SetModel(object_model_1);
    joint->Load(parent_link,object_link,gazebo::math::Pose());
    joint->Attach(parent_link,object_link);
    joint->SetHighStop(0,0);
    joint->SetLowStop(0,0);
    
    return true;
}

gazebo::physics::LinkPtr ObjectAttacher::HELPER_getLink(std::string full_scoped_link_name)
{
    size_t lastcolon = full_scoped_link_name.rfind(":");
    if (lastcolon == std::string::npos)
    {
      yError () << "Unable to parse model name: " << full_scoped_link_name;
      return gazebo::physics::LinkPtr();
    }
    std::string model_name = full_scoped_link_name.substr(0,lastcolon-1); 
    physics::ModelPtr p_model=_world->GetModel(model_name);
    if (!p_model)
    {
      yError () << "Unable to find model: " << model_name;
      return gazebo::physics::LinkPtr();
    }
    
    gazebo::physics::Link_V model_links = p_model->GetLinks();
    for(int i=0; i < model_links.size(); i++ )
    {
        std::string candidate_link = model_links[i]->GetScopedName();
        if( candidate_link==full_scoped_link_name )
        {
            return model_links[i];
        }
    }
    yError () << "Unable to find link: " << full_scoped_link_name << "belonging to model: " <<model_name;
    return gazebo::physics::LinkPtr();
}

bool ObjectAttacher::detach(const string& id)
{
    gazebo::physics::ModelPtr object_model = _world->GetModel(id);
    if(!object_model)
    {
        yError() << "Object " << id << " does not exist in gazebo";
        return false;
    }
    
    yError() << "^^" << object_model->GetJointCount();
    gazebo::physics::JointPtr joint = object_model->GetJoint("magnet_joint");
    if(!joint)
    {
        yError() << "Joint not found";
       return false;
    }
    
    return true;

}