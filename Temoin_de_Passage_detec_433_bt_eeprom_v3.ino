/*
Temoin de Passage

Recepteur 433 + allumage led en fonction passage
Appuie prologé sur le bouton pour enregistrer les different capteur dans eeprom (aprés coupure electrique, on retrouve nos parametre)
Dans le setup, aprés 5-6s d'appuie, toutes les led s'allume puis la led correspondant au capteur a regler s'allume, lors d'une recepteur 433Mhz la led clignote puis on reclic sur le bouton pour paramétrer le suivant
@Christoams 01/07/2015
*/

//Variable temps d'allumage led lors detection passage
long TempDetec = 300000; //temps en milliseconde



//Déclaration variable pour le calcul de temps
long temps[16] ; //variable qui stocke la mesure du temps 16 case dans un tableau
int ValTemp[16] ; //variable qui indique si le temps c'est finis ou non 16 compteurs possible

//Valeur de la durée de la tempo
int Tempo[16]; ; //Init de la tempo 

int debug = 0; //variable débug (retour moniteur série)

//433
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

//Bouton appuie long
   int temps_defini = 5000;   //définition du temps de maintien désiré en ms
   int boutonPin1 = A0;  //pin du bouton analog
   int boutonReset = A4; //pin bouton RAZ EEPROM
   long debut_appui1;
   boolean variable1 = false;
   int etat_bouton1;
   int dernier_etat_bouton1 = LOW;
   int etat_led1 = LOW;
   int Menu = 0; //Menu :0=pas d'appuie long, 1=appuie long, 2,3,4 etc different >Menu
   int MenuMax = 14; //nombre de menu
   int DebugMenu = 0; //Debug 1=on, 0=off
   int Temporaire = 0; //variable temporaire

//EEPROM
//Needed to access the eeprom read write functions
#include <EEPROM.h> 

//Stockage des N° de capteur (lu au démarage de l'arduino dans une EEPROM)
long number1 ;
long number2 ;
long number3 ;
long number4 ;
long number5 ;
long number6 ;
long number7 ;
long number8 ;
long number9 ;
long number10 ;
long number11 ;
long number12 ;
long number13 ;
long number14 ;
long number15 ;
long number16 ;

//declaration multiple des led
int led[14]={3,3,3,3,4,5,6,7,8,9,10,11,12,13};              //Déclaration des pattes des leds, de 0 a 16, les pattes utilisé pour autre chose dois être renseigné avec une patte libre
                                                            //Patte 2 reservé au recepteur 433Mhz d'ou les 3 premiere definition a 3 (patte 0,1,2)

//Fonction d'ecriture d'une variable long sur 4 bit
//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }

//Fonction de lecture d'une variable long sur 4 bit
//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
long EEPROMReadlong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }

void setup()
      {
  if ((DebugMenu > 0)|| (debug > 0)){
      Serial.begin(9600);
      }
      //fonction temp
      for(int i=0;i<16;i++){
                    temps[i] = millis(); //copie du temps écoulé depuis démarrage µc dans la variable temps
                    }
                    
      //led control initialisation
      for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    pinMode(led[i],OUTPUT);       //Indique que les leds sont des sorties
                    digitalWrite(led[i], LOW);    // RAZ des leds déclarées
                    delay(10); 
                   }
      
      //433
      mySwitch.enableReceive(0);
      pinMode(2, INPUT); //patte 2 en input
      
      //lecture des N° de capteur dans l'EEPROM
      
      number1 = EEPROMReadlong(0);
      if (DebugMenu == 1){
      Serial.print("Capteur 1 = ");
      Serial.println(number1);
      }
      
      number2 = EEPROMReadlong(4);
      if (DebugMenu == 1){
      Serial.print("Capteur 2 = ");
      Serial.println(number2);
      }
      
      number3 = EEPROMReadlong(8);
      if (DebugMenu == 1){
      Serial.print("Capteur 3 = ");
      Serial.println(number3);
      }
      
      number4 = EEPROMReadlong(12);
      if (DebugMenu == 1){
      Serial.print("Capteur 4 = ");
      Serial.println(EEPROMReadlong(12));
      }  
      number5 = EEPROMReadlong(16);
      
      if (DebugMenu == 1){
      Serial.print("Capteur 5 = ");
      Serial.println(number5);
      }
      number6 = EEPROMReadlong(20);
      if (DebugMenu == 1){
      Serial.print("Capteur 6 = ");
      Serial.println(number6);
      }
      
      number7 = EEPROMReadlong(24);
      if (DebugMenu == 1){
      Serial.print("Capteur 7 = ");
      Serial.println(number7);
      }
      
      number8 = EEPROMReadlong(28);
      if (DebugMenu == 1){
      Serial.print("Capteur 8 = ");
      Serial.println(number8);
      }
      
      number9 = EEPROMReadlong(32);
      if (DebugMenu == 1){
      Serial.print("Capteur 9 = ");
      Serial.println(number9);
      }
      
      number10 = EEPROMReadlong(36);
      if (DebugMenu == 1){
      Serial.print("Capteur 10 = ");
      Serial.println(number10);
      }
      
      number11 = EEPROMReadlong(40);
      if (DebugMenu == 1){
      Serial.print("Capteur 11 = ");
      Serial.println(number11);
      }
      
      number12 = EEPROMReadlong(44);
      if (DebugMenu == 1){
      Serial.print("Capteur 12 = ");
      Serial.println(number12);
      }
      
      number13 = EEPROMReadlong(48);
      if (DebugMenu == 1){
      Serial.print("Capteur 13 = ");
      Serial.println(number13);
      }
      
      number14 = EEPROMReadlong(52);
      if (DebugMenu == 1){
      Serial.print("Capteur 14 = ");
      Serial.println(number14);
      }
      
      number15 = EEPROMReadlong(56);
      if (DebugMenu == 1){
      Serial.print("Capteur 15 = ");
      Serial.println(number15);
      }
      
      number16 = EEPROMReadlong(60);
      if (DebugMenu == 1){
      Serial.print("Capteur 16 = ");
      Serial.println(number16);
      }  
    }

void loop()
      {
//lancement de la fonction temp avec en variable le N° de la case du tableau 
temp(3, TempDetec);
temp(4, TempDetec);
temp(5, TempDetec);
temp(6, TempDetec);
temp(7, TempDetec);
temp(8, TempDetec);
temp(9, TempDetec);
temp(10, TempDetec);
temp(11, TempDetec);
temp(12, TempDetec);
temp(13, TempDetec);
        
//Bouton      
Bouton(); //fonction Bouton appuie long Menu
      
      
//EEPROM lors que l'on rentre dans le setup des capteur  
      if (Menu >= 1){
        
       if (Menu == 1){Menu = 3;}
        
        //Serial.println("debut d'ecriture dans EEPROM");
        if (Temporaire == 1){
        for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], HIGH);    // RAZ des leds déclarées
                    delay(10); 
                   }
                   delay(500);
                   for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], LOW);    // RAZ des leds déclarées
                    delay(10); 
                   }
                   delay(500);
                   
                   Temporaire =0; //raz variable
                   }
        //Allume la led correspondant au capteur a regler
        digitalWrite(led[Menu], HIGH);    // RAZ des leds déclarées
                   
        //433
 if (mySwitch.available()) {
 
    int value = mySwitch.getReceivedValue();
    
  Serial.print("Dispositif N° = ");
  Serial.println(value);
  
                    digitalWrite(led[Menu], LOW);    // RAZ des leds déclarées
                    delay(500); 
                    digitalWrite(led[Menu], HIGH);    // RAZ des leds déclarées
 
    // on remet à zero le timer
  //while (!Serial) ;
      //Ecriture dans EEPROM
      if (Menu == 1){
      EEPROMWritelong(0, value);
      }
      if (Menu == 2){  
      EEPROMWritelong(4, value);
                     }
      if (Menu == 3){
      EEPROMWritelong(8, value);
                    }
     if (Menu == 4){
      EEPROMWritelong(12, value);
                    }
      if (Menu == 5){
      EEPROMWritelong(16, value);
                     }
      if (Menu == 6){
      EEPROMWritelong(20, value);
                    }
      if (Menu == 7){
      EEPROMWritelong(24, value);
                     }
      if (Menu == 8){
      EEPROMWritelong(28, value);
                    }
      if (Menu == 9){
      EEPROMWritelong(32, value);
                    }
      if (Menu == 10){
      EEPROMWritelong(36, value);
                      }
      if (Menu == 11){
      EEPROMWritelong(40, value);
                      }
      if (Menu == 12){
      EEPROMWritelong(44, value);
                      }
      if (Menu == 13){
      EEPROMWritelong(48, value);
                      }
      if (Menu == 14){
      EEPROMWritelong(52, value);
                      }
      if (Menu == 15){
      EEPROMWritelong(56, value);
                      }
      if (Menu == 16){
      EEPROMWritelong(60, value);
                      }


    mySwitch.resetAvailable();
                                          }

}

if (Menu ==0){ //fonctionnement normal, pas de setup

//433
 if (mySwitch.available()) {
 
    int value = mySwitch.getReceivedValue();
    
  Serial.print("Dispositif N° = ");
  Serial.println(value);
 
    // on remet à zero le timer
  while (!Serial) ;
 
 if (value == number1){
   ValTemp[1] = 1;
   }
  if (value == number2){
   ValTemp[2] = 1;
   }
  if (value == number3){
   ValTemp[3] = 1;
   }
  if (value == number4){
   ValTemp[4] = 1;
   }
  if (value == number5){
   ValTemp[5] = 1;
   } 
    if (value == number6){
   ValTemp[6] = 1;
   } 
    if (value == number7){
   ValTemp[7] = 1;
   } 
    if (value == number8){
   ValTemp[8] = 1;
   } 
    if (value == number9){
   ValTemp[9] = 1;
   } 
    if (value == number10){
   ValTemp[10] = 1;
   } 
    if (value == number11){
   ValTemp[11] = 1;
   } 
    if (value == number12){
   ValTemp[12] = 1;
   } 
    if (value == number13){
   ValTemp[13] = 1;
   } 
   
 
    mySwitch.resetAvailable();
                                          }
                                          }

  
if ((Menu == 0) & (DebugMenu == 1)){
delay(1000);
      Serial.print("Capteur 1 = ");
      Serial.println(EEPROMReadlong(0));

      Serial.print("Capteur 2 = ");
      Serial.println(EEPROMReadlong(4));
      
      Serial.print("Capteur 3 = ");
      Serial.println(EEPROMReadlong(8));

      Serial.print("Capteur 4 = ");
      Serial.println(EEPROMReadlong(12));

      Serial.print("Capteur 5 = ");
      Serial.println(EEPROMReadlong(16));

      Serial.print("Capteur 6 = ");
      Serial.println(EEPROMReadlong(20));

      Serial.print("Capteur 7 = ");
      Serial.println(EEPROMReadlong(24));

      Serial.print("Capteur 8 = ");
      Serial.println(EEPROMReadlong(28));

      Serial.print("Capteur 9 = ");
      Serial.println(EEPROMReadlong(32));

      Serial.print("Capteur 10 = ");
      Serial.println(EEPROMReadlong(36));

      Serial.print("Capteur 11 = ");
      Serial.println(EEPROMReadlong(40));

      Serial.print("Capteur 12 = ");
      Serial.println(EEPROMReadlong(44));

      Serial.print("Capteur 13 = ");
      Serial.println(EEPROMReadlong(48));

      Serial.print("Capteur 14 = ");
      Serial.println(EEPROMReadlong(52));

      Serial.print("Capteur 15 = ");
      Serial.println(EEPROMReadlong(56));

      Serial.print("Capteur 16 = ");
      Serial.println(EEPROMReadlong(60));
      
      }
      }
      
      
      void Bouton(){
        //RAZ EEPROM si bouton Reset appuié
      if ((analogRead(boutonReset) > 800)){
       delay(20);
       EEPROMWritelong(0, 0);
       EEPROMWritelong(4, 0);
       EEPROMWritelong(8, 0);
       EEPROMWritelong(12, 0);
       EEPROMWritelong(16, 0);
       EEPROMWritelong(20, 0);
       EEPROMWritelong(24, 0);
       EEPROMWritelong(28, 0);
       EEPROMWritelong(32, 0);
       EEPROMWritelong(34, 0);
       EEPROMWritelong(38, 0);
       EEPROMWritelong(42, 0);
       EEPROMWritelong(46, 0);
       EEPROMWritelong(52, 0);
       EEPROMWritelong(56, 0);
       
       //lecture eeprom dans variable
       number1 = EEPROMReadlong(0);
       number2 = EEPROMReadlong(4);
       number3 = EEPROMReadlong(8);
       number4 = EEPROMReadlong(12);
       number5 = EEPROMReadlong(16);
       number6 = EEPROMReadlong(20);
       number7 = EEPROMReadlong(24);
       number8 = EEPROMReadlong(28);
       number9 = EEPROMReadlong(32);
       number10 = EEPROMReadlong(36);
       number11 = EEPROMReadlong(40);
       number12 = EEPROMReadlong(44);
       number13 = EEPROMReadlong(48);
       number14 = EEPROMReadlong(52);
       number15 = EEPROMReadlong(56);
       number16 = EEPROMReadlong(60);
       //chenillard reset
      for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], HIGH);    // RAZ des leds déclarées
                    delay(200); 
                   }
        for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], LOW);    // RAZ des leds déclarées
                    delay(200); 
                   }            
       }
       
     //detection appuie long pour parametrage capteur  
     if ((analogRead(boutonPin1) > 600)){
          etat_bouton1 = HIGH;
           Temporaire = 1;}
          else
         {etat_bouton1 = LOW;}         //lecture de l'état des boutons
         
     //    Serial.print("Bouton = ");
     //    Serial.println(etat_bouton1);
      if (etat_bouton1 == HIGH && dernier_etat_bouton1 == LOW){      //détection de l'appui sur le bouton 1
         debut_appui1 = millis();                                   //initialisation du compteur 1
         variable1 = true;          //enregistrement du changement d'état du bouton 1
         
         if (Menu >=1){
           Menu = (Menu + 1) ;
          if (DebugMenu == 1){
                           Serial.print("Menu = ");
                           Serial.println(Menu);
                          }
           if (Menu >= MenuMax)
                             {
                               Serial.print("toto");
                               Menu = 0; //on revient a 0
                                if (DebugMenu == 1){
                                                     Serial.print("Menu sort = ");
                                                     Serial.println(Menu);
                                                    }
      //recopie des nouvelle valeur capteur depuis EEPROM
      number1 = EEPROMReadlong(0);
      if (DebugMenu == 1){
      Serial.print("Capteur 1 = ");
      Serial.println(number1);
      }
      
      number2 = EEPROMReadlong(4);
      if (DebugMenu == 1){
      Serial.print("Capteur 2 = ");
      Serial.println(number2);
      }
      
      number3 = EEPROMReadlong(8);
      if (DebugMenu == 1){
      Serial.print("Capteur 3 = ");
      Serial.println(number3);
      }
      
      number4 = EEPROMReadlong(12);
      if (DebugMenu == 1){
      Serial.print("Capteur 4 = ");
      Serial.println(EEPROMReadlong(12));
      }  
      number5 = EEPROMReadlong(16);
      
      if (DebugMenu == 1){
      Serial.print("Capteur 5 = ");
      Serial.println(number5);
      }
      number6 = EEPROMReadlong(20);
      if (DebugMenu == 1){
      Serial.print("Capteur 6 = ");
      Serial.println(number6);
      }
      
      number7 = EEPROMReadlong(24);
      if (DebugMenu == 1){
      Serial.print("Capteur 7 = ");
      Serial.println(number7);
      }
      
      number8 = EEPROMReadlong(28);
      if (DebugMenu == 1){
      Serial.print("Capteur 8 = ");
      Serial.println(number8);
      }
      
      number9 = EEPROMReadlong(32);
      if (DebugMenu == 1){
      Serial.print("Capteur 9 = ");
      Serial.println(number9);
      }
      
      number10 = EEPROMReadlong(36);
      if (DebugMenu == 1){
      Serial.print("Capteur 10 = ");
      Serial.println(number10);
      }
      
      number11 = EEPROMReadlong(40);
      if (DebugMenu == 1){
      Serial.print("Capteur 11 = ");
      Serial.println(number11);
      }
      
      number12 = EEPROMReadlong(44);
      if (DebugMenu == 1){
      Serial.print("Capteur 12 = ");
      Serial.println(number12);
      }
      
      number13 = EEPROMReadlong(48);
      if (DebugMenu == 1){
      Serial.print("Capteur 13 = ");
      Serial.println(number13);
      }
      
      number14 = EEPROMReadlong(52);
      if (DebugMenu == 1){
      Serial.print("Capteur 14 = ");
      Serial.println(number14);
      }
      
      number15 = EEPROMReadlong(56);
      if (DebugMenu == 1){
      Serial.print("Capteur 15 = ");
      Serial.println(number15);
      }
      
      number16 = EEPROMReadlong(60);
      if (DebugMenu == 1){
      Serial.print("Capteur 16 = ");
      Serial.println(number16);
      } 
                       
                       //sortie du Menu Setup, toutes les led clignotes                                 
                    for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], HIGH);    // RAZ des leds déclarées
                    delay(10); 
                   }
                   delay(500);
                   for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], LOW);    // RAZ des leds déclarées
                    delay(10); 
                   }
                   delay(500);
                   for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], HIGH);    // RAZ des leds déclarées
                    delay(10); 
                   }
                   delay(500);
                   for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], LOW);    // RAZ des leds déclarées
                    delay(10); 
                   }
                   delay(500);
                    for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], HIGH);    // RAZ des leds déclarées
                    delay(10); 
                   }
                   delay(500);
                   for(int i=3;i<14;i++){                  //Boucle parcourant toutes les variables de 3 à 14
                    digitalWrite(led[i], LOW);    // RAZ des leds déclarées
                    delay(10); 
                   }
                                     
                             }
                               
                       }
          
    }
   
   
      if ( variable1 == true && etat_bouton1 == HIGH && dernier_etat_bouton1 == HIGH){    //le bouton 1 était et est toujours appuyé
                                                                                 
         if ((millis() - debut_appui1) >= temps_defini){             //est-ce que le temps choisi est écoulé ?                                 
            etat_led1 = HIGH;                              //déclaration d'allumage de la led 1 
            //delay(2000);
                                      
            if (Menu == 0){
                           Menu = 3; //on rentre dans le menu aprés appuie long
                           if (DebugMenu == 1){
                                               Serial.print("Menu = ");
                                               Serial.println(Menu);
                                               }
                           }
               
         }             
      }
      
      if (etat_bouton1 == LOW){                              //extinction de la led 1 si le bouton est relaché
         etat_led1 = LOW;
      }
      
      dernier_etat_bouton1 = etat_bouton1;                       //actualisation de l'état du bouton
}


int temp (int i, long z){      //Fontion temp, change la variable ValTemp et allume une led pendand x seconde (voir Tempo)
  
                if (ValTemp[i] == 1){  //on verifie que ValTemp soit a 1 pour allumer la led puis lancer la tempo, il repassera alors a 0 une fois le temps finis
                                     
                                     digitalWrite(i, HIGH); //On allume la led
                                      delay(20);
                                      temps[i] = millis(); //on stocke la nouvelle heure
                                      ValTemp[i] = 2; //etat de la variable a 2 pour rentrer dans la tempo
                                     
                                      if (debug == 1){ //option de dégug 
                                                      Serial.print("temp N°");
                                                      Serial.print(i);
                                                      Serial.print(" = ");
                                                      Serial.println(ValTemp[i]);
                                                      Serial.println("temp = ");
                                                      Serial.print(millis());
                                                      Serial.println("tempo = ");
                                                      Serial.println(temps[i]);
                                                      Serial.println("Led");
                                                      Serial.print(i);
                                                      Serial.println("durée = ");
                                                      Serial.print(z);
                                                      }
                                                     
                                      
                                      }
                 if (ValTemp[i] == 2){  //on verifie que ValTemp soit a 2 pour lancer la tempo, il repassera alors a 0 une fois le temps finis 
                 
                                      //on compare l'ancienne valeur du temps et la valeur sauvée
                                      if((millis() - temps[i]) > z)
                                                                      {
                                                                        
                                                                        ValTemp[i] = 0; //RAZ ValTemp pour ne plus utiliser la tempo
                                                                        if (debug == 1){ //option de dégug 
                                                                                        Serial.print("temp N°");
                                                                                        Serial.print(i);
                                                                                        Serial.print(" = ");
                                                                                        Serial.println(ValTemp[i]);
                                                                                        Serial.print("temp = ");
                                                                                        Serial.print(millis());
                                                                                        Serial.print("tempo = ");
                                                                                        Serial.println(temps[i]);
                                                                                        Serial.print("Led");
                                                                                        Serial.print(i);
                                                                                        Serial.print("durée = ");
                                                                                        Serial.print(z);
                                                                                        }
                                                                                       
                                                                        digitalWrite(i, LOW); //On eteind la led
                                                                     
                                                                        }
                                     }
               }
   
 
