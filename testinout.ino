#include <PID_v1.h>

int pin_resistencia = 1;
int puerta = 7;
int bin;
int adc_temp;
int pwmVar = 0;
int pwm = 5;
int rele = 2;
int releVar = 0;
int analogVout = 2;
int analogRef = 3;
int auxrel;
int adc_vout, adc_ref, refaux;
int i, adc_sum = 0, adc;
double Iref317 ,Vref317;
double aux1, aux2, aux3, resistencia;
double auxt1, temp, auxt2 = 100.0 * 0.003850;
double pid_out, setpoint = 25.0;

//Specify the links and initial tuning parameters
PID myPID(&temp, &pid_out, &setpoint,2,5,1, DIRECT);

/*
 * PID(&Input, &Output, &Setpoint, Kp, Ki, Kd, Direction)
  Parameters
  Input: The variable we're trying to control (double)
  Output: The variable that will be adjusted by the pid (double)
  Setpoint: The value we want to Input to maintain (double)
  Kp, Ki, Kd: Tuning Parameters. these affect how the pid will chage the output. (double>=0)
  Direction: Either DIRECT or REVERSE. determines which direction the output will move when
  faced with a given error. DIRECT is most common
 */

void serialEvent()
{
  //------------------------------------------------------------
  /*
   * leo los valores que me llegan desde el labview y los paso de 
   * ASCII a enteros 
   */
  //------------------------------------------------------------
  
  pwmVar = Serial.parseInt();
  setpoint = pwmVar;
  releVar = Serial.parseInt();
  /*Serial.print("pase");
  Serial.println(releVar);*/
  if(releVar == 0)
  {
    digitalWrite(rele, LOW);
    //Serial.println("ejecute low");
  }
  else
  {
    digitalWrite(rele, HIGH);
    //Serial.println("ejecute high");
  }
}

void setup() {
  // put your setup code here, to run once:

  //abro la comunicacion serie
  Serial.begin(9600);

  //declaro inputs y outputs
  pinMode(puerta, INPUT_PULLUP);
  pinMode(pwm, OUTPUT);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);

  //inicio el PID
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //------------------------------------------------------------
  //leo entradas
  //------------------------------------------------------------
  
  //leo el potenciometro
  //filtro para evitar ruidos, leyendo 100 veces y tomo el promedio
  for(i = 0; i < 100; i++)
  {
    adc = analogRead(pin_resistencia);
    adc_sum += adc;
  }
  
  adc_temp = adc_sum / 100;
  
  //reinicializo el sumador
  adc_sum = 0;
  
  //leo el valor de tension en Vout y REF del 317
  adc_vout =analogRead(analogVout);
  adc_ref = analogRead(analogRef);

  //me fijo si la compuerta esta abierta. open= 1, close=0
  bin = digitalRead(puerta);


  //------------------------------------------------------------
  /*
   * proceso los valores analogicos para poder conocer la temperatura
   * supongo que utilizo una PT100
   */
  //------------------------------------------------------------

  //calculo el valor de referencia para poder calcular la cantidad de corriente que entrega el 317
  //refaux es el valor en formato de 10 bits
  refaux = adc_vout - adc_ref;

  //lo paso a valor de tension
  Vref317 = refaux *(5.0/1024.0);
  Iref317 = Vref317 / 560.0;

  //calculo el valor de la resistencia = ( 5 * ((lectura / 1024) - (Vref/56)) ) / (Vref / 560)
  aux1 = adc_temp / 1024.0;
  aux2 = Vref317 / 56.0;
  aux3 = 5.0 * (aux1 - aux2);
  resistencia = aux3 / Iref317;

  //calculo la temperatura en base a la aproximacion de Callendar - Van Dusen
  //donde Rt = R0 * (1 + alpha*t) con alpha=0.003850, despejando la temperatura
  //queda t = (Rt - R0)/(R0 * alpha)

  auxt1 = resistencia - 100.0;
  temp = auxt1 / auxt2; //auxt2 esta hardcodeado en la declaracion de variables para hacer mas rapido el loop

  //------------------------------------------------------------
  //llamo al PID para que tome una accion
  //------------------------------------------------------------

  myPID.Compute();

  //------------------------------------------------------------
  //mando los valores al labview
  //------------------------------------------------------------
  

  //valor de la lectura de temperatura
  Serial.print(temp);
  Serial.print(",");
  //valor de la salida del pid
  //Serial.print("pid_out:");
  Serial.print(pid_out);
  Serial.print(",");
  
  //valor de la compuerta  abierta = 0 ; cerrada = 1
  Serial.println(bin);

  //comando la salida pwm
  analogWrite(pwm, pid_out);

  delay(5);
}
