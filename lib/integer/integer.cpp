#include "integer.h"
#include "math.h"

void f_intgr(float* vector, int len, float time_window_intgr){
    int i=0;
    float suma=0;
    float dt;
    dt = time_window_intgr/len;
    //Serial.println(dt);
    for(i=0;i<=len;i++){
        //Serial.println(vector[i]);
        suma = suma + (vector[i])*dt;
        vector[i] = suma;
        //Serial.println(vector[i]);
    }
}
float calc_pend(float* vector,int len){
    int i = 0;
    float pend = 0;
    pend = (vector[len]-vector[0])/(len);

    return pend;
}
float f_der(float* vector, int len){
    int i;
    float high_var;
    float min_vector = 999999, max_vector = 0;

    for(i=0;i<len;i++){
        if(vector[i] <= min_vector)
            min_vector = vector[i];
        if(vector[i] >= max_vector)
            max_vector = vector[i];
    }

    high_var = fabs(max_vector - min_vector);

    return high_var;
}
double calc_media(float* vector, int len){

    int i=0, acum=0;
    double resultado;
    for(i=0;i<len;i++){
        acum+= vector[i];
    }
    resultado = acum/len;
    return resultado;
}
double calc_dev_estandar(float* vector, int len){

    int i, acum=0;
    double media, resultado;
    media = calc_media(vector, len);

    for(i=0;i<len;i++){
        acum+= pow(vector[i]-media,2);
    }

    resultado = sqrt((double)acum/(double)(len));
    return resultado;
}