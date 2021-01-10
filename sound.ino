const int buzz = A0;
const int btn[4] = {2, 3, 4, 5};
const int btnin[4] = {A1, A2, A3, A4};
const int led[4] = {9, 8, 7, 6};
const int ledin[3] = {12, 11, 10};
const int mel[7][12] = {{65, 69, 73, 77, 82, 87, 92, 97, 103, 110, 116, 123},
  {130, 138, 146, 155, 164, 174, 184, 195, 207, 220, 233, 246},
  {261, 277, 293, 311, 329, 349, 369, 391, 415, 440, 466, 493},
  {523, 554, 587, 622, 659, 698, 739, 783, 830, 880, 932, 987},
  {1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567, 1661, 1760, 1864, 1975},
  {2093, 2217, 2349, 2489, 2637, 2793, 2959, 3135, 3322, 3520, 3729, 3951},
  {4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902}
};
const int song[3][100] = {{0, 5, 5, 5, 0, 2, 2, 0, 0, 5, 5, 7, 7, 9, -1, 5, -1, 9, -1, 9, -1, 10, 10, 10, -1, 7, -1, 7, -1, 9, 9, 9, -1, 5, -1, 5, -1, 7, 7, 7, -1, 4, 0, 2, 4, 5, -1, 5, -2},
  {4, 2, 0, 2, 4, 4, 4, -1, 2, 2, 2, -1, 4, 7, 7, -1, 4, 2, 0, 2, 4, 4, 4, -1, 2, 2, 4, 2, 0, -2},
  {7, 7, 9, 9, 7, 7, 4, -1, 7, 7, 4, 4, 2, -1, 7, 7, 9, 9, 7, 7, 4, -1 , 7, 4, 2, 4, 0, -2}
};

int oktab = 3;
int buff = 0;
int oktabbuff = 0;
int playbuff = 0;
int practicebuff = 0;
int mode = 0;
char ssong = 0;
int playsong = 0;
bool flag = false;

void setup() {
  pinMode(buzz, OUTPUT);
  for (int i = 0 ; i < 4; i++) {
    if (i < 3)pinMode(ledin[i], OUTPUT);
    pinMode(led[i], OUTPUT);
    pinMode(btn[i], INPUT);
    pinMode(btnin[i], OUTPUT);
  }
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledin[i], HIGH);
  }
  Serial.begin(9600);
}

void loop() {
  if (mode == 1) {
    switch (ssong) {
      case '1':
        playsong = 0;
        break;
      case '2':
        playsong = 1;
        break;
      case '3':
        playsong = 2;
        break;
      default:
        for (int i = 0; i < 3; i++) {
          tone(buzz, 1000);
          delay(150);
          noTone(buzz);
          delay(70);
        }
        playsong = -1;
        break;
    }
    if (playsong >= 0) {
      for (int i = 0 ; i < 100; i++) {
        if (song[playsong][i] >= 0) {
          flag = false;
          digitalWrite(ledin[song[playsong][i] / 4], LOW);
          digitalWrite(led[song[playsong][i] % 4], HIGH);
          digitalWrite(btnin[(song[playsong][i] / 4) + 1], HIGH);
          while (!flag) {
            int input = digitalRead(btn[song[playsong][i] % 4]);
            if (input == 1) {
              tone(buzz, mel[oktab][song[playsong][i]]);
            }
            if (input != practicebuff && practicebuff == 1) {
              noTone(buzz);
              flag = true;
            }
            practicebuff = input;
          }
          digitalWrite(led[song[playsong][i] % 4], LOW);
          digitalWrite(ledin[song[playsong][i] / 4], HIGH);
          digitalWrite(btnin[song[playsong][i + 1] / 4], LOW);
        }
        else if (song[playsong][i] == -2) {
          break;
        }
      }
    }
    mode = 0;
  }
  else if (mode == 2) {
    switch (ssong) {
      case 'a':
        playsong = 0;
        break;
      case 'b':
        playsong = 1;
        break;
      case 'c':
        playsong = 2;
        break;
      default:
        for (int i = 0; i < 3; i++) {
          tone(buzz, 1000);
          delay(150);
          noTone(buzz);
          delay(70);
        }
        playsong = -1;
        break;
    }
    if (playsong >= 0) {
      for (int i = 0; i < 100; i++) {
        if (song[playsong][i] >= 0) {
          digitalWrite(ledin[song[playsong][i] / 4], LOW);
          digitalWrite(led[song[playsong][i] % 4], HIGH);
          tone(buzz, mel[oktab][song[playsong][i]]);
          delay(300);
          noTone(buzz);
          delay(100);
          digitalWrite(led[song[playsong][i] % 4], LOW);
          digitalWrite(ledin[song[playsong][i] / 4], HIGH);
        }
        else if (song[playsong][i] == -1) {
          delay(400);
        }
        else {
          break;
        }
      }
    }
    mode = 0;
  }
  else {
    while (Serial.available()) {
      char c = (char)Serial.read();
      if (c >= '0' && c <= '9') {
        mode = 1;
        ssong = c;
      }
      else if (c >= 'a' && c <= 'z') {
        mode = 2;
        ssong = c;
      }
      delay(5);
    }
    for (int i = 0; i < 4; i++) {
      digitalWrite(btnin[i], HIGH);
      if (i != 0)digitalWrite(ledin[i - 1], LOW);
      for (int j = 0; j < 4; j++) {
        int input = digitalRead(btn[j]);
        int a = i * 4 + j - 4;
        if (buff != input && buff != 1) {
          if (a == -1 && oktabbuff != a) {
            if (oktab < 6) {
              oktab++;
            }
          }
          else if (a == -2 && oktabbuff != a) {
            if (oktab > 0) {
              oktab--;
            }
          }
          else if(a>=0 && a<=11) {
            tone(buzz, mel[oktab][a]);
            digitalWrite(led[j], HIGH);
            flag = false;
            while (!flag) {
              int input2 = digitalRead(btn[j]);
              if (input2 != playbuff && playbuff == 1) {
                noTone(buzz);
                flag = true;
              }
              playbuff = input2;
            }
          }
          oktabbuff = a;
        }
        buff = input;
        digitalWrite(led[j], LOW);
      }
      digitalWrite(btnin[i], LOW);
      if (i != 0)digitalWrite(ledin[i - 1], HIGH);
    }
  }
}
