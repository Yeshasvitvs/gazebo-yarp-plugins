// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <ObjectAttacher.h>
#include <yarp/os/idl/WireTypes.h>

namespace GazeboYarpPlugins {


class ObjectAttacher_enableGravity : public yarp::os::Portable {
public:
  std::string id;
  bool enable;
  bool _return;
  void init(const std::string& id, const bool enable);
  virtual bool write(yarp::os::ConnectionWriter& connection) override;
  virtual bool read(yarp::os::ConnectionReader& connection) override;
};

class ObjectAttacher_attachUnscoped : public yarp::os::Portable {
public:
  std::string object_name;
  std::string object_link_name;
  std::string robot_name;
  std::string robot_link_name;
  bool _return;
  void init(const std::string& object_name, const std::string& object_link_name, const std::string& robot_name, const std::string& robot_link_name);
  virtual bool write(yarp::os::ConnectionWriter& connection) override;
  virtual bool read(yarp::os::ConnectionReader& connection) override;
};

class ObjectAttacher_detachUnscoped : public yarp::os::Portable {
public:
  std::string object_name;
  std::string object_link_name;
  bool _return;
  void init(const std::string& object_name, const std::string& object_link_name);
  virtual bool write(yarp::os::ConnectionWriter& connection) override;
  virtual bool read(yarp::os::ConnectionReader& connection) override;
};

bool ObjectAttacher_enableGravity::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("enableGravity",1,1)) return false;
  if (!writer.writeString(id)) return false;
  if (!writer.writeBool(enable)) return false;
  return true;
}

bool ObjectAttacher_enableGravity::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void ObjectAttacher_enableGravity::init(const std::string& id, const bool enable) {
  _return = false;
  this->id = id;
  this->enable = enable;
}

bool ObjectAttacher_attachUnscoped::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(5)) return false;
  if (!writer.writeTag("attachUnscoped",1,1)) return false;
  if (!writer.writeString(object_name)) return false;
  if (!writer.writeString(object_link_name)) return false;
  if (!writer.writeString(robot_name)) return false;
  if (!writer.writeString(robot_link_name)) return false;
  return true;
}

bool ObjectAttacher_attachUnscoped::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void ObjectAttacher_attachUnscoped::init(const std::string& object_name, const std::string& object_link_name, const std::string& robot_name, const std::string& robot_link_name) {
  _return = false;
  this->object_name = object_name;
  this->object_link_name = object_link_name;
  this->robot_name = robot_name;
  this->robot_link_name = robot_link_name;
}

bool ObjectAttacher_detachUnscoped::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("detachUnscoped",1,1)) return false;
  if (!writer.writeString(object_name)) return false;
  if (!writer.writeString(object_link_name)) return false;
  return true;
}

bool ObjectAttacher_detachUnscoped::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void ObjectAttacher_detachUnscoped::init(const std::string& object_name, const std::string& object_link_name) {
  _return = false;
  this->object_name = object_name;
  this->object_link_name = object_link_name;
}

ObjectAttacher::ObjectAttacher() {
  yarp().setOwner(*this);
}
bool ObjectAttacher::enableGravity(const std::string& id, const bool enable) {
  bool _return = false;
  ObjectAttacher_enableGravity helper;
  helper.init(id,enable);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool ObjectAttacher::enableGravity(const std::string& id, const bool enable)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool ObjectAttacher::attachUnscoped(const std::string& object_name, const std::string& object_link_name, const std::string& robot_name, const std::string& robot_link_name) {
  bool _return = false;
  ObjectAttacher_attachUnscoped helper;
  helper.init(object_name,object_link_name,robot_name,robot_link_name);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool ObjectAttacher::attachUnscoped(const std::string& object_name, const std::string& object_link_name, const std::string& robot_name, const std::string& robot_link_name)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool ObjectAttacher::detachUnscoped(const std::string& object_name, const std::string& object_link_name) {
  bool _return = false;
  ObjectAttacher_detachUnscoped helper;
  helper.init(object_name,object_link_name);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool ObjectAttacher::detachUnscoped(const std::string& object_name, const std::string& object_link_name)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool ObjectAttacher::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  reader.expectAccept();
  if (!reader.readListHeader()) { reader.fail(); return false; }
  yarp::os::ConstString tag = reader.readTag();
  bool direct = (tag=="__direct__");
  if (direct) tag = reader.readTag();
  while (!reader.isError()) {
    // TODO: use quick lookup, this is just a test
    if (tag == "enableGravity") {
      std::string id;
      bool enable;
      if (!reader.readString(id)) {
        reader.fail();
        return false;
      }
      if (!reader.readBool(enable)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = enableGravity(id,enable);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "attachUnscoped") {
      std::string object_name;
      std::string object_link_name;
      std::string robot_name;
      std::string robot_link_name;
      if (!reader.readString(object_name)) {
        reader.fail();
        return false;
      }
      if (!reader.readString(object_link_name)) {
        reader.fail();
        return false;
      }
      if (!reader.readString(robot_name)) {
        reader.fail();
        return false;
      }
      if (!reader.readString(robot_link_name)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = attachUnscoped(object_name,object_link_name,robot_name,robot_link_name);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "detachUnscoped") {
      std::string object_name;
      std::string object_link_name;
      if (!reader.readString(object_name)) {
        reader.fail();
        return false;
      }
      if (!reader.readString(object_link_name)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = detachUnscoped(object_name,object_link_name);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "help") {
      std::string functionName;
      if (!reader.readString(functionName)) {
        functionName = "--all";
      }
      std::vector<std::string> _return=help(functionName);
      yarp::os::idl::WireWriter writer(reader);
        if (!writer.isNull()) {
          if (!writer.writeListHeader(2)) return false;
          if (!writer.writeTag("many",1, 0)) return false;
          if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<std::string> ::iterator _iterHelp;
          for (_iterHelp = _return.begin(); _iterHelp != _return.end(); ++_iterHelp)
          {
            if (!writer.writeString(*_iterHelp)) return false;
           }
          if (!writer.writeListEnd()) return false;
        }
      reader.accept();
      return true;
    }
    if (reader.noMore()) { reader.fail(); return false; }
    yarp::os::ConstString next_tag = reader.readTag();
    if (next_tag=="") break;
    tag = tag + "_" + next_tag;
  }
  return false;
}

std::vector<std::string> ObjectAttacher::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("enableGravity");
    helpString.push_back("attachUnscoped");
    helpString.push_back("detachUnscoped");
    helpString.push_back("help");
  }
  else {
    if (functionName=="enableGravity") {
      helpString.push_back("bool enableGravity(const std::string& id, const bool enable) ");
      helpString.push_back("Enable/disables gravity for an object ");
      helpString.push_back("@param id object id ");
      helpString.push_back("@param enable 1 to enable gravity, 0 otherwise ");
      helpString.push_back("@return returns true or false on success failure ");
    }
    if (functionName=="attachUnscoped") {
      helpString.push_back("bool attachUnscoped(const std::string& object_name, const std::string& object_link_name, const std::string& robot_name, const std::string& robot_link_name) ");
      helpString.push_back("Attach an object to a link of the robot - takes unscoped names as arguments ");
      helpString.push_back("@param object_name string that identifies object in gazebo (returned after creation or spawnning) ");
      helpString.push_back("@param object_link_name name of the link of the object ");
      helpString.push_back("@param robot_name name of the robot ");
      helpString.push_back("@param robot_link_name name of the link to which the object_link_name has to be attached ");
      helpString.push_back("@return true if success, false otherwise ");
    }
    if (functionName=="detachUnscoped") {
      helpString.push_back("bool detachUnscoped(const std::string& object_name, const std::string& object_link_name) ");
      helpString.push_back("Detach a previously attached object. ");
      helpString.push_back("@param object_name string that identifies object in gazebo (returned after creation or spawnning) ");
      helpString.push_back("@param object_link_name name of the link from which to detach the joint created using attachUnscoped ");
      helpString.push_back("@return true if success, false otherwise ");
    }
    if (functionName=="help") {
      helpString.push_back("std::vector<std::string> help(const std::string& functionName=\"--all\")");
      helpString.push_back("Return list of available commands, or help message for a specific function");
      helpString.push_back("@param functionName name of command for which to get a detailed description. If none or '--all' is provided, print list of available commands");
      helpString.push_back("@return list of strings (one string per line)");
    }
  }
  if ( helpString.empty()) helpString.push_back("Command not found");
  return helpString;
}
} // namespace


