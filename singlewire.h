#ifndef SINGLEWIRE_H
#define SINGLEWIRE_H

class SingleWire {
  private:
    int pin;
    int bitrate;
    long bitTiming;
    bool alwaysOutput;
  public:
    SingleWire(int setPin, int setBitrate, bool setAlwaysOutput=false) {
      pin = setPin;
      bitrate = setBitrate;
      bitTiming = 1000000 / bitrate;
      alwaysOutput = setAlwaysOutput;

      if (!alwaysOutput) {
        pinMode(pin, INPUT);
      }
    }

    // getters and setters
    void setPin(int setPin) {
      pin = setPin;
    }

    int getPin() {
      return pin;
    }

    void setBitrate(int setBitrate) {
      bitrate = setBitrate;
      bitTiming = 1000000 / bitrate;
    }

    int getBitrate() {
      return bitrate;
    }

    // transmitting
    void sendStartSequence() {
      pinMode(pin, OUTPUT);

      digitalWrite(pin, LOW);
      delayMicroseconds(400);
      digitalWrite(pin, HIGH);
      delayMicroseconds(bitTiming);
    }
    void sendEndSequence() {
      digitalWrite(pin, LOW);
      delayMicroseconds(400);
      digitalWrite(pin, HIGH);
      delayMicroseconds(bitTiming);

      if (!alwaysOutput) {
        pinMode(pin, INPUT);
      }
    }

    void sendDWord(unsigned long data) { // long integer or similar
      sendStartSequence();

      for (int i = 0; i < 32; i++) {
        if (bitRead(data, i) == 1) {
          digitalWrite(pin, HIGH);
        } else {
          digitalWrite(pin, LOW);
        }

        delayMicroseconds(bitTiming);
      }

      sendEndSequence();
    }

    void sendDword(word data1, word data2) {
      sendStartSequence();

      for (int i = 0; i < 16; i++) {
        if (bitRead(data1, i) == 1) {
          digitalWrite(pin, HIGH);
        } else {
          digitalWrite(pin, LOW);
        }

        delayMicroseconds(bitTiming);
      }

      for (int i = 0; i < 16; i++) {
        if (bitRead(data2, i) == 1) {
          digitalWrite(pin, HIGH);
        } else {
          digitalWrite(pin, LOW);
        }

        delayMicroseconds(bitTiming);
      }

      sendEndSequence();
    }

    void sendWord(word data) { // normal integer or similar
      sendStartSequence();

      for (int i = 0; i < 16; i++) {
        if (bitRead(data, i) == 1) {
          digitalWrite(pin, HIGH);
        } else {
          digitalWrite(pin, LOW);
        }

        delayMicroseconds(bitTiming);
      }

      sendEndSequence();
    }

    void sendByte(byte data) { // normal integer or similar
      sendStartSequence();

      for (int i = 0; i < 8; i++) {
        if (bitRead(data, i) == 1) {
          digitalWrite(pin, HIGH);
        } else {
          digitalWrite(pin, LOW);
        }

        delayMicroseconds(bitTiming);
      }

      sendEndSequence();
    }

    // receiving
    void waitForData() {
      if (alwaysOutput) {return;}
      pinMode(pin, INPUT);
      while (digitalRead(pin));
      // received low start bit
      while (!digitalRead(pin));
      // change to high signal, waiting 1.5 the bit timing before receiving data
      delayMicroseconds(bitTiming + (bitTiming/2));
    }
    void verifyDataSent() {
      if (alwaysOutput) {return;}
      pinMode(pin, INPUT);
      while (digitalRead(pin));
      // received high start bit
      while (!digitalRead(pin));
      // received low end bit, end of data sequence
    }

    unsigned long receiveDWord() {
      if (alwaysOutput) {return 0;}
      waitForData();
      unsigned long data = 0;

      for (int i = 0; i < 32; i++) {
        bitWrite(data, i, digitalRead(pin));

        delayMicroseconds(bitTiming);
      }

      verifyDataSent();

      return data;
    }

    word receiveWord() {
      if (alwaysOutput) {return 0;}
      waitForData();
      word data = 0;

      for (int i = 0; i < 16; i++) {
        bitWrite(data, i, digitalRead(pin));

        delayMicroseconds(bitTiming);
      }

      verifyDataSent();

      return data;
    }

    byte receiveByte() {
      if (alwaysOutput) {return 0;}
      waitForData();
      byte data = 0;

      for (int i = 0; i < 8; i++) {
        bitWrite(data, i, digitalRead(pin));

        delayMicroseconds(bitTiming);
      }

      verifyDataSent();

      return data;
    }
};

#endif