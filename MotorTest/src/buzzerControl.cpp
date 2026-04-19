#include <Arduino.h>

int buzzerPin = 7;

void setupBuzzer(int pin) {
  buzzerPin = pin;
  pinMode(buzzerPin, OUTPUT);
}

// Function to play a sound or melody
void playMelody(int melody[], int noteDuration[], int length) {
  for (int i = 0; i < length; i++) {
    if (melody[i] == 0) {
      noTone(buzzerPin);
    } else {
      tone(buzzerPin, melody[i], noteDuration[i]);
    }
    delay(noteDuration[i] * 1.30);
  }
  noTone(buzzerPin);
}

void playMelody(int melody[], int length, int tempo) {
  int notes = length / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;

  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
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

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzerPin, melody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(buzzerPin);
  }
}
