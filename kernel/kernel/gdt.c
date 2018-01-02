#include <kernel/gdt.h>

void globalDescriptorTableInit()
{
    segmentDescriptorInit(&(gdt.NullSegmentSelector),0,0,0);
    segmentDescriptorInit(&(gdt.UnusedSegmentSelector), 0,0,0);
    segmentDescriptorInit(&(gdt.CodeSegmentSelector),0,64*1024*1024,0x9A);
    segmentDescriptorInit(&(gdt.DataSegmentSelector),0,64*1024*1024,0x92);

    uint32_t i[2];
    i[0] = sizeof(struct GlobalDescriptorTable) << 16;
    i[1] = (uint32_t)&gdt;

    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i)+2));
}

void segmentDescriptorInit(struct SegmentDescriptor *sd, uint32_t base, uint32_t limit, uint8_t flags)
{
    uint8_t *target = (uint8_t)&sd;

    if(limit <= 65536)
    {
        target[6] = 0x40;
    }
    else
    {
        if((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
        
        target[6] = 0xC0;
    }

    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    target[5] = flags;
}

uint32_t base(struct SegmentDescriptor *s)
{
    uint8_t *target = (uint8_t)&s;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

uint32_t limit(struct SegmentDescriptor *s)
{
    uint8_t *target = (uint8_t)&s;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];
    if((target[6] & 0xC0) == 0xC0)
        result = (result << 12) | 0xFFF;
    return result;
}

uint16_t codeSegmentSelector(struct GlobalDescriptorTable gdt)
{
    return (uint8_t *)&gdt.CodeSegmentSelector - (uint8_t *)&gdt;
}

uint16_t dataSegmentSelector(struct GlobalDescriptorTable gdt)
{
    return (uint8_t *)&gdt.DataSegmentSelector - (uint8_t *)&gdt;
}