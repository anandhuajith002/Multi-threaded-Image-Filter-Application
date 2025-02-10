// #ifndef FILTER_SEG_H
// #define FILTER_SEG_H

// #include"types.hpp"


// class FilterSegment
// {
//     public:
    
//     static void convertToGray(IMGSEG *seg)
//     {
//         for(int y=seg->startRow; y< seg->numRows+seg->startRow; y++)
//         {
//             for(int x=0; x<seg->width; x++)
//             {
//                 int index = (y * seg->width +x) * seg->channels;
//                 us_ch r = seg->img_data[index];
//                 us_ch g = seg->img_data[index+1];
//                 us_ch b = seg->img_data[index+2];
//                 us_ch gray = static_cast<us_ch>(0.299f * r + 0.587f * g + 0.114f * b);
//                 seg->img_data[index]=seg->img_data[index+1]=seg->img_data[index+2]=gray;
//             }
//         }
//     }
// };


// #endif