#ifndef TYPES_H
#define TYPES_H

typedef struct ImgSeg
{
    unsigned char *img_data;
    int startRow;
    int numRows;
    int width;
    int channels;
    int segsize;
}IMGSEG;

typedef unsigned char us_ch;

bool checkAVX2Support() {
    #ifdef _MSC_VER // Microsoft compiler
        int cpuInfo[4];
        __cpuid(cpuInfo, 7);
        return (cpuInfo[1] & (1 << 5)) != 0;
    #else // GCC, Clang
        __builtin_cpu_init();
        return __builtin_cpu_supports("avx2");
    #endif
}

#endif