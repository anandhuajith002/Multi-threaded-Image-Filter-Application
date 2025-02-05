#ifndef IMG_SEG_H
#define IMG_SEG_H

#include<vector>
#include<cmath>
#include <stdexcept>
#include"image.hpp"


class ImageSegment
{
    public:
    struct Img_Seg
    {
        unsigned char *img_data;
        int startRow;
        int numRows;
        int width;
        int channels;
    };

    static vector< Img_Seg>segmentImageforThread(Image image,int numThread)
    {
        //error handling
        if (image.img_data == nullptr) {
            throw std::invalid_argument("Image cannot be segmented");
        }
        if (numThread == 0) {
            throw std::invalid_argument("Number of threads must be greater than 0");
        }


        //created no of segments based on no of threads
        vector< Img_Seg> segments;
        segments.reserve(numThread);

        const int baseRowPerSegment = image.height / numThread;
        int remainingRow = image.height % numThread;

        for(int ind = 0; ind < numThread; ind++)
        {
            Img_Seg temp;
            temp.width = image.width;
            temp.channels = image.channels;

            temp.startRow = ind * baseRowPerSegment; 
            temp.numRows = baseRowPerSegment;
            if(remainingRow > 0)
            {
                temp.numRows++;
                remainingRow--;
            }
            temp.img_data = image.img_data + temp.startRow * temp.width * temp.channels;
            segments.push_back(temp);
        }
        return segments;
    }

    // static function to check size of segment 
    static size_t sizeOfSegment(Img_Seg seg)
    {
        int resolution = seg.numRows * seg.width;

        return resolution*seg.channels;
    }
    
};




#endif