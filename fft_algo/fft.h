#ifndef _FFT_H_
#define _FFT_H_

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define MAX_SIZE 16384       //16k 16*16384

// define complex struct 
struct Complex_{
     double real;
     double imag; 
};
typedef struct Complex_ Complex;

void FFT_Interface(void*buffer,ssize_t size);

#endif
