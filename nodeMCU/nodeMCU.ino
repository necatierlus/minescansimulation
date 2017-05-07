
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

    char ssid[] = "AndroidAP";    //  your network SSID (name) 
    char pass[] = "ohcd2313";   // your network password


 //   char ssid[] = "OFE";    //  your network SSID (name) 
   // char pass[] = "onur55furkan51emrecan16";   // your network password
    WiFiClient  client;

unsigned long myChannelNumber = 265485;
const char * myWriteAPIKey="J93TNLFECQ7WI8GU";
const char * myReadAPIKey = "W351PFNZ52NLRGDI";

const int motor[]={D5,D6}; //sagIleri=D5, solIleri=D6,
const int led=D4;// Üst Mavi
const int mayinSensoru=D1;

unsigned int sayac=0;

const int turSensoru[]={D7,D8}; //Sağ ve sol tekerlek mesafe ölçüçm sensörleri
int sensorSec=0;

int turKontrol=0;
int turSayisi=0;

int donusTuru=40;
int donus=0;
int yonSayisi=0;

int hareketIzni=1;
int qKontrol=0;
int qtr8;
int msinyal=1;

int serverGidecekDeger=0;

void setup() {

   Serial.begin(115200);
   ThingSpeak.begin(client);
   
  for (int i=0;i<2;i++){
    pinMode(motor[i],OUTPUT);
    digitalWrite(motor[i], LOW); 
  }
  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW); 

  pinMode(mayinSensoru, INPUT);

   pinMode(turSensoru[0], INPUT);
   pinMode(turSensoru[1], INPUT);

   while (WiFi.status()!=WL_CONNECTED)
   {
    BuzzerLed();
    }
   digitalWrite(led, LOW); 
  while (1)
  {
      int deger = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
      if (deger==9)
        break;
       delay(100);
       Serial.println(deger);
       Serial.println("Deneme");
       
  }
    
}

void loop() { 
if (yonSayisi<=10)
{
  sayac++;
  if (sayac>30000)
    sayac=0;
  
  //********************************************************************************************************** 
    
    //Tekerleklerdeki Beyaz çizgileri okuma 
    if (digitalRead(turSensoru[sensorSec])>0){
      
      if (turKontrol==0){
        turKontrol=1;
        turSayisi++;
        }
      }
    else
      turKontrol=0;
    //***********************************************************************************************************
    //Robotun Hareketi düz git veya Dön
    
    if (hareketIzni==1){
      if (donus==0){
        if (turSayisi<=100){
          ileriGit();
        }
        else 
        { 
          donus=1;
          turSayisi=0;
          sensorSec=(yonSayisi+1)%2; //Burada Robotun hangi yöne döneceğini belirliyoruz.
         }
      }
      else
        donusYap();
    }
    //**********************************************************************************************************
 //Mayın Sensörleri 

      qtr8=digitalRead(mayinSensoru);
      Serial.println(qtr8);
    //********************************************************************************************************
      
    if ((qtr8>0) && (qKontrol==0))
    { //Burada herhangi bir mayın bulundu ve ilk bulunma sinyalini aldı ise
        robotuDurdur(); //Robotu durduruyoruz
        hareketIzni=0; //Robotun hareket etmesini engelliyoruz.
        serverGidecekDeger=((turSayisi*100)+yonSayisi)*10+1; //burada tursayısı yönsayısı ve devam işaretinin hepsini tek bir değere çeviriyoruz.
        sendServer(serverGidecekDeger); 
        hareketIzni=receiveServer(); //Roboto hareket izni veriyoruz
        if (hareketIzni==1)
          qKontrol=1;
          
        //------------------------------------------------------------------------------------------------------
        /*Burada NodeMCU' dan gelen değere göre Robotu hareket ettiriyoruz.Robota hareket izni veriyoruz fakat hala aynı mayının 
          etkisi altında olduğundan qKontrol=1 yaparak tekrar sisteme mayın bulundu uyarısının verilmesini engelliyoruz.*/
         
        //------------------------------------------------------------------------------------------------------
      }
      if (qtr8<1) //Burada tamamen mayınlı alanın dışına çıktığında yeni bir alana girdiğinde tekrar uyarıda bulunsun diye qKontrol=0 yapılıyor
      {
        qKontrol=0;

       }
  }
  else
  for (int i=0;i<2;i++){
    pinMode(motor[i],OUTPUT);
    digitalWrite(motor[i], LOW); 
  }
}

void ileriGit(){
  digitalWrite(motor[0], HIGH); 
  digitalWrite(motor[1], HIGH); 
}
void robotuDurdur(){
  
  digitalWrite(motor[0], LOW); 
  digitalWrite(motor[1], LOW); 
}
void donusYap(){
  if (turSayisi<donusTuru){
      digitalWrite(motor[yonSayisi%2], HIGH); 
      digitalWrite(motor[(yonSayisi+1)%2], LOW); 
  }
  else{
    turSayisi=0;
    donus=0;
    yonSayisi++;
  }

}
void sendServer(int deger)
{  
  while (1)
  {
      ThingSpeak.writeField(myChannelNumber, 1, deger, myWriteAPIKey);
      BuzzerLed();
      int oku = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
      if (oku==deger)
        break;
  }
}

int receiveServer()
{  
  while (1)
  {
      int serverGelenDeger = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
      if (serverGelenDeger==(serverGidecekDeger-1))
        break;
       BuzzerLed();
       
  }
  digitalWrite(led,LOW);
  return 1;

}
void BuzzerLed()
{
    msinyal=-msinyal;     
   if (msinyal>0){
      digitalWrite(led,HIGH);
   }
   else
   {
     digitalWrite(led,LOW);
   }
   delay(100);
}

