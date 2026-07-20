#define PWM_PIN PB1
#define TRIG_PIN PD2  
#define ECHO_PIN PD3  
float citeste_senzor(){
  PORTD&=~(1<<TRIG_PIN);
  for(volatile uint16_t j=0;j<50;j++); 
  PORTD|=(1<<TRIG_PIN);
  for(volatile uint16_t j=0;j<250;j++); // Aprox 10us
  PORTD&=~(1<<TRIG_PIN);
  uint16_t timeout=0;
  while(!(PIND & (1 << ECHO_PIN))){//astept ca pinul ECHO sa devina HIGH 
    timeout++;
    if(timeout>200000)
      return 0;}

uint16_t start=TCNT1;//folosesc valoarea curenta a Timer 1;
while(PIND&(1<<ECHO_PIN));//astept pana pinul ECHO devine LOW
uint16_t sfarsit=TCNT1;
uint16_t durata=sfarsit-start;//calculez durata ca diferenta intre cele doua momente 
return (durata*0.5*0.0343)/2.0;//0.5 durata unui pas dupa aplicarea prescalerului de 8, 0.0343 cm/us viteza sunetului si impart la 2 pentru ca sunetul parcurge distanta dus-intors
}
void setup() {
  Serial.begin(9600);

  DDRB|=(1<<PWM_PIN); //setez pinul ca iesire, conform datasheet
  DDRD|=(1<<TRIG_PIN);//TRIG ca iesire 
  DDRD&=~(1<<ECHO_PIN);//ECHO ca intrare

  TCCR1A=(1<<COM1A1)|(1<<WGM11);//timer 1
  TCCR1B=(1<<WGM13)|(1<<WGM12)|(1<<CS11);//mod fast PWM, prescaler 8 pentru a obtine o perioada de 20 ms si frecventa de 50 Hz
  ICR1=39999;//setez ICR1 drept TOP pentru a controla Duty Cycle-ul folosind OCR1A
}

void loop() {
  OCR1A=2000;
  delay(500);
  float distanta0=citeste_senzor();
  Serial.print("D0: ");
  Serial.println(distanta0);

  OCR1A=3000;
  delay(500);
  float distanta90=citeste_senzor();
  Serial.print("D90: ");
  Serial.println(distanta90);

  OCR1A=4000;
  delay(500);
  float distanta180=citeste_senzor();
  Serial.print("D180: ");
  Serial.println(distanta180);

}
