#include <functional>

#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>

#include <ignition/math/Helpers.hh>
#include <ignition/math/Vector2.hh>

using ignition::math::Vector3d;

namespace gazebo {
class KeyboardController : public ModelPlugin
{
public:
  KeyboardController() {}

  void Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/)
  {
    // Get a pointer to each wheel joint
    // TODO: joint names should be parameterized
    this->left_front = _model->GetJoint("front-left-wheel-joint");
    this->left_back = _model->GetJoint("back-left-wheel-joint");
    this->right_front = _model->GetJoint("front-right-wheel-joint");
    this->right_back = _model->GetJoint("back-right-wheel-joint");

    // TODO: check for invalid joints (null pointers)

    // Listen to the update event. This event is broadcast every
    // simulation iteration.
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      std::bind(&KeyboardController::OnUpdate, this));

    // Initialize transport
    this->node = transport::NodePtr(new transport::Node());
    this->node->Init();

    this->keyboardSub = this->node->Subscribe(
      "~/keyboard/keypress", &KeyboardController::OnKeyPress, this, true);

    // TODO: create reset like this
    // https://bitbucket.org/osrf/gazebo/src
    // /676a14f7642c3516e6a779ac276e8898cd7d0f79
    // /plugins/KeysToCmdVelPlugin.cc?at=default&fileviewer=file-view-default
  }

  // Called by the world update start event
public:
  void OnUpdate()
  {
    this->left_front->SetVelocity(0, left_speed);
    this->left_back->SetVelocity(0, left_speed);
    this->right_front->SetVelocity(0, right_speed);
    this->right_back->SetVelocity(0, right_speed);
  }

  void OnKeyPress(ConstAnyPtr& _msg)
  {
    const auto key = static_cast<const unsigned int>(_msg->int_value());

    std::cout << key << std::endl;

    if (key == stop_key) {
      left_speed = 0;
      right_speed = 0;
    } else if (key == forward_key) {
      left_speed = max_wheel_speed;
      right_speed = max_wheel_speed;
    } else if (key == backward_key) {
      left_speed = -max_wheel_speed;
      right_speed = -max_wheel_speed;
    } else if (key == left_key) {
      left_speed = -max_wheel_speed;
      right_speed = max_wheel_speed;
    } else if (key == right_key) {
      left_speed = max_wheel_speed;
      right_speed = -max_wheel_speed;
    }
  }

private:
  // Pointer to each wheel joint
  physics::JointPtr left_front;
  physics::JointPtr left_back;
  physics::JointPtr right_front;
  physics::JointPtr right_back;

  // Pointer to the update event connection
  event::ConnectionPtr updateConnection;

  // Control
  double max_wheel_speed = 8.0;
  double left_speed = 0.0;
  double right_speed = 0.0;

  // Keyboard codes
  unsigned int stop_key = 32;
  unsigned int forward_key = 16777235;//38;
  unsigned int backward_key = 16777237;//40;
  unsigned int left_key = 16777234;//37;
  unsigned int right_key = 16777236;//39;

  // Node for communication.
  transport::NodePtr node;

  // Subscribe to keyboard messages.
  transport::SubscriberPtr keyboardSub;
};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(KeyboardController)
}
