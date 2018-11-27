#include <Servo.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(22, 24, 26, 28, 11, 12); //actualizamos para usar con el mega
char dato;
Servo hombro, codo, pinza, base, muneca;
int phombro, pcodo, ppinza, pbase, pmuneca, contador=0;

const int PIN_HOMBRO = 5;
const int ANG_MIN_HOMBRO=10;
const int POSICION_INICIAL_HOMBRO=135; //Se debe de cambiar por la lectura actual del servo, para eso vamos en primer lugar generar un codigo que lo que haga sea leer las posiciones y las devuelva por el puerto serie 
const int ANG_MAX_HOMBRO=180;

const int PIN_CODO = 6;
const int ANG_MIN_CODO=10;
const int POSICION_INICIAL_CODO=90;
const int ANG_MAX_CODO=180;

const int PIN_PINZA = 10;
const int ANG_MIN_PINZA=6;
const int POSICION_INICIAL_PINZA=30;
const int ANG_MAX_PINZA=70;

const int PIN_muneca = 9;
const int ANG_MIN_muneca=10;
const int POSICION_INICIAL_muneca=86;
const int ANG_MAX_muneca=180;

const int PIN_BASE = 3;
const int ANG_MIN_BASE=10;
const int POSICION_INICIAL_BASE=86;
const int ANG_MAX_BASE=180;

const int PASO_ANGULO = 9;

volatile int state = LOW;
boolean ban = false;

int method =0;
byte phase='z';
int address=0;
int motores=0;
int interruptPin = 2;

//metodos de eeprom
void readPos();
void clearEEPROM();
void writePos();
void secuencia2();
void buttonPressed();
String readString;

void setup()
{
  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  // Print a message to the LCD
  lcd.print("Modo");
  hombro.attach(PIN_HOMBRO);
  codo.attach(PIN_CODO);
  pinza.attach(PIN_PINZA);
  base.attach(PIN_BASE);
  muneca.attach(PIN_muneca);
  inicializarServos();
  Serial.begin(9600);
  //attachInterrupt(0,buttonPressed,RISING);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPressed, CHANGE);
}


char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}


void inicializarServos(){
     // Leer posicion inicial
    phombro=hombro.read();
    pcodo=codo.read();
    ppinza=pinza.read();
    pbase=base.read();
    pmuneca = muneca.read();
    readPos();
    //Escribir posicion inicial
    //hombro.write(POSICION_INICIAL_HOMBRO);
    //codo.write(POSICION_INICIAL_CODO);
    //pinza.write(POSICION_INICIAL_PINZA);
    //base.write(POSICION_INICIAL_BASE);
    //muneca.write(POSICION_INICIAL_muneca);
    
}

void loop()
{
  readString = Serial.readString();
 /*while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the String readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }*/
  //Serial.print(readString.length());

  
  if(readString.length() >=2){
    lcd.setCursor(0, 1);
    lcd.print("Cargando...      ");
    //Serial.print(readString);
    int num = readString.toInt();
    if(num >10 && num<178 && motores==0){
        motores=1;
        //Serial.print("Entro aqui 1");
        hombro.write(num);
      }else if(num >10 && num<178 && motores==1){
        motores=2;
        //Serial.print("Entro aqui 2");
        codo.write(num);
      } else if(num >10 && num<178 && motores==2){
          motores=3;
          //Serial.print("Entro aqui 3");
          pinza.write(num);
      }   else if(num >10 && num<178 && motores==3){
            motores=4;
            base.write(num);
      }     else if(num >10 && num<178 && motores==4){
              motores=5;
              muneca.write(num);
      }
      lcd.setCursor(0, 1);
    lcd.print("Secuencia...      ");
  }else if(readString.length()==1){
    lcd.setCursor(0, 1);
    lcd.print("Manual      ");
    dato=readString[0];
    //Serial.print(dato);
    switch(dato)
      {
        //Hombro -------------------------------------------
        case 'w': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Hombro");
        //Serial.println(phombro);
          if(phombro>ANG_MAX_HOMBRO){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  phombro=phombro+PASO_ANGULO;
                  hombro.write(phombro);
                  }           

          break;  
        }
        case 's': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Hombro");
        //Serial.println(phombro);
          if(phombro<ANG_MIN_HOMBRO){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  phombro=phombro-PASO_ANGULO;
                  hombro.write(phombro);

                  }
              
          break;  
        }
      // Codo -------------------------------------------
      case 'q': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Codo");
        //Serial.println(pcodo);
          if(pcodo>ANG_MAX_CODO){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  pcodo=pcodo+PASO_ANGULO;
                  codo.write(pcodo);
                  }           

          break;  
        }
        case 'e':
        {
          lcd.setCursor(0, 1);
          lcd.print("Manual      ");
          //Serial.println("Codo");
          //Serial.println(pcodo);
          if(pcodo<ANG_MIN_CODO){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  pcodo=pcodo-PASO_ANGULO;
                  codo.write(pcodo);
                  }
              
          break;  
        }
        // Pinza -------------------------------------------
        case 'p': 
        {
        lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Pinza");
        //Serial.println(ppinza);
          if(ppinza>ANG_MAX_PINZA){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  ppinza=ppinza+PASO_ANGULO;
                  pinza.write(ppinza);
                  }           

          break;  
        }    
        case 'o': 
        {
         lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println(ppinza);
          if(ppinza<ANG_MIN_PINZA){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  ppinza=ppinza-PASO_ANGULO;
                  pinza.write(ppinza);
                  }
              
          break;  
        }
        //Base -------------------------------------------
        case 'a': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Base");
        //Serial.println(pbase);
          if(pbase>ANG_MAX_BASE){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  pbase=pbase+PASO_ANGULO;
                  base.write(pbase);
                  }           

          break;  
        } 
        case 'd': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
          //Serial.println("Base");
          //Serial.println(pbase);
          if(pbase<ANG_MIN_BASE){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  pbase=pbase-PASO_ANGULO;
                  base.write(pbase);
                  }
              
          break;  
        }
        case '3': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("muneca");
        //Serial.println(pmuneca);
          if(pmuneca>ANG_MAX_muneca){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  pmuneca=pmuneca+PASO_ANGULO;
                  muneca.write(pmuneca);
                  }           

          break;  
        } 
        case '4': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
         // Serial.println("Muñeca");
          //Serial.println(pmuneca);
          if(pmuneca<ANG_MIN_muneca){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  pmuneca=pmuneca-PASO_ANGULO;
                  muneca.write(pmuneca);
                  }
              
          break;  
        }
        case '1':
        {
          secuencia1();
          break;
        }
        case '2':
        {
          secuencia2();
          break;
        }
        case 'g':
        {
          Serial.println(hombro.read());
          Serial.println(codo.read());
          Serial.println(pinza.read());
          Serial.println(base.read());
          Serial.println(muneca.read());
          
          break;
        }
        default: {
          //Serial.println("Caracter no valido");
          break;
        }
      }
  }
/*  if (Serial.available() > 0){
 /*     
      //dato=Serial.read();
      dato=readString;
      Serial.print(dato);
      if(dato >'10' && dato<'178' && motores==0){
        motores=1;
        hombro.write(dato);
      }
      if(dato >'10' && dato<'178' && motores==1){
        motores=2;
        hombro.write(dato);
      }
      if(dato >'10' && dato<'178' && motores==2){
        motores=3;
        hombro.write(dato);
      }
      if(dato >'10' && dato<'178' && motores==3){
        motores=4;
        hombro.write(dato);
      }
      if(dato >'10' && dato<'178' && motores==4){
        motores=5;
        hombro.write(dato);
      }
      */
/*      switch(dato)
      {
        //Hombro -------------------------------------------
        case 'w': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Hombro");
        //Serial.println(phombro);
          if(phombro>ANG_MAX_HOMBRO){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  phombro=phombro+PASO_ANGULO;
                  hombro.write(phombro);
                  }           

          break;  
        }
        case 's': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Hombro");
        //Serial.println(phombro);
          if(phombro<ANG_MIN_HOMBRO){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  phombro=phombro-PASO_ANGULO;
                  hombro.write(phombro);

                  }
              
          break;  
        }
      // Codo -------------------------------------------
      case 'q': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Codo");
        //Serial.println(pcodo);
          if(pcodo>ANG_MAX_CODO){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  pcodo=pcodo+PASO_ANGULO;
                  codo.write(pcodo);
                  }           

          break;  
        }
        case 'e':
        {
          lcd.setCursor(0, 1);
          lcd.print("Manual      ");
          //Serial.println("Codo");
          //Serial.println(pcodo);
          if(pcodo<ANG_MIN_CODO){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  pcodo=pcodo-PASO_ANGULO;
                  codo.write(pcodo);
                  }
              
          break;  
        }
        // Pinza -------------------------------------------
        case 'p': 
        {
        lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Pinza");
        //Serial.println(ppinza);
          if(ppinza>ANG_MAX_PINZA){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  ppinza=ppinza+PASO_ANGULO;
                  pinza.write(ppinza);
                  }           

          break;  
        }    
        case 'o': 
        {
         lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println(ppinza);
          if(ppinza<ANG_MIN_PINZA){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  ppinza=ppinza-PASO_ANGULO;
                  pinza.write(ppinza);
                  }
              
          break;  
        }
        //Base -------------------------------------------
        case 'a': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("Base");
        //Serial.println(pbase);
          if(pbase>ANG_MAX_BASE){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  pbase=pbase+PASO_ANGULO;
                  base.write(pbase);
                  }           

          break;  
        } 
        case 'd': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
          //Serial.println("Base");
          //Serial.println(pbase);
          if(pbase<ANG_MIN_BASE){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  pbase=pbase-PASO_ANGULO;
                  base.write(pbase);
                  }
              
          break;  
        }
        case '3': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
        //Serial.println("muneca");
        //Serial.println(pmuneca);
          if(pmuneca>ANG_MAX_muneca){
                //Serial.println("N");
                break;
            } else {
                  writePos();
                  pmuneca=pmuneca+PASO_ANGULO;
                  muneca.write(pmuneca);
                  }           

          break;  
        } 
        case '4': 
        {
          lcd.setCursor(0, 1);
        lcd.print("Manual      ");
         // Serial.println("Muñeca");
          //Serial.println(pmuneca);
          if(pmuneca<ANG_MIN_muneca){
                  //Serial.println("N");
                  break;
                } else {
                  writePos();
                  pmuneca=pmuneca-PASO_ANGULO;
                  muneca.write(pmuneca);
                  }
              
          break;  
        }
        case '1':
        {
          secuencia1();
          break;
        }
        case '2':
        {
          secuencia2();
          break;
        }
        case 'g':
        {
          Serial.println(hombro.read());
          //Serial.println(hombro.read());
          Serial.println(muneca.read());
          Serial.println(pinza.read());
          Serial.println(codo.read());
          Serial.println(base.read());
          break;
        }
        default: {
          //Serial.println("Caracter no valido");
          break;
        }
      }
  }*/
   //delay(100);
}

void secuencia1(){
  method=1;
//Servo hombro, codo, pinza, base, muneca;
// set the cursor to column 0, line 1
// (line 1 is the second row, since counting begins with 0)
lcd.setCursor(0, 1);
// print the number of seconds since reset:
lcd.print("Secuencia 1");
pinza.write(80);
muneca.write(90);
hombro.write(90);
codo.write(90);
base.write(90);

int pos = 80;
int a = 80;
for(pos = 90; pos >=0; pos--)
{
  pinza.write(pos);
  delay(5);
}
for(pos = 0; pos <= 90; pos++)
{
  pinza.write(pos);
  delay(5);
}
for(pos = 90; pos >=0; pos--)
{
  pinza.write(pos);
  delay(5);
}
for(pos = 0; pos <= 90; pos++)
{
  pinza.write(pos);
  delay(5);
}

for(pos = 90; pos >= 0; pos--)
{
  pinza.write(pos);
  delay(5);
}

for(pos = 0; pos <= 90; pos++)
{
  pinza.write(pos);
  delay(5);
}
delay(500);


//ahora corregimos el siguiente movimiento

for(pos = 90; pos <= 180; pos++)
{
  muneca.write(pos);
  delay(5);
}

for(pos= 180; pos >=0; pos--)
{
  muneca.write(pos);
  delay(5);
}

for(pos= 0; pos <=90; pos++)
{
  muneca.write(pos);
  delay(5);
}

for(pos= 90; pos >=0 ; pos--)
{
  base.write(pos);
  delay(10);  
}


for(pos=90; pos >=0; pos--)
{ 
  codo.write(pos);
  hombro.write(pos);
  delay(10);
}
 
for(pos= 0; pos<= 50; pos++)
{
  hombro.write(pos);
  delay(10);
}

for(pos= 90; pos>=0; pos--)
{
  pinza.write(pos);
  delay(10);
}


 for(pos= 0; pos <=150; pos++)
{
  codo.write(pos);
  hombro.write(50+(2*pos)/3);
  delay(10);
}


for(pos= 0; pos<=180; pos++)
{
  base.write(pos);
  muneca.write(90+pos/2);
  delay(10);
}


for(pos=150; pos>=0; pos--)
 { 
  codo.write(pos);
  hombro.write(50+(2*pos/3));
  muneca.write(90+3*pos/5);
  delay(10);
 }

for(pos= 0; pos<=90; pos++)
{
  pinza.write(pos);
  delay(10);
}

for(pos= 0; pos<=90; pos++)
{
  codo.write(pos);
  hombro.write(50+4*pos/9);
  base.write(180-pos);
  delay(10);
}

//delay(12000);
}

void secuencia2(){
//Servo hombro, codo, pinza, base, muneca;

lcd.setCursor(0, 1);
lcd.print("Secuencia 2");
base.write(80);
muneca.write(80);
hombro.write(120);
codo.write(120);
pinza.write(75);
int pos = 90;
int a = 90;
for(pos = 90; pos >=0; pos--)
{
  base.write(pos);
  delay(5);
}
for(pos = 0; pos <= 90; pos++)
{
  pinza.write(pos);
  delay(5);
}
for(pos = 90; pos >=0; pos--)
{
  base.write(pos);
  delay(5);
}
for(pos = 0; pos <= 90; pos++)
{
  pinza.write(pos);
  delay(5);
}
for(pos = 90; pos >= 0; pos--)
{
  base.write(pos);
  delay(5);
}
for(pos = 0; pos <= 90; pos++)
{
  pinza.write(pos);
  delay(5);
}
delay(500);


//ahora corregimos el siguiente movimiento
for(pos = 90; pos <= 180; pos++)
{
  muneca.write(pos);
  delay(5);
}
for(pos= 180; pos >=0; pos--)
{
  muneca.write(pos);
  delay(5);
}
for(pos= 0; pos <=90; pos++)
{
  muneca.write(pos);
  delay(5);
}
for(pos= 90; pos >=0 ; pos--)
{
  base.write(pos);
  delay(10);  
}
for(pos=90; pos >=0; pos--)
{ 
  codo.write(pos);
  hombro.write(pos);
  delay(10);
}

for(pos= 0; pos<= 50; pos++)
{
  hombro.write(pos);
  delay(10);
}

for(pos= 90; pos>=0; pos--)
{
  base.write(pos);
  delay(10);
}


 for(pos= 0; pos <=150; pos++)
{
  codo.write(pos);
  hombro.write(50+(2*pos)/3);
  delay(10);
}


for(pos= 0; pos<=180; pos++)
{
  base.write(pos);
  muneca.write(90+pos/2);
  delay(10);
}


for(pos=150; pos>=0; pos--)
 { 
  codo.write(pos);
  hombro.write(50+(2*pos/3));
  muneca.write(90+3*pos/5);
  delay(10);
 }

for(pos= 0; pos<=90; pos++)
{
  base.write(pos);
  delay(10);
}

for(pos= 0; pos<=90; pos++)
{
  codo.write(pos);
  hombro.write(50+4*pos/9);
  base.write(180-pos);
  delay(10);
}

//delay(12000);
}

void readPos()
{
  hombro.write(EEPROM.read(0));
  codo.write(EEPROM.read(1));
  pinza.write(EEPROM.read(2));
  base.write(EEPROM.read(3));
  muneca.write(EEPROM.read(4));
  //method=EEPROM.read(5);
  //phase =EEPROM.read(6);
  /*if(method==1){
    secuencia1();
  }else if(method==2){
    secuencia2();
  }*/
}
 
void clearEEPROM()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    if(EEPROM.read(i) != 0)                     //skip already "empty" addresses
    {
      EEPROM.write(i, 0);                       //write 0 to address i
    }
  }
  Serial.println("EEPROM erased");
  address = 0;                                  //reset address counter
}
 
void writePos()
{
  EEPROM.write(0,hombro.read());
  EEPROM.write(1,codo.read());
  EEPROM.write(2,pinza.read());
  EEPROM.write(3,base.read());
  EEPROM.write(4,muneca.read());
  //EEPROM.write(5,method);
  //EEPROM.write(6,phase);
}

//metemos la interrupcion
void buttonPressed(){
  state != state; 
  while(state==HIGH){
     lcd.setCursor(0, 0);
     lcd.print("PARO      ");
  }
  
}
