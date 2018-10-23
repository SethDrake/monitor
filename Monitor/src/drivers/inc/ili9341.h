
#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "stm32f10x.h"

//TFT resolution 240*320
#define TFT_MIN_X	0
#define TFT_MIN_Y	0
#define TFT_MAX_X	319
#define TFT_MAX_Y	239

class ILI9341 {
public:
	ILI9341();
	~ILI9341();
	void setupHw(SPI_TypeDef* spi, uint16_t spiPrescaler, GPIO_TypeDef* controlPort, uint16_t rsPin, uint16_t resetPin, uint16_t csPin);
	void init(void);
	void enable(short on);
	void sleep(short on);
	uint32_t readID();
	void clear(uint16_t color);
	void setWindow(uint16_t startX, uint16_t startY, uint16_t stopX, uint16_t stopY);
	void pixelDraw(uint16_t xpos, uint16_t ypos, uint16_t color);
	void lineDraw(uint16_t ypos, uint16_t* line, uint32_t size);
	void fillScreen(uint16_t xstart, uint16_t ystart, uint16_t xstop, uint16_t ystop, uint16_t color);
	void putChar(uint16_t x, uint16_t y, uint8_t chr, uint16_t charColor, uint16_t bkgColor);
	void printf(uint16_t x, uint16_t y, const char *format, ...);
	void printf(uint16_t x, uint16_t y, uint16_t charColor, uint16_t bkgColor, const char *format, ...);
	void drawBattery(uint16_t x, uint16_t y, uint8_t level, uint16_t color, uint16_t bkgColor);
	void bufferDraw(uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize, uint16_t* buf);
	void drawBorder(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height, uint16_t borderWidth, uint16_t color);
	void DMATXInterrupt();
	uint8_t IsDataSending();
	void resetIsDataSending();
	void setDisableDMA(uint8_t isDisable);
	void setColor(uint16_t color, uint16_t bgColor);
	void setFont(const unsigned char font[]);
	void setLandscape(void);
	void setPortrait(void);
	uint16_t RGB888ToRGB565(uint8_t r, uint8_t g, uint8_t b);
	bool isReady(void);
	
protected:
	SPI_TypeDef* spi;
	volatile uint16_t spiPrescaler;
	GPIO_TypeDef* controlPort;
	volatile uint16_t rsPin;
	volatile uint16_t resetPin;
	volatile uint16_t csPin;

	volatile uint8_t isDataSending;
	volatile uint8_t manualCsControl;
	volatile uint8_t disableDMA;
	volatile uint16_t color;
	volatile uint16_t bgColor;
	const unsigned char *font;
	volatile bool isLandscape;
	volatile bool isOk;

private:
	void setSPISpeed(uint16_t prescaler);
	void setCol(uint16_t StartCol, uint16_t EndCol);
	void setPage(uint16_t StartPage, uint16_t EndPage);
	void sendCmd(uint8_t index);
	void sendByteInt(uint8_t data);
	void sendWordInt(uint16_t data);
	void sendData(uint8_t data);
	void sendWord(uint16_t data);
	void sendWord16bitMode(uint16_t data);
	void sendWords(uint16_t data1, uint16_t data2);
	void putString(const char str[], uint16_t x, uint16_t y, uint16_t charColor, uint16_t bkgColor);
	void initDMAforSendSPI(uint16_t* buffer, uint32_t size, uint8_t singleColor);
	void DMATXStart();
	void switchCs(short BitVal);
	void switchRs(short BitVal);
	void switchReset(short BitVal);
	uint16_t getColorByLevel(uint8_t level);
	
	uint8_t readByte();
};

#define RED			0xf800
#define PURPLE		0xf310
#define GREEN		0x07e0
#define BLUE		0x001f
#define BLACK		0x0000
#define YELLOW		0xffe0
#define ORANGE		0xfc00
#define WHITE		0xffff
#define CYAN		0x07ff
#define BRIGHT_RED	0xf810
#define GRAY1		0x8410
#define GRAY2		0x4208

#define ILI9341_DEVICE_CODE_READ_REG    0x00
#define ILI9341_SOFT_RESET_REG  		0x01
#define ILI9341_IDENTINFO_R_REG 		0x04
#define ILI9341_STATUS_R_REG    		0x09
#define ILI9341_POWERMODE_R_REG 		0x0A
#define ILI9341_MADCTL_R_REG    		0x0B
#define ILI9341_PIXFORMAT_R_REG 		0x0C
#define ILI9341_IMGFORMAT_R_REG 		0x0D
#define ILI9341_SIGMODE_R_REG   		0x0E
#define ILI9341_SD_RESULT_R_REG 		0x0F
#define ILI9341_SLEEP_ENTER_REG 		0x10
#define ILI9341_SLEEP_OUT_REG   		0x11
#define ILI9341_PARTIALMODE_REG 		0x12
#define ILI9341_NORDISPMODE_REG 		0x13
#define ILI9341_INVERSIONOFF_REG        0x20
#define ILI9341_INVERSIONON_REG 		0x21
#define ILI9341_GAMMASET_REG    		0x26
#define ILI9341_DISPLAYOFF_REG  		0x28
#define ILI9341_DISPLAYON_REG   		0x29
#define ILI9341_COLADDRSET_REG  		0x2A
#define ILI9341_PAGEADDRSET_REG 		0x2B
#define ILI9341_MEMORYWRITE_REG 		0x2C
#define ILI9341_COLORSET_REG    		0x2D
#define ILI9341_MEMORYREAD_REG  		0x2E
#define ILI9341_PARTIALAREA_REG 		0x30
#define ILI9341_VERTSCROLL_REG  		0x33
#define ILI9341_TEAREFFECTLINEOFF_REG	0x34
#define ILI9341_TEAREFFECTLINEON_REG	0x35
#define ILI9341_MEMACCESS_REG   		0x36
#define ILI9341_VERSCRSRART_REG 		0x37
#define ILI9341_IDLEMODEOFF_REG 		0x38
#define ILI9341_IDLEMODEON_REG  		0x39
#define ILI9341_PIXFORMATSET_REG		0x3A
#define ILI9341_WRITEMEMCONTINUE_REG	0x3C
#define ILI9341_READMEMCONTINUE_REG		0x3E
#define ILI9341_SETTEATSCAN_REG 		0x44
#define ILI9341_GETSCANLINE_REG 		0x45
#define ILI9341_WRITEBRIGHT_REG 		0x51
#define ILI9341_READBRIGHT_REG  		0x52
#define ILI9341_WRITECTRL_REG   		0x53
#define ILI9341_READCTRL_REG    		0x54
#define ILI9341_WRITECABC_REG   		0x55
#define ILI9341_READCABC_REG    		0x56
#define ILI9341_WRITECABCMB_REG 		0x5E
#define ILI9341_READCABCMB_REG  		0x5F
#define ILI9341_RGB_ISCTL_REG   		0xB0
#define ILI9341_FRAMECTL_NOR_REG		0xB1
#define ILI9341_FRAMECTL_IDLE_REG		0xB2
#define ILI9341_FRAMECTL_PARTIAL_REG	0xB3
#define ILI9341_INVERCTL_REG    		0xB4
#define ILI9341_BLANKPORCTL_REG 		0xB5
#define ILI9341_FUNCTONCTL_REG  		0xB6
#define ILI9341_ENTRYMODE_REG   		0xB7
#define ILI9341_BLIGHTCTL1_REG  		0xB8
#define ILI9341_BLIGHTCTL2_REG  		0xB9
#define ILI9341_BLIGHTCTL3_REG  		0xBA
#define ILI9341_BLIGHTCTL4_REG  		0xBB
#define ILI9341_BLIGHTCTL5_REG  		0xBC
#define ILI9341_BLIGHTCTL7_REG  		0xBE
#define ILI9341_BLIGHTCTL8_REG  		0xBF
#define ILI9341_POWERCTL1_REG   		0xC0
#define ILI9341_POWERCTL2_REG   		0xC1
#define ILI9341_VCOMCTL1_REG    		0xC5
#define ILI9341_VCOMCTL2_REG    		0xC7
#define ILI9341_POWERCTLA_REG   		0xCB
#define ILI9341_POWERCTLB_REG   		0xCF
#define ILI9341_NVMEMWRITE_REG  		0xD0
#define ILI9341_NVMEMPROTECTKEY_REG		0xD1
#define ILI9341_NVMEMSTATUS_REG 		0xD2
#define ILI9341_READID4_REG     		0xD3
#define ILI9341_READID1_REG     		0xDA
#define ILI9341_READID2_REG     		0xDB
#define ILI9341_READID3_REG     		0xDC
#define ILI9341_POSGAMMACORRECTION_REG	0xE0
#define ILI9341_NEGGAMMACORRECTION_REG	0xE1
#define ILI9341_DIGGAMCTL1_REG  		0xE2
#define ILI9341_DIGGAMCTL2_REG  		0xE3
#define ILI9341_DIVTIMCTL_A_REG 		0xE8
#define ILI9341_DIVTIMCTL_B_REG 		0xEA
#define ILI9341_POWONSEQCTL_REG 		0xED
#define ILI9341_ENABLE_3G_REG   		0xF2
#define ILI9341_INTERFCTL_REG   		0xF6
#define ILI9341_PUMPRATIOCTL_REG		0xF7


#endif //__ILI9341_H__
