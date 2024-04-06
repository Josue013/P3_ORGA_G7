#include <HashDecoder.h>
#include <InfraredTypes.h>
#include <IrDecoder.h>
#include <IrReader.h>
#include <IrReceiver.h>
#include <IrReceiverPoll.h>
#include <IrReceiverSampler.h>
#include <IrSender.h>
#include <IrSenderNonMod.h>
#include <IrSenderPwm.h>
#include <IrSenderPwmHard.h>
#include <IrSenderPwmSoft.h>
#include <IrSenderPwmSoftDelay.h>
#include <IrSenderPwmSpinWait.h>
#include <IrSenderSimulator.h>
#include <IrSequence.h>
#include <IrSequenceReader.h>
#include <IrSignal.h>
#include <IrWidget.h>
#include <IrWidgetAggregating.h>
#include <MultiDecoder.h>
#include <Nec1Decoder.h>
#include <Nec1Renderer.h>
#include <Pronto.h>
#include <Rc5Decoder.h>
#include <Rc5Renderer.h>
#include <AccelStepper.h> // Librería para controlar el stepper motor


#include <Servo.h>
/*
#define S0 4		// S0 a pin 4
#define S1 5		// S1 a pin 5
#define S2 6		// S2 a pin 6
#define S3 7		// S3 a pin 7
#define salidaTCS 8	// salidaTCS a pin 8*/


#define S0 A8		// S0 a pin 4
#define S1 A9		// S1 a pin 5
#define S2 A10		// S2 a pin 6
#define S3 A11		// S3 a pin 7
#define salidaTCS A12 	// salidaTCS a pin 8
#define STEPS_PER_REVOLUTION 200

Servo Servo1;
AccelStepper stepper(1, 2, 3); 

void setup() {
  pinMode(S0, OUTPUT);		// pin 4 como salida
  pinMode(S1, OUTPUT);		// pin 5 como salida
  pinMode(S2, OUTPUT);		// pin 6 como salida
  pinMode(S3, OUTPUT);		// pin 7 como salida
  pinMode(salidaTCS, INPUT);	// pin 8 como salida
  
  Servo1.attach(7);
  digitalWrite(S0,HIGH);	// establece frecuencia de salida
  digitalWrite(S1,LOW);		// del modulo al 20 por ciento
  stepper.setMaxSpeed(1000); // Velocidad máxima del motor
  stepper.setAcceleration(500); // Aceleración del motor

  
  Serial.begin(9600);		// inicializa monitor serie a 9600 bps
}

void loop() {
  digitalWrite(S2,LOW);			// establece fotodiodos
  digitalWrite(S3,LOW);			// con filtro rojo
  int rojo = pulseIn(salidaTCS, LOW);	// obtiene duracion de pulso de salida del sensor
  delay(200);				// demora de 200 mseg
  
  digitalWrite(S2,HIGH);		// establece fotodiodos
  digitalWrite(S3,HIGH);		// con filtro verde
  int verde = pulseIn(salidaTCS, LOW);	// obtiene duracion de pulso de salida del sensor
  delay(200);				// demora de 200 mseg
  
  digitalWrite(S2,LOW);			// establece fotodiodos
  digitalWrite(S3,HIGH);		// con filtro azul
  int azul = pulseIn(salidaTCS, LOW);	// obtiene duracion de pulso de salida del sensor
  delay(200);				// demora de 200 mseg
  
  Serial.print("R:");			// muestra texto
  Serial.print(rojo);			// muestra valor de variable rojo

  Serial.print("\t");			// espacio de tabulacion

  Serial.print("V:");			// muestra texto
  Serial.print(verde);			// muestra valor de variable verde

  Serial.print("\t");			// espacio de tabulacion

  Serial.print("A:");			// muestra texto
  Serial.println(azul);			// muestra valor de variable azul
  					// y salto de linea
  
  if ( rojo < 150 && verde > 300 && azul > 60){		// si valores dentro del rango
    Serial.println("ROJO");				// muestra texto
  } 
    else if ( verde < 300 && rojo > 300 && azul > 80){	// si valores dentro del rango
    Serial.println("VERDE");				// muestra texto
      // Si es verde, girar 45 grados
    stepper.move(45 * STEPS_PER_REVOLUTION / 360);
    stepper.runToPosition();
    delay(10000); // Esperar 10 segundos
    // Volver a la posición inicial
    stepper.move(-45 * STEPS_PER_REVOLUTION / 360);
    stepper.runToPosition();
  }
    else if ( azul < 50 && rojo > 200 && verde > 100){	// si valores dentrol del rango
    Serial.println("AZUL");	
    Servo1.write(90);
     delay(2000);			// muestra texto
  }
  else{
    Serial.println("no se registró ningun  color");
     
  }

Servo1.write(0);
}
//exit(0); //descomentar para probarlo