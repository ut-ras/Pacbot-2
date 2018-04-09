#ifndef ENC_H
#define ENC_H

typedef void (*enc_cb_t)(int);

struct Enc {
    int encA;
    int encB;
    int levA;
    int levB;
    int lastGpio;
    long pos;
};

typedef struct Enc Enc;

Enc* createEnc(int encA, int encB);

void destroyEnc(Enc* enc);

#endif
