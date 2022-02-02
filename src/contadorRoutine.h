int total = 0;
int aforo = 28;              //<<<<<<<<<<<< AFORO
int total_aux = 0;
int aforo_aux = 0;

int value = 0;

String mensaje = "";

//========== variables para la subrutina - censusPeople() ===========
byte Sensor_1 = LOW;
byte Sensor_2 = LOW;
#define Sensor_1 4
#define Sensor_2 5
//#define PtoBuzzer 2 /////////<<<<<<<<< SALIDA -> BUZZER



int Sensor1_Time = 0;
int Sensor2_Time = 0;
int Same_Time = 0;

int Last_Sensor1_Time = 0;
int Last_Sensor2_Time = 0;
int Last_Same_Time = 0;

int Sensor1_Min_Time = 1;  // SISTEMA ANTI - RUIDO PARA
int Sensor2_Min_Time = 1;  // SISTEMA ANTI - RUIDO PARA
int Sametime_Min_Time = 0; // KOMATSU ANTI - RUIDO PARA
unsigned int last_value = 0;


// int cont=0;

String A = "X";
String B = "X";
String C = "X";
String D = "X";

void censusPeople()
{
  ////////////////////////////////////////////////////////////
  // TIEMPO (NO EXACTO ES SOLO UN CONTADOR) PARA EL LOW DEL SENSOR_1
  if (digitalRead(Sensor_1) == LOW)
  {
    Sensor1_Time++;
    // Serial.println("Sensor1_Time = "+ String(Sensor1_Time));
    if (Sensor1_Time != 0)
    {
      Last_Sensor1_Time = Sensor1_Time;
      // Serial.println(Last_Sensor1_Time);
    }
  }
  else
  {
    Sensor1_Time = 0;
  }

  // TIEMPO (NO EXACTO ES SOLO UN CONTADOR) PARA EL LOW DEL SENSOR_2
  if (digitalRead(Sensor_2) == LOW)
  {
    Sensor2_Time++;
    // Serial.println("Sensor2_Time = "+ String(Sensor2_Time));
    if (Sensor2_Time != 0)
    {
      Last_Sensor2_Time = Sensor2_Time;
      // Serial.println(Last_Sensor2_Time);
    }
  }
  else
  {
    Sensor2_Time = 0;
  }

  // TIEMPO (NO EXACTO ES SOLO UN CONTADOR) PARA EL LOW DEL SENSOR_2
  if (digitalRead(Sensor_1) == LOW && digitalRead(Sensor_2) == LOW)
  {
    Same_Time++;
    // Serial.println("Same_Time  = "+ String(Same_Time));
    if (Same_Time != 0)
    {
      Last_Same_Time = Same_Time;
      // Serial.println(Last_Same_Time);
    }
  }
  else
  {
    Same_Time = 0;
  }

  // Imp_Conteo_Tiempo_Real();
  // mpresion_ABCD();
  // Solo_Cont();

  // A - A - A - A - A - A - A - A - A - A - A - A - A - A - A - A - A - A -
  // A - A - A - A - A - A - A - A - A - A - A - A - A - A - A - A - A - A -
  // ASIGNANDO "A" SENSOR 1
  if (digitalRead(Sensor_1) == LOW && A == "X")
  {
    A = "1E";
  }
  // ASIGNANDO "A" SENSOR 2
  if (digitalRead(Sensor_2) == LOW && A == "X")
  {
    A = "2E";
  }

  // LIBERANDO "A" SENSOR 1
  if (digitalRead(Sensor_1) == HIGH && A == "1E" && B == "X" && C == "X" && D == "X")
  {
    A = "X";
  }

  // LIBERANDO "A" SENSOR 2
  if (digitalRead(Sensor_2) == HIGH && A == "2E" && B == "X" && C == "X" && D == "X")
  {
    A = "X";
  }
  // B - B - B - B - B - B - B - B - B - B - B - B - B - B - B - B - B - B -
  // B - B - B - B - B - B - B - B - B - B - B - B - B - B - B - B - B - B -
  // ASIGNANDO "B" SENSOR 1
  if (digitalRead(Sensor_1) == LOW && A == "2E")
  {
    B = "1E";
  }
  // ASIGNANDO "B" SENSOR 2
  if (digitalRead(Sensor_2) == LOW && A == "1E")
  {
    B = "2E";
  }

  // LIBERANDO "B" SENSOR 1
  if (digitalRead(Sensor_1) == HIGH && A == "2E" && B == "1E" && C == "X" && D == "X")
  {
    B = "X";
  }

  // LIBERANDO "B" SENSOR 2
  if (digitalRead(Sensor_2) == HIGH && A == "1E" && B == "2E" && C == "X" && D == "X")
  {
    B = "X";
  }
  // C - C - C - C - C - C - C - C - C - C - C - C - C - C - C - C - C - C -
  // C - C - C - C - C - C - C - C - C - C - C - C - C - C - C - C - C - C -
  // ASIGNANDO "C" SENSOR 1
  if (digitalRead(Sensor_1) == HIGH && A == "1E" && B == "2E")
  {
    C = "1S";
  }
  // ASIGNANDO "C" SENSOR 2
  if (digitalRead(Sensor_2) == HIGH && A == "2E" && B == "1E")
  {
    C = "2S";
  }

  // LIBERANDO "C" SENSOR 1
  if (digitalRead(Sensor_1) == LOW && A == "1E" && B == "2E" && C == "1S" && D == "X")
  {
    C = "X";
  }

  // LIBERANDO "C" SENSOR 2
  if (digitalRead(Sensor_2) == LOW && A == "2E" && B == "1E" && C == "2S" && D == "X")
  {
    C = "X";
  }

  // D - D - D - D - D - D - D - D - D - D - D - D - D - D - D - D - D - D -
  // D - D - D - D - D - D - D - D - D - D - D - D - D - D - D - D - D - D -

  // ASIGNANDO "D" SENSOR 1
  if (digitalRead(Sensor_1) == HIGH && A == "2E" && B == "1E" && C == "2S")
  {
    D = "1S";
  }
  // ASIGNANDO "D" SENSOR 2
  if (digitalRead(Sensor_2) == HIGH && A == "1E" && B == "2E" && C == "1S")
  {
    D = "2S";
  }

  if (abcd == "low")
  {
    //////////////////////// LOW //////////////////////////////77
    // Serial.println("low");
    if (digitalRead(Sensor_2) == HIGH && A == "1E" && B == "2E")
    { // && B == "2E" C == "1S" && D == "2S"

      // FILTRO POR TIEMPO DE PULSO (TIEMPO SENSOR 1, TIEMPO SENSOR 2, AL MISMO TIEMPO
      if (Last_Sensor1_Time >= Sensor1_Min_Time && Last_Sensor2_Time >= Sensor2_Min_Time && Last_Same_Time >= Sametime_Min_Time)
      {

        // Serial.println ("ENTRÓ");

        // Serial.println ("PERSONA ENTRANDO");
        // Realiza la acción deseada

        // cli(); //Deshabilitar interrupciones
        //ingreso++;
        total++;
      }
      else
      {
        Serial.println("CONTEO DE ENTRADA POR RUIDO, NO VÁLIDO");

        // Imp_Conteo_Ultimos_Tiempos();
      }

      A = "X";
      B = "X";
      C = "X";
      D = "X";
    }

    if (digitalRead(Sensor_1) == HIGH && A == "2E" && B == "1E")
    { //&& B == "1E" && C == "2S" && D == "1S"

      // FILTRO POR TIEMPO DE PULSO (TIEMPO SENSOR 1, TIEMPO SENSOR 2, AL MISMO TIEMPO
      if (Last_Sensor1_Time >= Sensor1_Min_Time && Last_Sensor2_Time >= Sensor2_Min_Time && Last_Same_Time >= Sametime_Min_Time)
      {

        // Serial.println ("SALIÓ");

        // Serial.println ("PERSONA SALIENDO");
        // cli(); //Deshabilitar interrupciones
        //egreso++;
        if ((total - 1) >= 0 ) total--;

        // if (total < 0)
        //    {
        //     total = 0;
        //     egreso--;
        //    }
      }
      else
      {
        Serial.println("CONTEO DE SALIDA POR RUIDO, NO VÁLIDO");

        // IMPRESION DE ULTIMO TIEMPO REGISTRADO
        // Imp_Conteo_Ultimos_Tiempos();
      }

      A = "X";
      B = "X";
      C = "X";
      D = "X";
    }
  }
  else if (abcd == "medium")
  {
    //////////////////////// MEDIUM //////////////////////////////77
    // Serial.println("meduim");
    if (digitalRead(Sensor_2) == HIGH && A == "1E" && B == "2E" && C == "1S")
    { // && B == "2E" C == "1S" && D == "2S"

      // FILTRO POR TIEMPO DE PULSO (TIEMPO SENSOR 1, TIEMPO SENSOR 2, AL MISMO TIEMPO
      if (Last_Sensor1_Time >= Sensor1_Min_Time && Last_Sensor2_Time >= Sensor2_Min_Time && Last_Same_Time >= Sametime_Min_Time)
      {

        // Serial.println ("ENTRÓ");

        // Serial.println ("PERSONA ENTRANDO");
        // Realiza la acción deseada

        // cli(); //Deshabilitar interrupciones
        //ingreso++;
        total++;
      }
      else
      {
        Serial.println("CONTEO DE ENTRADA POR RUIDO, NO VÁLIDO");

        // Imp_Conteo_Ultimos_Tiempos();
      }

      A = "X";
      B = "X";
      C = "X";
      D = "X";
    }

    if (digitalRead(Sensor_1) == HIGH && A == "2E" && B == "1E" && C == "2S")
    { //&& B == "1E" && C == "2S" && D == "1S"

      // FILTRO POR TIEMPO DE PULSO (TIEMPO SENSOR 1, TIEMPO SENSOR 2, AL MISMO TIEMPO
      if (Last_Sensor1_Time >= Sensor1_Min_Time && Last_Sensor2_Time >= Sensor2_Min_Time && Last_Same_Time >= Sametime_Min_Time)
      {

        // Serial.println ("SALIÓ");

        // Serial.println ("PERSONA SALIENDO");
        // cli(); //Deshabilitar interrupciones
        //egreso++;
        if ((total - 1) >= 0 ) total--;

        // if (total < 0)
        //    {
        //     total = 0;
        //     egreso--;
        //    }
      }
      else
      {
        Serial.println("CONTEO DE SALIDA POR RUIDO, NO VÁLIDO");

        // IMPRESION DE ULTIMO TIEMPO REGISTRADO
        // Imp_Conteo_Ultimos_Tiempos();
      }

      A = "X";
      B = "X";
      C = "X";
      D = "X";
    }
  }
  else
  {
    //////////////////////// HIGH //////////////////////////////77
    // Serial.println("high");
    if (digitalRead(Sensor_2) == HIGH && A == "1E" && B == "2E" && C == "1S" && D == "2S")
    { // && B == "2E" C == "1S" && D == "2S"

      // FILTRO POR TIEMPO DE PULSO (TIEMPO SENSOR 1, TIEMPO SENSOR 2, AL MISMO TIEMPO
      if (Last_Sensor1_Time >= Sensor1_Min_Time && Last_Sensor2_Time >= Sensor2_Min_Time && Last_Same_Time >= Sametime_Min_Time)
      {

        // Serial.println ("ENTRÓ");

        // Serial.println ("PERSONA ENTRANDO");
        // Realiza la acción deseada

        // cli(); //Deshabilitar interrupciones
        //ingreso++;
        total++;
      }
      else
      {
        Serial.println("CONTEO DE ENTRADA POR RUIDO, NO VÁLIDO");

        // Imp_Conteo_Ultimos_Tiempos();
      }

      A = "X";
      B = "X";
      C = "X";
      D = "X";
    }

    if (digitalRead(Sensor_1) == HIGH && A == "2E" && B == "1E" && C == "2S" && D == "1S")
    { //&& B == "1E" && C == "2S" && D == "1S"

      // FILTRO POR TIEMPO DE PULSO (TIEMPO SENSOR 1, TIEMPO SENSOR 2, AL MISMO TIEMPO
      if (Last_Sensor1_Time >= Sensor1_Min_Time && Last_Sensor2_Time >= Sensor2_Min_Time && Last_Same_Time >= Sametime_Min_Time)
      {

        // Serial.println ("SALIÓ");

        // Serial.println ("PERSONA SALIENDO");
        // cli(); //Deshabilitar interrupciones
        //egreso++;
        if ((total - 1) >= 0 ) total--;

        // if (total < 0)
        //    {
        //     total = 0;
        //     egreso--;
        //    }
      }
      else
      {
        Serial.println("CONTEO DE SALIDA POR RUIDO, NO VÁLIDO");

        // IMPRESION DE ULTIMO TIEMPO REGISTRADO
        // Imp_Conteo_Ultimos_Tiempos();
      }

      A = "X";
      B = "X";
      C = "X";
      D = "X";
    }
  }

  if (last_value != total)
  {
    if (last_value < total)
    {

      //Serial.print("entro | "); // entro
      Serial.println("/" + String(aforo) + "/" + String(total) + "/"); // entro
    }
    else
    {
      //Serial.print("salio | ");  // salio
      Serial.println("/" + String(aforo) + "/" + String(total) + "/"); // entro
    }
    last_value = total;
    counter_millis = 0;
  }

}