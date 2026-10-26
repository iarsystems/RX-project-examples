/* Adapted to IAR Embedded Workbench */
/* this file is shared between the PC side and target side font code...modify together */
/* set if raster is RLE encoded */
#define EFNT_RLE 0x0001
/* set if raster is 8bpp */
#define EFNT_8BPP 0x0010
/* set if raster is 1bpp */
#define EFNT_1BPP 0x0020
/* set if raster is 4bpp */
#define EFNT_4BPP 0x0040

#ifdef __ICCRX__
#pragma pack(1)
#endif
typedef struct
{
  unsigned long char_code;
  signed long raster_offset;
  signed short raster_style;
  signed short width;
  signed short height;
  signed short pitch;
  signed short left;
  signed short bottom;
  signed short advance;
}GAPI_Eglyph_t;

typedef struct
{
  unsigned long signature;
  char family_name[32];
  char style_name[32];
  signed long num_glyphs;
  signed short render_style;
  signed short render_size;

  struct
  {
    signed short xMin;
    signed short yMin;
    signed short xMax;
    signed short yMax;
  } bbox;
  signed short ascender;
  signed short descender;
  signed short height;
  signed short max_advance_width;
  signed short max_advance_height;
  signed short underline_position;
  signed short underline_thickness;
}GAPI_Efont_t;
#ifdef __ICCRX__

#endif

#define RENDER_BOLD 0x0001
#define RENDER_ITALIC 0x0002
#define RENDER_LITTLE_ENDIAN 0x0004
#define RENDER_NO_RLE 0x0008
#define RENDER_FIXED_WIDTH 0x0010
#define RENDER_8BPP 0x0100
#define RENDER_1BPP 0x0200
#define RENDER_4BPP 0x0400

