int ENC_A = PD_2;
int ENC_B = PD_3;

int ticks = 0;
int top = 0;
int bottom = 0;

// Reads 1/4th of possible edges
void enc() {
    if(digitalRead(PD_3)) {
        --ticks;
    } else {
        ++ticks;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(PD_2, INPUT);
    pinMode(PD_3, INPUT);
    attachInterrupt(PD_2, enc, RISING);
}

void loop() {
    top = ticks > top ? ticks : top;
    bottom = ticks < bottom ? ticks : bottom;
    Serial.print("Ticks: ");
    Serial.print(ticks);
    Serial.print("\t\tMin: ");
    Serial.print(bottom);
    Serial.print("\t\tMax: ");
    Serial.println(top);
}
