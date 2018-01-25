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

#include "applyexplorationwrench.h"

namespace gazebo
{
    GZ_REGISTER_MODEL_PLUGIN(ApplyExplorationWrench)

ApplyExplorationWrench::ApplyExplorationWrench()
{
}

ApplyExplorationWrench::~ApplyExplorationWrench()
{
    m_rpcThread.stop();
    this->m_updateConnection.reset();
}

void ApplyExplorationWrench::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
    if (!this->m_yarpNet.checkNetwork())
    {
        yError() << "ApplyExplorationWrench: yarp network not found";
        return;
    }

    this->m_myModel = _model;

    bool configuration_loaded = false;

    // Read robot name
    if (_sdf->HasElement("robotNamefromConfigFile"))
    {
        std::string iniRobotName = _sdf->Get<std::string>("robotNamefromConfigFile");
        std::string iniRobotNamePath = gazebo::common::SystemPaths::Instance()->FindFileURI(iniRobotName);

        if(iniRobotNamePath != "" && this->m_iniParams.fromConfigFile(iniRobotNamePath.c_str())) 
        {
            yarp::os::Value robotNameParam = m_iniParams.find("gazeboYarpPluginsRobotName");
            this->robotName = robotNameParam.asString();

            m_rpcThread.setRobotName(robotName);
            m_rpcThread.setRobotModel(_model);
            configuration_loaded = true;
            
        } 
        else
        {
            yError() << "ApplyExplorationWrench: Failed to load configuration file";
            return;
        }
    }
    else
    {
        this->robotName = _model->GetName();
        m_rpcThread.setRobotName(robotName);
        configuration_loaded = true;
        
    }

    // Starting RPC thread to read desired wrench to be applied
    if(!m_rpcThread.start())
    {
        yError() << "ApplyExplorationWrench: Failed to start RPCThread";
    }

    this->m_updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&ApplyExplorationWrench::applyWrench,this));
}

}

void ApplyExplorationWrench::applyWrench()
{
    m_rpcThread.m_lock.lock();
    for(int i = 0; i < m_rpcThread.wrenchesVectorPtr->size(); i++)
    {
        bool duration_check = m_rpcThread.wrenchesVectorPtr->at(i)->duration_done;
        if(duration_check==false)
        {
            m_rpcThread.wrenchesVectorPtr->at(i)->applyWrench();
        }
    }
    m_rpcThread.m_lock.unlock();
}

void RPCServerThread::setRobotModel(physics::ModelPtr robotModel)
{
    m_robotModel = robotModel;
}


bool RPCServerThread::threadInit()
{
    if(!m_rpcPort.open(std::string ("/"+m_robotName + "/applyExplorationWrench/rpc:i").c_str()))
    {
        yError() << "ApplyExplorationWrench: RPC port /applyExplorationWrench failed to open";
        return false;
    }

    return true;
}

void RPCServerThread::run()
{
    while(!isStopping())
    {
        yarp::os::Bottle command;
        m_rpcPort.read ( command,true );
        if(command.get(0).asString() == "help")
        {
            this->m_reply.addVocab ( yarp::os::Vocab::encode ( "many" ) );
            this->m_reply.addString ( "Insert a command with the following format:" );
            this->m_reply.addString ( "[link] [duration]" );
            this->m_reply.addString ( "e.g. second_link_handle 10");
            this->m_reply.addString ( "[link]:     (string) Link ID of the robot as specified in robot's SDF" );
            this->m_reply.addString ( "[duration]: (double) Duration of the applied force in seconds" );
            this->m_reply.addString ( "Note: The reference frame is the base/root robot frame with x pointing backwards and z upwards.");
            this->m_rpcPort.reply ( this->m_reply );
        }
        else{
            if((command.size() == 2) && command.get(0).isString() && (command.get(1).isDouble() || command.get(1).isInt()))
            {
                this->m_reply.addString ( "[ACK] Correct command format" );
                this->m_rpcPort.reply ( m_reply );
                m_lock.lock();
                
                // new-command flag
                command.addInt(1);
                m_cmd = command;
                m_lock.unlock();

               boost::shared_ptr<ExplorationWrench> newWrench(new ExplorationWrench);
               if(newWrench->setWrench(m_robotModel,m_cmd))
               {
                   wrenchesVectorPtr->push_back(newWrench);
               }
               else yError() << "ApplyExplorationWrench: Failed to start a new wrench";
            }
            else
            {
                this->m_reply.clear();
                this->m_reply.addString ( "ERROR: Incorrect command format" );
                this->m_rpcPort.reply ( this->m_reply );
            }
        }
        m_reply.clear();
        command.clear();
    }
}

void RPCServerThread::setRobotName ( std::string robotName )
{
    this->m_robotName = robotName;
}

void RPCServerThread::threadRelease()
{
    yarp::os::Thread::threadRelease();
    m_rpcPort.close();
}

yarp::os::Bottle RPCServerThread::getCmd()
{
    return m_cmd;
}
