#include<iostream>
#include "include/stb_image.h"
#include "include/stb_image_write.h" 
#include "image.hpp"
#include "filters.hpp"
#include "imagesegment.hpp"
#include<thread>

using namespace std;

int main(int argc,char** argv)
{
    Image img(argv[1]);
    
    // Filters filter(img);

    int num = thread::hardware_concurrency();

    ImageSegment::segmentImageforThread(img,num);
    // filter.convertToGrayscale();
    // filter.save_filter_img();




        // Load an image from file
    
    return 0;



}