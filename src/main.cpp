#include <Arduino.h>
// ESTA VERSION DEL NANO ES GENERAL PARA TODOS LOS ESP DE KOMATSU

#include <DMD2.h>
#include <SPI.h>
#include <Separador.h> // liberia para separa string (se usa para separar valores en la trama de datos de comunicación con el UDP)

#include <fonts/SystemFont5x7.h>
#include <fonts/Droid_Sans_12.h>

#include <fonts/Arial_Black_16.h>
#include <fonts/Arial14.h>

// const int PIRPin = 2; ///////// PIR

Separador s; // instancia para separar string

// Set Width to the number of displays wide you have
const int WIDTH = 3;
// const int WIDTH = 1;

// You can change to a smaller font (two lines) by commenting this line,
// and uncommenting the line after it:
const uint8_t *FONT1 = Arial14;
const uint8_t *FONT2 = Arial_Black_16;
const uint8_t *FONT3 = SystemFont5x7;
const uint8_t *FONT4 = Droid_Sans_12;

SoftDMD dmd(WIDTH, 1);      // DMD controls the entire display
DMD_TextBox box(dmd, 0, 2); // "box" provides a text box to automatically write to/scroll the display

//**********************************
int PtoBuzzer = 12;
//**********************************
int cont_disp = 0;
const char *next = "connecting";

/////// Varibles para parpadear la pantalla /////////
unsigned long tiempo_millis = 0;
unsigned long tiempo_aux = 0;
bool parpadeo = false;
bool parpadeo_aux = false;

/////// Varibles de configuración de conteo  /////////
float inactivity_hours_reset = 2.5; // valores por defecto
int count_dalay_milisegundos = 200;
String abcd = "medium";
String estado = "on";

////////////////////////////////////////7

unsigned long int time_millis = 0;
unsigned long int before_time_millis = 0;
unsigned long int counter_millis = 0;

#include <contadorRoutine.h>

//********************************** RUTINES *********************************
//****************************************************************************

//////////////////////////////////////7
bool activacionBuzzer = false;
void Pantalla(bool buzzerActivate = true)
{
  Serial.println("Pantalla 0");
  if (total < aforo)
  {
    digitalWrite(PtoBuzzer, LOW);
    box.clear();
    box.print((String) " A:" + aforo + " T:");
    box.print(total);
  }
  else if (total > aforo)
  {
    if (buzzerActivate)
    {
      activacionBuzzer = true;
      digitalWrite(PtoBuzzer, HIGH);
      Serial.println("Superación de aforo BUZZER");
    }
    // delay(4000);
    // digitalWrite(PtoBuzzer, LOW);
    // Serial.println("Superación de aforo BUZZER DESACTIVADO");
  }
}

void Pantalla2()
{
  // Serial.println("Pantalla 2");
  if (parpadeo && !parpadeo_aux)
  {
    box.clear();
    box.print((String) "*no entrar");
    Serial.println("no entrar");
  }
  else if (parpadeo && parpadeo_aux)
  {
    box.clear();
    box.print((String) "*LLENO: ");
    box.print(total);
    Serial.println("LLENO");
  }
}

void Pantalla1()
{
  // Serial.println("Pantalla 1");
  if (aforo != 0 && parpadeo && !parpadeo_aux)
  {
    box.clear();
    box.print((String) " A:" + aforo + " T:");
    box.print(total);
    Serial.println("A:" + String(aforo) + " T:" + String(total));
  }
  else if (aforo != 0 && parpadeo && parpadeo_aux)
  {
    box.clear();
    box.print((String) "*LLENO: ");
    box.print(total);
    Serial.println("LLENO");
  }
}

void no_conecto_wifi()
{
  Serial.println((String) + "*** !!!! WIFI NO CONECTADO  !!!!!***");
  box.clear();
  box.print((String) "NULL WIFI*");
  delay(10000);
}

//****************************************************************************
//****************************************************************************

void setup()
{
  PORTD = 0b00111111;
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(FONT2);
  dmd.begin();
  pinMode(PtoBuzzer, OUTPUT); // D12   BUZZER
  // pinMode(PIRPin, INPUT);
  // Serial.println("hello world");
}

void loop()
{
  while (cont_disp <= 10) // mientras el esp8266 no se conecta a la red
  {
    box.print(*next);
    delay(350);
    next++;
    cont_disp++; // pantalla de connecting ...
  }


  if (counter_millis > count_dalay_milisegundos)
    censusPeople(); // tiempo ms delay de conteo

  if (total != total_aux)
  { // si total cambia de valor se actualiza la pantalla
    if (total_aux > total)
    {
      Pantalla(false); // si algien salió entonces no ejecuta el buzzer
    }
    else
    {
      Pantalla();
    }
    total_aux = total; // actualizamos valores
  }
  if (activacionBuzzer)
  {
    if (counter_millis > 4000)
    { // cuando pasen mas de 4s
      activacionBuzzer = false;
      digitalWrite(PtoBuzzer, LOW);
      Serial.println("Superación de aforo BUZZER DESACTIVADO");
    }
  }

  if (Serial.available())
  {
    // delay(10);s
    mensaje = Serial.readStringUntil('\n');
    if (mensaje.startsWith("!!!!!", 0))
    {
      no_conecto_wifi();
    }
    else if (mensaje.startsWith("/", 0)) // si es que el string empieza con / aforo y total recibidos por el esp
    {
      aforo = s.separa(mensaje, '/', 1).toInt();
      total = s.separa(mensaje, '/', 2).toInt();
    }
    else if (mensaje.startsWith("@", 0))
    {
      // mensaje esperado = @on@medium@32@0@3.5@200@
      estado = s.separa(mensaje, '@', 1); // estado
      abcd = s.separa(mensaje, '@', 2);   // estado
      aforo = s.separa(mensaje, '@', 3).toInt();
      total = s.separa(mensaje, '@', 4).toInt();
      inactivity_hours_reset = s.separa(mensaje, '@', 5).toFloat();
      count_dalay_milisegundos = s.separa(mensaje, '@', 6).toInt();
      Serial.println();
      Serial.println("Configuración recibida:");
      Serial.println(" - estado: " + estado);
      Serial.println(" - abcd: " + abcd);                                             // OK
      Serial.println(" - aforo: " + String(aforo));                                   // OK
      Serial.println(" - inactivity_hours_reset: " + String(inactivity_hours_reset)); // OK
      Serial.println(" - count_dalay_milisegundos: " + String(count_dalay_milisegundos));
    }
  }
  if (total != total_aux || aforo != aforo_aux) // si los datos cambiaron
  {
    Pantalla();        // actualizamos pantalla
    total_aux = total; // actualizamos valores
    aforo_aux = aforo;
    Serial.println((String) + "\nACT DISPLAY >> aforo " + aforo + " total " + total + "\n");
  }

  // rutinas para el reseteo del total por inactividad
  time_millis = millis();
  if (time_millis < 500)
  {                         // cuando millis desb orde despues de 50d aprox.
    before_time_millis = 0; // volvemos el tiempo_aux a 0;
  }

  if (before_time_millis != time_millis)
  {
    before_time_millis = time_millis;
    counter_millis++;
    // Serial.println("Segundos transcurridos: " + String(counter_millis));
  }
  if (counter_millis >= (3600000 * inactivity_hours_reset))
  { //(60000*horas_inactividad_max)    60000 = 1min    1hora(60min)= 3600000
    total = 0;
    counter_millis = 0;
    Serial.println("Reseteo del total por tiempo de innactividad");
  }

  /////////// rutinas para parpadeo de display //////////
  parpadeo = false;
  tiempo_millis = millis();
  if (tiempo_millis < 500)
  {                 // cuando millis desborde despues de 50d aprox.
    tiempo_aux = 0; // volvemos el tiempo_aux a 0;
  }
  if ((tiempo_millis - tiempo_aux) > 1000)
  {
    tiempo_aux = tiempo_millis;
    parpadeo = true;
    parpadeo_aux = !parpadeo_aux;
  }

  /////// rutinas para el display /////////////7
  if (aforo > 0 && aforo == total)
  {
    Pantalla1(); // DISPLAY ( A:00 T:00 || **LLENO)
    digitalWrite(PtoBuzzer, LOW);
  }

  if (total > aforo)
  {
    Pantalla2();
  }
}