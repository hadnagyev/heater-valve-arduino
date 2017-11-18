/*-----( Import needed libraries )-----*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
#include <LiquidCrystal.h>
/*-----( Declare Constants )-----*/
#define ulazni_senzor_temp 7 /*-(Connect to Pin )-*/
#define izlazni_senzor_temp 8 /*-(Connect to Pin)-*/
#define kontaktor_zatvoren //kontaktor kada je ventil skroz zatvoren
#define kontaktor_otvoren //kontaktor kada je ventil skroz otvoren

/*-----( Declare objects )-----*/
Servo myservo;  // create servo object to control a servo
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire ourWire(ulazni_senzor_temp);
OneWire ourWire1(izlazni_senzor_temp);
/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature ulazni_senzor(&ourWire);
DallasTemperature izlazni_senzor(&ourWire1);
/*-----( Declare Variables )-----*/
int polozaj_ventila = 180; //polozaj servo motora na ventilu
int potpuno_otvoren_ventil = 180; //polozaj servo motora kada je ventil potpuno otvoren
int potpuno_zatvoren_ventil = 0; //polozaj servo motora kada je ventil potpuno zatvoren
int zatvoren_kontaktor = 6; 
int otvoren_kontaktor = 13;
int stepen_u_inicijalizaciji = 1; //po koliko stepeni da pomera ventil pri inicijalizaciji ventila. kada trazi skroz otvoreno i zatvoreno stanje
int brojac_otvaranja_pri_inicijalizaciji = 0;
int pauza_posle_temp = 1000; //pauza posle citanja oba senzora za temperaturu u milisekundama -- generalna pauza za sve
double razlika_u_temperaturama = 0;
int stepen_otvaranja_ventila=1; //za koliko da otvara pri razlici u temperaturi
int ulazna_temp;
int izlazna_temp;
int apsolutni_polozaj_ventila;
int broj_iteracija=0;
void setup() /*----( SETUP: RUNS ONCE )----*/
{
  lcd.begin(16, 2);
  lcd.print("inicijal'zacija");
  Serial.begin(9600);
  pinMode(zatvoren_kontaktor, INPUT);
  pinMode(otvoren_kontaktor, INPUT);
  myservo.attach(9);
  myservo.write(polozaj_ventila);  
  delay(10000);
while (digitalRead(zatvoren_kontaktor) == LOW){
  myservo.write(polozaj_ventila-stepen_u_inicijalizaciji);
  polozaj_ventila = polozaj_ventila-stepen_u_inicijalizaciji;
  Serial.println(polozaj_ventila-stepen_u_inicijalizaciji);
  brojac_otvaranja_pri_inicijalizaciji = brojac_otvaranja_pri_inicijalizaciji + stepen_u_inicijalizaciji;
  delay(500);
}
potpuno_zatvoren_ventil = polozaj_ventila;

delay(3000);
while (digitalRead(otvoren_kontaktor) == HIGH){
  myservo.write(polozaj_ventila+stepen_u_inicijalizaciji);
  polozaj_ventila = polozaj_ventila+stepen_u_inicijalizaciji;
  Serial.println(polozaj_ventila+stepen_u_inicijalizaciji);
  brojac_otvaranja_pri_inicijalizaciji = brojac_otvaranja_pri_inicijalizaciji + stepen_u_inicijalizaciji;
  delay(500);
}
Serial.print(potpuno_otvoren_ventil);
Serial.print(potpuno_zatvoren_ventil);
potpuno_otvoren_ventil = polozaj_ventila; //pronadjen polozaj kontaktora skroz otvoreno i postavljen kao inicijalna vrednost otvorenog ventila 

delay(1000);

Serial.println("YourDuino.com: Temperature Sensor Test Program");
Serial.println("Temperature Sensor: DS18B20");
delay(1000);
Serial.println(potpuno_zatvoren_ventil);
Serial.println(potpuno_otvoren_ventil);



/*-( Start up the DallasTemperature library )-*/
ulazni_senzor.begin();
}/*--(end setup )---*/


void loop() /*----( LOOP: RUNS CONSTANTLY )----*/
{

ulazni_senzor.requestTemperatures(); // Send the command to get temperatures
izlazni_senzor.requestTemperatures();

razlika_u_temperaturama = ulazni_senzor.getTempCByIndex(0)-izlazni_senzor.getTempCByIndex(0);
razlika_u_temperaturama = abs(razlika_u_temperaturama);
  lcd.setCursor(6, 1);
if(razlika_u_temperaturama>1){
  if(polozaj_ventila>potpuno_otvoren_ventil-1){
  }else if(digitalRead(otvoren_kontaktor) == LOW){
 lcd.print("OTVOREN");
  }else{
  myservo.write(polozaj_ventila+stepen_otvaranja_ventila);
  polozaj_ventila = polozaj_ventila+stepen_otvaranja_ventila;
 Serial.println("otvaram ");
 lcd.print("otvaram ");
  delay(pauza_posle_temp);
  }
}

if(razlika_u_temperaturama<1){
    if(polozaj_ventila<potpuno_zatvoren_ventil+15){
    }else if(digitalRead(zatvoren_kontaktor) == HIGH){
 lcd.print("ZATVOREN");
  }else{
    myservo.write(polozaj_ventila-stepen_otvaranja_ventila);
    polozaj_ventila = polozaj_ventila-stepen_otvaranja_ventila;
     Serial.println("zatvaram ");
     lcd.print("zatvaram ");
  delay(pauza_posle_temp);
  }
}
 lcd.print("              ");
 if(digitalRead(zatvoren_kontaktor) == HIGH){
  lcd.print("ZATVOREN");
 }
  if(digitalRead(otvoren_kontaktor) == LOW){
  lcd.print("OTVOREN");
 }
Serial.println("polozaj ventila, potpuno otvoren, potpuno zatvoren");
Serial.print(polozaj_ventila);
Serial.print("               ");
Serial.print(potpuno_otvoren_ventil);
Serial.print("               ");
Serial.println(potpuno_zatvoren_ventil);
Serial.print("Ulazni senzor temperature = ");
Serial.print(ulazni_senzor.getTempCByIndex(0));
Serial.println(" stepeni C");

Serial.print("Izlazni senzor temperature = ");
Serial.print(izlazni_senzor.getTempCByIndex(0));
Serial.println(" stepeni C");
Serial.println(" ------------------- ");
delay(pauza_posle_temp);
  lcd.setCursor(0, 0);
  lcd.print("UT");
  ulazna_temp = ulazni_senzor.getTempCByIndex(0);
  lcd.print(ulazna_temp);
  lcd.print(" ");
  lcd.print("IT");
  izlazna_temp = izlazni_senzor.getTempCByIndex(0);
  lcd.print(izlazna_temp);
  lcd.print(" RT");
  lcd.print(razlika_u_temperaturama);
  lcd.setCursor(0, 1);
  lcd.print("PV");
apsolutni_polozaj_ventila=(polozaj_ventila-potpuno_zatvoren_ventil)*100/(potpuno_otvoren_ventil-potpuno_zatvoren_ventil);
lcd.print(" ");
  Serial.println(apsolutni_polozaj_ventila);
  if(apsolutni_polozaj_ventila==100){
    apsolutni_polozaj_ventila=99;
  }
  if(apsolutni_polozaj_ventila<10){
lcd.setCursor(2, 1);
  lcd.print("0");
  lcd.print(int(apsolutni_polozaj_ventila));
  }else{
      lcd.print(int(apsolutni_polozaj_ventila));
  }

  if(ulazna_temp<30){
    if(broj_iteracija>14400){
      broj_iteracija=0;
      myservo.write(potpuno_zatvoren_ventil);
      delay(5000);
      myservo.write(potpuno_otvoren_ventil);
      polozaj_ventila = potpuno_otvoren_ventil;
      delay(5000);
    }
  }
  broj_iteracija++;
  Serial.print("br iteracija: ");
    Serial.println(broj_iteracija);
  
}/* --(end main loop )-- */

/* ( THE END ) */

