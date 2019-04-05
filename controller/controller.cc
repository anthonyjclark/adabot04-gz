//
// Publish velocity commands to the UGV
//

#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>
#include <ignition/math/Pose3.hh>

void
pose_cb(ConstPosesStampedPtr& posesStamped)
{
  // std::cout << posesStamped->DebugString();

  ::google::protobuf::int32 sec = posesStamped->time().sec();
  ::google::protobuf::int32 nsec = posesStamped->time().nsec();
  std::cout << "Read time: sec: " << sec << " nsec: " << nsec << std::endl;

  for (int i = 0; i < posesStamped->pose_size(); ++i) {
    const ::gazebo::msgs::Pose& pose = posesStamped->pose(i);
    std::string name = pose.name();
    if (name == std::string("ugv")) {
      const ::gazebo::msgs::Vector3d& position = pose.position();

      double x = position.x();
      double y = position.y();
      double z = position.z();

      std::cout << "Read position: x: " << x << " y: " << y << " z: " << z
                << " i: " << i << std::endl;
    }
  }
}

int
main(int _argc, char** _argv)
{
  // Load gazebo
  gazebo::client::setup(_argc, _argv);

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Publish to a Gazebo topic
  gazebo::transport::PublisherPtr pub =
    node->Advertise<gazebo::msgs::Pose>("~/ugv/vel_cmd");

  // Wait for a publisher to connect
  pub->WaitForConnection();

  // Listen to UGV chassis pose
  gazebo::transport::SubscriberPtr sub =
    node->Subscribe("~/pose/info", pose_cb);

  // Publisher loop
  while (true) {
    // Publish at 10 Hz
    gazebo::common::Time::MSleep(100);

    // Generate a pose
    ignition::math::Pose3d pose(1, 0, 0, 0, 0, 3.14);

    // Convert to a pose message
    gazebo::msgs::Pose msg;
    gazebo::msgs::Set(&msg, pose);

    pub->Publish(msg);
  }

  // Shut everything down.
  gazebo::client::shutdown();
}
