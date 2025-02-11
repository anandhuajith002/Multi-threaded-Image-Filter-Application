#ifndef IMGSEG_H
#define IMGSEG_H

#include<vector>
#include<cmath>
#include <stdexcept>
#include<iostream>
#include"image.hpp"
#include "types.hpp"

using namespace std;

class ImageSegment
{
    public:
    
    static vector< IMGSEG >segmentImageforThread(Image image,int numThread)
    {
        //error handling
        if (image.imgData == nullptr) {
            throw std::invalid_argument("Image cannot be segmented");
        }
        if (numThread == 0) {
            throw std::invalid_argument("Number of threads must be greater than 0");
        }


        //created no of segments based on no of threads
        vector< IMGSEG> segments;
        segments.reserve(numThread);

        const int baseRowPerSegment = image.height / numThread;
        int remainingRow = image.height % numThread;
        int currentRow=0;

        for(int ind = 0; ind < numThread; ind++)
        {
            IMGSEG temp;
            temp.width = image.width;
            temp.channels = image.channels;


            temp.startRow = currentRow; 
            temp.numRows = baseRowPerSegment;
            if(remainingRow > 0)
            {
                temp.numRows++;
                remainingRow--;
            }
            currentRow += temp.numRows;

            //calculating size and reverving
            temp.segsize = temp.numRows * temp.width * temp.channels;
            temp.img_data = new us_ch[temp.segsize];

            // Copy only the required data for this segment
            memcpy(temp.img_data, image.imgData + temp.startRow * temp.width * temp.channels, temp.segsize);

            segments.push_back(temp);


        }
        return segments;
    }


   
   static unsigned char* reconstructImage(std::vector<IMGSEG> segments, int fHeight, int fWidth, int channels)
{
    // Allocate memory for final image
    unsigned char* finalImg = new unsigned char[fHeight * fWidth * channels]();

    for (const auto &seg : segments)
    {
        int row = seg.startRow;
        
        for (int i = 0; i < seg.numRows; i++)
        {
            for (int j = 0; j < seg.width; j++)
            {
                // Bounds checking (though shouldn't be needed if segments were created correctly)
                if (row + i >= fHeight || j >= fWidth)
                    continue;

                int segIndex = (i * seg.width + j) * channels;
                int imgIndex = ((row + i) * fWidth + j) * channels;

                for (int c = 0; c < channels; c++)
                {
                    finalImg[imgIndex + c] = seg.img_data[segIndex + c];
                }
            }
        }
    }
    
    return finalImg;
}





    // static function to check size of segment 
    static size_t sizeOfSegment(IMGSEG seg)
    {
        int resolution = seg.numRows * seg.width;

        return resolution*seg.channels;
    }
    
};




#endif