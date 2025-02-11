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


int main(int argc,char** argv)
{


    //load image
    cout<<"Multi-Threaded Image Filter Application Starting...."<<endl;
    Image Img(argv[1]);
    cout<<"[INFO]: Image Loaded"<<endl;
    

    //initialise thread pool
    int numThreads = thread::hardware_concurrency()-1;
    
    vector<thread> threads;
    threads.reserve(numThreads);
    cout<<"[INFO]: Initialised Threads"<<endl;



    //divide image into segments
    vector <IMGSEG> segments= ImageSegment::segmentImageforThread(Img, numThreads);
    cout<<"[INFO]: Divided Image into Segments"<<endl;

    
    
    //assign segments to threads
    for(int i=0;i<numThreads;i++)
    {
        threads.emplace_back(FilterSegment :: convertToGray,ref(segments[i]));
    }
    cout<<"[INFO]: Assigned Segments to Threads"<<endl;
    

    //wait for all threads to complete
    cout<<"[INFO]: Waiting for all threads to complete"<<endl;
    for(auto &thread : threads)
    {
        thread.join();
    }




    Img.fil_data= ImageSegment::reconstructImage(segments,Img.height,Img.width,Img.channels);


    Img.saveImage("output_img/image.jpg",Img.fil_data);
    

    Img.free();
    return 0;



}