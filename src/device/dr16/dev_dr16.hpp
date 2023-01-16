#pragma once

#include <device.hpp>

#include "comp_cmd.hpp"
#include "comp_utils.hpp"

namespace Device {
class DR16 {
 public:
  /* 控制方式选择 */
  typedef enum {
    DR16_CTRL_SOURCE_SW,
    DR16_CTRL_SOURCE_MOUSE,
  } ControlSource;

  /* 拨杆开关 */
  typedef enum {
    DR16_SW_L_POS_TOP = 0,
    DR16_SW_L_POS_BOT,
    DR16_SW_L_POS_MID,
    DR16_SW_R_POS_TOP,
    DR16_SW_R_POS_BOT,
    DR16_SW_R_POS_MID,
    DR16_SW_POS_NUM
  } SwitchPos;

  /* 键盘按键 */
  typedef enum {
    KEY_W = DR16_SW_POS_NUM,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_SHIFT,
    KEY_CTRL,
    KEY_Q,
    KEY_E,
    KEY_R,
    KEY_F,
    KEY_G,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_L_CLICK,
    KEY_R_CLICK,
    KEY_NUM,
  } Key;

  constexpr uint32_t ShiftWith(Key key) { return key + 1 * KEY_NUM; }
  constexpr uint32_t CtrlWith(Key key) { return key + 2 * KEY_NUM; }
  constexpr uint32_t ShiftCtrlWith(Key key) { return key + 3 * KEY_NUM; }

  typedef struct __attribute__((packed)) {
    uint16_t ch_r_x : 11;
    uint16_t ch_r_y : 11;
    uint16_t ch_l_x : 11;
    uint16_t ch_l_y : 11;
    uint8_t sw_r : 2;
    uint8_t sw_l : 2;
    int16_t x;
    int16_t y;
    int16_t z;
    uint8_t press_l;
    uint8_t press_r;
    uint16_t key;
    uint16_t res;
  } Data;

  DR16();

  /**
   * @brief 开始接收
   *
   * @return true 成功
   * @return false 失败
   */
  bool StartRecv();

  /**
   * @brief 控制选择
   *
   */
  void PraseRC();

  /**
   * @brief 离线处理
   *
   */
  void Offline();

  /**
   * @brief 数据包损坏
   *
   * @return true 损坏
   * @return false 完好
   */
  bool DataCorrupted();

  static DR16::Data data_;

  Data last_data_;

  ControlSource ctrl_source_ = DR16_CTRL_SOURCE_SW;

  System::Semaphore new_;

  System::Thread thread_;

  Message::Event event_;

  Message::Topic<Component::CMD::Data> cmd_tp_;

  Component::CMD::Data cmd_;
};
}  // namespace Device
