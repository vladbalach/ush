#include "libmx.h"

void mx_print_unicode(wchar_t c) {
   char b[5] = {0};

   if (c < 0x80)
      b[0] = (c >> 0 & 0x7F) | 0x00;
   else if (c < 0x0800) {
      b[0] = (c >> 6 & 0x1F) | 0xC0;
      b[1] = (c >> 0 & 0x3F) | 0x80;
   }
   else if (c < 0x010000) {
      b[0] = (c >> 12 & 0x0F) | 0xE0;
      b[1] = (c >> 6 & 0x3F) | 0x80;
      b[2] = (c >> 0 & 0x3F) | 0x80;
   }
   else if (c < 0x110000) {
      b[0] = (c >> 18 & 0x07) | 0xF0;
      b[1] = (c >> 12 & 0x3F) | 0x80;
      b[2] = (c >> 6 & 0x3F) | 0x80;
      b[3] = (c >> 0 & 0x3F) | 0x80;
   }
   write(1, b, mx_strlen(b));
}
