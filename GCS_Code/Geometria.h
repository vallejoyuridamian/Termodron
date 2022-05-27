#ifndef Geometria_h
#define Geometria_h

#include "Arduino.h"

const float RadioDeLaTierra=6371000;									

class Pendiente;
class Punto;

class Punto {
  public:
    int64_t latitud;
    int64_t longitud;
    int32_t altitud;
    boolean menosQueD;
    float debo;

    //// Constructores
    // Crea un punto vacio
    Punto();
    // Crea un punto de coordenadas latitud=a,longitud=b altura=c
    Punto(int64_t a,int64_t b,int64_t c);
    // Crea un punto en el segmento AB tal que este a una distancia d de A, si B esta mas cerca, voy a B 
    Punto(Punto A,Punto B,float d);
    // Crea un punto en el segmento BC tal que el segmento APunto es paralelo a PendA, si el punto queda pasando C, voy hasta C
    Punto(Punto A, Pendiente PendA, Punto B, Punto C);
    // Crea un punto dummy con pendiente Pendiente alejado de A 5 unidades en el sentido de la latitud
    Punto(Punto A, float Pendiente);
    

};

/////////////// Clase Pendiente ////////////////

class Pendiente {
  public:
    float m;
    boolean inf;

    // Pendiente vacia
    Pendiente();
    // Pendiente entre los puntos A y B
    Pendiente(Punto A,Punto B);

};

float distancia(Punto A, Punto B);
float distanciaTrucha(Punto A, Punto B);										

// Constructores de Punto
Punto::Punto(){
};

Punto::Punto(int64_t a,int64_t b,int64_t c){
  latitud=a;
  longitud=b;
  altitud=c;
};

Punto::Punto(Punto A, float Pendiente){
  //latitud=A.latitud+5;
  latitud=A.latitud+500;
  longitud=int64_t(Pendiente*((latitud-A.latitud)+A.longitud/Pendiente));
  altitud=A.altitud;  
};

Punto::Punto(Punto A, Punto B, float d){
  menosQueD=false;
  float dAB = distancia(A,B);
  if (dAB < d){
    latitud=B.latitud;
    longitud=B.longitud;
    altitud=B.altitud;
    menosQueD=true;
    debo = d - dAB;
  }
  else{
    latitud=int64_t((B.latitud-A.latitud)*d/dAB+A.latitud);
    longitud=int64_t((B.longitud-A.longitud)*d/dAB+A.longitud);
    altitud=B.altitud;
  }
};

Punto::Punto(Punto A, Pendiente PendA, Punto B, Punto C){
  
  menosQueD=false;
  Pendiente* PendBC = new Pendiente(B,C);
 
  //latitud = int64_t((A.longitud-B.longitud+PendBC->m*B.latitud-PendA.m*A.latitud)/(PendBC->m-PendA.m));
  latitud = int64_t(((A.longitud/PendA.m/PendBC->m-B.longitud/PendA.m/PendBC->m+B.latitud/PendA.m-A.latitud/PendBC->m))/((PendBC->m-PendA.m)/PendA.m/PendBC->m));

 
  //longitud = int64_t(PendA.m*(latitud-A.latitud)+A.longitud);
  longitud = int64_t((B.latitud - A.latitud + A.longitud/PendA.m - B.longitud/PendBC->m)/(1/PendA.m - 1/PendBC->m)); // ESTO ES LO QUE HAY QUE CAMBIAR


  altitud=A.altitud;
  //float distanciaBPunto = sqrt(sq(latitud-B.latitud)+sq(longitud-B.longitud));
  float distanciaBPunto = RadioDeLaTierra*sqrt(sq((latitud-B.latitud)*1e-7*PI/180)+sq((longitud-B.longitud)*1e-7*PI/180*cos((latitud+B.latitud)/2*1e-7*PI/180)));
  float distanciaBC=distancia(B,C);
  if (distanciaBPunto > distanciaBC){
    latitud=C.latitud;
    longitud=C.longitud;
    menosQueD=true;
  }
};

Pendiente::Pendiente(){
};

float distancia(Punto A, Punto B){
  float distancia;
  distancia = RadioDeLaTierra*sqrt(sq((A.latitud-B.latitud)*1e-7*PI/180)+sq((A.longitud-B.longitud)*1e-7*PI/180*cos((A.latitud+B.latitud)/2*1e-7*PI/180)));
  return distancia;
}

Pendiente::Pendiente(Punto A, Punto B){
  if(B.latitud != A.latitud){
    m=float((B.longitud-A.longitud))/float((B.latitud-A.latitud));
    inf=false;
  }
  else{
    m=0;
    inf=true;
  }

};

// (des)Proyecta una distancia sobre el segmento P1P2 de manera de alejarme de P1 una distanca dist en direccion perpendicuar a Pend
float recalcularDist(float dist, Pendiente Pend, Punto P1, Punto P2){
  float PendientePerp = -1/Pend.m;
  float result;
  Punto P3(P1, PendientePerp);
 
  
  result = abs(dist*((distanciaTrucha(P3,P1))*(distanciaTrucha(P2,P1))/((P3.latitud-P1.latitud)*(P2.latitud-P1.latitud)+(P3.longitud-P1.longitud)*(P2.longitud-P1.longitud))));
  return result;	
}

// Proyecta una distancia sobre el segmento P1P2 de manera de alejarme de P1 una distanca dist en direccion perpendicuar a Pend
float desrecalcularDist(float dist, Pendiente Pend, Punto P1, Punto P2){
  float PendientePerp = -1/Pend.m;
  float result;
  Punto P3(P1, PendientePerp);
  result =  abs(dist/(((distanciaTrucha(P3,P1))*(distancia(P2,P1)))/
  ((P3.latitud-P1.latitud)*(P2.latitud-P1.latitud)+(P3.longitud-P1.longitud)
  *(P2.longitud-P1.longitud))));   
  return result;
}

float distanciaTrucha(Punto A, Punto B){
  float distancia;
  distancia = sqrt(sq((A.latitud-B.latitud))+sq((A.longitud-B.longitud)));
  return distancia;
}
#endif
