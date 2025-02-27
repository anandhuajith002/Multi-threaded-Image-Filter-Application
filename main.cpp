#include<iostream>
#include<thread>
#include "include/stb_image.h"
#include "include/stb_image_write.h" 
#include "image.hpp"
#include "filters.hpp"
#include "imagesegment.hpp"
#include "types.hpp"
#include "filtersegment.hpp"

using namespace std;


//int main(int argc,char** argv)
int main()
{


    //load image
    cout<<"Multi-Threaded Image Filter Application Starting...."<<endl;
    Image Img("pic/pic_com.jpg");
    cout<<"[INFO]: Image Loaded"<<endl;
    

    //initialise thread pool
    int numThreads = thread::hardware_concurrency()-1;
    
    vector<thread> threads;
    threads.reserve(numThreads);
    cout<<"[INFO]: Initialised Threads"<<endl;



    //divide image into segments
    vector <IMGSEG> segments= ImageSegment::segmentImageforThread(Img, numThreads);
    cout<<"[INFO]: Divided Image into Segments"<<endl;

    ImageSegment::saveSegmentToFile(segments[2],"seg.txt");    


    // FilterSegment::convertToGray(segments[0]);
    
    //assign segments to threads
    for(int i=0;i<numThreads;i++)
    {
        threads.emplace_back(FilterSeg :: convertToGray, ref(segments[i]));
    }
    cout<<"[INFO]: Assigned Segments to Threads"<<endl;
    

    //wait for all threads to complete
    cout<<"[INFO]: Waiting for all threads to complete"<<endl;
    for(auto &thread : threads)
    {
        thread.join();
    }
    cout<<"[INFO]: All threads completed execution"<<endl;





    cout<<"[INFO]: Reconstructing the segmented Image"<<endl;

    Img.fil_data= ImageSegment::reconstructImage(segments,Img.height,Img.width,Img.channels);

    cout<<"[INFO]: Saving the Filtered Image"<<endl;

    Img.saveImage("output_img/image_simd.jpg",Img.fil_data);
    
    cout<<"[INFO]: Filtering Image completed"<<endl;

    Img.free();
    return 0;



}