#include <functional>

#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>

#include <ignition/math/Vector2.hh>
#include <ignition/math/Helpers.hh>


using ignition::math::Vector3d;

// https://github.com/anthonyjclark/adabot-snn/blob/master/sim/fsm/fsm.cpp

// Direction controller
struct ControlState {
  std::string name;
  double left_speed, right_speed;
  double to_left_lo, to_left_hi;
  double to_right_lo, to_right_hi;
  double to_forward_lo, to_forward_hi;

  std::string transition(double angle) {
    if (to_left_lo < angle && angle < to_left_hi) {
      return "left";
    } else if (to_right_lo < angle && angle < to_right_hi) {
      return "right";
    } else if (to_forward_lo < angle && angle < to_forward_hi) {
      return "forward";
    } else {
      return name;
    }
  }
};


namespace gazebo {
class GridPattern : public ModelPlugin
{
public:

  GridPattern()
  {

  }

  void Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/)
  {
    // Store the pointer to the model and chassis
    this->model = _model;

    // TODO: parameterize
    this->chassis = this->model->GetLink("chassis");

    // Get a pointer to each wheel joint
    // TODO: joint names should be parameterized
    this->left_front = this->model->GetJoint("front-left-wheel-joint");
    this->left_back = this->model->GetJoint("back-left-wheel-joint");
    this->right_front = this->model->GetJoint("front-right-wheel-joint");
    this->right_back = this->model->GetJoint("back-right-wheel-joint");

    // TODO: check for invalid joints (null pointers)


    // Waypoint targets
    double dist = 3.5;

    this->targets.push_back({ dist,     0, 0});
    this->targets.push_back({    0,  dist, 0});
    this->targets.push_back({ dist,  dist, 0});
    this->targets.push_back({-dist,  dist, 0});
    this->targets.push_back({    0, -dist, 0});
    this->targets.push_back({-dist, -dist, 0});
    this->targets.push_back({-dist,     0, 0});
    this->targets.push_back({ dist, -dist, 0});
    this->targets.push_back({    0,     0, 0});

    this->target_idx = 0;

    // Control
    double wheel_speed = 8;
    double fsm_rad = 10 * 0.0174533;
    double rad_buffer_factor = 2;

    this->fsm["forward"] = {"forward",
      wheel_speed, wheel_speed,
      -2*IGN_PI, -fsm_rad,
      fsm_rad, 2*IGN_PI,
      1, -1
    };
    this->fsm["left"] = {"left",
      wheel_speed, -wheel_speed,
      1, -1,
      1, -1,
      -fsm_rad/rad_buffer_factor, 2*IGN_PI
    };
    this->fsm["right"] = {"right",
      -wheel_speed, wheel_speed,
      1, -1,
      1, -1,
      -2*IGN_PI, fsm_rad/rad_buffer_factor
    };

    this->current_state = "forward";

    // Listen to the update event. This event is broadcast every
    // simulation iteration.
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      std::bind(&GridPattern::OnUpdate, this));
  }

  // Called by the world update start event
public:
  void OnUpdate()
  {
    // Current robot transform
    auto chassis_pose = this->chassis->WorldPose();
    auto rot = chassis_pose.Rot();//.Euler().Y();
    auto pos = chassis_pose.Pos();
    pos.Z(0);

    // Get angle and distance to target
    // auto local_to_global = AngleAxisd(yaw, Vector3d(0, 1, 0));
    // auto local_to_global =

    // auto Va = local_to_global * Vector3d(1, 0, 0);
    auto Va = rot * Vector3d(1, 0, 0);
    auto Vb = this->targets[this->target_idx] - pos;
    auto Vn = Vector3d(0, 0, 1);

    double target_angle = std::atan2(Va.Cross(Vb).Dot(Vn), Va.Dot(Vb));
    double target_dist = (pos - this->targets[this->target_idx]).Length();

    // Update target if close enough
    if (target_dist < 0.1) {
      if (++this->target_idx >= this->targets.size()) {
        this->target_idx = 0;
      }
    }

    // Update control state
    this->current_state = fsm[this->current_state].transition(target_angle);
    double left_speed = this->fsm[this->current_state].left_speed;
    double right_speed = this->fsm[this->current_state].right_speed;

    this->left_front->SetVelocity(0, left_speed);
    this->left_back->SetVelocity(0, left_speed);

    this->right_front->SetVelocity(0, right_speed);
    this->right_back->SetVelocity(0, right_speed);
  }

private:
  // Pointer to the model
  physics::ModelPtr model;

  // Pointer to the chassis
  physics::LinkPtr chassis;

  // Pointer to each wheel joint
  physics::JointPtr left_front;
  physics::JointPtr left_back;
  physics::JointPtr right_front;
  physics::JointPtr right_back;

  // Pointer to the update event connection
  event::ConnectionPtr updateConnection;

  // Waypoint targets
  std::vector<Vector3d> targets;
  long target_idx;

  // Control
  std::unordered_map<std::string, ControlState> fsm;
  std::string current_state;

};

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(GridPattern)
}
