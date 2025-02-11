#ifndef IMAGE_H
#define IMAGE_H

#include "include/stb_image.h"
#include "include/stb_image_write.h" 
using namespace std;


class Image
{
    public:

    //variables
    char *file_name;
    unsigned char* imgData;
    unsigned char* fil_data;
    int size,height,width,channels;

    //constructor
    Image(char *name)
    {
        file_name=name;
        imgData = stbi_load(file_name, &width, &height, &channels, 0);
        // dest_folder="output_img/";

        if (imgData == nullptr) 
        {
            cerr << "[INFO]: !!!Error!!! Could not load image\n";
            exit(0);
        }
        size=height*width*channels;
    }
    void free()
    {
        stbi_image_free(imgData);
    }

    void saveImage(const char* fname,const unsigned char *data)
    {
        stbi_write_jpg(fname,width,height,channels,data,90);
    }



};



#endif
