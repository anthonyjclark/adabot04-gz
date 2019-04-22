#include <functional>

#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>

#include <ignition/math/Vector2.hh>
#include <ignition/math/Helpers.hh>


using ignition::math::Vector3d;

namespace gazebo {
class KeyboardController : public ModelPlugin
{
public:

  KeyboardController()
  {

  }

  void Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/)
  {
    // Get a pointer to each wheel joint
    // TODO: joint names should be parameterized
    this->left_front = this->_model->GetJoint("front-left-wheel-joint");
    this->left_back = this->_model->GetJoint("back-left-wheel-joint");
    this->right_front = this->_model->GetJoint("front-right-wheel-joint");
    this->right_back = this->_model->GetJoint("back-right-wheel-joint");

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

void OnKeyPress(ConstAnyPtr &_msg)
{
  const auto key = static_cast<const unsigned int>(_msg->int_value());

  double linearVel = 0., angularVel = 0.;
  bool linearVelSet = false, angularVelSet = false;

  auto &message = this->dataPtr->keyboardControlMessage;

  if (key == STOP_KEY)
  {
    linearVel = 0.;
    linearVelSet = true;

    angularVel = 0.;
    angularVelSet = true;
  }
  else if (key == FORWARD_KEY) {
      linearVel = this->dataPtr->maxLinearVel;
      linearVelSet = true;
    }
    else if (key == BACKWARD_KEY)
    {
      linearVel = this->dataPtr->minLinearVel;
      linearVelSet = true;
    }

    if (linearVelSet)
    {
      const auto oldLinearVel = message->position().x();

      if (!ignition::math::equal(linearVel, oldLinearVel))
      {
        const auto increment = ignition::math::signum(linearVel) *
            this->dataPtr->linearIncrement;
        linearVel = ignition::math::clamp(oldLinearVel + increment,
          this->dataPtr->minLinearVel, this->dataPtr->maxLinearVel);
      }
    }

    if (std::find(this->dataPtr->keys->left.begin(),
                  this->dataPtr->keys->left.end(), key) !=
      this->dataPtr->keys->left.end())
    {
      angularVel = -this->dataPtr->maxAngularVel;
      angularVelSet = true;
    }
    else if (std::find(this->dataPtr->keys->right.begin(),
                       this->dataPtr->keys->right.end(), key)
      != this->dataPtr->keys->right.end())
    {
      angularVel = this->dataPtr->maxAngularVel;
      angularVelSet = true;
    }

    if (angularVelSet)
    {
      const auto oldAngularVel =
        msgs::ConvertIgn(message->orientation()).Euler().Z();

      if (!ignition::math::equal(angularVel, oldAngularVel))
      {
        const auto increment = ignition::math::signum(angularVel) *
            this->dataPtr->angularIncrement;
        angularVel = ignition::math::clamp(oldAngularVel + increment,
            -this->dataPtr->maxAngularVel, this->dataPtr->maxAngularVel);
      }
    }
  }

  if (linearVelSet)
  {
    message->mutable_position()->set_x(linearVel);
  }

  if (angularVelSet)
  {
    auto yaw = ignition::math::Quaterniond::EulerToQuaternion(
      0, 0, angularVel);
    msgs::Set(message->mutable_orientation(), yaw);
  }

  if (linearVelSet || angularVelSet)
  {
    this->dataPtr->cmdVelPub->Publish(*message);
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


  // Maximum linear velocity (for forward driving, positive) (m/s).
  double maxLinearVel = 1.0;

  // Maximum angular velocity (positive value) (rad/s).
  double maxAngularVel = 1.0;

  // The value to add/subtract every time the linear velocity key is
  /// pressed (strictly positive value, m/s).
  double linearIncrement = 0.5;

  // The value to add/subtract every time the angular velocity key is
  /// pressed (strictly positive value, m/s).
  double angularIncrement = 0.5;

  // Node for communication.
  transport::NodePtr node;

  // Subscribe to keyboard messages.
  transport::SubscriberPtr keyboardSub;


};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(KeyboardController)
}
