#ifndef IMAGE_H
#define IMAGE_H

#include<bits/stdc++.h>
#include "include/stb_image.h"
#include "include/stb_image_write.h" 
using namespace std;


class Image
{
    public:

    //variables
    char *file_name;
    unsigned char* img_data;
    int height,width,channels;

    //constructor
    Image(char *name)
    {
        file_name=name;
        img_data = stbi_load(file_name, &width, &height, &channels, 0);
        // dest_folder="output_img/";

        if (img_data == nullptr) 
        {
            cerr << "[INFO]: !!!Error!!! Could not load image\n";
            exit(0);
        }
    }
    void free()
    {
        stbi_image_free(img_data);
    }

    
};





#endif
