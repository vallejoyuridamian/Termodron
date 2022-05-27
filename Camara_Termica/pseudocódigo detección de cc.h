// Estás parado en el primer punto
manchamax=0

for todos los puntos de imagen

	punto(x,y)
	cola=array(puntosmax)=0 //todos en 0
	cola[0]=punto(x,y)
	mancha=array(puntosmax)=0 //todos en 0
	i=0
	salir=false
	while salir is false	
		//Te fijás si el punto de la derecha   y el de abajo son calientes, si lo son los agregás a la cola y a la mancha
		cola[i]=0 // se anula el valor actual, se "borra de la cola"
		if imagen(x+1,y) > umbral
			imagen(x+1,y) --> va a la cola y a la mancha
			salirx=false
		if imagen(x,y+1) > umbral
			imagen(x,y+1) --> va a la cola y a la mancha
			saliry=false
		
		
		//La cola y la mancha
		i++
		cola[i]=punto
		mancha[i]=punto

		//salís si la cola está vacía
		if(sum(cola))=0
			salir=true
		else
			punto(x,y)=último elemento no nulo de la cola
	end

	if size(mancha) > size(manchamax)
		manchamax=mancha
end