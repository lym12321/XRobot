#include "robot.hpp"

#include "system.hpp"

/* clang-format off */
Robot::WearLabIMU::Param param = {
  .bmi088_rot =
  {
    .rot_mat =
    {
      {+0, -1, +0},
      {+1, +0, +0},
      {+0, +0, +1},
    },
  },

  .led = {
    .gpio = BSP_GPIO_LED,
    .timeout = 200,
  }
};
/* clang-format on */

void robot_init() {
  auto init_thread_fn = [](void* arg) {
    static_cast<void>(arg);

    System::Init();

    Robot::WearLabIMU robot(param);

    while (1) {
      System::Thread::Sleep(UINT32_MAX);
    }
  };

  System::Thread init_thread;

  init_thread.Create(init_thread_fn, static_cast<void*>(NULL), "init_thread_fn",
                     INIT_TASK_STACK_DEPTH, System::Thread::REALTIME);
}
