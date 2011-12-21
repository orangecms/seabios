//  QEMU Cirrus CLGD 54xx VGABIOS Extension.
//
// Copyright (C) 2009  Kevin O'Connor <kevin@koconnor.net>
//  Copyright (c) 2004 Makoto Suzuki (suzu)
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include "vgatables.h" // cirrus_init
#include "biosvar.h" // GET_GLOBAL
#include "util.h" // dprintf


/****************************************************************
 * tables
 ****************************************************************/

struct cirrus_mode_s {
    /* + 0 */
    u16 mode;
    u16 width;
    u16 height;
    u16 depth;
    /* + 8 */
    u16 hidden_dac; /* 0x3c6 */
    u16 *seq; /* 0x3c4 */
    u16 *graph; /* 0x3ce */
    u16 *crtc; /* 0x3d4 */
    /* +16 */
    u8 bitsperpixel;
    u8 vesacolortype;
    u8 vesaredmask;
    u8 vesaredpos;
    u8 vesagreenmask;
    u8 vesagreenpos;
    u8 vesabluemask;
    u8 vesabluepos;
    /* +24 */
    u8 vesareservedmask;
    u8 vesareservedpos;
};

/* VGA */
static u16 cseq_vga[] VAR16 = {0x0007,0xffff};
static u16 cgraph_vga[] VAR16 = {0x0009,0x000a,0x000b,0xffff};
static u16 ccrtc_vga[] VAR16 = {0x001a,0x001b,0x001d,0xffff};

/* extensions */
static u16 cgraph_svgacolor[] VAR16 = {
    0x0000,0x0001,0x0002,0x0003,0x0004,0x4005,0x0506,0x0f07,0xff08,
    0x0009,0x000a,0x000b,
    0xffff
};
/* 640x480x8 */
static u16 cseq_640x480x8[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1107,
    0x580b,0x580c,0x580d,0x580e,
    0x0412,0x0013,0x2017,
    0x331b,0x331c,0x331d,0x331e,
    0xffff
};
static u16 ccrtc_640x480x8[] VAR16 = {
    0x2c11,
    0x5f00,0x4f01,0x4f02,0x8003,0x5204,0x1e05,0x0b06,0x3e07,
    0x4009,0x000c,0x000d,
    0xea10,0xdf12,0x5013,0x4014,0xdf15,0x0b16,0xc317,0xff18,
    0x001a,0x221b,0x001d,
    0xffff
};
/* 640x480x16 */
static u16 cseq_640x480x16[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1707,
    0x580b,0x580c,0x580d,0x580e,
    0x0412,0x0013,0x2017,
    0x331b,0x331c,0x331d,0x331e,
    0xffff
};
static u16 ccrtc_640x480x16[] VAR16 = {
    0x2c11,
    0x5f00,0x4f01,0x4f02,0x8003,0x5204,0x1e05,0x0b06,0x3e07,
    0x4009,0x000c,0x000d,
    0xea10,0xdf12,0xa013,0x4014,0xdf15,0x0b16,0xc317,0xff18,
    0x001a,0x221b,0x001d,
    0xffff
};
/* 640x480x24 */
static u16 cseq_640x480x24[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1507,
    0x580b,0x580c,0x580d,0x580e,
    0x0412,0x0013,0x2017,
    0x331b,0x331c,0x331d,0x331e,
    0xffff
};
static u16 ccrtc_640x480x24[] VAR16 = {
    0x2c11,
    0x5f00,0x4f01,0x4f02,0x8003,0x5204,0x1e05,0x0b06,0x3e07,
    0x4009,0x000c,0x000d,
    0xea10,0xdf12,0x0013,0x4014,0xdf15,0x0b16,0xc317,0xff18,
    0x001a,0x321b,0x001d,
    0xffff
};
/* 800x600x8 */
static u16 cseq_800x600x8[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1107,
    0x230b,0x230c,0x230d,0x230e,
    0x0412,0x0013,0x2017,
    0x141b,0x141c,0x141d,0x141e,
    0xffff
};
static u16 ccrtc_800x600x8[] VAR16 = {
    0x2311,0x7d00,0x6301,0x6302,0x8003,0x6b04,0x1a05,0x9806,0xf007,
    0x6009,0x000c,0x000d,
    0x7d10,0x5712,0x6413,0x4014,0x5715,0x9816,0xc317,0xff18,
    0x001a,0x221b,0x001d,
    0xffff
};
/* 800x600x16 */
static u16 cseq_800x600x16[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1707,
    0x230b,0x230c,0x230d,0x230e,
    0x0412,0x0013,0x2017,
    0x141b,0x141c,0x141d,0x141e,
    0xffff
};
static u16 ccrtc_800x600x16[] VAR16 = {
    0x2311,0x7d00,0x6301,0x6302,0x8003,0x6b04,0x1a05,0x9806,0xf007,
    0x6009,0x000c,0x000d,
    0x7d10,0x5712,0xc813,0x4014,0x5715,0x9816,0xc317,0xff18,
    0x001a,0x221b,0x001d,
    0xffff
};
/* 800x600x24 */
static u16 cseq_800x600x24[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1507,
    0x230b,0x230c,0x230d,0x230e,
    0x0412,0x0013,0x2017,
    0x141b,0x141c,0x141d,0x141e,
    0xffff
};
static u16 ccrtc_800x600x24[] VAR16 = {
    0x2311,0x7d00,0x6301,0x6302,0x8003,0x6b04,0x1a05,0x9806,0xf007,
    0x6009,0x000c,0x000d,
    0x7d10,0x5712,0x2c13,0x4014,0x5715,0x9816,0xc317,0xff18,
    0x001a,0x321b,0x001d,
    0xffff
};
/* 1024x768x8 */
static u16 cseq_1024x768x8[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1107,
    0x760b,0x760c,0x760d,0x760e,
    0x0412,0x0013,0x2017,
    0x341b,0x341c,0x341d,0x341e,
    0xffff
};
static u16 ccrtc_1024x768x8[] VAR16 = {
    0x2911,0xa300,0x7f01,0x7f02,0x8603,0x8304,0x9405,0x2406,0xf507,
    0x6009,0x000c,0x000d,
    0x0310,0xff12,0x8013,0x4014,0xff15,0x2416,0xc317,0xff18,
    0x001a,0x221b,0x001d,
    0xffff
};
/* 1024x768x16 */
static u16 cseq_1024x768x16[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1707,
    0x760b,0x760c,0x760d,0x760e,
    0x0412,0x0013,0x2017,
    0x341b,0x341c,0x341d,0x341e,
    0xffff
};
static u16 ccrtc_1024x768x16[] VAR16 = {
    0x2911,0xa300,0x7f01,0x7f02,0x8603,0x8304,0x9405,0x2406,0xf507,
    0x6009,0x000c,0x000d,
    0x0310,0xff12,0x0013,0x4014,0xff15,0x2416,0xc317,0xff18,
    0x001a,0x321b,0x001d,
    0xffff
};
/* 1024x768x24 */
static u16 cseq_1024x768x24[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1507,
    0x760b,0x760c,0x760d,0x760e,
    0x0412,0x0013,0x2017,
    0x341b,0x341c,0x341d,0x341e,
    0xffff
};
static u16 ccrtc_1024x768x24[] VAR16 = {
    0x2911,0xa300,0x7f01,0x7f02,0x8603,0x8304,0x9405,0x2406,0xf507,
    0x6009,0x000c,0x000d,
    0x0310,0xff12,0x8013,0x4014,0xff15,0x2416,0xc317,0xff18,
    0x001a,0x321b,0x001d,
    0xffff
};
/* 1280x1024x8 */
static u16 cseq_1280x1024x8[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1107,
    0x760b,0x760c,0x760d,0x760e,
    0x0412,0x0013,0x2017,
    0x341b,0x341c,0x341d,0x341e,
    0xffff
};
static u16 ccrtc_1280x1024x8[] VAR16 = {
    0x2911,0xc300,0x9f01,0x9f02,0x8603,0x8304,0x9405,0x2406,0xf707,
    0x6009,0x000c,0x000d,
    0x0310,0xff12,0xa013,0x4014,0xff15,0x2416,0xc317,0xff18,
    0x001a,0x221b,0x001d,
    0xffff
};
/* 1280x1024x16 */
static u16 cseq_1280x1024x16[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1707,
    0x760b,0x760c,0x760d,0x760e,
    0x0412,0x0013,0x2017,
    0x341b,0x341c,0x341d,0x341e,
    0xffff
};
static u16 ccrtc_1280x1024x16[] VAR16 = {
    0x2911,0xc300,0x9f01,0x9f02,0x8603,0x8304,0x9405,0x2406,0xf707,
    0x6009,0x000c,0x000d,
    0x0310,0xff12,0x4013,0x4014,0xff15,0x2416,0xc317,0xff18,
    0x001a,0x321b,0x001d,
    0xffff
};

/* 1600x1200x8 */
static u16 cseq_1600x1200x8[] VAR16 = {
    0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1107,
    0x760b,0x760c,0x760d,0x760e,
    0x0412,0x0013,0x2017,
    0x341b,0x341c,0x341d,0x341e,
    0xffff
};
static u16 ccrtc_1600x1200x8[] VAR16 = {
    0x2911,0xc300,0x9f01,0x9f02,0x8603,0x8304,0x9405,0x2406,0xf707,
    0x6009,0x000c,0x000d,
    0x0310,0xff12,0xa013,0x4014,0xff15,0x2416,0xc317,0xff18,
    0x001a,0x221b,0x001d,
    0xffff
};

static struct cirrus_mode_s cirrus_modes[] VAR16 = {
    {0x5f,640,480,8,0x00,
     cseq_640x480x8,cgraph_svgacolor,ccrtc_640x480x8,8,
     4,0,0,0,0,0,0,0,0},
    {0x64,640,480,16,0xe1,
     cseq_640x480x16,cgraph_svgacolor,ccrtc_640x480x16,16,
     6,5,11,6,5,5,0,0,0},
    {0x66,640,480,15,0xf0,
     cseq_640x480x16,cgraph_svgacolor,ccrtc_640x480x16,16,
     6,5,10,5,5,5,0,1,15},
    {0x71,640,480,24,0xe5,
     cseq_640x480x24,cgraph_svgacolor,ccrtc_640x480x24,24,
     6,8,16,8,8,8,0,0,0},

    {0x5c,800,600,8,0x00,
     cseq_800x600x8,cgraph_svgacolor,ccrtc_800x600x8,8,
     4,0,0,0,0,0,0,0,0},
    {0x65,800,600,16,0xe1,
     cseq_800x600x16,cgraph_svgacolor,ccrtc_800x600x16,16,
     6,5,11,6,5,5,0,0,0},
    {0x67,800,600,15,0xf0,
     cseq_800x600x16,cgraph_svgacolor,ccrtc_800x600x16,16,
     6,5,10,5,5,5,0,1,15},

    {0x60,1024,768,8,0x00,
     cseq_1024x768x8,cgraph_svgacolor,ccrtc_1024x768x8,8,
     4,0,0,0,0,0,0,0,0},
    {0x74,1024,768,16,0xe1,
     cseq_1024x768x16,cgraph_svgacolor,ccrtc_1024x768x16,16,
     6,5,11,6,5,5,0,0,0},
    {0x68,1024,768,15,0xf0,
     cseq_1024x768x16,cgraph_svgacolor,ccrtc_1024x768x16,16,
     6,5,10,5,5,5,0,1,15},

    {0x78,800,600,24,0xe5,
     cseq_800x600x24,cgraph_svgacolor,ccrtc_800x600x24,24,
     6,8,16,8,8,8,0,0,0},
    {0x79,1024,768,24,0xe5,
     cseq_1024x768x24,cgraph_svgacolor,ccrtc_1024x768x24,24,
     6,8,16,8,8,8,0,0,0},

    {0x6d,1280,1024,8,0x00,
     cseq_1280x1024x8,cgraph_svgacolor,ccrtc_1280x1024x8,8,
     4,0,0,0,0,0,0,0,0},
    {0x69,1280,1024,15,0xf0,
     cseq_1280x1024x16,cgraph_svgacolor,ccrtc_1280x1024x16,16,
     6,5,10,5,5,5,0,1,15},
    {0x75,1280,1024,16,0xe1,
     cseq_1280x1024x16,cgraph_svgacolor,ccrtc_1280x1024x16,16,
     6,5,11,6,5,5,0,0,0},

    {0x7b,1600,1200,8,0x00,
     cseq_1600x1200x8,cgraph_svgacolor,ccrtc_1600x1200x8,8,
     4,0,0,0,0,0,0,0,0},

    {0xfe,0,0,0,0,cseq_vga,cgraph_vga,ccrtc_vga,0,
     0xff,0,0,0,0,0,0,0,0},
};


/****************************************************************
 * helper functions
 ****************************************************************/

static struct cirrus_mode_s *
cirrus_get_modeentry(u8 mode)
{
    struct cirrus_mode_s *table_g = cirrus_modes;
    while (table_g < &cirrus_modes[ARRAY_SIZE(cirrus_modes)]) {
        u16 tmode = GET_GLOBAL(table_g->mode);
        if (tmode == mode)
            return table_g;
        table_g++;
    }
    return NULL;
}

static void
cirrus_switch_mode_setregs(u16 *data, u16 port)
{
    for (;;) {
        u16 val = GET_GLOBAL(*data);
        if (val == 0xffff)
            return;
        outw(val, port);
        data++;
    }
}

static u16
cirrus_get_crtc(void)
{
    if (inb(VGAREG_READ_MISC_OUTPUT) & 1)
        return VGAREG_VGA_CRTC_ADDRESS;
    return VGAREG_MDA_CRTC_ADDRESS;
}

static void
cirrus_switch_mode(struct cirrus_mode_s *table)
{
    // Unlock cirrus special
    outw(0x1206, VGAREG_SEQU_ADDRESS);
    cirrus_switch_mode_setregs(GET_GLOBAL(table->seq), VGAREG_SEQU_ADDRESS);
    cirrus_switch_mode_setregs(GET_GLOBAL(table->graph), VGAREG_GRDC_ADDRESS);
    cirrus_switch_mode_setregs(GET_GLOBAL(table->crtc), cirrus_get_crtc());

    outb(0x00, VGAREG_PEL_MASK);
    inb(VGAREG_PEL_MASK);
    inb(VGAREG_PEL_MASK);
    inb(VGAREG_PEL_MASK);
    inb(VGAREG_PEL_MASK);
    outb(GET_GLOBAL(table->hidden_dac), VGAREG_PEL_MASK);
    outb(0xff, VGAREG_PEL_MASK);

    u8 vesacolortype = GET_GLOBAL(table->vesacolortype);
    u8 v = vgahw_get_single_palette_reg(0x10) & 0xfe;
    if (vesacolortype == 3)
        v |= 0x41;
    else if (vesacolortype)
        v |= 0x01;
    vgahw_set_single_palette_reg(0x10, v);
}

static u8
cirrus_get_memsize(void)
{
    // get DRAM band width
    outb(0x0f, VGAREG_SEQU_ADDRESS);
    u8 v = inb(VGAREG_SEQU_DATA);
    u8 x = (v >> 3) & 0x03;
    if (x == 0x03) {
        if (v & 0x80)
            // 4MB
            return 0x40;
        // 2MB
        return 0x20;
    }
    return 0x04 << x;
}

static void
cirrus_enable_16k_granularity(void)
{
    outb(0x0b, VGAREG_GRDC_ADDRESS);
    u8 v = inb(VGAREG_GRDC_DATA);
    outb(v | 0x20, VGAREG_GRDC_DATA);
}

static void
cirrus_clear_vram(u16 param)
{
    cirrus_enable_16k_granularity();
    u8 count = cirrus_get_memsize() * 4;
    u8 i;
    for (i=0; i<count; i++) {
        outw((i<<8) | 0x09, VGAREG_GRDC_ADDRESS);
        memset16_far(SEG_GRAPH, 0, param, 16 * 1024);
    }
    outw(0x0009, VGAREG_GRDC_ADDRESS);
}

int
cirrus_set_video_mode(u8 mode, u8 noclearmem)
{
    dprintf(1, "cirrus mode %d\n", mode);
    SET_BDA(vbe_mode, 0);
    struct cirrus_mode_s *table_g = cirrus_get_modeentry(mode);
    if (table_g) {
        cirrus_switch_mode(table_g);
        if (!noclearmem)
            cirrus_clear_vram(0xffff);
        SET_BDA(video_mode, mode);
        return 1;
    }
    table_g = cirrus_get_modeentry(0xfe);
    cirrus_switch_mode(table_g);
    dprintf(1, "cirrus mode switch regular\n");
    return 0;
}

static int
cirrus_check(void)
{
    outw(0x9206, VGAREG_SEQU_ADDRESS);
    return inb(VGAREG_SEQU_DATA) == 0x12;
}


/****************************************************************
 * extbios
 ****************************************************************/

static void
cirrus_extbios_80h(struct bregs *regs)
{
    u16 crtc_addr = cirrus_get_crtc();
    outb(0x27, crtc_addr);
    u8 v = inb(crtc_addr + 1);
    if (v == 0xa0)
        // 5430
        regs->ax = 0x0032;
    else if (v == 0xb8)
        // 5446
        regs->ax = 0x0039;
    else
        regs->ax = 0x00ff;
    regs->bx = 0x00;
    return;
}

static void
cirrus_extbios_81h(struct bregs *regs)
{
    // XXX
    regs->ax = 0x0100;
}

static void
cirrus_extbios_82h(struct bregs *regs)
{
    u16 crtc_addr = cirrus_get_crtc();
    outb(0x27, crtc_addr);
    regs->al = inb(crtc_addr + 1) & 0x03;
    regs->ah = 0xAF;
}

static void
cirrus_extbios_85h(struct bregs *regs)
{
    regs->al = cirrus_get_memsize();
}

static void
cirrus_extbios_9Ah(struct bregs *regs)
{
    regs->ax = 0x4060;
    regs->cx = 0x1132;
}

extern void a0h_callback(void);
ASM16(
    // fatal: not implemented yet
    "a0h_callback:"
    "cli\n"
    "hlt\n"
    "retf");

static void
cirrus_extbios_A0h(struct bregs *regs)
{
    struct cirrus_mode_s *table_g = cirrus_get_modeentry(regs->al & 0x7f);
    regs->ah = (table_g ? 1 : 0);
    regs->si = 0xffff;
    regs->di = regs->ds = regs->es = regs->bx = (u32)a0h_callback;
}

static void
cirrus_extbios_A1h(struct bregs *regs)
{
    regs->bx = 0x0e00; // IBM 8512/8513, color
}

static void
cirrus_extbios_A2h(struct bregs *regs)
{
    regs->al = 0x07; // HSync 31.5 - 64.0 kHz
}

static void
cirrus_extbios_AEh(struct bregs *regs)
{
    regs->al = 0x01; // High Refresh 75Hz
}

void
cirrus_extbios(struct bregs *regs)
{
    // XXX - regs->bl < 0x80 or > 0xaf call regular handlers.
    switch (regs->bl) {
    case 0x80: cirrus_extbios_80h(regs); break;
    case 0x81: cirrus_extbios_81h(regs); break;
    case 0x82: cirrus_extbios_82h(regs); break;
    case 0x85: cirrus_extbios_85h(regs); break;
    case 0x9a: cirrus_extbios_9Ah(regs); break;
    case 0xa0: cirrus_extbios_A0h(regs); break;
    case 0xa1: cirrus_extbios_A1h(regs); break;
    case 0xa2: cirrus_extbios_A2h(regs); break;
    case 0xae: cirrus_extbios_AEh(regs); break;
    default: break;
    }
}


/****************************************************************
 * vesa calls
 ****************************************************************/

#if 0
static u16
cirrus_vesamode_to_mode(u16 vesamode)
{
    // XXX - convert assembler
    return 0;
}

static u8
cirrus_get_bpp_bytes(void)
{
    // XXX - convert assembler
    return 0;
}

static void
cirrus_set_line_offset(u16 new_line_offset)
{
    // XXX - convert assembler
}

static u16
cirrus_get_line_offset(void)
{
    // XXX - convert assembler
    return 0;
}

static u16
cirrus_get_line_offset_entry(void *table)
{
    // XXX - convert assembler
    return 0;
}

static void
cirrus_set_start_addr(void *addr)
{
    // XXX - convert assembler
}

static void *
cirrus_get_start_addr(void)
{
    // XXX - convert assembler
    return NULL;
}
#endif

static void
cirrus_vesa_00h(struct bregs *regs)
{
    // XXX - convert assembler
}

static void
cirrus_vesa_01h(struct bregs *regs)
{
    // XXX - convert assembler
}

static void
cirrus_vesa_02h(struct bregs *regs)
{
    // XXX - convert assembler
}

static void
cirrus_vesa_03h(struct bregs *regs)
{
    // XXX - convert assembler
}

// XXX - add cirrus_vesa_05h_farentry to vgaentry.S

static void
cirrus_vesa_05h(struct bregs *regs)
{
    // XXX - convert assembler
}

static void
cirrus_vesa_06h(struct bregs *regs)
{
    // XXX - convert assembler
}

static void
cirrus_vesa_07h(struct bregs *regs)
{
    // XXX - convert assembler
}

static void
cirrus_vesa_10h(struct bregs *regs)
{
    // XXX - convert assembler
}

static void
cirrus_vesa_not_handled(struct bregs *regs)
{
    debug_stub(regs);
    regs->ax = 0x014f;
}

void
cirrus_vesa(struct bregs *regs)
{
    switch (regs->al) {
    case 0x00: cirrus_vesa_00h(regs); break;
    case 0x01: cirrus_vesa_01h(regs); break;
    case 0x02: cirrus_vesa_02h(regs); break;
    case 0x03: cirrus_vesa_03h(regs); break;
    case 0x05: cirrus_vesa_05h(regs); break;
    case 0x06: cirrus_vesa_06h(regs); break;
    case 0x07: cirrus_vesa_07h(regs); break;
    case 0x10: cirrus_vesa_10h(regs); break;
    default:   cirrus_vesa_not_handled(regs); break;
    }
}


/****************************************************************
 * init
 ****************************************************************/

void
cirrus_init(void)
{
    dprintf(1, "cirrus init\n");
    if (! cirrus_check())
        return;
    dprintf(1, "cirrus init 2\n");

    // memory setup
    outb(0x0f, VGAREG_SEQU_ADDRESS);
    u8 v = inb(VGAREG_SEQU_DATA);
    outb(((v & 0x18) << 8) | 0x0a, VGAREG_SEQU_ADDRESS);
    // set vga mode
    outw(0x0007, VGAREG_SEQU_ADDRESS);
    // reset bitblt
    outw(0x0431, VGAREG_GRDC_ADDRESS);
    outw(0x0031, VGAREG_GRDC_ADDRESS);
}
