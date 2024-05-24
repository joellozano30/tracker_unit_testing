#ifndef INTEGER_H
#define INTEGER_H
void f_intgr(float* vector, int len, float time_window_intgr);
float calc_pend(float* vector,int len);
float f_der(float* vector, int len);
float calc_media(float* vector, int len);
float calc_dev_estandar(float* vector, int len);
#endif