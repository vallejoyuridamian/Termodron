#ifndef Geometria_h
#define Geometria_h

#include "Arduino.h"

class Pendiente;
class Punto;

class Punto {
  public:
    float latitud;
    float longitud;
    float altitud;
    boolean menosQueD;
    float debo;

    //// Constructores
    // Crea un punto vacio
    Punto();
    // Crea un punto de coordenadas latitud=a,longitud=b
    Punto(float a,float b,float c);
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

// Constructores de Punto
Punto::Punto(){
};

Punto::Punto(float a,float b,float c){
  latitud=a;
  longitud=b;
  altitud=c;
};

Punto::Punto(Punto A, float Pendiente){
  latitud=A.latitud+5;
  longitud=Pendiente*(latitud-A.latitud)+A.longitud;
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
    latitud=(B.latitud-A.latitud)*d/dAB+A.latitud;
    longitud=(B.longitud-A.longitud)*d/dAB+A.longitud;
    altitud=B.altitud;
  }
};

Punto::Punto(Punto A, Pendiente PendA, Punto B, Punto C){
  menosQueD=false;
  Pendiente* PendBC = new Pendiente(B,C);
  latitud = (A.longitud-B.longitud+PendBC->m*B.latitud-PendA.m*A.latitud)/(PendBC->m-PendA.m);
  longitud = PendA.m*(latitud-A.latitud)+A.longitud;
  altitud=A.altitud;
  float distanciaBPunto = sqrt(sq(latitud-B.latitud)+sq(longitud-B.longitud));
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
  distancia= sqrt(sq(A.latitud-B.latitud)+sq(A.longitud-B.longitud));
  return distancia;
}

Pendiente::Pendiente(Punto A, Punto B){
  if(B.latitud != A.latitud){
    m=(B.longitud-A.longitud)/(B.latitud-A.latitud);
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
  Punto P3(P1, PendientePerp);
  return abs(dist*(distancia(P3,P1)*distancia(P2,P1)/((P3.latitud-P1.latitud)*(P2.latitud-P1.latitud)+(P3.longitud-P1.longitud)*(P2.longitud-P1.longitud))));     
}

// Proyecta una distancia sobre el segmento P1P2 de manera de alejarme de P1 una distanca dist en direccion perpendicuar a Pend
float desrecalcularDist(float dist, Pendiente Pend, Punto P1, Punto P2){
  float PendientePerp = -1/Pend.m;
  Punto P3(P1, PendientePerp);
  return abs(dist/(distancia(P3,P1)*distancia(P2,P1)/((P3.latitud-P1.latitud)*(P2.latitud-P1.latitud)+(P3.longitud-P1.longitud)*(P2.longitud-P1.longitud))));     
}

#endif
