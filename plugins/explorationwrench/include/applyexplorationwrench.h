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

#ifndef APPLYEXPLORATIONWRENCH_H
#define APPLYEXPLORATIONWRENCH_H

#include <explorationwrench.h>
#include <boost/shared_ptr.hpp>

class RPCServerThread: public yarp::os::Thread
{
private:
    
    yarp::os::RpcServer m_rpcPort;
    yarp::os::Bottle m_cmd;
    yarp::os::Bottle m_reply;
    
    std::string m_robotName;
    physics::ModelPtr m_robotModel;
    
public:
    
    boost::mutex m_lock;
    
    boost::shared_ptr< std::vector< boost::shared_ptr<ExplorationWrench> > > wrenchesVectorPtr{new std::vector< boost::shared_ptr<ExplorationWrench>>()};
    
    
    virtual bool threadInit();
    virtual void run();
    virtual void threadRelease();
    
    void setRobotName(std::string robotName);
    void setRobotModel(physics::ModelPtr robotModel);
    
    yarp::os::Bottle getCmd();
};

namespace gazebo
{
    class ApplyExplorationWrench: public ModelPlugin
    {
    public:
        
        ApplyExplorationWrench();
        ~ApplyExplorationWrench();
        
        void Load (physics::ModelPtr _model, sdf::ElementPtr _sdf);
        void applyWrench();
   
    private:
        
        yarp::os::Network m_yarpNet;
        RPCServerThread m_rpcThread;
        yarp::os::Property m_iniParams;
        
        physics::ModelPtr m_myModel;
        std::string robotName;
        
        boost::mutex m_lock;
        event::ConnectionPtr m_updateConnection;
        
    };
}

#endif // APPLYEXPLORATIONWRENCH_H
