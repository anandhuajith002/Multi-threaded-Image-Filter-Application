#include <vector>
#include <stdexcept>
#include <cmath>

class ImageSegmenter {
public:
    struct ImageSegment {
        unsigned char* data;
        size_t startRow;
        size_t numRows;
        size_t width;
        size_t channels;
    };

    /**
     * Segments image data into chunks based on number of threads
     * @param imageData Pointer to the image data array
     * @param height Total height of the image in pixels
     * @param width Width of the image in pixels
     * @param channels Number of color channels
     * @param numThreads Number of threads to split the image into
     * @return Vector of ImageSegment structures containing the segmented data
     */
    static std::vector<ImageSegment> segmentImageByThreads(
        unsigned char* imageData,
        size_t height,
        size_t width,
        size_t channels,
        size_t numThreads
    ) {
        if (imageData == nullptr) {
            throw std::invalid_argument("Image data pointer cannot be null");
        }
        if (numThreads == 0) {
            throw std::invalid_argument("Number of threads must be greater than 0");
        }
        if (height == 0 || width == 0 || channels == 0) {
            throw std::invalid_argument("Image dimensions must be greater than 0");
        }

        std::vector<ImageSegment> segments;
        segments.reserve(numThreads);

        // Calculate base rows per thread and remaining rows
        size_t baseRowsPerThread = height / numThreads;
        size_t remainingRows = height % numThreads;
        size_t currentRow = 0;

        // Create segments
        for (size_t threadIdx = 0; threadIdx < numThreads; ++threadIdx) {
            size_t rowsForThisThread = baseRowsPerThread;
            if (threadIdx < remainingRows) {
                rowsForThisThread++;
            }

            // Calculate pointer to the start of this segment
            size_t offset = currentRow * width * channels;
            ImageSegment segment{
                imageData + offset,  // Pointer to start of segment data
                currentRow,         // Starting row
                rowsForThisThread,  // Number of rows in this segment
                width,              // Width of the image
                channels           // Number of channels
            };

            segments.push_back(segment);
            currentRow += rowsForThisThread;
        }

        return segments;
    }

    /**
     * Helper function to get the total size in bytes for a segment
     * @param segment The image segment
     * @return Total size in bytes
     */
    static size_t getSegmentSize(const ImageSegment& segment) {
        return segment.numRows * segment.width * segment.channels;
    }
};




/////////////////////////////////////////////////////////////////


#include <iostream>
#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>  // For image loading, but you can use any image library

// Function to process a single segment
void processSegment(const ImageSegmenter::ImageSegment& segment) {
    // Example processing function
    // This could be your actual image processing logic
    for (size_t row = 0; row < segment.numRows; ++row) {
        for (size_t col = 0; col < segment.width; ++col) {
            for (size_t c = 0; c < segment.channels; ++c) {
                size_t idx = (row * segment.width + col) * segment.channels + c;
                // Example: Increase brightness by 50
                segment.data[idx] = std::min(255, static_cast<int>(segment.data[idx]) + 50);
            }
        }
    }
}

int main() {
    try {
        // 1. Load your image (example using OpenCV, but you can use any method)
        cv::Mat image = cv::imread("your_image.jpg");
        if (image.empty()) {
            throw std::runtime_error("Failed to load image");
        }

        // 2. Get image properties
        size_t height = image.rows;
        size_t width = image.cols;
        size_t channels = image.channels();
        
        // 3. Determine number of threads (e.g., based on CPU cores)
        size_t numThreads = std::thread::hardware_concurrency();
        // Make sure we don't create more threads than rows
        numThreads = std::min(numThreads, height);

        std::cout << "Processing image with dimensions: " << width << "x" << height 
                  << " using " << numThreads << " threads" << std::endl;

        // 4. Create segments
        auto segments = ImageSegmenter::segmentImageByThreads(
            image.data,           // pointer to image data
            height,              // image height
            width,               // image width
            channels,            // number of channels
            numThreads           // number of threads
        );

        // 5. Create and launch threads for processing
        std::vector<std::thread> threads;
        threads.reserve(numThreads);

        for (const auto& segment : segments) {
            // Launch a new thread for each segment
            threads.emplace_back(processSegment, std::ref(segment));
        }

        // 6. Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }

        // 7. Save the processed image (if using OpenCV)
        cv::imwrite("processed_image.jpg", image);

        std::cout << "Image processing completed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}