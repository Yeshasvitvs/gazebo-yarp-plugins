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

#ifndef EXPLORATIONWRENCH_H
#define EXPLORATIONWRENCH_H

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <stdio.h>
#include <string>
#include <memory>
#include <cmath>
#include <random>

#include "gazebo/gazebo.hh"
#include <gazebo/physics/PhysicsEngine.hh>
#include <gazebo/common/Events.hh>
#include <gazebo/common/Plugin.hh>
#include <gazebo/transport/Node.hh>
#include <gazebo/physics/Link.hh>
#include <gazebo/physics/World.hh>
#include <gazebo/physics/Model.hh>
#include <GazeboYarpPlugins/common.h>
#include <gazebo/math/gzmath.hh>

#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/Bottle.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>
#include <yarp/os/Vocab.h>
#include <yarp/os/Log.h>
#include <yarp/os/LogStream.h>
#include <yarp/math/Math.h>

#include <boost/lexical_cast.hpp>

#define PI 3.14159265

using namespace gazebo;

class ExplorationWrench
{
private:
    
    static int count;
    float color[4];
    struct wrenchCommand
    {
        std::string link_name;
        gazebo::math::Vector3 force;
        gazebo::math::Vector3 torque;
        double duration;
    };
    
    gazebo::math::Vector3 force_limit;
    gazebo::math::Vector3 torque_limit;
        
    std::unique_ptr<wrenchCommand> wrenchPtr{new wrenchCommand()};
    
   double tick;
   double tock;
   gazebo::math::Vector3 *force_;
   gazebo::math::Vector3 *torque_;
   
   bool model_has_link;
   physics::ModelPtr model;
   physics::LinkPtr link;
   physics::Link_V model_links;
   
   transport::NodePtr m_node;
   transport::PublisherPtr m_visPub;
   msgs::Visual m_visualMsg;
   
   event::ConnectionPtr updateConnection;
    
public:
    
    bool duration_done;
    
    ExplorationWrench();
    ~ExplorationWrench();
    
    bool getLink();
    bool generateExplorationWrench(physics::ModelPtr&,yarp::os::Bottle&);
    bool setWrench(physics::ModelPtr&, yarp::os::Bottle&);
    void applyWrench();
};

#endif // EXPLORATIONWRENCH_H
