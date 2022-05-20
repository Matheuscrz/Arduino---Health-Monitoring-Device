/**
 * @file main.cpp
 * @author Matheus Lima da Cruz (matheuslimasof.eng@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <Arduino.h>
//Configuração de Função que substituira o delay
unsigned long time = 0;
//Biblioteca para modulo Bluethoot
#include <SoftwareSerial.h>
//Definindo portas para modulo bluethoot
SoftwareSerial bluethoot(10, 11);

//Bibliotecas para sensor de temperatura
#include <OneWire.h>
#include <DallasTemperature.h>
//Setando Pino de dados do modulo de temperatura
#define ONE_WIRE_BUS A1
OneWire oneWire(ONE_WIRE_BUS); //Protocolo OneWire
DallasTemperature sensors(&oneWire); //Encaminha referências OneWire para o sensor

//Biblioteca para sensor de batimentos
#include <PulseSensorPlayground.h>
//Configurando interrupções de baixo nível para captura de BPMs mais precisa.
#define USE_ARDUINO_INTERRUPTS true 
const int PulseWire = A0;
int Limite = 550;

PulseSensorPlayground pulseSensor; //Criação de Instancia do sensor de batimentos

int valor = 0;
int LED = 13;

void setup() {
  Serial.begin(9600);
  //Inicia a biblioteca do sensor de temperatura
  sensors.begin(); 
  //Configura o PulseSensor atribuindo variáveis a ele.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED); //Led ligado quando batimento detectado seguindo de acordo com o BPM.
  pulseSensor.setThreshold(Limite);
}

void loop() {
  time = millis(); //Definindo tempo
  if (Serial.available()){
    Serial.println("Deseja verificar os batiementos e a temperatura?");
    Serial.println("Digite 1 para verificar ou 2 para sair");
    char caractere = (char)Serial.read();
    if (caractere == '1'){
      Serial.println("Verificando...");
      sensors.requestTemperatures(); //Requisita temperatura
      Serial.print(sensors.getTempCByIndex(0)); //Imprime temperatura
      while(millis() < time + 20){
      //Configurações do Sensor de BPM
        int BPM = pulseSensor.getBeatsPerMinute(); //Captura BPM
        if (pulseSensor.sawStartOfBeat()){ //Testa constantemente se há batimentos
          Serial.println("BPM: ");
          Serial.println(BPM);
        }
      }
    }
    else if (caractere == '2') {
    Serial.println("Saindo...");
    }
    else {
    Serial.println("Digite 1 para verificar ou 2 para sair");
    }
  }
}
