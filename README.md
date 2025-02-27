# Multi-Threaded Image Filter Application

A high-performance C++ application that applies image filters using multi-threading to optimize processing speed. The application currently supports grayscale conversion with an architecture designed to easily accommodate additional filters.

## Features

- Multi-threaded image processing for optimal performance
- Automatic thread count optimization based on hardware
- Image segmentation for parallel processing
- Currently supports grayscale filter
- Extensible architecture for adding new filters

## Prerequisites

- C++ compiler with C++11 support or higher
- STB Image library
- CMake (recommended for building)

## Project Structure

```
.
├── include/
│   ├── stb_image.h
│   └── stb_image_write.h
├── src/
│   ├── image.hpp
│   ├── filters.hpp
│   ├── imagesegment.hpp
│   ├── types.hpp
│   ├── filtersegment.hpp
│   └── main.cpp
├── output_img/
│   └── image.jpg
├── CMakeLists.txt
└── README.md
```

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/image-filter-app.git
cd image-filter-app
```

2. Build the project:
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the application from the command line by providing an input image path:

```bash
./image_filter input_image.jpg
```

The processed image will be saved as `output_img/image.jpg`.

## How It Works

1. **Image Loading**: The application loads the input image using the STB Image library.

2. **Thread Initialization**: 
   - Automatically determines optimal thread count based on hardware
   - Creates a thread pool for parallel processing

3. **Image Segmentation**:
   - Divides the input image into segments
   - Each segment is processed independently

4. **Parallel Processing**:
   - Assigns image segments to available threads
   - Currently applies grayscale conversion
   - Architecture supports adding more filters

5. **Image Reconstruction**:
   - Processed segments are combined into the final image
   - Result is saved to the output directory

## Implementation Details

### Key Components

- `Image`: Handles image loading, saving, and memory management
- `ImageSegment`: Manages image segmentation and reconstruction
- `FilterSegment`: Applies filters to image segments
- `filters`: Contains filter implementations
- `types`: Defines custom types used across the application

### Multi-threading

The application uses C++11 threads for parallel processing:
- Thread count is optimized based on available CPU cores (`hardware_concurrency()-1`)
- Each thread processes an independent image segment
- Thread synchronization is handled through `join()`

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Future Enhancements

- Additional filter types
- Custom thread pool implementation
- GPU acceleration support
- Real-time image processing
- Batch processing support
- Filter chaining
- Command-line arguments for filter selection

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- STB Image library for image processing
- C++ Standard Library for threading support

## Author

Anandhu Ajith