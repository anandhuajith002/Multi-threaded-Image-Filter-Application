#ifndef TYPES_H
#define TYPES_H

typedef struct ImgSeg
{
    unsigned char *img_data;
    int startRow;
    int numRows;
    int width;
    int channels;
}IMGSEG;

typedef unsigned char us_ch;

#endif