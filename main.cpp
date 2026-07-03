#define PH_PIN A0          
#define IONIZADOR_PIN 9      
#define INTERVALO_LECTURA 1000  


float voltage, phValue;
float calibration_4 = 1.75;   
float calibration_7 = 2.55;  
float calibration_10 = 3.25; 

bool ionizadorActivo = false;
unsigned long ultimoTiempo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(IONIZADOR_PIN, OUTPUT);
  digitalWrite(IONIZADOR_PIN, LOW);  

}

void loop() {
  if (millis() - ultimoTiempo > INTERVALO_LECTURA) {
    
    
    phValue = leerPH();
    controlarIonizador(phValue);
    
    Serial.print("pH: ");
    Serial.print(phValue, 2);
    Serial.print(" | Ionizador: ");
    Serial.println(ionizadorActivo ? "ACTIVO" : "APAGADO");
    
    ultimoTiempo = millis();
  }
}

float leerPH() {
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    int sensorValue = analogRead(PH_PIN);
    voltage = sensorValue * (5.0 / 1024.0); 
    sum += voltage;
    delay(10);
  }
  voltage = sum / 10;
  
  if (voltage <= calibration_7) {
    phValue = 7.0 - ((calibration_7 - voltage) * 3.0 / (calibration_7 - calibration_4));
  } else {
    phValue = 7.0 + ((voltage - calibration_7) * 3.0 / (calibration_10 - calibration_7));
  }
  
  return phValue;
}

void controlarIonizador(float ph) {
  if (ph > 7.6) {
    digitalWrite(IONIZADOR_PIN, LOW);
    ionizadorActivo = false;
  } 
  else if (ph < 7.2) {
    digitalWrite(IONIZADOR_PIN, HIGH);
    ionizadorActivo = true;
  }
}
