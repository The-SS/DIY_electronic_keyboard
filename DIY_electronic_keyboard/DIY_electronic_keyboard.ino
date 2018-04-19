// DIY Electronic Keyboard
// Author:
// Sleiman Safaoui
// Github:
// The-SS
// Email:
// Sleiman.Safaoui@utdallas.edu
// snsafaoui@gmail.com
//
// April 15, 2018
//
// See build guide for circuit diagrams

/* Summary
  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * Buzzer to pin 13
 * Note buttons to pin A0
 * Function buttons to pin A5
*/


// files to include
#include "pitches.h"
#include <LiquidCrystal.h>

// mapping pins
#define buzzer 13 


// notes
int notes[] = {
  NOTE_C1 , NOTE_CS1, NOTE_D1 , NOTE_DS1, NOTE_E1 , NOTE_F1 , NOTE_FS1, NOTE_G1 , NOTE_GS1, NOTE_A1 , NOTE_AS1, NOTE_B1 ,
  NOTE_C2 , NOTE_CS2, NOTE_D2 , NOTE_DS2, NOTE_E2 , NOTE_F2 , NOTE_FS2, NOTE_G2 , NOTE_GS2, NOTE_A2 , NOTE_AS2, NOTE_B2 ,
  NOTE_C3 , NOTE_CS3, NOTE_D3 , NOTE_DS3, NOTE_E3 , NOTE_F3 , NOTE_FS3, NOTE_G3 , NOTE_GS3, NOTE_A3 , NOTE_AS3, NOTE_B3 ,
  NOTE_C4 , NOTE_CS4, NOTE_D4 , NOTE_DS4, NOTE_E4 , NOTE_F4 , NOTE_FS4, NOTE_G4 , NOTE_GS4, NOTE_A4 , NOTE_AS4, NOTE_B4 ,
  NOTE_C5 , NOTE_CS5, NOTE_D5 , NOTE_DS5, NOTE_E5 , NOTE_F5 , NOTE_FS5, NOTE_G5 , NOTE_GS5, NOTE_A5 , NOTE_AS5, NOTE_B5 ,
  NOTE_C6 , NOTE_CS6, NOTE_D6 , NOTE_DS6, NOTE_E6 , NOTE_F6 , NOTE_FS6, NOTE_G6 , NOTE_GS6, NOTE_A6 , NOTE_AS6, NOTE_B6 ,
  NOTE_C7 , NOTE_CS7, NOTE_D7 , NOTE_DS7, NOTE_E7 , NOTE_F7 , NOTE_FS7, NOTE_G7 , NOTE_GS7, NOTE_A7 , NOTE_AS7, NOTE_B7 ,
  NOTE_C8 , NOTE_CS8, NOTE_D8 , NOTE_DS8, NOTE_E8 , NOTE_F8 };

// variables for obtaining the index of the note to be played
char note; // C,D,E,F,G,A,B
char fxn; // #: sharp, b: flat (U:up, D: down not included)
int C = 0;
int D = 2;
int E = 4;
int F = 5;
int G = 7;
int A = 9;
int B = 11;
int NA = -100; //no tone
int octave_idx = 0; // index of current octave (starts at 0)
int note_idx; // index of note in array notes (starts at 0)
int num_notes; // number of notes in the array notes (initialized upon start of code)
int num_note_per_octave = 12; // total number of notes per octave
int num_octaves = 8; // number of octaves

// general variables
int duration = 100; // duration of playing note

// LCD display
LiquidCrystal lcd(7, 8, 9, 10, 6, 12);

void setup() {
num_notes = sizeof(notes)/2;
Serial.begin(9600);
lcd.begin(16,2);
lcd.setCursor(6,0);
lcd.print("DIY");
lcd.setCursor(1,1);
lcd.print("MUSIC KEYBOARD");
delay(1000);
}

void loop() {
  
// Get input note (C,D,E,F,G,A,B) and input function(#,b,U,D)
  int note_val = analogRead(A0);
  int fxn_val = analogRead(A5);
  Serial.println(note_val);

  // Determine note value (1st step in determining index of note)
  if (note_val >= 990 & note_val <= 1020) //C 
  {
    note_idx = C;
    note = 'C';
    Serial.println("C");
    }
  else if(note_val >= 910 & note_val <= 950)//D
  {
    note_idx = D;
    note = 'D';
    Serial.println("D");
    }
  else if(note_val >= 820 & note_val <= 870)//E
  {
    note_idx = E;
    note = 'E';
    Serial.println("E");
    }
  else if(note_val >= 630 & note_val <= 720)//F
  {
    note_idx = F;
    note = 'F';
    Serial.println("F");
    }
  else if(note_val >= 485 & note_val <= 535)//G
  {
    note_idx = G;
    note = 'G';
    Serial.println("G");
    }
  else if(note_val >= 70 & note_val <= 200)//A
  {
    note_idx = A;
    note = 'A';
    Serial.println("A");
    }
  else if(note_val >= 10 & note_val <= 55)//B
  {
    note_idx = B;
    note = 'B';
    Serial.println("B");
    }
  else
  {
    note = 'n'; // no note
    note_idx = NA;
    Serial.println("no tone");
    }

  // Determine if note is S or F and if the octave has changed
    if (fxn_val >= 990 & fxn_val <= 1020) //#
  {
    note_idx += 1;
    fxn = '#';
    Serial.println("#");
    }
  else if(fxn_val >= 910 & fxn_val <= 950)//b  
  {
    note_idx -= 1;
    fxn = 'b';
    Serial.println("b");
    }
  else if(fxn_val >= 830 & fxn_val <= 870)//U 
  {
    if (octave_idx == num_octaves-1) // highest pitchs --> loop to lowest pitches
    {octave_idx = 0;}
    else
    {octave_idx += 1;}
    fxn = 'n'; // no # or b
    Serial.println("U");
    }
  else if(fxn_val >= 660 & fxn_val <= 700)//D 
  {
    if (octave_idx == 0) // lowest pitchs --> loop to highest pitches
    {octave_idx = num_octaves -1;}
    else
    {octave_idx -= 1;}
    fxn = 'n'; // no # or b
    Serial.println("D");
    }
  else
  {
    fxn = 'n'; // no # or b
    Serial.println("no fxn");
  }

  // Determine note index (note_idx) in the array notes
  note_idx = octave_idx*num_note_per_octave + note_idx;

  // Output to buzzer
  if (note_idx > num_notes-1 | note_idx < 0 )
  {
    noTone(buzzer);
  }
  else
  {
    tone(buzzer, notes[note_idx], duration);
  } 
    
  // display LCD message
  String msg_1 = "Note: "; // first line message (note)
  String msg_2 = "Frequency: "; // second line message (frequency) 
  String space = " ";
  String dash = "-";
  octave_idx += 1; //base 0 to base 1
  lcd.clear();
    // first message
  lcd.setCursor(3,0);
  if (note =='n') // if there is not note pressed (regardless of # or b)
  {
    lcd.print(msg_1+dash+dash+octave_idx);
  }
  else if (fxn =='n') // if there is a note pressed, but not a # or b
  {
    lcd.print(msg_1+note+space+octave_idx);
  }
  else // if there is a # or b note
  {
    lcd.print(msg_1+note+fxn+octave_idx);
  }
    // second message
  lcd.setCursor(0,1);
  if (note =='n') // if there is not note pressed --> no frequency
  {
    lcd.print(msg_2); 
  }
  else // if there is a note pressed
  {
    lcd.print(msg_2+notes[note_idx]);
  }
  octave_idx -= 1; //base 1 to base 0

    delay(90);

}





