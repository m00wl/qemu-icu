#include "qemu/osdep.h"
#include "qapi/error.h" /* for error_fatal() handler */
#include "hw/sysbus.h"  /* for sysbus register functions */
#include "hw/misc/icu.h"

static uint64_t icu_read(void *opaque, hwaddr addr, unsigned int size)
{
    IcuState *s = opaque;
    uint64_t ret = 0;
    
    if (addr % 8) {
        printf("warning:\treading from unaligned memory address\n"
               "warning:\tthis read will yield wrong data\n");
    }

    switch (addr) {
    case ICU_FEATURES:
        ret = s->features;
        break;
    case ICU_EXT_CMD:
        ret = s->ext_cmd;
        break;
    case ICU_COMMAND:
        ret = s->command;
        break;
    case ICU_DATA:
        ret = s->data;
        break;
    case ICU_ARG1:
        ret = s->arg1;
        break;
    case ICU_CUR_TIME:
        ret = s->cur_time;
        break;
    case ICU_PRINT:
        ret = s->print;
        break;
    case ICU_EP_START ... ICU_EP_END: ;
        uint64_t offset = ((addr - ICU_EP_START) / ICU_REG_SIZE);
        printf("info:\treading register %02lu of endpoint %02lu\n",
               offset % ICU_EP_REGS, offset / ICU_EP_REGS);
        ret = s->eps[offset / ICU_EP_REGS].ep[offset % ICU_EP_REGS];
        break;
    default:
        ret = 0xDDDDEEEEAAAADDDD;
        break;
    }

    printf("read:\t%04u bytes from addr:\t0x%08lx with data:\t0x%lx\n",
           size, addr, ret);

    return ret;
}

static void icu_write(void *opaque, hwaddr addr, uint64_t value,
                        unsigned int size)
{
    IcuState *s = opaque;

    if (addr % 8) {
        printf("warning:\twriting to unaligned memory address\n"
               "warning:\tthis write will fail\n");
    }

    switch (addr) {
    case ICU_FEATURES:
        s->features = value;
        break;
    case ICU_EXT_CMD:
        s->ext_cmd = value;
        break;
    case ICU_COMMAND:
        s->command = value;
        break;
    case ICU_DATA:
        s->data = value;
        break;
    case ICU_ARG1:
        s->arg1 = value;
        break;
    case ICU_CUR_TIME:
        s->cur_time = value;
        break;
    case ICU_PRINT:
        s->print = value;
        break;
    case ICU_EP_START ... ICU_EP_END: ;
        uint64_t offset = ((addr - ICU_EP_START) / ICU_REG_SIZE);
        printf("info:\twriting to register %02lu of endpoint %02lu\n",
               offset % ICU_EP_REGS, offset / ICU_EP_REGS);
        s->eps[offset / ICU_EP_REGS].ep[offset % ICU_EP_REGS] = value;
        break;
    default:
        printf("error:\tthe following write is currently not supported:");
        return;
    }

    printf("write:\t%4u bytes to addr:\t0x%08lx with data:\t0x%lx\n",
           size, addr, value);
}

static const MemoryRegionOps icu_ops = {
    .read = icu_read,
    .write = icu_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 1,
        .max_access_size = 8
    },
    .impl = {
        .min_access_size = 1,
        .max_access_size = 8
    }
};

static void icu_instance_init(Object *obj)
{
    IcuState *s = ICU(obj);
    
    memory_region_init_io(&s->iomem, obj, &icu_ops, s, TYPE_ICU, 0x1000);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->iomem);
    s->features = 0x1;
    s->ext_cmd = 0x0;
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
