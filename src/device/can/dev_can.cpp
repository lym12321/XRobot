#include "dev_can.hpp"

#include <stdbool.h>
#include <string.h>

#include "comp_utils.hpp"
#include "om.h"

using namespace Device;

Message::Topic<CAN::Pack>* CAN::can_tp_[BSP_CAN_NUM];
System::Semaphore* CAN::can_sem_[BSP_CAN_NUM];

static CAN::Pack pack;

CAN::CAN() {
  bsp_can_init();

  for (int i = 0; i < BSP_CAN_NUM; i++) {
    can_tp_[i] = static_cast<Message::Topic<CAN::Pack>*>(
        System::Memory::Malloc(sizeof(Message::Topic<CAN::Pack>)));
    new (can_tp_[i])
        Message::Topic<CAN::Pack>(("dev_can_" + std::to_string(i)).c_str());

    can_sem_[i] = static_cast<System::Semaphore*>(
        System::Memory::Malloc(sizeof(System::Semaphore)));
    new (can_sem_[i])
        System::Semaphore(BSP_CAN_MAILBOX_NUM, BSP_CAN_MAILBOX_NUM);
  }

  auto rx_callback = [](bsp_can_t can, void* arg) {
    (void)(arg);

    while (bsp_can_get_msg(can, pack.data, &(pack.index)) == BSP_OK) {
      can_tp_[can]->PublishFromISR(pack);
    }
  };

  auto tx_cplt_callback = [](bsp_can_t can, void* arg) {
    (void)(arg);

    CAN::can_sem_[can]->GiveFromISR();
  };

  for (int i = 0; i < BSP_CAN_NUM; i++) {
    bsp_can_register_callback((bsp_can_t)i, CAN_RX_MSG_CALLBACK, rx_callback,
                              NULL);
    bsp_can_register_callback((bsp_can_t)i, CAN_TX_CPLT_CALLBACK,
                              tx_cplt_callback, NULL);
  }
}

bool CAN::SendPack(bsp_can_t can, Pack& pack) {
  CAN::can_sem_[can]->Take(UINT32_MAX);

  return bsp_can_trans_packet(can, pack.index, pack.data) == BSP_OK;
}

bool CAN::Subscribe(Message::Topic<CAN::Pack>& tp, bsp_can_t can,
                    uint32_t index, uint32_t num) {
  ASSERT(num > 0);

  can_tp_[can]->RangeDivide(tp, OM_PRASE_STRUCT(Pack, index), index, num - 1);
  return true;
}
