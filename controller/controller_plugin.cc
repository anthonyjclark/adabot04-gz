#include <functional>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo {
class ModelPush : public ModelPlugin
{
public:
  void Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/)
  {
    // Store the pointer to the model
    this->model = _model;

    // Get a pointer to each wheel joint
    // TODO: joint names should be parameterized
    this->left_front = this->model->GetJoint("front_left_wheel_joint");
    this->left_back = this->model->GetJoint("back_left_wheel_joint");
    this->right_front = this->model->GetJoint("front_right_wheel_joint");
    this->right_back = this->model->GetJoint("back_right_wheel_joint");

    // TODO: check for invalid joints (null pointers)

    // Listen to the update event. This event is broadcast every
    // simulation iteration.
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      std::bind(&ModelPush::OnUpdate, this));
  }

  // Called by the world update start event
public:
  void OnUpdate()
  {
    double left_speed = 4;
    this->left_front->SetVelocity(0, left_speed);
    this->left_back->SetVelocity(0, left_speed);

    double right_speed = 2;
    this->right_front->SetVelocity(0, right_speed);
    this->right_back->SetVelocity(0, right_speed);
  }

private:
  // Pointer to the model
  physics::ModelPtr model;

  // Pointer to the update event connection
  event::ConnectionPtr updateConnection;

  // Pointer to each wheel joint
  physics::JointPtr left_front;
  physics::JointPtr left_back;
  physics::JointPtr right_front;
  physics::JointPtr right_back;
};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
