/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/872
  ----> http://www.adafruit.com/products/871
  ----> http://www.adafruit.com/products/870

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "Board.h"


Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

void display_board(const Board & board) {
  for(size_t y=0; y<board.size(); y++) {
    for(size_t x=0; x<board.size(); x++) {
      if(board.get(x, y)) {
        matrix.drawPixel(x, y, LED_ON);
      } else {
        matrix.drawPixel(x, y, LED_OFF);
      }
    }
  }
  matrix.writeDisplay();
}

void next(const Board & src, Board & dest) {
  for(size_t y=0; y<src.size(); y++) {
    for(size_t x=0; x<src.size(); x++) {
      int neighbors = src.getModulus(x-1, y+1) + src.getModulus(x, y+1) + src.getModulus(x+1, y+1) +
                      src.getModulus(x-1, y)                            + src.getModulus(x+1, y) +
                      src.getModulus(x-1, y-1) + src.getModulus(x, y-1) + src.getModulus(x+1, y-1);
      bool live = true;
      
      if(src.get(x, y)) {
        if(neighbors < 2 || 3 < neighbors) {
          live = false;
        }
      } else {
        if(neighbors != 3) {
          live = false;
        }
      }
      
      dest.set(x, y, live);
    }
  }
}

void display_iter(const Board & src, Board & dest) {
  display_board(src);
  delay(100);
  next(src, dest);
}

void randomize(Board & board) {
  for(size_t y=0; y<board.size(); y++) {
    for(size_t x=0; x<board.size(); x++) {
      board.set(x, y, random(2));
    }
  }
}

Board boardeven;
Board boardodd;
bool even = true;

void setup() {
  matrix.begin(0x70);  // pass in the address
  
  randomSeed(analogRead(0));
  
  randomize(boardeven);
  
  //boardeven.set(1, 0, true);
  //boardeven.set(2, 1, true);
  //boardeven.set(0, 2, true);
  //boardeven.set(1, 2, true);
  //boardeven.set(2, 2, true);
  even = true;
}

void loop() {
  if(even) {
    display_iter(boardeven, boardodd);
  } else {
    display_iter(boardodd, boardeven);
  }
  even = !even;
  if(boardeven == boardodd) {
    delay(1000);
    randomize(boardeven);
    even = true;
  }
}
