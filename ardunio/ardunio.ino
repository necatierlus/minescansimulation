#include <Servo.h>
Servo myservomotor;

  const int led[]={2,3,4,5,6,7,8}; //Arka Sağ sinyal=2, Arka sol sinyal=3,Üst Kırmızı=4,Dönence kırmızı=5,Arka Sağ kırmızı=6, Arka Sol kırmızı=7, farlar 8,9
  
  unsigned int sayac=0;
  int derece=20;
  int artis=1;
  int sinyal=1;
  int msinyal=1;
  int hiz=11;
  const int sagTekerlekSensor=A0;
  const int solTekerlekSensor=A1;
  const int s1=12;
  const int s2=13;
  const int mayinSensoru[]={A2,A3,A4,A5};
  const int s3=9;
  int qtr8=0;  



void setup() {
     pinMode(hiz,OUTPUT);
     analogWrite(hiz,100);
      myservomotor.attach(10);  // arduinonun 10. pinini çıkış yaptık.
    //myservomotor.write(0);
        
     for (int i=0;i<7;i++){
        pinMode(led[i], OUTPUT);
        digitalWrite(led[i], HIGH); 
      }

      pinMode(sagTekerlekSensor,INPUT);
      pinMode(solTekerlekSensor,INPUT);

      pinMode(s1,OUTPUT);
      pinMode(s2,OUTPUT);

      for (int i=0;i<4;i++)
      pinMode(mayinSensoru[i], INPUT);

       pinMode(s3,OUTPUT);

}

void loop() { 
  
  if (analogRead(sagTekerlekSensor)>90)
      digitalWrite(s1,HIGH);
  else
      digitalWrite(s1,LOW);
     
  if (analogRead(solTekerlekSensor)>90)
      digitalWrite(s2,HIGH);
  else
      digitalWrite(s2,LOW);

    qtr8=0;
     for (int i=0;i<4;i++){
       if (analogRead(mayinSensoru[i])>800)
       qtr8++;
     }
     if (qtr8>0)
        digitalWrite(s3,HIGH);
     else
        digitalWrite(s3,LOW);

    sayac++;
    if (sayac>30000)
      sayac=0;
    
    if (sayac%100==0)
      derece+=artis;
    if (derece>=130)
      artis=-1;
    if (derece<=20)
      artis=1;
      myservomotor.write(derece);
      
      if (sayac%2000==0)
      sinyal=-sinyal;
      if (sinyal>0)
      {
        digitalWrite(led[0],HIGH);
        digitalWrite(led[1],LOW);
      }
      else
      {
        digitalWrite(led[0],LOW);
        digitalWrite(led[1],HIGH);
      }
}
