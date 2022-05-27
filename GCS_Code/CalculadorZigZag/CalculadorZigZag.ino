#include "Geometria.h"

//////// DEFINICIONES //////////
const int PERIMETROMAXPUNTOS= 3;
const int MISIONMAXPUNTOS = 70;
int N = 3; // Cantidad de puntos
int k = N;
int z,i,j = 0;
float distEnPer=0;
float distEntrePer = 2e-3;
float debo = 0;
boolean unoEsN = true;
boolean runOnce = true;
Punto Punto1esN,PuntoAux,PuntoAux2;
Punto Perimetro[PERIMETROMAXPUNTOS];
Punto Mision[MISIONMAXPUNTOS];
Pendiente Pend;
float ruido=0.003;
long escala=1e7;

void setup(){ 
  Serial.begin(57600);
}

void loop(){

  if (runOnce){


    Perimetro[0]=Punto(-34.9235901+random(-ruido,ruido),-56.1604858+random(-ruido,ruido));
    Perimetro[1]=Punto(-34.9194274+random(-ruido,ruido),-56.1575331+random(-ruido,ruido));
    Perimetro[2]=Punto(-34.9232101+random(-ruido,ruido),-56.1530807+random(-ruido,ruido));
    //Perimetro[3]=Punto(4000+random(-ruido,ruido),5000+random(-ruido,ruido));
    //Perimetro[4]=Punto(5000+random(-ruido,ruido),1000+random(-ruido,ruido));

   
    // Primero ponemos todos los puntos en 0
    for (int i = 0; i < MISIONMAXPUNTOS;i++){
      Mision[i].latitud= 0;
      Mision[i].longitud = 0;
    }
  
    // Los primeros dos puntos los cargo en la mision ya que ira por el perimetro
    for (int i = 0; i < 2;i++){
      Mision[i]= Perimetro[i];
    }  
    
    i=0;
    k=N;
    j=2;
    PuntoAux2= Punto(Perimetro[1].latitud,Perimetro[1].longitud);
    Pend= Pendiente(Perimetro[0],Perimetro[1]);
    Punto1esN= Punto(Perimetro[0].latitud,Perimetro[0].longitud);
  
    while (i<k-2){
      //Recorre el perimetro del lado de las i (indice creciente de los puntos)
      //Serial.println("Recorre el perimetro del lado de las i (indice creciente de los puntos)");
      distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Perimetro[i+2]);
      PuntoAux= Punto(PuntoAux2,Perimetro[i+2],distEnPer);
      
      if(PuntoAux.menosQueD){
        i++;
        //Serial.print("Aumento i recorriendo el perimetro por el lado de las i ");
        //Serial.println(i);
        if(i==k-2){
          //Serial.println("Termino");
            Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud);
            break;
        }
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud);
        j++;
        if (j == MISIONMAXPUNTOS -1){
          break;  
        }
        debo =desrecalcularDist(PuntoAux.debo, Pend, PuntoAux, Perimetro[i]);
        debo =recalcularDist(debo, Pend, PuntoAux, Perimetro[i+2]);
        PuntoAux2= Punto(PuntoAux.latitud, PuntoAux.longitud);
        PuntoAux= Punto(PuntoAux,Perimetro[i+2],debo);
      }
      Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
          break;  
      }
      //  Cruza al lado de los k (indice decreciente de los puntos)
      //Serial.println("Cruza al lado de los k (indice decreciente de los puntos)");
      if(unoEsN){
        PuntoAux2= Punto(PuntoAux, Pend, Punto1esN, Perimetro[k-1]);
      }
      else{
        PuntoAux2= Punto(PuntoAux, Pend, Perimetro[k], Perimetro[k-1]);
      }
      if(PuntoAux2.menosQueD){
        k--;
        //Serial.print("Disminuyo k cruzando al lado de las k");
        //Serial.println(k);
        unoEsN=false;
        Pend= Pendiente(PuntoAux,PuntoAux2);
      }
      Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
          break;  
      }
      // Recorre el perimetro del lado de las k
      //Serial.println("Recorre el perimetro del lado de las k");
      distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Perimetro[k-1]);  
      PuntoAux = Punto(PuntoAux2,Perimetro[k-1],distEnPer);
      if(PuntoAux.menosQueD){
        k--;
        //Serial.print("Disminuyo k recorriendo el perimetro por el lado de las k ");
        //Serial.println(k);
        unoEsN=false;
        if(i==k-1){
          break;
        }
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud);
        j++;
        if (j == MISIONMAXPUNTOS -1){
          break;  
        }
        debo =desrecalcularDist(PuntoAux.debo, Pend, PuntoAux2, Perimetro[k]);
        debo =recalcularDist(debo, Pend, PuntoAux, Perimetro[k-1]);
        PuntoAux2= Punto(PuntoAux.latitud, PuntoAux.longitud);
        PuntoAux= Punto(PuntoAux,Perimetro[k-1],debo);
      }
      Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
          break;  
      }
      // Cruza al lado de las i  
      //Serial.println("Cruza al lado de las i");
      PuntoAux2= Punto(PuntoAux,Pend,Perimetro[i+1],Perimetro[i+2]);
      if(PuntoAux2.menosQueD){
        i++;
        //Serial.print("Aumento i cruzando al lado de las i");
        //Serial.println(i);
        Pend= Pendiente(PuntoAux,PuntoAux2);
      }
      Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
          break;  
      }
      z=z+1;
      //Serial.println();
      //Serial.print("Z vale: ");
      //Serial.println(z);
      if (z > 799){
        break;
      }
    }
    Serial.print("Al final z vale ");
    Serial.println(z);
    Serial.println("Si z es 800 es que quedo en un loop o algo raro");
    Serial.print("i vale ");
    Serial.print(i);
    Serial.print(" k vale ");
    Serial.println(k);    
    


    Serial.print("El perimetro tiene ");
    Serial.print(N);
    Serial.println(" puntos de coordenadas: ");
    for(int i=0;i<N;i++){
      //Serial.print("Punto ");
      //Serial.print(i);
      //Serial.print(": ");
      //Serial.print("X= ");
      Serial.print(Perimetro[i].latitud*escala);
      //Serial.print(" , longitud = ");
      Serial.print(",");
      Serial.println(Perimetro[i].longitud*escala);
    }
    Serial.print(Perimetro[0].latitud*escala);
    //Serial.print(" , longitud = ");
    Serial.print(",");
    Serial.println(Perimetro[0].longitud*escala);

    Serial.print("La Mision tiene ");
    Serial.print(j+1);
    Serial.println(" puntos de coordenadas: ");
    for(int i=0;i<=j;i++){
      //Serial.print("Punto ");
      //Serial.print(i);
      //Serial.print(": ");
      //Serial.print("latitud= ");
      Serial.print(Mision[i].latitud*escala);
      //Serial.print(" , longitud = ");
      Serial.print(",");
      Serial.println(Mision[i].longitud*escala);
    }
  }
  runOnce = false;
}

