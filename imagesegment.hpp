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
        if (image.img_data == nullptr) {
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
        // const int baseRowPerSegment = 10 / numThread;
        // int remainingRow = 10 % numThread;
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
            temp.img_data = image.img_data + temp.startRow * temp.width * temp.channels;
            segments.push_back(temp);
            // cout<<"start row"<<temp.startRow<<">> no of rows "<<temp.numRows<<endl;
        }
        return segments;
    }

static us_ch* reconstructImage(std::vector<IMGSEG> segments, int fHeight, int fWidth)
{
    us_ch* finalImg = new us_ch[fHeight * fWidth * 3](); // Allocate memory

    for (auto &seg : segments)
    {
        int row = seg.startRow;
        int col = seg.width;

        for (int i = 0; i < seg.numRows; i++)
        {
            for (int j = 0; j < seg.width; j++)
            {
                int segIndex = ((i * seg.width) + j) * seg.channels;
                int imgIndex = (((row + i) * fWidth) + (col + j)) * seg.channels;

                // Ensure index is within bounds
                if (imgIndex + 2 < fHeight * fWidth * 3)
                {
                    finalImg[imgIndex] = finalImg[imgIndex + 1] = finalImg[imgIndex + 2] = seg.img_data[segIndex];
                }
            }
        }
    }
    return finalImg; // Return dynamically allocated array (must be freed by the caller)
}


    // static function to check size of segment 
    static size_t sizeOfSegment(IMGSEG seg)
    {
        int resolution = seg.numRows * seg.width;

        return resolution*seg.channels;
    }
    
};




#endif