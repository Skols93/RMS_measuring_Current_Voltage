#include "math.h"
#include <avr/sleep.h>

const double iREF = 1.067; // reference inside voltage of 1.1 volts  (referenten vnatreshen napon od 1.1 (meram 1.067v za poprecizna vrednost da mi dade))
//double REF = 1.070;
const float offset = 0.53; // offset (divide the referent voltage with 2)=0.53  ( 0.53 za 1.1 REF voltage,2.5 za 5v REF)
const float odnos = 37.10; // ratio of the voltage transformer (odnos na naponskiot transformator 233/6.3(otprilika ,jas merev i taka presmetav))
int a1 = A0; // channel A0 for current transformer
int a2 = A1; // channel A1 for voltage transformer
//const double vRMS = 230.0;

const double nNavivki = 500; // windings of the current transformer (navivki na strujno trafo)
const double rOtpornik = 55; // resistor conected to the ends of the current transformer (otpor da nema visoki naponi na strujno trafo)

const unsigned int nSemplovi = 600;// number of samples for sampling values of the current and voltage (semplovi za merenje na naponot i strujata)
unsigned long msec = 0;
float time = 0.0;
int sample = 0;
float totalCharge = 0.0;
float averageAmps = 0.0;
float ampSeconds = 0.0;
float wattHours  = 0.0;
float ampHours = 0.0;
float kWh = 0.0;
float skapaT = 5.5;
 
 void setup() {

  
 //ADMUX = iREF;
 // ADCSRA |= 1<<ADEN;
 // ADCSRA |= 1 << ADIE;
 // ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));

//  sleep_enable();
//  set_sleep_mode(SLEEP_MODE_ADC);
  Serial.begin(9600);
  pinMode(a1,INPUT);
  pinMode(a2,INPUT);
}

void loop() {
  double semplovi; //promenliva za izmerenata struja
  double semplovi1;//promenliva za izmeren napon
  double napon;// promenliva za strujata
  double voltage;//promenliva za napon
  double iPrimar;//promenlivata za da go dobijam naponot od izlezot na strujnoto trafo
  double acc=0.0; //sumiranje na kvadriranite vrednosti od struja
  double acc1 = 0.0; //promenlivi za sumiranje na kvadriranite vrednosti od napon
  double iRMS;
  double vRMS ;
  double appPower;

  for(int i=0;i < nSemplovi ; i++)
  {
  
    analogReference(INTERNAL); //vnatresen referenten napon(toj pogore od 1.1v) za podobra rezolucija na izmeren napon
    
    semplovi = analogRead(0); //na kanal 0 mi e strujnoto trafo
    

    semplovi1 = analogRead(1);  //na kanal 1 mi e naponskoto trafo
    
    napon = (semplovi * iREF / 1024);
    napon = napon - offset;
    //vo napon ja smestuvam izmereneta vrednost i odzimam offset bidejki imam virtuelna nula 

    voltage = (semplovi1*1055.832)/1024); // 0.696*37*41=1055.832
    voltage = voltage - (offset*1504.7);
   //36.7*41=1504.7
 //voltage promenliva za naponot od naponsko trafo i odzemam offset da dobijam vistinska vrednost 
    iPrimar = (napon * nNavivki)/ rOtpornik;
    //Serial.println(semplovi1);
    
    acc += pow(iPrimar,2);
    acc1 += pow(voltage,2);
    //gi sumiram semplovite koi se staveni na kvadrat
  }
    iRMS = sqrt(acc / nSemplovi);
    vRMS = sqrt(acc1 / nSemplovi);
    //RMS vrednosti za struja i napon
    
    appPower = vRMS * iRMS;
    //Serial.print(" iRMS = ");
    //Serial.print(iRMS);
    Serial.print(" vRMS = ");
    Serial.println(vRMS);
    //Serial.print(" appPower = ");
    //Serial.println(appPower);
   //debagirav i zatoa se komentirani liniive pogore
    sample = sample + 1;
    msec = millis();

    time = (float)msec/1000.0;

    totalCharge = totalCharge + iRMS;
    averageAmps = totalCharge/sample;
    ampSeconds = averageAmps*time;
    ampHours = ampSeconds/3600;
    wattHours = vRMS * ampHours;
    kWh = wattHours / 1000;
    float cena = kWh * skapaT;
//    Serial.print(" WattHours = ");
//    Serial.print(wattHours);
//    Serial.print(" kWatthours = ");
//    Serial.print(kWh);
//    Serial.print(" Cena = ");
//    Serial.print(cena);
//    Serial.println(" denari");
    delay(100);
}
