/*
  8 step sequencer with different waveforms

  Press both bottoms at the same time to start cycle.

  30.5.2017 ChrisMicro
*/

#include "neolib.h"
#include "neoSoundPittix.h"


typedef struct
{
  uint16_t f_Hz;
  uint8_t waveType;
} sound_t;

sound_t Sounds[8];

void setup()
{
  neobegin();

  // start sound
  beginNeoPittix();
  setFrequency(440);

  pixels.setPixelColor( 0, COLOR_GREEN );
  pixels.show();
}

uint8_t  LedPosition   = 0;
uint8_t  ColorPosition = 0;
uint32_t Colors[] = { COLOR_GREEN, COLOR_RED, COLOR_BLUE, COLOR_PINK};


#define POITHYSTERESIS 30

int16_t oldPotiLeft;
int16_t oldPotiRight;

uint8_t State = 0;
uint16_t Counter = 1000;

void loop()
{
  pixels.setPixelColor( LedPosition, COLOR_BLACK );   // clear pixel

  uint8_t b = wasButtonPressed();

  if (b)
  {
    if ( b == BUTTON_LEFT                )   LedPosition--   ;
    if ( b == BUTTON_RIGHT               )   LedPosition++   ;
    if ( b == BUTTON_RIGHT + BUTTON_LEFT )
    {
      ColorPosition++ ;
      if (State == 0)State = 1;
      else State = 0;
    }

    LedPosition   &= 0b00000111; // prevent out of range
    ColorPosition &= 0b00000011; // prevent out of range

    pixels.setPixelColor( LedPosition, Colors[ColorPosition] );
    pixels.show();

    setFrequency( Sounds[ LedPosition ].f_Hz );
  }

  int16_t lp = getPoti( POTI_LEFT );   // range 0..1023
  int16_t rp = getPoti( POTI_RIGHT );  // range 0..1023

  if (State == 0)
  {
    if ( abs(rp - oldPotiRight) > POITHYSTERESIS )
    {
      setWaveformFine(rp);
      oldPotiRight = rp;
      Sounds[ LedPosition ].waveType = rp;
    }

    if ( abs(lp - oldPotiLeft) > POITHYSTERESIS )
    {
      setFrequency(lp);
      oldPotiLeft = lp;
      Sounds[ LedPosition ].f_Hz = lp;
    }
  }

  if (State == 1)
  {
    Counter--;
    if (Counter == 0)
    {

      Counter = lp;

      if (Counter > 500) Counter = 500;
      if (Counter < 50) Counter = 50;


      LedPosition++;
      LedPosition   &= 0b00000111; // prevent out of range
      pixels.setPixelColor( LedPosition, COLOR_PINK );
      pixels.show();

      setFrequency( Sounds[ LedPosition ].f_Hz );
      setWaveformFine( Sounds[ LedPosition ].waveType );
    }

  }

  delay(2);


}




