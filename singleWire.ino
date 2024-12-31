#include "singlewire.h"
#include "notes.h"

void transmit() {
  Serial.begin(115200);
  SingleWire sw(12, 5000, true);

  int tempo = 114;
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;

  while (true) {
    delay(10);

    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
      // calculates the duration of each note
      divider = melody[thisNote + 1];
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }

      Serial.print("Note # ");
      Serial.print(thisNote / 2);
      Serial.print(" Note : ");
      Serial.print(melody[thisNote]);
      Serial.print(" Duration: ");
      Serial.println(divider);

      // we only play the note for 90% of the duration, leaving 10% as a pause
      sw.sendWord(melody[thisNote]);
      delay(1);
      sw.sendWord((int(noteDuration * 0.9)));
      delay(noteDuration);
    }

    delay(2000);
  }
}

void receive() {
  // Serial.begin(115200);
  SingleWire sw(12, 5000);

  pinMode(14, OUTPUT);

  unsigned int note = 0;
  unsigned int duration = 0;

  while (true) {
    note = sw.receiveWord();
    duration = sw.receiveWord();

    if (note > 6000) {
      continue;
    }
    if (duration > 1000) {
      continue;
    }

    tone(11, note, duration);
    delay(duration);
    noTone(11);
  }
}

void setup() {
  transmit();
  // receive();
}

void loop() {
}
