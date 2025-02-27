#ifndef FILTER_SEG_H
#define FILTER_SEG_H

#include "types.hpp"
#include <iostream>
#include <emmintrin.h>    // SSE2
#include <tmmintrin.h>    // SSSE3 for _mm_shuffle_epi8
#include <intrin.h>       // For __cpuid (Windows)
#include <cstring>

using namespace std;

class FilterSeg
{
    private:
        /// Check if CPU supports SSSE3 (bit 9 of ECX when calling CPUID with function 1)
    static bool supportsSSSE3() 
    {
        int cpuInfo[4];
        __cpuid(cpuInfo, 1);
        return (cpuInfo[2] & (1 << 9)) != 0;
    }


    public:
    static void convertToGrayFallback(IMGSEG &seg)
    {
        for(int i=0;i<seg.numRows;i++)
        {

            for(int j=0;j<seg.width;j++)
            {
                int index = (i*seg.width +j)*seg.channels;
                us_ch r = seg.img_data[index];
                us_ch g = seg.img_data[index + 1];
                us_ch b = seg.img_data[index + 2];
                us_ch gray = static_cast<us_ch>(0.299f * r + 0.587f * g + 0.114f * b);
                seg.img_data[index]=seg.img_data[index+1]= seg.img_data[index+2]=gray;
            }
        }
    }
    static void convertToGray(IMGSEG &seg)
    {
        static bool printFlag = false;
        if (!supportsSSSE3() && ! printFlag)
        {
            cout << "[INFO]: Executing in normal (scalar) mode." << endl;
            printFlag=true;
            convertToGrayFallback(seg);
            return;
        }
        else if(!printFlag)
        {
            printFlag=true;
            cout << "[INFO]: Executing in SIMD mode." << endl;
        }


        const __m128i coeff_r = _mm_set1_epi16(77);
        const __m128i coeff_g = _mm_set1_epi16(150);
        const __m128i coeff_b = _mm_set1_epi16(29);
        const __m128i rounding = _mm_set1_epi16(128);
        const __m128i zero = _mm_setzero_si128();

        for (int i = 0; i < seg.numRows; i++)
        {
            int j;
            us_ch gray[4];

            for (j = 0; j <= seg.width - 4; j += 4)
            {
                int index = (i * seg.width + j) * seg.channels;
                __m128i data = _mm_loadu_si128((const __m128i *)(seg.img_data + index));

                // Extract R, G, B channels
                __m128i r_bytes = _mm_shuffle_epi8(data, _mm_setr_epi8(0, 3, 6, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));
                __m128i g_bytes = _mm_shuffle_epi8(data, _mm_setr_epi8(1, 4, 7, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));
                __m128i b_bytes = _mm_shuffle_epi8(data, _mm_setr_epi8(2, 5, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1));

                // Convert from 8-bit to 16-bit
                __m128i r = _mm_unpacklo_epi8(r_bytes, zero);
                __m128i g = _mm_unpacklo_epi8(g_bytes, zero);
                __m128i b = _mm_unpacklo_epi8(b_bytes, zero);

                // Multiply by coefficients
                __m128i mul_r = _mm_mullo_epi16(r, coeff_r);
                __m128i mul_g = _mm_mullo_epi16(g, coeff_g);
                __m128i mul_b = _mm_mullo_epi16(b, coeff_b);

                // Sum, add rounding, and shift
                __m128i sum = _mm_add_epi16(_mm_add_epi16(mul_r, mul_g), mul_b);
                __m128i gray16 = _mm_srli_epi16(_mm_add_epi16(sum, rounding), 8);

                // Pack to 8-bit
                __m128i gray8 = _mm_packus_epi16(gray16, gray16);
                std::memcpy(gray, &gray8, sizeof(int));  // Copy 4 grayscale values safely

                // Store grayscale pixels back
                for (int k = 0; k < 4; k++)
                {
                    int pixel_offset = index + k * seg.channels;
                    if (seg.channels == 3)
                    {
                        seg.img_data[pixel_offset] = gray[k];      // R
                        seg.img_data[pixel_offset + 1] = gray[k];  // G
                        seg.img_data[pixel_offset + 2] = gray[k];  // B
                    }
                }
            }

            // Scalar loop for remaining pixels
            for (; j < seg.width; j++)
            {
                int index = (i * seg.width + j) * seg.channels;
                us_ch r = seg.img_data[index];
                us_ch g = seg.img_data[index + 1];
                us_ch b = seg.img_data[index + 2];
                us_ch gray = static_cast<us_ch>(0.299f * r + 0.587f * g + 0.114f * b);
                seg.img_data[index] = seg.img_data[index + 1] = seg.img_data[index + 2] = gray;
            }
        }
    }


};

// class FilterSegment {
//     public:
//         static void convertToGray(IMGSEG &seg) 
//         {
//             // Create temporary array for grayscale values
//             std::vector<us_ch> grayArray(seg.width * seg.numRows);
            
//             const __m256 weights = _mm256_set_ps(0.0f, 0.114f, 0.587f, 0.299f,
//                                                0.0f, 0.114f, 0.587f, 0.299f);
    
//             int pixelsPerIteration = 8;
//             int alignedWidth = (seg.width / pixelsPerIteration) * pixelsPerIteration;
    
//             // First compute all grayscale values
//             for(int i = 0; i < seg.numRows; i++) 
//             {
//                 int rowOffset = i * seg.width * seg.channels;
                
//                 for(int j = 0; j < alignedWidth; j += pixelsPerIteration) 
//                 {
//                     int index = (rowOffset + j) * seg.channels;
                    
//                     __m256i rgb = _mm256_loadu_si256((__m256i*)&seg.img_data[index]);
//                     __m256 rgbFloat = _mm256_cvtepi32_ps(rgb);
//                     __m256 gray = _mm256_dp_ps(rgbFloat, weights, 0x77);
//                     __m256i grayInt = _mm256_cvtps_epi32(gray);
                    
//                     // Store to temporary array
//                     int grayValues[8];
//                     _mm256_storeu_si256((__m256i*)grayValues, grayInt);
                    
//                     for (int i = 0; i < 8; i++) {
//                         unsigned char ch = static_cast<unsigned char>(grayValues[i]);
//                         filter_data[index] = filter_data[index + 1] = filter_data[index + 2] = gray;

//                     }

//                 }
                
//                 // Handle remaining pixels
//                 for(int j = alignedWidth; j < seg.width; j++) 
//                 {
//                     int index = (rowOffset + j) * seg.channels;
//                     us_ch r = seg.img_data[index];
//                     us_ch g = seg.img_data[index + 1];
//                     us_ch b = seg.img_data[index + 2];
                    
//                     grayArray[grayRowOffset + j] = static_cast<us_ch>(0.299f * r + 0.587f * g + 0.114f * b);
//                 }
//             }
    
//             // Now copy grayscale values back to original array
//             for(int i = 0; i < seg.numRows; i++) 
//             {
//                 for(int j = 0; j < seg.width; j++) 
//                 {
//                     int srcIndex = i * seg.width + j;
//                     int destIndex = (i * seg.width + j) * seg.channels;
                    
//                     // Copy same grayscale value to all three channels
//                     seg.img_data[destIndex] = grayArray[srcIndex];
//                     seg.img_data[destIndex + 1] = grayArray[srcIndex];
//                     seg.img_data[destIndex + 2] = grayArray[srcIndex];
//                 }
//             }
//         }
    
//         static void convertToGrayFallback(IMGSEG &seg) 
//         {
//             // Create temporary array for grayscale values
//             std::vector<us_ch> grayArray(seg.width * seg.numRows);
    
//             // First compute all grayscale values
//             for(int i = 0; i < seg.numRows; i++)
//             {
//                 for(int j = 0; j < seg.width; j++)
//                 {
//                     int srcIndex = (i * seg.width + j) * seg.channels;
//                     int destIndex = i * seg.width + j;
                    
//                     us_ch r = seg.img_data[srcIndex];
//                     us_ch g = seg.img_data[srcIndex + 1];
//                     us_ch b = seg.img_data[srcIndex + 2];
    
//                     grayArray[destIndex] = static_cast<us_ch>(0.299f * r + 0.587f * g + 0.114f * b);
//                 }
//             }
    
//             // Copy grayscale values back to original array
//             for(int i = 0; i < seg.numRows; i++)
//             {
//                 for(int j = 0; j < seg.width; j++)
//                 {
//                     int srcIndex = i * seg.width + j;
//                     int destIndex = (i * seg.width + j) * seg.channels;
                    
//                     seg.img_data[destIndex] = grayArray[srcIndex];
//                     seg.img_data[destIndex + 1] = grayArray[srcIndex];
//                     seg.img_data[destIndex + 2] = grayArray[srcIndex];
//                 }
//             }
//         }
//     };

#endif