#include "flower.h"
#include "petals.h"
#include "servo.h"
#include "touch.h"
#include "ws2812b.h"
#include "stdlib.h"
#include "time.h"
#include "timer.h"

#define SERVO_OPEN 180
#define SERVO_SAFE_MIDDLE 90
#define SERVO_CLOSED 0

#define RED 0
#define GREEN 1
#define BLUE 2

#define LED_NUM 7

struct RGB_Color_Struct myrgb[LED_NUM]={0};

extern u16 seedCount;

byte mode = MODE_FADED;
//byte mode = MODE_RAINBOW;
float currentRGB[] = {0, 0, 0};	
float changeRGB[] = {0, 0, 0};
byte newRGB[] = {0, 0, 0};
int servoChange = 1; //open
int servoPosition = SERVO_SAFE_MIDDLE;
int counter = 0;


u8 PetalsBloom(int j);
u8 PetalsFade(int j);
void PrepareCrossFadeBloom(unsigned int duration);
void Rainbow(int j);
u8 OpenPetals(void);
u8 ClosePetals(void);
void PixelsUnifiedColor(struct RGB_Color_Struct* color, struct RGB_Color_Struct color0);
void PrepareCrossFade(byte red, byte green, byte blue, unsigned int duration);
u8 CrossFade(void);
struct RGB_Color_Struct ColorWheel(byte wheelPos);


void FlowerInit(void){
	TouchExitInit();				  //Initial touch sensor pin.
	PetalsInit();					  //Initial Timer2&4 into PWM mode.
	Ws2812bInit();					  //Initial RGB led pin.
	ReflashRGB(myrgb);				  //Initial RGB led color.(black to close led)
	ServoInit();					  //Initial servo pin.
	ServoControl(servoPosition);	  //Set servo initial position.
}



void FlowerTask(void){
	u8 done = 1;
	switch(mode){
		case MODE_BLOOM: 
			PrepareCrossFadeBloom(500);
			ChangeMode(MODE_BLOOMING);
			break;

		case MODE_BLOOMING:
			done = CrossFade() && done;
			done = OpenPetals() && done;
			done = PetalsBloom(counter) && done;
			if (done) {
				ChangeMode(MODE_BLOOMED);
			}
			break;

		case MODE_FADE:
			PrepareCrossFade(0, 0, 0, 800);
			ChangeMode(MODE_FADING);
			break;

		case MODE_FADING:
			done = CrossFade() && done;
			done = ClosePetals() && done;
			done = PetalsFade(counter) && done;
			if (done) {
				ChangeMode(MODE_FADED);
			}
			break;

		case MODE_FADED:
			//PrepareCrossFade(140, 70, 0, 140);
			ChangeMode(MODE_FALLINGASLEEP);
			break;

		case MODE_FALLINGASLEEP:
			done = CrossFade() && done;
			done = ClosePetals() && done;
			if (done) {
				ChangeMode(MODE_SLEEPING);
			}
			break;

		case MODE_RAINBOW:
			Rainbow(counter);
			break;
		
	}
	counter++;
}

void ChangeMode(byte NewMode){
	srand(seedCount);	 			  //Initial random seed.
	if (mode != NewMode) {
		mode = NewMode;
		counter = 0;
	}
}

//petals animations
u8 PetalsBloom(int j) {
	if (j < 250) {
		return 0; // delay
	}
	if (j > 750) {
		return 1;
	}
	PetalsPWMSet((j - 250) / 2);
	return 0;
}

u8 PetalsFade(int j) {
	if (j > 510) {
		return 1;
	}
	PetalsPWMSet((510 - j) / 2);
	return 0;
}


// animations

void PrepareCrossFadeBloom(unsigned int duration) {
	byte color = rand()%5;
	switch (color) {
		case 0: // white
			PrepareCrossFade(140, 140, 140, duration);
			break;
		case 1: // red
			PrepareCrossFade(140, 5, 0, duration);
			break;
		case 2: // blue
			PrepareCrossFade(30, 70, 170, duration);
			break;
		case 3: // pink
			PrepareCrossFade(140, 0, 70, duration);
			break;
		case 4: // orange
			PrepareCrossFade(255, 70, 0, duration);
			break;
  }
}

void Rainbow(int j) {
	uint16_t i;
	byte num = LED_NUM - 1;
	SetOneRgb(myrgb, num, 100, 100, 100);
	for (i = 0; i < num; i++) {
		SetOneColor(myrgb, i, ColorWheel(((i * 256 / num) + j) & 255));
	}
	ReflashRGB(myrgb);
}

// servo function 

u8 OpenPetals() {
	if (servoPosition >= SERVO_OPEN) {
		return 1;
	}
	servoPosition ++;
	ServoControl(servoPosition);
	return 0;
}

u8 ClosePetals() {
	if (servoPosition <= SERVO_CLOSED) {
		return 1;
	}
	servoPosition --;
	ServoControl(servoPosition);
	return 0;
}

// utility function

	void PixelsUnifiedColor(struct RGB_Color_Struct *color, struct RGB_Color_Struct color0) {
	for (unsigned int i = 0; i < LED_NUM - 1; i++) {
		SetAllColor(color, color0);
	}
	ReflashRGB(color);
}

void PrepareCrossFade(byte red, byte green, byte blue, unsigned int duration) {
	float rchange = red - currentRGB[RED];
	float gchange = green - currentRGB[GREEN];
	float bchange = blue - currentRGB[BLUE];
	
	changeRGB[RED] = rchange / (float) duration;
	changeRGB[GREEN] = gchange / (float) duration;
	changeRGB[BLUE] = bchange / (float) duration;

	newRGB[RED] = red;
	newRGB[GREEN] = green;
	newRGB[BLUE] = blue;

	//Serial.print(newRGB[RED]);
	//Serial.print(" ");
	//Serial.print(newRGB[GREEN]);
	//Serial.print(" ");
	//Serial.print(newRGB[BLUE]);
	//Serial.print(" (");
	//Serial.print(changeRGB[RED]);
	//Serial.print(" ");
	//Serial.print(changeRGB[GREEN]);
	//Serial.print(" ");
	//Serial.print(changeRGB[BLUE]);
	//Serial.println(")");
}

u8 CrossFade() {
	struct RGB_Color_Struct color;
	
	if (currentRGB[RED] == newRGB[RED] && currentRGB[GREEN] == newRGB[GREEN] && currentRGB[BLUE] == newRGB[BLUE]) {
		return 1;
	}
	for (byte i = 0; i < 3; i++) {
		if (changeRGB[i] > 0 && currentRGB[i] < newRGB[i]) {
			currentRGB[i] = currentRGB[i] + changeRGB[i];
		}
		else if (changeRGB[i] < 0 && currentRGB[i] > newRGB[i]) {
			currentRGB[i] = currentRGB[i] + changeRGB[i];
		}
		else {
			currentRGB[i] = newRGB[i];
		}
	}
	color.red = currentRGB[RED];
	color.green = currentRGB[GREEN];
	color.blue = currentRGB[BLUE];
	PixelsUnifiedColor(myrgb, color);
  /*
    Serial.print(currentRGB[RED]);
    Serial.print(" ");
    Serial.print(currentRGB[GREEN]);
    Serial.print(" ");
    Serial.print(currentRGB[BLUE]);
    Serial.println();
  */
	return 0;
}

struct RGB_Color_Struct ColorWheel(byte wheelPos) {
	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	struct RGB_Color_Struct color;
	wheelPos = 255 - wheelPos;
	if (wheelPos < 85) {
		color.red = 255 - wheelPos * 3;
		color.green = 0;
		color.blue = wheelPos * 3;
		return color;
	}	
	if (wheelPos < 170) {
		wheelPos -= 85;
		color.red = 0;
		color.green = wheelPos * 3;
		color.blue = 255 - wheelPos * 3;
		return color;
	}
	wheelPos -= 170;
	color.red = wheelPos * 3;
	color.green = 255 - wheelPos * 3;
	color.blue = 0;
	return color;
	
}

