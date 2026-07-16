#define TRIG_PIN 2
#define ECHO_PIN 3
void setup() {
  DDRD|=(1<<TRIG_PIN);  // TRIG ca ieșire
  DDRD&=~(1<<ECHO_PIN); // ECHO ca intrare, "asculta" reflexia ultrasunetelor

  Serial.begin(9600);
}

void loop() {
  volatile uint16_t i;
  PORTD&=~(1<<TRIG_PIN);// ma asigur ca plec dintr-o stare cunoscuta, de la low la high
  delayMicroseconds(1);
  PORTD|=(1<<TRIG_PIN);//trigger input, TRIG pe high
  for(i=0;i<100;i++);//fortez procesorul sa incrementeze de 100 de ori, metoda mai precisa decat delay
    PORTD&=~(1<<TRIG_PIN);//mentin pinul TRIG sa stea pe high 10 secunde, apoi il pun pe low 
  while(!(PIND&(1<<ECHO_PIN)));// asteapta  
  unsigned long count=0;
  while(PIND&(1<<ECHO_PIN)) {// ECHO e high, ecoul "s-a intors"
    count++; // incep numaratoarea 
    if(count>5000)
      break;
  }
  float distance=count/120.0;
  Serial.print("Distanta: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);//in datasheet e recomandat sa folosim cicluri de masurare de 60 ms pentru a prezice interferenta echo cu trig, folosesc de 100 pentru siguranta 
}
