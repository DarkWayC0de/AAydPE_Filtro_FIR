#include <cstdio>

#define NV 7000
#define NC 25

#define BLOQUE 10

int readdata(FILE *file,int ini, int size, float *v_data){
	int i = ini;
	int a = 0;
    while ( a != EOF && i < size){
    	a = fscanf(file,"%f,",&v_data[i]);
	   	i++;
	}
    if(i < size){
    	return i;
    }
    return 0;
}
void writedata(FILE *file,int ini, int size, float *v){
	int i = ini;
    while ( i < size){
    	float a = v[i];
    	if((a < 0.0001f && a > -0.0f)|| (a > -0.0001f && a < 0.0f)    ){
    		fprintf(file,"%.4e\n",a);
    	}else{
    		fprintf(file,"%.8f\n",a);
    	}
	   	i++;
	}
}
void filterfir(float* v_coef, int sz_coef, float* v_data, float* v_result){
	int i = 0;
	for(i = 0; i < sz_coef ; i++){
		int indice=0;
		int iteraciones = (sz_coef/BLOQUE);
		int resto = (sz_coef%BLOQUE);
		while(iteraciones-- > 0){
			v_result[i] += v_data[indice+10+i] * v_coef[indice+10 -1];
			v_result[i] += v_data[indice+9+i] * v_coef[indice+9 -1];
			v_result[i] += v_data[indice+8+i] * v_coef[indice+8 -1];
			v_result[i] += v_data[indice+7+i] * v_coef[indice+7 -1];
			v_result[i] += v_data[indice+6+i] * v_coef[indice+6 -1];
			v_result[i] += v_data[indice+5+i] * v_coef[indice+5 -1];
			v_result[i] += v_data[indice+4+i] * v_coef[indice+4 -1];
			v_result[i] += v_data[indice+3+i] * v_coef[indice+3 -1];
			v_result[i] += v_data[indice+2+i] * v_coef[indice+2 -1];
			v_result[i] += v_data[indice+1+i] * v_coef[indice+1 -1];
			indice +=BLOQUE;
		}
		switch(resto) {
			case 9: v_result[i] += v_data[indice+9+i] * v_coef[indice+9 -1];
			case 8: v_result[i] += v_data[indice+8+i] * v_coef[indice+8 -1];
			case 7: v_result[i] += v_data[indice+7+i] * v_coef[indice+7 -1];
			case 6: v_result[i] += v_data[indice+6+i] * v_coef[indice+6 -1];
			case 5: v_result[i] += v_data[indice+5+i] * v_coef[indice+5 -1];
			case 4: v_result[i] += v_data[indice+4+i] * v_coef[indice+4 -1];
			case 3: v_result[i] += v_data[indice+3+i] * v_coef[indice+3 -1];
			case 2: v_result[i] += v_data[indice+2+i] * v_coef[indice+2 -1];
			case 1: v_result[i] += v_data[indice+1+i] * v_coef[indice+1 -1];
			case 0: ;
		}
	}
}
void v_0(float *v,int ini, int sz){
	int i = ini;
    for(; i<sz;i++){
	   	v[i] = 0;
	}
}

void mv_data(float *v, int mnc){
	int i = 0;
	for(i = 0; i < mnc; i++){
		v[i]=v[mnc+i];
	}
}

int main()
{ 
		float v_datos[NC*2];
		float v_resul[NC];
	    float v_coef[NC];

	    FILE *fcoeficientes = fopen("../Coeficientes50.csv","r");
	    if(fcoeficientes == NULL){
	    	printf("No se pudo abrir archivo de coeficientes");
	    	return 1;
	    }
	    FILE *fdatos = fopen("../musica4.csv","r");
	    if(fdatos == NULL){
	    	printf("No se pudo abrir archivo de datos");
	    	return 2;
	    }
	    FILE *fsalida = fopen("../salida.csv","w");
	    if(fsalida == NULL){
	    	printf("No se pudo abrir archivo de salida");
	    	return 3;
	    }



	    readdata(fcoeficientes,0,NC,v_coef);
	    v_0(v_datos,0,NC);
	    int ok =readdata(fdatos,NC,NC*2,v_datos);
	    if(ok != 0){
	    	printf("Conjunto de datos mas corto que conjunto de coeficientes");
	    	return 4;
	    }
	    int valores_restantes = NV-NC;

	    do {
			v_0(v_resul,0,NC);
			filterfir(v_coef,NC,v_datos,v_resul);
			writedata(fsalida,0,NC,v_resul);
			mv_data(v_datos,NC);

			if( valores_restantes <NC){
				readdata(fdatos,NC,NC+valores_restantes,v_datos);
				ok = NC+valores_restantes;
			}else{
				ok =readdata(fdatos,NC,NC*2,v_datos);
			}
			valores_restantes -= NC;
	    } while(ok==0);

	    v_0(v_datos,ok,NC*2);
	    v_0(v_resul,0,NC);
	    filterfir(v_coef,NC,v_datos,v_resul);
	    writedata(fsalida,0,NC,v_resul);
	    mv_data(v_datos,NC);
	    v_0(v_datos,NC,NC*2);
	    v_0(v_resul,0,NC);
	    filterfir(v_coef,NC,v_datos,v_resul);
	    int resto= ((NV/NC)+1)*NC;
	    resto=(NV+NC)-resto;
	    writedata(fsalida,0,resto+1,v_resul);

	    fclose(fcoeficientes);
	    fclose(fdatos);
	    fclose(fsalida);

	    return 0;
}



