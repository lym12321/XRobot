#pragma once

#include "comp_actuator.hpp"
#include "comp_cmd.hpp"
#include "comp_filter.hpp"
#include "comp_mixer.hpp"
#include "comp_pid.hpp"
#include "comp_triangle.hpp"
#include "dev_mit_motor.hpp"

/*          L1              LEFT   L4  RIGHT  */
/* M_FRONT ☉---☉ M_BACK       ☉----------☉    */
/*        /     \             |          |    */
/*       /       \L2          |          |    */
/*      /         \           |          |    */
/*     ◉           ◉          ◉          ◉    */
/*      \         /           |          |    */
/*       \  ___  /L3          |          |    */
/*        /     \            ---        ---   */
/*       |   ◉   |           | |        | |   */
/*        \ ___ /            ---        ---   */
namespace Module {
class WheelLeg {
 public:
  typedef enum { LEFT, RIGHT, LEG_NUM } Leg;
  typedef enum { FRONT, BACK, LEG_MOTOR_NUM } LegMotor;

  typedef enum {
    Relax, /* 放松模式，电机不输出 */
    Break, /* 电机保持当前角度 */
    Squat, /* 稳定模式，以较低高度补偿角度 */
    Jump,  /* 短时间内移动至最高高度后，切换至稳定模式 */
  } Mode;

  typedef enum {
    ChangeModeRelax,
    ChangeModeBreak,
    ChangeModeSquat,
    ChangeModeJump,
  } ChassisEvent;

  typedef struct {
    float l1;
    float l2;
    float l3;
    float l4;

    struct {
      float high_max;
      float high_min;
    } limit;

    float leg_max_angle;

    float motor_zero[LEG_NUM * LEG_MOTOR_NUM];

    const std::vector<Component::CMD::EventMapItem> event_map;

    Component::PosActuator::Param leg_actr[LEG_NUM * LEG_MOTOR_NUM];

    Device::MitMotor::Param leg_motor[LEG_NUM * LEG_MOTOR_NUM];
  } Param;

  typedef struct {
    float motor_angle[LEG_MOTOR_NUM];
    Component::Type::Line diagonal;
    Component::Type::Polar2 whell_polar;
    Component::Type::Position2 whell_pos;
  } Feedback;

  typedef struct {
    Component::Type::Position2 whell_pos;
    float motor_angle[LEG_MOTOR_NUM];
  } Setpoint;

  WheelLeg(Param& param, float sample_freq);

  void UpdateFeedback();

  void Control();

  void SetMode(Mode mode);

  Param param_;

  float dt_;

  uint32_t last_wakeup_;

  uint32_t now_;

  Component::PosActuator* leg_actuator_[LEG_NUM * LEG_MOTOR_NUM];

  Device::MitMotor* leg_motor_[LEG_NUM * LEG_MOTOR_NUM];

  Component::Type::Eulr eulr_;

  Setpoint setpoint_[LEG_NUM];

  Feedback feedback_[LEG_NUM];

  Mode mode_ = Relax;

  System::Semaphore ctrl_lock_;

  System::Thread thread_;
};
}  // namespace Module
