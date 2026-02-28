#include <cstdint>

void testlines(uint16_t color);
void testdrawtext(char *text, uint16_t color);
void testdrawrects(uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testfastlines(uint16_t color1, uint16_t color2);
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);
void testtriangles(void);
void testfilledtriangles(void);
void testroundrects(void);
void tftPrintTest(void);
void readSensors();