#ifndef HW_ICU_H
#define HW_ICU_H

#include "qom/object.h"

#define ICU_REG_SIZE  0x08

#define ICU_FEATURES  0x000
#define ICU_EXT_CMD   0x008
#define ICU_COMMAND   0x010
#define ICU_DATA      0x018
#define ICU_ARG1      0x020
#define ICU_CUR_TIME  0x028
#define ICU_PRINT     0x030
#define ICU_EP_REGS   3
#define ICU_EP_CNT    64
#define ICU_EP_START  0x038
#define ICU_EP_END    0x630

#define TYPE_ICU "riscv.icu"

typedef struct icu_ep_t { uint64_t ep[3]; } icu_ep_t;

typedef struct IcuState IcuState;
DECLARE_INSTANCE_CHECKER(IcuState, ICU, TYPE_ICU)
struct IcuState {
  /* private */
  SysBusDevice parent_obj;

  /* public */
  MemoryRegion iomem;
  uint64_t features;
  uint64_t ext_cmd;
  uint64_t command;
  uint64_t data;
  uint64_t arg1;
  uint64_t cur_time;
  uint64_t print;
  icu_ep_t eps[64];
};

DeviceState *icu_create(hwaddr);

#endif