#ifndef FILTERS_H
#define FILTERS_H

#include "types.hpp"

class Filters
{
    public:

    us_ch* filter_data; 
    int height,width,channels;

    Filters(Image img)
    {
        filter_data=img.img_data;
        height=img.height;
        width=img.width;
        channels=img.channels;
    }

    void convertToGrayscale()
    {
        for (int i = 0; i < height; ++i) 
        {
            for (int j = 0; j < width; ++j) 
            {
                int index = (i * width + j) * channels;
                us_ch r = filter_data[index];
                us_ch g = filter_data[index + 1];
                us_ch b = filter_data[index + 2];
                us_ch gray = static_cast<us_ch>(0.299f * r + 0.587f * g + 0.114f * b);
                filter_data[index] = filter_data[index + 1] = filter_data[index + 2] = gray;
            }
        }
    }


    void save_filter_img()
    {
        if (!stbi_write_jpg("output_img/gray.jpg", width, height, channels,filter_data, 100)) {
        cerr << "Error: Could not save processed image\n";
        }
    }

};



#endif