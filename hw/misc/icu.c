#include "qemu/osdep.h"
#include "qapi/error.h" /* for error_fatal() handler */
#include "hw/sysbus.h"  /* for sysbus register functions */
#include "hw/misc/icu.h"

#define TYPE_ICU "icu_rom"
typedef struct IcuState IcuState;
DECLARE_INSTANCE_CHECKER(IcuState, ICU, TYPE_ICU)

#define KEY     0xC0FFEE
#define REG_ID  0x0
#define REG_KEY 0x8
#define CHIP_ID 0x1C200001

struct IcuState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    uint64_t reg0;
    uint64_t chip_id;
};

static uint64_t icu_read(void *opaque, hwaddr addr, unsigned int size)
{
    IcuState *s = opaque;
    
    switch (addr) {
    case REG_ID:
        return s->chip_id;
        break;
    case REG_KEY:
        return s->reg0;
        break;
    default:
        return 0xDEAD;
        break;
    }
    return 0;
}

static const MemoryRegionOps icu_ops = {
    .read = icu_read,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void icu_instance_init(Object *obj)
{
    IcuState *s = ICU(obj);
    
    memory_region_init_io(&s->iomem, obj, &icu_ops, s, TYPE_ICU, 0x100);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->iomem);

    s->reg0 = KEY;
    s->chip_id = CHIP_ID;
}

/* create new type to define info about ICU device */
static const TypeInfo icu_info = {
    .name = TYPE_ICU,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(IcuState),
    .instance_init = icu_instance_init,
};
static void icu_register_types(void)
{
    type_register_static(&icu_info);
}
type_init(icu_register_types)

/* function to create the ICU device */
DeviceState *icu_create(hwaddr addr)
{
    DeviceState *dev = qdev_new(TYPE_ICU);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, addr);
    return dev;
}