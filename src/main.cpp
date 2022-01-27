#include <Arduino.h>
// ESTA VERSION DEL NANO ES GENERAL PARA TODOS LOS ESP DE KOMATSU

#include <DMD2.h>
#include <SPI.h>
#include <Separador.h>            // liberia para separa string (se usa para separar valores en la trama de datos de comunicación con el UDP)

#include <fonts/SystemFont5x7.h>
#include <fonts/Droid_Sans_12.h>


#include <fonts/Arial_Black_16.h>
#include <fonts/Arial14.h>

const int PIRPin= 2;            ///////// PIR

Separador s;                                        // instancia para separar string

// Set Width to the number of displays wide you have
const int WIDTH = 3;
//const int WIDTH = 1;


// You can change to a smaller font (two lines) by commenting this line,
// and uncommenting the line after it:
const uint8_t *FONT1 = Arial14;
const uint8_t *FONT2 = Arial_Black_16;
const uint8_t *FONT3 = SystemFont5x7;
const uint8_t *FONT4 = Droid_Sans_12;


SoftDMD dmd(WIDTH,1);  // DMD controls the entire display
DMD_TextBox box(dmd, 0, 2);  // "box" provides a text box to automatically write to/scroll the display
//*********************************************

unsigned int total = 0;
unsigned int aforo = 0;
unsigned int total_aux = 0;
unsigned int aforo_aux = 0;
String mensaje = "";

//**********************************
int PtoBuzzer = 12;
//**********************************
int cont_disp = 0;
const char *next = "connecting";


//********************************** RUTINES *********************************
//****************************************************************************
/////// Varibles para parpadear la pantalla /////////
unsigned long tiempo_millis=0;
unsigned long tiempo_aux=0;
bool parpadeo = false;
bool parpadeo_aux = false;
//////////////////////////////////////7


void Pantalla()
{
    if(total < aforo)
    {
      digitalWrite(PtoBuzzer, LOW);
      box.clear();         
      box.print((String) " A:" + aforo + " T:");
      box.print(total);
    }
    else if(total > aforo){
      digitalWrite(PtoBuzzer, HIGH);
      Serial.println("Superación de aforo BUZZER");
      delay(4000);
      digitalWrite(PtoBuzzer, LOW);
      Serial.println("Superación de aforo BUZZER DESACTIVADO");
    }
}

void Pantalla2(){
  if (parpadeo && !parpadeo_aux){
    box.clear();
    box.print((String) "*no entrar");
    Serial.println("no entrar");
  }
  else if (parpadeo && parpadeo_aux){
    box.clear();
    box.print((String) "*LLENO: ");
    box.print(total);
    Serial.println("LLENO");
  }
}

void Pantalla1(){
  if (aforo != 0 && parpadeo && !parpadeo_aux){
    box.clear();         
    box.print((String) " A:" + aforo + " T:");
    box.print(total);
    Serial.println("A:" +String(aforo) + " T:"+String(total));
  }
  else if (aforo!=0 && parpadeo && parpadeo_aux){
    box.clear();
    box.print((String) "*LLENO: ");
    box.print(total);
    Serial.println("LLENO");
  }
}

void no_conecto_wifi()
{
  Serial.println((String)+ "*** !!!! WIFI NO CONECTADO  !!!!!***");
  box.clear();
  box.print((String) "NULL WIFI*");
  delay(10000);
}

//****************************************************************************
//****************************************************************************


void setup() {

  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(FONT2);
  dmd.begin();
  pinMode(PtoBuzzer, OUTPUT); // D12   BUZZER
  pinMode(PIRPin, INPUT);

}
int total_before = 0;
int aforo_before = 0;
int value = 0;
void loop() 
{ 
  while (cont_disp <= 10)      // mientras el esp8266 no se conecta a la red
  {
    box.print(*next);
    delay(350);
    next++;
    cont_disp++;
  }
  
  if (Serial.available())
  {
    //delay(10);
    mensaje = Serial.readStringUntil('\n');
    if (mensaje.startsWith("!!!!!", 0))
    {
      no_conecto_wifi();
    }
    if (mensaje.startsWith("/", 0))     // si es que el string empieza con /
    {
      aforo = s.separa(mensaje,'/',1 ).toInt();
      total = s.separa(mensaje,'/',2 ).toInt();
    }
    
    if (total != total_aux || aforo != aforo_aux)       // si los datos cambiaron
    {
      Pantalla();           // actualizamos pantalla
      total_aux = total;    // actualizamos valores
      aforo_aux = aforo;
      Serial.println((String)+ "aforo "+ aforo + " total " + total);
    }
  }
  parpadeo=false;
  /////////// rutinas para parpadeo de display //////////
  tiempo_millis = millis();
  if (tiempo_millis < 500){ //cuando millis desborde despues de 50d aprox.
    tiempo_aux = 0;         // volvemos el tiempo_aux a 0;
  }
  if((tiempo_millis - tiempo_aux) > 1000){
    tiempo_aux = tiempo_millis;
    parpadeo = true;
    parpadeo_aux = !parpadeo_aux;
  }


  /////// rutinas para el display /////////////7
  if(aforo>0 && aforo == total){
    Pantalla1();      // DISPLAY ( A:00 T:00 || **LLENO)
    value= digitalRead(PIRPin);
    if (value && parpadeo){     // Si de detectamos a personas acercandose
      digitalWrite(PtoBuzzer, HIGH);
      Serial.print("Alarma -------- ");
    }
    else{
      digitalWrite(PtoBuzzer, LOW);
    }
  } else {
    digitalWrite(PtoBuzzer, LOW);
  }

  if (total>aforo){
    Pantalla2();
  }
}