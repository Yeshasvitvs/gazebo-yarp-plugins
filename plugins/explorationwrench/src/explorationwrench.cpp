/*
 * Copyright (c) 2018, <copyright holder> <email>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "explorationwrench.h"

int ExplorationWrench::count = 0;

ExplorationWrench::ExplorationWrench()
{
    
    srand(rand()%100);
    color[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    color[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    color[2] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    color[3] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    
    count++;
    tick = yarp::os::Time::now();
    duration_done = false;
    
    force_limit = (0.25,0.25,0.25);
    torque_limit =  (0.25,0.25,0.25);
}

bool ExplorationWrench::getLink()
{
    model_links = model->GetLinks();
    
    for(int i = 0; i < model_links.size(); i++)
    {
        
        std::string candidate_link_name = model_links[i]->GetScopedName();
        
        std::size_t lastcolon = candidate_link_name.rfind(":");
        std::string unscoped_link_name =  candidate_link_name.substr(lastcolon+1,std::string::npos);
        
        if(unscoped_link_name == wrenchPtr->link_name)
        {
            link = model_links[i];
            
            //Wrench Visual
            this->m_node = transport::NodePtr(new gazebo::transport::Node());
            this->m_node->Init(model->GetWorld()->GetName());
            m_visPub = this->m_node->Advertise<msgs::Visual>("~/visual",100);
            
            // Set the visual's name. This should be unique.
            std::string visual_name = "__" + wrenchPtr->link_name + "__CYLINDER_VISUAL__" + boost::lexical_cast<std::string>(count);
            m_visualMsg.set_name (visual_name);

            // Set the visual's parent. This visual will be attached to the parent
            m_visualMsg.set_parent_name(model->GetScopedName());

            // Create a cylinder
            msgs::Geometry *geomMsg = m_visualMsg.mutable_geometry();
            geomMsg->set_type(msgs::Geometry::CYLINDER);
            geomMsg->mutable_cylinder()->set_radius(0.0015);
            geomMsg->mutable_cylinder()->set_length(.15);

            // Don't cast shadows
            m_visualMsg.set_cast_shadows(false);
            model_has_link = true;
            break;
        }
    }
    if(!model_has_link)
    {
        yError() << "ExplorationWrench: could not find the link in the gazebo model";
        return false;
    }
    return true;
}

bool ExplorationWrench::setWrench(physics::ModelPtr& _model, yarp::os::Bottle& cmd)
{
    model = _model;
    wrenchPtr->link_name = cmd.get(0).asString();
    
    if(!getLink()) 
    {
        yError() << "ApplyExplorationWrench: specified link is not found in the gazebo model";
        return false;
    }
    
    wrenchPtr->duration = cmd.get(1).asDouble();
    wrenchPtr->frequency = 1/wrenchPtr->duration;
    
    return true;
}


bool ExplorationWrench::applyWrench()
{
    std::random_device seed; //Will be used to obtain a seed for the random number engine
    std::mt19937 eng(seed()); //Standard mersenne_twister_engine seeded with rd()
        
    std::uniform_real_distribution<> randomInitForceX(-force_limit.x, force_limit.x);
    std::uniform_real_distribution<> randomInitForceY(-force_limit.y, force_limit.y);
    std::uniform_real_distribution<> randomInitForceZ(-force_limit.z, force_limit.z);
    
    gazebo::math::Vector3 randomInitForce;
    randomInitForce = (randomInitForceX(eng),randomInitForceY(eng),randomInitForceZ(eng));
    
    std::uniform_real_distribution<> randomInitTorqueX(-torque_limit.x, torque_limit.x);
    std::uniform_real_distribution<> randomInitTorqueY(-torque_limit.y, torque_limit.y);
    std::uniform_real_distribution<> randomInitTorqueZ(-torque_limit.z, torque_limit.z);
            
    gazebo::math::Vector3 randomInitTorque;
    randomInitTorque = (randomInitTorqueX(eng),randomInitTorqueY(eng),randomInitTorqueZ(eng));
    
    yInfo() << "Initial Wrench Values : " << randomInitForce.x << " " << randomInitForce.y << " " << randomInitForce.z \
                                        << " " << randomInitTorque.x << " " << randomInitTorque.y << " " << randomInitTorque.z;
         
    tock = yarp::os::Time::now();
    double time_elapsed = tock-tick; 
    if( time_elapsed < wrenchPtr->duration)
    {
        yInfo() << "Time elapsed: " << time_elapsed;
        yInfo() << "Frequency:" << wrenchPtr->frequency;
        yInfo() << "f*t: " << wrenchPtr->frequency*time_elapsed;
        yInfo() << "Sine Value: " << sin(2*PI*wrenchPtr->frequency*time_elapsed);
        wrenchPtr->force.x = randomInitForce.x*sin(2*PI*wrenchPtr->frequency*time_elapsed);
        wrenchPtr->force.y = randomInitForce.y*sin(2*PI*wrenchPtr->frequency*time_elapsed);
        wrenchPtr->force.z = randomInitForce.z*sin(2*PI*wrenchPtr->frequency*time_elapsed);
        
        wrenchPtr->torque.x = randomInitTorque.x*sin(2*PI*wrenchPtr->frequency*time_elapsed);
        wrenchPtr->torque.y = randomInitTorque.y*sin(2*PI*wrenchPtr->frequency*time_elapsed);
        wrenchPtr->torque.z = randomInitTorque.z*sin(2*PI*wrenchPtr->frequency*time_elapsed);
                   
        link->AddForce(wrenchPtr->force);
        link->AddTorque(wrenchPtr->torque);
            
        math::Vector3 linkCoGPos = link->GetWorldCoGPose().pos;
        math::Vector3 newZ = wrenchPtr->force.Normalize();
        math::Vector3 newX = newZ.Cross(math::Vector3::UnitZ);
        math::Vector3 newY = newZ.Cross(newX);
        math::Matrix4 rotation = math::Matrix4 (newX[0],newY[0],newZ[0],0,newX[1],newY[1],newZ[1],0,newX[2],newY[2],newZ[2],0, 0, 0, 0, 1);
        math::Quaternion forceOrientation = rotation.GetRotation();
        math::Pose linkCoGPose (linkCoGPos - rotation*math::Vector3(0,0,0.075),forceOrientation);
        
        tock = yarp::os::Time::now();
        
        #if GAZEBO_MAJOR_VERSION >= 7
            msgs::Set(m_visualMsg.mutable_pose(), linkCoGPose.Ign());
        #else
            msgs::Set(m_visualMsg.mutable_pose(), linkCoGPose);
        #endif
        
        msgs::Set(m_visualMsg.mutable_material()->mutable_ambient(),common::Color(color[0],color[1],color[2],color[3]));
        m_visualMsg.set_visible(1);
        m_visPub->Publish(m_visualMsg);
    }
    else
    {
        m_visualMsg.set_visible(0);
        m_visPub->Publish(m_visualMsg);
        duration_done = true;
    }
}

ExplorationWrench::~ExplorationWrench()
{
    count--;
}


