#ifndef FILTERS_H
#define FILTERS_H


class Filters
{
    public:

    unsigned char* filter_data; 
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
        for (int y = 0; y < height; ++y) 
        {
            for (int x = 0; x < width; ++x) 
            {
                int index = (y * width + x) * channels;
                unsigned char r = filter_data[index];
                unsigned char g = filter_data[index + 1];
                unsigned char b = filter_data[index + 2];
                unsigned char gray = static_cast<unsigned char>(0.299f * r + 0.587f * g + 0.114f * b);
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