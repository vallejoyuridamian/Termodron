#include "Geometria.h"
#include "Constantes.h"

#define ConsolaMon


void setup() {

  Consola.begin(115200);

  
  coordenadas[0].latitud = -349194863;
  coordenadas[0].longitud = -561660864;
  coordenadas[0].altitud = 5;
  
  coordenadas[1].latitud = -349194817;
  coordenadas[1].longitud = -561665646;
  coordenadas[1].altitud = 5;
  
  coordenadas[2].latitud = -349192899;
  coordenadas[2].longitud = -561668373;
  coordenadas[2].altitud = 5;
  
  coordenadas[3].latitud = -349192845;
  coordenadas[3].longitud = -561666979;
  coordenadas[3].altitud = 5;
  
  
  CalcularTrayectoria(coordenadas,4);
    

}

void loop() {
  
}


//**************************
//Función cálculo de trayectoria

int CalcularTrayectoria(Punto* Perimetro,int counter){
  
  Punto Punto1esN,PuntoAux,PuntoAux2;
  Pendiente Pend;
  float distEnPer = 0;
  float debo = 0;
  int i = 0;
  int j= 0;
  int z = 0;
  int N = counter;
  int k = counter;
  boolean unoEsN = true;
  
  // Los primeros dos puntos los cargo en la mision ya que ira por el perimetro
  for (int i = 0; i < 2;i++){
    Mision[i].latitud= Perimetro[i].latitud;
    Mision[i].longitud= Perimetro[i].longitud;
    Mision[i].altitud= Perimetro[i].altitud;
  }
  i = 0;
  k = N;
  j = 2; 
  PuntoAux2= Punto(Perimetro[1].latitud,Perimetro[1].longitud,Perimetro[0].altitud);
  Pend= Pendiente(Perimetro[0],Perimetro[1]);
  Punto1esN= Punto(Perimetro[0].latitud,Perimetro[0].longitud,Perimetro[0].altitud);

  while (i<k-2){
    //Recorre el perimetro del lado de las i (indice creciente de los puntos)

    distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Perimetro[i+2]);
    PuntoAux= Punto(PuntoAux2,Perimetro[i+2],distEnPer);
    if(PuntoAux.menosQueD){
      i++;
      Serial.print("Aumento i recorriendo el perimetro por el lado de las i ");
      Serial.println(i);
      if(i>=k-2){
      Serial.println("Termino");
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
        break;
      }
      Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
        break;  
      }
      debo = desrecalcularDist(PuntoAux.debo, Pend, PuntoAux, Perimetro[i]);
      debo = recalcularDist(debo, Pend, PuntoAux, Perimetro[i+2]);
      PuntoAux2 = Punto(PuntoAux.latitud, PuntoAux.longitud,Perimetro[0].altitud);
      PuntoAux = Punto(PuntoAux,Perimetro[i+2],debo);

      if(PuntoAux.menosQueD){
        i++;
        Serial.print("Aumento i recorriendo el perimetro por el lado de las i ");
        Serial.println(i);
        if(i>=k-2){
        Serial.println("Termino");
          Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
          j++;
          break;
        }
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
        if (j == MISIONMAXPUNTOS -1){
          break;  
        }
      }
    }
    Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);

    Serial.println();
    Serial.println();
    Serial.println();
    Serial.print("J vale ");
    Serial.println(j);
    Serial.print("Mision[j].latitud vale ");
    Serial.println(int32_t(Mision[j].latitud));
    Serial.print("Mision[j].longitud ");
    Serial.println(int32_t(Mision[j].longitud));
    Serial.println();
    Serial.println();
    Serial.println();

    
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }
    //  Cruza al lado de los k (indice decreciente de los puntos)
    Serial.println("Cruza al lado de los k (indice decreciente de los puntos)");
    
    if(unoEsN){
    
    Serial.println("uno es n");

    Serial.println();
    Serial.println();
    Serial.print("PuntoAux.latitud vale ");
    Serial.println(int32_t(PuntoAux.latitud));
    Serial.print("PuntoAux.longitud vale ");
    Serial.println(int32_t(PuntoAux.longitud));
    Serial.println();
    Serial.println();  

    Serial.println();
    Serial.println();
    Serial.print("Punto1esN.latitud vale ");
    Serial.println(int32_t(Punto1esN.latitud));
    Serial.print("Punto1esN.longitud vale ");
    Serial.println(int32_t(Punto1esN.longitud));
    Serial.println();
    Serial.println();


    Serial.println();
    Serial.println();
    Serial.print("Perimetro[k-1].latitud vale ");
    Serial.println(int32_t(Perimetro[k-1].latitud));
    Serial.print("Perimetro[k-1].longitud vale ");
    Serial.println(int32_t(Perimetro[k-1].longitud));
    Serial.println();
    Serial.println();

    Serial.println();
    Serial.println();
    Serial.print("Pend.m vale ");
    Serial.println(Pend.m);
    Serial.print("Pend.inf vale ");
    Serial.println(Pend.inf);
    Serial.println();
    Serial.println();

      
    PuntoAux2= Punto(PuntoAux, Pend, Punto1esN, Perimetro[k-1]);
    
    }
    else{
      Serial.println("uno NO es n");
      PuntoAux2= Punto(PuntoAux, Pend, Perimetro[k], Perimetro[k-1]);
    }
    if(PuntoAux2.menosQueD){
      k--;
      Serial.print(F("Disminuyo k cruzando al lado de las k"));
      Serial.println(k);
      unoEsN=false;
      Pend= Pendiente(PuntoAux,PuntoAux2);
      if(i>=k-2){
        Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
        j++;
        break;
      }
    }

    
    Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
    
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.print("J vale ");
    Serial.println(j);
    Serial.print("Mision[j].latitud vale ");
    Serial.println(int32_t(Mision[j].latitud));
    Serial.print("Mision[j].longitud ");
    Serial.println(int32_t(Mision[j].longitud));
    Serial.println();
    Serial.println();
    Serial.println();
    
    
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }
    // Recorre el perimetro del lado de las k
    Serial.println(F("Recorre el perimetro del lado de las k"));
    /*Serial.print("Pend vale ");
    Serial.println(Pend.m);
    Serial.print("j vale ");
    Serial.println(j);
    */
    
    distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Perimetro[k-1]);  
    PuntoAux = Punto(PuntoAux2,Perimetro[k-1],distEnPer);
    if(PuntoAux.menosQueD){
      k--;
      Serial.print(F("Disminuyo k recorriendo el perimetro por el lado de las k "));
      Serial.println(k);
      unoEsN=false;
      if(i>=k-2){
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
      }
      Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
      j++;
      if (j == MISIONMAXPUNTOS -1){
        break;  
      }
      debo =desrecalcularDist(PuntoAux.debo, Pend, PuntoAux2, Perimetro[k]);
      debo =recalcularDist(debo, Pend, PuntoAux, Perimetro[k-1]);
      PuntoAux2= Punto(PuntoAux.latitud, PuntoAux.longitud,Perimetro[0].altitud);
      PuntoAux= Punto(PuntoAux,Perimetro[k-1],debo);

      if(PuntoAux.menosQueD){
        k--;
        Serial.print(F("Disminuyo k recorriendo el perimetro por el lado de las k "));
        Serial.println(k);
        unoEsN=false;
        if(i>=k-2){
          Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
          j++;
          break;
        }
        Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
        j++;
        if (j == MISIONMAXPUNTOS -1){
          break;  
        }
      }
    }
    Mision[j]=Punto(PuntoAux.latitud,PuntoAux.longitud,Perimetro[0].altitud);
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }
    // Cruza al lado de las i  
    Serial.println(F("Cruza al lado de las i"));
    PuntoAux2= Punto(PuntoAux,Pend,Perimetro[i+1],Perimetro[i+2]);
    if(PuntoAux2.menosQueD){
      i++;
      Serial.print(F("Aumento i cruzando al lado de las i"));
      Serial.println(i);
      Pend= Pendiente(PuntoAux,PuntoAux2);
      if(i>=k-2){
        Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
        j++;
        break;
      }     
    }
    Mision[j]=Punto(PuntoAux2.latitud,PuntoAux2.longitud,Perimetro[0].altitud);
    j++;
    if (j == MISIONMAXPUNTOS -1){
        break;  
    }

    z=z+1;
    if (z > 799){
      break;
    }
  }
  
  #ifdef ConsolaMon
  Consola.print(F("Al final z vale "));
  Consola.println(z);
  Consola.println(F("Si z es 800 es que quedo en un loop o algo raro"));
  Consola.print(F("i vale "));
  Consola.print(i);
  Consola.print(F(" k vale "));
  Consola.println(k);    
  Consola.print(F("El perimetro tiene "));
  Consola.print(N);
  Consola.println(F(" puntos de coordenadas: "));
  for(int i=0;i<N;i++){
    Consola.print(int32_t(Perimetro[i].latitud));
    Consola.print(",");
    Consola.println(int32_t(Perimetro[i].longitud));
  }
  Consola.print(int32_t(Perimetro[0].latitud));
  Consola.print(",");
  Consola.println(int32_t(Perimetro[0].longitud));
  Consola.print(F("La Mision tiene "));
  Consola.print(j);
  Consola.println(F(" puntos de coordenadas: "));
  for(int i=0;i<j;i++){
    Consola.print(int32_t(Mision[i].latitud));
    Consola.print(",");
    Consola.println(int32_t(Mision[i].longitud));
  }
  #endif
  return j;
}
