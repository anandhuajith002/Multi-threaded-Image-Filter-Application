#ifndef IMGSEG_H
#define IMGSEG_H

#include<vector>
#include<cmath>
#include <stdexcept>
#include<iostream>
#include"image.hpp"
#include "types.hpp"
 #include <fstream>
#include <iomanip>

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
            temp.segsize = ImageSegment::sizeOfSegment(temp);
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
   

    static void saveSegmentToFile(const IMGSEG& segment, const std::string& filename) 
    {
        std::ofstream outFile(filename, std::ios::out);
    if (!outFile) {
        throw std::runtime_error("Failed to open file for writing");
    }

    // Write segment metadata
    outFile << "Width: " << segment.width << "\n";
    outFile << "Channels: " << segment.channels << "\n";
    outFile << "Start Row: " << segment.startRow << "\n";
    outFile << "Num Rows: " << segment.numRows << "\n";
    outFile << "Segment Size: " << segment.segsize << "\n";
    outFile << "Hex Data:\n";

    int rowSize = segment.width * segment.channels; // Number of bytes per row

    for (int row = 0; row < segment.numRows; row++) {
        for (int col = 0; col < rowSize; col++) {
            int index = row * rowSize + col;
            outFile << std::hex << std::setw(2) << std::setfill('0') 
                    << static_cast<int>(segment.img_data[index]) << " ";
        }
        outFile << "\n"; // New line after each row
    }

    outFile.close();
    }






    // static function to check size of segment 
    static size_t sizeOfSegment(IMGSEG seg)
    {
        int resolution = seg.numRows * seg.width;

        return resolution*seg.channels;
    }
    
};




#endif