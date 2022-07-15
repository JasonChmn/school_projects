#define TAILLE_UNIT 21
#define TAILLE_TAB TAILLE_UNIT*TAILLE_UNIT
#define BAUD_SPEED 9600

void convertBoolTab(byte input[TAILLE_TAB], bool output[TAILLE_UNIT][TAILLE_UNIT]);
void SerialprintBoolTab(bool tab[TAILLE_UNIT][TAILLE_UNIT]);

// Pins pour les positifs (lignes)
int P_SER_Pin = 11;   
int P_RCLK_Pin = 12;  
int P_SRCLK_Pin = 13; 
// Pins pour les négatifs (colonnes)
int N_SER_Pin = 2;   
int N_RCLK_Pin = 3;  
int N_SRCLK_Pin = 4;

int N_16 = 5;
int N_17 = 6;
int N_18 = 7;
int N_19 = 8;
int N_20 = 9;

// Ligne nulle {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

bool image_nulle[21][21] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},   
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };

                             
bool image_smiley[21][21] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
                              {0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0},
                              {0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
                              {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
                              {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
                              {0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0},
                              {0,1,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,1,0,0},
                              {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
                              {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                              {1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0},
                              {1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0},
                              {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                              {0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0},
                              {0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0},   
                              {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
                              {0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
                              {0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
                              {0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0},
                              {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0}, 
                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };
                              
bool image_AE[21][21] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},   
                            {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };

//How many of the shift registers - change this
#define p_number_of_74hc595s 3
#define n_number_of_74hc595s 2
 
//do not touch
#define p_numOfRegisterPins p_number_of_74hc595s * 8
#define n_numOfRegisterPins n_number_of_74hc595s * 8
 
boolean p_registers[p_numOfRegisterPins];
boolean n_registers[n_numOfRegisterPins+5];

/** Delai entre chaque ligne : 21*X < 20 ms --> X=0.9 pour la persistance retinienne **/
int mon_delai = 0.9;
 
void setup(){
  
  Serial.begin(BAUD_SPEED);
  
  pinMode(P_SER_Pin, OUTPUT);
  pinMode(P_RCLK_Pin, OUTPUT);
  pinMode(P_SRCLK_Pin, OUTPUT);
  
  pinMode(N_SER_Pin, OUTPUT);
  pinMode(N_RCLK_Pin, OUTPUT);
  pinMode(N_SRCLK_Pin, OUTPUT);
  
  pinMode(N_16, OUTPUT);
  pinMode(N_17, OUTPUT);
  pinMode(N_18, OUTPUT);
  pinMode(N_19, OUTPUT);
  pinMode(N_20, OUTPUT);
 
  //reset all register pins
  clearRegisters();
  writeRegisters();
}               
 
//set all register pins to LOW
void clearRegisters(){
  int i=0;
  for(i = p_numOfRegisterPins - 1; i >=  0; i--){
     p_registers[i] = LOW;
  }
  for(i = n_numOfRegisterPins - 1; i >=  0; i--){
     n_registers[i] = LOW;
  }
  for(i = 16; i<21 ; i++) {
     n_registers[i] = LOW; 
  }
} 
 
//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){
   int i =0;
  digitalWrite(P_RCLK_Pin, LOW);
  digitalWrite(N_RCLK_Pin, LOW);
 
  for(i = p_numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(P_SRCLK_Pin, LOW);
 
    int val = p_registers[i];
 
    digitalWrite(P_SER_Pin, val);
    digitalWrite(P_SRCLK_Pin, HIGH);
 
  }
  
  for(i = n_numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(N_SRCLK_Pin, LOW);
 
    int val = n_registers[i];
    digitalWrite(N_SER_Pin, !val);
    
    digitalWrite(N_SRCLK_Pin, HIGH);
  }
  
   for(i = 16; i <  21; i++){
    int val = n_registers[i];
     if (i == 16 ) {
        digitalWrite(N_16, !val);
     } else if (i == 17) {
        digitalWrite(N_17, !val);
     } else if (i == 18) {
        digitalWrite(N_18, !val);
     } else if (i == 19) {
        digitalWrite(N_19, !val);
     } else if (i == 20) {
        digitalWrite(N_20, !val);
     }
  }
  
  digitalWrite(N_RCLK_Pin, HIGH);
  digitalWrite(P_RCLK_Pin, HIGH);
 
}
 
//==============================================================================

void clear_lignes() {
  for ( int ligne=0; ligne<21; ligne++) {
     p_registers[ligne] = LOW; 
  }
}

void clear_colonnes() {
  for ( int colonne=0; colonne<21; colonne++) {
     n_registers[colonne] = LOW; 
  }
}


//============================================================================== 
 
//set an individual pin HIGH or LOW
void set_p_RegisterPin(int index, int value){
  p_registers[index] = value;
}

//set an individual pin HIGH or LOW
void set_n_RegisterPin(int index, int value){
  n_registers[index] = value;
}
 
 //==============================================================================================================
 
 //===========================================================================
//                 ==  Afficher_image pour X secondes  ==
//            Afficher une image sur un tableau 21*21
//===========================================================================
// Programme du tableau de LED
/** Afficher une image pendant X ms */
void afficher_image(bool mon_tab[21][21], int temps_ms) {
  // Temps d'affichage de l'image
  long start_time = millis();
  long temps_total = 0;
  // RAZ
  clearRegisters();
  while ( temps_total < temps_ms ) {  
      afficher_image_une_fois( mon_tab );
      temps_total = millis()-start_time;
   }
}


/**
 *  Afficher l'image une seule fois = un balayage de toutes les lignes.
**/
void afficher_image_une_fois(bool mon_tab[21][21]) {
    // Traitement de toutes les lignes et colonnes
    for (int ligne=0;ligne<21;ligne++) {
        // Allumer la ligne
        set_p_RegisterPin(ligne,HIGH);
        // Allumer toutes les colonnes voulues
        for (int colonne=0;colonne<21;colonne++) {
            if (mon_tab[20-ligne][20-colonne]) {
               // Si la LED à la position (ligne,colonne) est à True, on l'allume
               set_n_RegisterPin(colonne,HIGH);
            }
        } // fin du for traitement colonnes
        
        // On a une ligne allumee et les colonnes correspondant aux LED de cette ligne
        // On les affiche
        writeRegisters();
        delay(mon_delai);  // Persistance retiniene ensuite qui dure 20ms
        clearRegisters();
    }
}

//===========================================================================
//                 ==  Allumer une ligne ou colonne pour X secondes  ==
//            Allumer une ligne sur un tableau 21*21
//===========================================================================
void allumer_ligne(float temps_ms, int n_ligne) {
  int colonne = 0;
  
  set_p_RegisterPin(n_ligne,HIGH);
  // Allumer toutes les colonnes
  for (colonne=0;colonne<21;colonne++) {
      set_n_RegisterPin(colonne,HIGH);
   
  } // fin du for traitement colonnes
  
  writeRegisters();
  delay(temps_ms);
  
  clearRegisters();
}
 

 void allumer_colonne(float temps_ms, int n_colonne) {
  int ligne = 0;
  
  // RAZ
  clearRegisters();
  
  set_n_RegisterPin(n_colonne,HIGH);
  // Allumer toutes les colonnes
  for (ligne=0;ligne<21;ligne++) {
      set_p_RegisterPin(ligne,HIGH);
  } // fin du for traitement colonnes
  
  writeRegisters();
  delay(temps_ms);
  
  clearRegisters();
  writeRegisters();
}
 
 //=============================================================
 //========== RECEPTION DE DATA ================================
 
 //des 0 et des 1 sont envoyés, mais sous forme de char 
//cad des '0' et des '1', sachant que '1' - '0' = 1 alors on soustrait '0' pour avoir des vrais 0 et 1
void convertBoolTab(byte input[TAILLE_TAB], bool output[TAILLE_UNIT][TAILLE_UNIT])
{
  int k=0;
  for(int i=0;i<TAILLE_TAB;i++)
  {
    int j = i%TAILLE_UNIT;
    if(j==0 && i!=0)
    {
      k++;
    }
    output[k][j] = input[i]-'0';
  }
}

void SerialprintBoolTab(bool tab[TAILLE_UNIT][TAILLE_UNIT])
{
  for(int i=0;i<TAILLE_UNIT;i++)
  {
    for(int j=0;j<TAILLE_UNIT;j++)
    {
      Serial.print(tab[i][j]); //pas besoin de remettre '0' ici car la fct toString est appelée ici, donc déjà conversion en char
    }
    Serial.println();
  }
}
 
 //==========================================================================================
 //============ AFFICHAGE DE LA DATA RECUE
 
 void boucle_image_recue() {
    bool imageReceived[TAILLE_UNIT][TAILLE_UNIT];
    byte bufferImageReceived[TAILLE_TAB];
    // Image smiley par defaut
    for (int k=0; k<21; k++) {
      for (int j=0; j<21; j++) {
        imageReceived[k][j] = image_smiley[k][j];
      }
    }
    // Debut de la boucle
    while(1) {
       if(Serial.available()/* && once*/) {
          Serial.readBytes(bufferImageReceived, TAILLE_TAB);
          convertBoolTab(bufferImageReceived, imageReceived);
          SerialprintBoolTab(imageReceived);
       }
       afficher_image_une_fois(imageReceived);
    } 
 }
 
 // =============================================================================================================
 
void loop(){
  clearRegisters();
    // Phase de demarrage
    for (int i=0; i<21; i++) {
       allumer_ligne( 30 , i ) ;
    }
    
    boucle_image_recue();
    //lancer_debug();
    clearRegisters();
    writeRegisters();
    while(true);
}

//=============================== DEBUGAGE =====================================

void lancer_debug() {
  
  clearRegisters();
  for (int k=0; k<21; k++) {
     set_p_RegisterPin(k,LOW);
     set_n_RegisterPin(k,LOW);
  } 
  // LIGNES POS
  /*
  set_n_RegisterPin(5,HIGH);
  while(true) {
     for (int lig=0; lig<21; lig++) {
        set_p_RegisterPin(lig,HIGH);
        delay(1000);
        writeRegisters();
        set_p_RegisterPin(lig,LOW);
        writeRegisters();
     } 
  }
  */
  // LIGNES NEG
  while(true) {
     for (int col=0; col<21; col++) {
       set_p_RegisterPin(0,HIGH);
       set_p_RegisterPin(1,HIGH);
       set_n_RegisterPin(col,HIGH);
       writeRegisters();
       delay(500);
       set_p_RegisterPin(0,LOW);
       set_p_RegisterPin(1,LOW);
       writeRegisters();
       delay(1000);
       set_n_RegisterPin(col,LOW);
     }
  }
  
}
