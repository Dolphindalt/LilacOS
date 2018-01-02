#ifndef GDT_H
#define GDT_H 1

#include <stdint.h>

struct SegmentDescriptor {
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t base_hi;
    uint8_t type;
    uint8_t flags_limit_hi;
    uint8_t base_vhi;
} __attribute__((packed));

struct GlobalDescriptorTable {
    struct SegmentDescriptor NullSegmentSelector;
    struct SegmentDescriptor UnusedSegmentSelector;
    struct SegmentDescriptor CodeSegmentSelector;
    struct SegmentDescriptor DataSegmentSelector;
};

struct GlobalDescriptorTable gdt;

void globalDescriptorTableInit();
void segmentDescriptorInit(struct SegmentDescriptor *sd, uint32_t base, uint32_t limit, uint8_t flags);
uint32_t base(struct SegmentDescriptor *);
uint32_t limit(struct SegmentDescriptor *);

uint16_t codeSegmentSelector(struct GlobalDescriptorTable gdt);
uint16_t dataSegmentSelector(struct GlobalDescriptorTable gdt);

#endif