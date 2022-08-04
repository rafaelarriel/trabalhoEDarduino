#include <Thermistor.h>
#include <SD.h>
#include <LiquidCrystal.h> 

//Inicializando o lcd
LiquidCrystal lcd(2,3,4,5,6,7);

//variavel do SD
File myFile; 
 
//Varialvel para leitura do sensor de temperatura
Thermistor temp(0);

//Estrutura Lista
//========================================================
class Noh_de_Lista{
friend class Lista;  
private:      
    float temperatura;
    Noh_de_Lista *prox;
public:
    Noh_de_Lista(float temp){
    temperatura = temp;
    prox = NULL;
  }
};

class Lista {
private:
  Noh_de_Lista *Inicio;
  Noh_de_Lista *Fim;
public:
  Lista();
  ~Lista();
  void Inserir_na_Lista(float temp);
  void Gravar_no_SD();
};

//Construtor da Lista
Lista::Lista(){
    Inicio = NULL;
    Fim = NULL;
}

//Destrutor da Lista
Lista::~Lista(){
    Noh_de_Lista* Aux = Inicio;
    while(Aux != NULL){
      Inicio = Inicio->prox;
      delete Aux;
      Aux = Inicio;
    }
    Inicio = NULL;
    Fim = NULL;    
}

//Funcao para inserir dados na lista 
void Lista::Inserir_na_Lista(float temp){
    if(Inicio == NULL){
      Inicio = new Noh_de_Lista(temp);
      Fim = Inicio;
    }
    else{
      Fim->prox = new Noh_de_Lista(temp);
      Fim = Fim->prox;
    }
}

//Funcao que grava os dados da Lista no SD
void Lista::Gravar_no_SD(){
  myFile = SD.open("Arquivo.txt", FILE_WRITE);  
  if (myFile) {
      Noh_de_Lista* Aux = Inicio;
      while (Aux != NULL){      
          myFile.println(Aux->temperatura);                 
          Aux = Aux->prox;      
      }   
      myFile.close();
      lcd.print("Dados Gravados");
      delay(2000);
      lcd.clear();
  } 
}
//=============================================================  





//Estrutura da Arvore Binaria de Busca
//=============================================================

class Noh_de_Arvore{
friend class Arvore;
private:
    float Temperatura;
    Noh_de_Arvore* Esq;
    Noh_de_Arvore* Dir;
public:
    Noh_de_Arvore(float T); 
    ~Noh_de_Arvore();      
    float Menor_Temperatura();
    float Maior_Temperatura();
    void Inserir_Na_Arvore_Recursivo(float T);
};

//Costrutor do no
Noh_de_Arvore::Noh_de_Arvore(float T){
     Temperatura = T;
     Esq = NULL;
     Dir = NULL;
}

//Destrutor do no
Noh_de_Arvore::~Noh_de_Arvore(){
    if (Esq != NULL){
      Esq->~Noh_de_Arvore();
    }
    if(Dir != NULL){
      Dir->~Noh_de_Arvore();
    }
    delete this;  
}

//Funcao para inserir recursivo
void Noh_de_Arvore::Inserir_Na_Arvore_Recursivo(float T){
    if (T < Temperatura){
        if (Esq == NULL){
            Esq = new Noh_de_Arvore(T);
        }
        else{
            Esq->Inserir_Na_Arvore_Recursivo(T);
        }
    }
    else{
        if (Dir == NULL){
            Dir = new Noh_de_Arvore(T);
        }
        else{
            Dir->Inserir_Na_Arvore_Recursivo(T);
        }
    }
}

//Funcao para achar a menor temperatura
float Noh_de_Arvore::Menor_Temperatura(){
    if (Esq != NULL){
        return Esq->Menor_Temperatura();
    }
    else{
        return Temperatura;
    }
}

//Funcao para achar maior temperatura
float Noh_de_Arvore::Maior_Temperatura(){
    if (Dir != NULL){
        return Dir->Maior_Temperatura();
    }
    else{
        return Temperatura;
    }
}


class Arvore{
private:
    Noh_de_Arvore* Raiz;
    float Soma_de_Temperaturas = 0;
    int Qtd_de_Dados = 0;
public:
    Arvore(){
        Raiz = NULL;
    }
    void Inserir_Na_Arvore(float T);
    float Maior_Temperatura_Na_Arvore();
    float Menor_Temperatura_Na_Arvore();
    float Media_de_Temperaturas();
    
};

void Arvore::Inserir_Na_Arvore(float T){
    if (Raiz == NULL){
        Raiz = new Noh_de_Arvore(T);
    }
    else{
        Raiz->Inserir_Na_Arvore_Recursivo(T);
    }

    Soma_de_Temperaturas += T;
    Qtd_de_Dados++;
}


float Arvore::Menor_Temperatura_Na_Arvore(){
    if (Raiz != NULL){
        return Raiz->Menor_Temperatura();
    }
    else{
        lcd.print("Arvore Vazia");
        delay(5000);
        lcd.clear();
    }
}

float Arvore::Maior_Temperatura_Na_Arvore(){
    if (Raiz != NULL){
        return Raiz->Maior_Temperatura();
    }
    else{
        lcd.print("Arvore Vazia");
        delay(5000);
        lcd.clear();
    }
}

//Funcao para achar a media das temperaturas inseridas na arvore
float Arvore::Media_de_Temperaturas(){
    return Soma_de_Temperaturas/Qtd_de_Dados;
}



//=============================================================================




Arvore* Arvore_de_Temperaturas = new Arvore();
Lista* Lista_de_Temperaturas = new Lista();

//Controlador do bota 1 e 2
int Ligado_1 = 0;
int Ligado_2 = 0;

//Variavel para receber temperaturas
float T;
long temps;

//Funcao principal do Arduino
//=======================================================================
void setup() {  
    Serial.begin(9600);
    //Inicializa o SD
    SD.begin();    
   
    
    //Inicializa o lcd
    lcd.begin(16,2);    
    pinMode(8, INPUT_PULLUP); 
    pinMode(9, INPUT_PULLUP);
    pinMode(10, OUTPUT);      
 }
//=========================================================================



//Funcao loop do Arduino
//==========================================================================
void loop() {

    //Se apertado a primeira vez ele aciona a leitura de temperaturas
    //Se apertado quando a leitura ja esta o ocorrendo ele a interrompe
    if (digitalRead(8) == LOW) { 
       if (Ligado_1 == 0){
          Ligado_1 = 1;
          delay(2000);
       }
       else{         
          delay(2000);
          lcd.clear();
          lcd.print("Encerrado");
          delay(2000);                   
          lcd.clear();
          lcd.print("Gravando no SD e ");
          lcd.setCursor(0,1);
          lcd.print("limpando memoria ");
          delay(5000);   
          lcd.clear(); 
          Lista_de_Temperaturas->Gravar_no_SD();
          Lista_de_Temperaturas->~Lista(); 
          lcd.clear();
          lcd.print("Memoria Limpa");
          delay(5000);
          lcd.clear();
          Ligado_1 = 2;
       }       
    }
    
    
    //Faz a leitura do sensor e grava a temperatura na lista a cada 1 segundo
    if (Ligado_1 == 1){            
        T = temp.getTemp(); 
        lcd.setCursor(0,0);
        lcd.print("GRAVANDO ...");            
        lcd.setCursor(1,1); 
        lcd.print(T);
        Lista_de_Temperaturas->Inserir_na_Lista(T); 
        Arvore_de_Temperaturas->Inserir_Na_Arvore(T);          
        delay(1000);
        lcd.clear();
    }
   
    if (digitalRead(9) == LOW){
        Ligado_2 = 1;   
        delay (2000);   
    }
    
    
    if (Ligado_2 == 1){   
      delay(1000);      
      lcd.clear();
      lcd.print("Menor Temp.");
      lcd.setCursor(1,1);       
      lcd.print(Arvore_de_Temperaturas->Menor_Temperatura_Na_Arvore());
      delay(5000);
      lcd.clear();
      lcd.print("Maior Temp.");
      lcd.setCursor(1,1);       
      lcd.print(Arvore_de_Temperaturas->Maior_Temperatura_Na_Arvore());
      delay(5000);
      lcd.clear();
      lcd.print("Temp. Media");
      lcd.setCursor(1,1);       
      lcd.print(Arvore_de_Temperaturas->Media_de_Temperaturas());
      delay(5000);
      lcd.clear(); 
      Ligado_2 = 0;  
    } 
}
//============================================================================






    

   
