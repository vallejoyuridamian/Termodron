void search_body(void) {

int T_max = 9000;
int T_min = 8070;
int pixels_min = 1;//(int) 800 / (h_n * 0.011 * h_n * 0.011);
int tam = 4800;       //int image_tam = image_x * image_y;

Indice A[4800];  //arreglo con los indices del cuerpo que va siendo encontrado
//Indice B[4800];  //areglo con los indices del cuerpo mayor que va siendo encontrado

int checked[image_x][image_y]; //arreglo auxiliar con los puntos ya chequeados

D.a = 0;
D.b = 0;
int t = 0;
for(int i=0; i<image_x; i++){
  for(int j=0; j<image_y; j++){
    checked[i][j]=0;
    A[t].a = 0;
    A[t].b = 0;
    //B[t].a = 0;
    //B[t].b = 0;
    t++;
  }
}
    
int h = 0; //indice de elementos agregados
int g = 0; //indice de elementos chequeados

for (int i=0; i<image_x; i++){
  for (int j=0; j<image_y; j++){
    if (( T_max > image[i][j]) && (image[i][j] > T_min) && (checked[i][j] == 0)){
    	A[0].a = i;
    	A[0].b = j;
    	checked[i][j] = 1;
	    h = 1;
	    g = 0;
	    while(g < h){
	        if(A[g].b < (image_y - 1)){
        	    if (( T_max > image[A[g].a][(A[g].b)+1]) && (image[A[g].a][(A[g].b)+1] > T_min) && (checked[A[g].a][(A[g].b)+1] == 0)){
                	A[h].a = A[g].a;
                	A[h].b = (A[g].b)+1;
            			checked[A[g].a][(A[g].b)+1] = 1;
                	h++;
            	}
        	}
        	if(A[g].a < (image_x - 1) && A[g].b < (image_y - 1)){
	            if (( T_max > image[(A[g].a)+1][(A[g].b)+1]) && (image[(A[g].a)+1][(A[g].b)+1] > T_min) && (checked[(A[g].a)+1][(A[g].b)+1] == 0)){
        	        A[h].a = (A[g].a)+1;
        	        A[h].b = (A[g].b)+1;
            			checked[(A[g].a)+1][(A[g].b)+1] = 1;
        	        h++;
        	    }
        	}
        	if(A[g].a < (image_x - 1)){
        	    if (( T_max > image[(A[g].a)+1][A[g].b]) && (image[(A[g].a)+1][A[g].b] > T_min) && (checked[(A[g].a)+1][A[g].b] == 0)){
        	        A[h].a = (A[g].a)+1;
        	        A[h].b = A[g].b;
            			checked[(A[g].a)+1][A[g].b] = 1;
        	        h++;
        	    }
        	}
        	if(A[g].a < (image_x - 1) && A[g].b >0){
        	    if (( T_max > image[(A[g].a)+1][(A[g].b)-1]) && (image[(A[g].a)+1][(A[g].b)-1] > T_min) && (checked[(A[g].a)+1][(A[g].b)-1] == 0)){
        	        A[h].a = (A[g].a)+1;
        	        A[h].b = (A[g].b)-1;
            			checked[(A[g].a)+1][(A[g].b)-1] = 1;
        	        h++;
        	    }
        	}
    	g++;	
	    }
    	if(g > s && g > pixels_min){
        	s = g;
      		for (int i=0; i<s; i++){ 
             D.a = D.a + A[i].a;
             D.b = D.b + A[i].b;
	       		//B[i].a = A[i].a;
	       		//B[i].b = A[i].b;
    		  }
	    }
	    r++;
    }
  }
}

D.a = D.a / s;
D.b = D.b / s;
}
