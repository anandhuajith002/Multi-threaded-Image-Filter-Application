# Multi-threaded Image Filter Application

## Project Overview
A high-performance image processing application that applies various filters to images using multi-threading for enhanced processing speed. The project implements custom image processing algorithms and filter operations without relying on external libraries (except for basic image I/O operations).

## Features
- Custom implementation of image processing algorithms
- Multi-threaded filter application for improved performance
- Image segmentation for parallel processing
- Support for various image filters:
  - Grayscale conversion
  - *(Planned)* Gaussian blur
  - *(Planned)* Image sharpening
  - *(Planned)* Other custom filters

## Project Structure
```
├── include/
│   ├── stb_image.h         # STB library for image I/O
│   └── stb_image_write.h
├── headers/
│   ├── image.hpp           # Image class definition
│   ├── filters.hpp         # Image filter implementations
│   ├── imagesegment.hpp    # Image segmentation for threading
│   └── thread.hpp          # Thread management
├── src/
│   └── main.cpp            # Main application entry point
├── output_img/             # Directory for processed images
└── README.md
```

## Technical Implementation
- **Image Class**: Handles image loading and basic operations
- **Filters Class**: Implements various image processing algorithms
- **ImageSegment Class**: Manages image segmentation for parallel processing
- **Thread Management**: Utilizes C++11 threads for parallel execution

## Dependencies
- C++11 or higher
- STB Image library (included in project)
- *(Future)* OpenCV (will be used only for image I/O and specific matrix operations)

## Building the Project
```bash
# Clone the repository
git clone [repository-url]

# Create build directory
mkdir build && cd build

# Build the project
cmake ..
make
```

## Usage
```bash
./image_filter <input_image_path>
```

Example:
```bash
./image_filter input.jpg
```

The processed image will be saved in the `output_img` directory.

## Current Implementation Details

### Image Processing
- Custom implementation of basic image operations
- Direct pixel manipulation without external libraries
- Support for various image formats through STB library

### Threading Model
- Dynamic thread allocation based on hardware concurrency
- Image segmentation for parallel processing
- Thread-safe implementation for filter applications

### Memory Management
- Efficient memory handling for large images
- Proper cleanup of resources
- Memory-mapped operations for better performance

## Planned Features
1. Additional filter implementations:
   - Gaussian Blur
   - Sharpening Filter
   - Edge Detection
   - Custom Convolution Filters
2. SIMD optimization for performance
3. OpenCV integration for specific operations
4. Performance benchmarking tools
5. Command-line interface for filter selection

## Contributing
Feel free to contribute to this project by:
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License
[Your chosen license]

## Acknowledgments
- STB Image library for basic image I/O operations
