Punto[] Mision = new Punto[20];
Punto PuntoAux, PuntoAux2, Punto1esN;
int i,k,z = 0; // contador
int N = 0; // contador de waypoints
PImage bg;
int state = 0;
float xBut = 500;
float yBut = 600;
float wBut = 150;
float hBut = 50;
float distEntrePer = 90;
float debo = 0;
float distEnPer=0;
Pendiente Pend;
boolean unoEsN = true;

void setup() {
  size(1181, 672);
  bg=loadImage("Captura.PNG");
  //background(bg);
}

void draw() {
  if(state==0){
    fill(220,220,220);
    rect(xBut,yBut,wBut,hBut);
    fill(0);
    text("Enviar Mision",xBut+35,yBut+30);
    if (i>0){
      fill(255,0,0);
      ellipse(Mision[i-1].x,Mision[i-1].y,5,5);
      fill(0);
      text(i,Mision[i-1].x+5,Mision[i-1].y+5);
    }
    if(i>1){
      stroke(0);
      line(Mision[i-1].x,Mision[i-1].y,Mision[i-2].x,Mision[i-2].y);
    }
  }
  else{
    if (state == 1){
      // Traza el ultimo segmento que cierra el perimetro
      stroke(0);  
      line(Mision[i-1].x,Mision[i-1].y,Mision[0].x,Mision[0].y);
      // Traza el primer segmento rojo entre los puntos 1 y 2
      stroke(255,0,0);
      line(Mision[0].x,Mision[0].y,Mision[1].x,Mision[1].y); 
      i=0;
      k=N;
      PuntoAux2 = new Punto(Mision[1].x,Mision[1].y);
      Pend = new Pendiente(Mision[0],Mision[1]);
      Punto1esN= new Punto(Mision[0].x,Mision[0].y);
      while (i<k-2){
        stroke(255,0,0);
        // Recorre el perimetro del lado de las i (indice creciente de los puntos)
        println("Recorre el perimetro del lado de las i (indice creciente de los puntos)");
        distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Mision[i+2]);
        PuntoAux = new Punto(PuntoAux2,Mision[i+2],distEnPer);
        if(PuntoAux.menosQueD){
          i++;
          print("Aumento i recorriendo el perimetro por el lado de las i ");
          println(i);
          if(i>=k-2){
            println("Termino");
            line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
            break;
          }
          line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
          debo =desrecalcularDist(PuntoAux.debo, Pend, PuntoAux, Mision[i]);
          debo =recalcularDist(debo, Pend, PuntoAux, Mision[i+2]);
          PuntoAux2 = new Punto(PuntoAux.x, PuntoAux.y);
          PuntoAux= new Punto(PuntoAux,Mision[i+2],debo);
          
          if(PuntoAux.menosQueD){
            i++;
            print("Aumento i recorriendo el perimetro por el lado de las i ");
            println(i);
            if(i>=k-2){
              println("Termino");
              line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
              break;
            }
          }
        }
        line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
        //  Cruza al lado de los k (indice decreciente de los puntos)
        println("Cruza al lado de los k (indice decreciente de los puntos)");
        if(unoEsN){
          PuntoAux2 = new Punto(PuntoAux, Pend, Punto1esN, Mision[k-1]);
        }
        else{
          PuntoAux2 = new Punto(PuntoAux, Pend, Mision[k], Mision[k-1]);
        }
        if(PuntoAux2.menosQueD){
          k--;
          print("Disminuyo k cruzando al lado de las k");
          println(k);
          unoEsN=false;
          Pend = new Pendiente(PuntoAux,PuntoAux2);
          if(i>=k-2){
            println("Termino");
            line(PuntoAux.x,PuntoAux.y,PuntoAux2.x,PuntoAux2.y);
            break;
          }  
        }
        line(PuntoAux.x,PuntoAux.y,PuntoAux2.x,PuntoAux2.y);
        // Recorre el perimetro del lado de las k
        println("Recorre el perimetro del lado de las k");
        distEnPer=recalcularDist(distEntrePer,Pend,PuntoAux2,Mision[k-1]);  
        PuntoAux = new Punto(PuntoAux2,Mision[k-1],distEnPer);
        if(PuntoAux.menosQueD){
          k--;
          print("Disminuyo k recorriendo el perimetro por el lado de las k ");
          println(k);
          unoEsN=false;
          if(i>=k-2){
            line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
            break;
          }
          line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
          debo =desrecalcularDist(PuntoAux.debo, Pend, PuntoAux2, Mision[k]);
          debo =recalcularDist(debo, Pend, PuntoAux, Mision[k-1]);
          PuntoAux2 = new Punto(PuntoAux.x, PuntoAux.y);
          PuntoAux= new Punto(PuntoAux,Mision[k-1],debo);
        
          if(PuntoAux.menosQueD){
            k--;
            print("Disminuyo k recorriendo el perimetro por el lado de las k ");
            println(k);
            unoEsN=false;
          if(i>=k-2){
            line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
            break;
          }
        }
      }
      line(PuntoAux2.x,PuntoAux2.y,PuntoAux.x,PuntoAux.y);
      // Cruza al lado de las i  
      println("Cruza al lado de las i");
      PuntoAux2 = new Punto(PuntoAux,Pend,Mision[i+1],Mision[i+2]);
      if(PuntoAux2.menosQueD){
        i++;
        print("Aumento i cruzando al lado de las i");
        println(i);
        Pend = new Pendiente(PuntoAux,PuntoAux2);
        if(i>=k-2){
          line(PuntoAux.x,PuntoAux.y,PuntoAux2.x,PuntoAux2.y);
          break;
        }
      }
      line(PuntoAux.x,PuntoAux.y,PuntoAux2.x,PuntoAux2.y);
      z=z+1;
      if (z > 799){
        break;
      }
    }
    state=2;
    print("Al final z vale ");
    println(z);
    println("Si z es 800 es que quedo en un loop o algo raro");
    }
  }
}

void mouseClicked(){
  if(state==0){
    if(mouseX>xBut && mouseX <xBut+wBut && mouseY>yBut && mouseY <yBut+hBut){
     println("Mision Enviada");
     state=1;  
    }
    else{
      Mision[i]= new Punto();
      Mision[i].x =mouseX;
      Mision[i].y =mouseY;
      i++;
      N++;
    }
  }
  else{
  }
}

public class Punto {
  public  int x;
  public  int y;
  public boolean menosQueD;
  public  float debo;

  Punto(){};
  Punto(int a, int b){
    x=a;
    y=b;
  }
  // Creo un punto en el segmento AB tal que este a una distancia d de A
  // si B esta mas cerca, voy a B
  Punto(Punto A, Punto B, float d){
    menosQueD=false;
    float dAB = distancia(A,B);
    if (dAB < d){
      x=B.x;
      y=B.y;
      menosQueD=true;
      debo = d - dAB;
    }
    else{
      x=int((B.x-A.x)*d/dAB+A.x);
      y=int((B.y-A.y)*d/dAB+A.y);
    }
  }
  // Creo un punto en el segmento BC tal que el segmento APunto es paralelo a PendA
  // Si el punto queda pasando C, voy hasta C
  Punto(Punto A, Pendiente PendA, Punto B, Punto C){
    menosQueD=false;
    Pendiente PendBC = new Pendiente(B,C);
    x = int((A.y-B.y+PendBC.m*B.x-PendA.m*A.x)/(PendBC.m-PendA.m));
    y = int(PendA.m*(x-A.x)+A.y);
    float distanciaBPunto = sqrt(sq(x-B.x)+sq(y-B.y));
    float distanciaBC=distancia(B,C);
    if (distanciaBPunto > distanciaBC){
      x=C.x;
      y=C.y;
      menosQueD=true;
    }
  }
  Punto(Punto A, float Pendiente){
    x=A.x+5;
    y=int(Pendiente*(x-A.x)+A.y);  
  }
}

public class Pendiente {
  public  float m;
  public  boolean inf;
  
  Pendiente(Punto A, Punto B){
    if(B.x != A.x){
    m=float((B.y-A.y))/float((B.x-A.x));
    inf=false;
    }
    else{
    m=0;
    inf=true;
    }  
  }
}

float distancia(Punto A, Punto B){
  float distancia;
  println(i);
  println(k);
  distancia= sqrt(sq(A.x-B.x)+sq(A.y-B.y));
  return distancia;
}

// (des)Proyecta una distancia sobre el segmento P1P2 de manera de alejarme de P1 una distanca dist en direccion perpendicuar a Pend
float recalcularDist(float dist, Pendiente Pend, Punto P1, Punto P2){
  float PendientePerp = -1/Pend.m;
  Punto P3 = new Punto(P1, PendientePerp);
  return abs(dist*(distancia(P3,P1)*distancia(P2,P1)/((P3.x-P1.x)*(P2.x-P1.x)+(P3.y-P1.y)*(P2.y-P1.y))));     
}

// Proyecta una distancia sobre el segmento P1P2 de manera de alejarme de P1 una distanca dist en direccion perpendicuar a Pend
float desrecalcularDist(float dist, Pendiente Pend, Punto P1, Punto P2){
  float PendientePerp = -1/Pend.m;
  Punto P3 = new Punto(P1, PendientePerp);
  return abs(dist/(distancia(P3,P1)*distancia(P2,P1)/((P3.x-P1.x)*(P2.x-P1.x)+(P3.y-P1.y)*(P2.y-P1.y))));     
}