// Sensor pH para control de ionizador de pileta
// Calibración para pH 4.01, 7.00, 10.01

#define PH_PIN A0           // Pin analógico del sensor pH
#define IONIZADOR_PIN 9     // Pin digital para relé del ionizador
#define INTERVALO_LECTURA 1000  // Leer cada 1 segundo

// Variables de calibración (ajustar según tu sensor)
float voltage, phValue;
float calibration_4 = 1.75;   // Voltaje medido en solución pH 4.01
float calibration_7 = 2.55;   // Voltaje medido en solución pH 7.00
float calibration_10 = 3.25;  // Voltaje medido en solución pH 10.01

bool ionizadorActivo = false;
unsigned long ultimoTiempo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(IONIZADOR_PIN, OUTPUT);
  digitalWrite(IONIZADOR_PIN, LOW);  // Ionizador apagado al inicio

}

void loop() {
  if (millis() - ultimoTiempo > INTERVALO_LECTURA) {
    
    // Leer sensor pH
    phValue = leerPH();
    
    // Control del ionizador
    controlarIonizador(phValue);
    
    // Mostrar en Serial Monitor
    Serial.print("pH: ");
    Serial.print(phValue, 2);
    Serial.print(" | Ionizador: ");
    Serial.println(ionizadorActivo ? "ACTIVO" : "APAGADO");
    
    ultimoTiempo = millis();
  }
}

float leerPH() {
  // Promedio de 10 lecturas para mayor precisión
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    int sensorValue = analogRead(PH_PIN);
    voltage = sensorValue * (5.0 / 1024.0);  // Convertir a voltaje
    sum += voltage;
    delay(10);
  }
  voltage = sum / 10;
  
  // Calcular pH con interpolación lineal
  if (voltage <= calibration_7) {
    phValue = 7.0 - ((calibration_7 - voltage) * 3.0 / (calibration_7 - calibration_4));
  } else {
    phValue = 7.0 + ((voltage - calibration_7) * 3.0 / (calibration_10 - calibration_7));
  }
  
  return phValue;
}

void controlarIonizador(float ph) {
  if (ph > 7.6) {
    // pH muy alto → REDUCIR ionización (apagar)
    digitalWrite(IONIZADOR_PIN, LOW);
    ionizadorActivo = false;
  } 
  else if (ph < 7.2) {
    // pH muy bajo → AUMENTAR ionización (encender)
    digitalWrite(IONIZADOR_PIN, HIGH);
    ionizadorActivo = true;
  }
  // Entre 7.2 y 7.6 → mantener estado actual (zona ideal)
}
