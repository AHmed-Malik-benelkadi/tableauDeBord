#include <LiquidCrystal.h> // Inclure la bibliothèque pour l'écran LCD

// Numéro des broches auxquelles sont
// connectées les LED
int ledBleue = 6;
int ledRouge = 7;
int ventilateur = 8;

int capteurTemp = A0;    // Port analogique pour lire la température

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialisation de l'écran LCD avec les broches correspondantes

int brightness = 0;       // Variable pour stocker la luminosité

// Définition des seuils d'humidité pour les différentes LED
const int seuilConfortable = 50;
const int seuilAlerte = 200;

// Configuration des broches de l'Arduino
const int pinHumidite = A1;
const int pinLedVerte = 10;
const int pinLedJaune = 9;

const int trigPin = A4;    // Broche TRIG du capteur de distance
const int echoPin = A3;   // Broche ECHO du capteur de distance
int buttonPin = 13;        // Broche utilisée pour le bouton
long duration, distance;


// le code dans cette fonction est exécuté une fois au début
void setup() {
  // indique que les broches des LED et du ventilateur sont des sorties :
  // on va modifier leur tension
  pinMode(ledBleue, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(ventilateur, OUTPUT);

  // Initialisation de l'écran LCD
  lcd.begin(16, 2);

  // On établit la liaison série à 9600 bits par seconde
  Serial.begin(9600);

  pinMode(pinLedVerte, OUTPUT);
  pinMode(pinLedJaune, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

 
// le code dans cette fonction est exécuté en boucle
void loop() {
  // On lit la tension en A0
  int valeurbrute = analogRead(capteurTemp);
  
  // La tension est un entier qui va de 0 à 1024 fois la tension de 5V
  float tension = (valeurbrute * 5 /1024.0);
  // On convertit en temperature en degré Celcius
  float temperature = (tension - 0.5) * 100;
  
  if (temperature > 22){
    digitalWrite(ledBleue, LOW);
    digitalWrite(ledRouge, LOW);
    digitalWrite(ventilateur, HIGH); // Démarrer le ventilateur
    Serial.print("Le the est trop chaud : ");
    lcd.setCursor(0, 0); // Positionner le curseur de l'écran LCD en haut à gauche
    lcd.print("Temp: "); // Afficher la température sur l'écran LCD
    lcd.print(temperature);
    lcd.print("C ");
  }  
  else if (temperature <= 22 && temperature >= 19){
    digitalWrite(ledBleue, LOW);
    digitalWrite(ventilateur, LOW); // Arrêter le ventilateur
    digitalWrite(ledRouge, HIGH);
    Serial.print("La temperature est correcte : ");
    lcd.setCursor(0, 0); // Positionner le curseur de l'écran LCD en haut à gauche
    lcd.print("Temp: "); // Afficher la température sur l'écran LCD
    lcd.print(temperature);
    lcd.print("C ");
  }  
  else if (temperature >= 10 && temperature < 19){
    digitalWrite(ledRouge, LOW);
    digitalWrite(ventilateur, LOW); // Arrêter le ventilateur
    digitalWrite(ledBleue, HIGH);
    Serial.print("La temperature est basse : ");
    lcd.setCursor(0, 0); // Positionner le curseur de l'écran LCD en haut à gauche
    lcd.print("Temp: "); // Afficher la température sur l'écran LCD
    lcd.print(temperature);
    lcd.print("C ");
  }
  else{
    digitalWrite(ledRouge, LOW);
    digitalWrite(ventilateur, LOW); // Arrêter le ventilateur
    digitalWrite(ledBleue, HIGH);
    Serial.print("La temperature est basse : ");
    lcd.setCursor(0, 0); // Positionner le curseur de l'écran LCD en haut à gauche
    lcd.print("Temp: "); // Afficher la température sur l'écran LCD
    lcd.print(temperature);
    lcd.print("C ");
  }
  // Lecture du taux d'humidité
  int humidite = analogRead(pinHumidite);

  // Conversion de la valeur lue en pourcentage
  float pourcentageHumidite = humidite * 100.0 / 1023.0;

  // Affichage de la valeur lue sur le moniteur série
  Serial.print("Humidite: ");
  Serial.print(pourcentageHumidite);
  Serial.println("%");

  // Allumage de la LED verte si l'humidité est confortable, sinon allumage de la LED jaune
  if (pourcentageHumidite <= seuilConfortable) {
    digitalWrite(pinLedVerte, HIGH);
    digitalWrite(pinLedJaune, LOW);
  } else if (pourcentageHumidite > seuilConfortable && pourcentageHumidite <= seuilAlerte) {
    digitalWrite(pinLedVerte, LOW);
    digitalWrite(pinLedJaune, HIGH);
  } else {
    digitalWrite(pinLedVerte, LOW);
    digitalWrite(pinLedJaune, LOW);
  }

  // Pause avant la lecture suivante du taux d'humidité
  //delay(1000);
  
   // Lecture de la valeur de la broche A0 pour obtenir la luminosité
  brightness = analogRead(A2);
  // Mesure de la distance à l'aide du capteur de distance HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

 // Affichage de la luminosité et de la distance sur l'afficheur LCD
  //lcd.setCursor(0, 0); // Positionne le curseur sur la deuxième ligne
  lcd.print("Luminosite:");
  lcd.print(brightness); // Affiche la valeur de la luminosité
  lcd.print(" Distance:");
  lcd.print(distance); // Affiche la valeur de la distance
 
  // Vérifie si le bouton est appuyé
  if (digitalRead(buttonPin) == LOW) {
  	delay(10); // Délai pour éviter les rebonds
    //Si le bouton est appuyé, fait défiler les informations sur l'afficheur LCD
    lcd.scrollDisplayLeft(); // Défilement vers la gauche
  }
  
  delay(100); // Délai pour rafraîchir la lecture de la luminosité et de la distance

 
}







