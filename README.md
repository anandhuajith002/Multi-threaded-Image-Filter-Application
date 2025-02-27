# Multi-Threaded SIMD Image Processing

A high-performance C++ application for image processing utilizing multi-threading and SIMD (Single Instruction, Multiple Data) instructions to achieve optimal performance on modern CPUs.

## Features

- **SIMD-Accelerated Image Processing**: Uses SSE/SSSE3 instructions to process multiple pixels simultaneously
- **Multi-Threading Support**: Divides image processing workload across available CPU cores
- **Automatic CPU Feature Detection**: Falls back to scalar implementation when SIMD features aren't available
- **RGB to Grayscale Conversion**: Efficiently converts color images to grayscale

## Requirements

- C++11 compatible compiler
- CPU with SSSE3 support for SIMD acceleration
- Windows platform (currently uses Windows-specific `__cpuid` intrinsic)

## Project Structure

- **main.cpp**: Application entry point and orchestration
- **image.hpp**: Image loading, manipulation and saving using stb_image
- **imagesegment.hpp**: Handles image division into segments for parallel processing
- **filtersegment.hpp**: Contains SIMD-optimized and fallback image filters
- **types.hpp**: Common type definitions and CPU feature detection

## How It Works

1. **Image Loading**: Loads an image using the STB image library
2. **Segmentation**: Divides the image into equal segments based on available CPU cores
3. **Parallel Processing**: Assigns each segment to a separate thread
4. **SIMD Processing**: Each thread applies SIMD-optimized filters when supported
5. **Reconstruction**: Combines processed segments back into a single image
6. **Output**: Saves the processed image to disk

## SIMD Implementation Details

The RGB to grayscale conversion uses SSSE3 SIMD instructions to process 4 pixels at once:

1. Uses `_mm_shuffle_epi8` to extract R, G, B channels from interleaved pixel data
2. Widens 8-bit color values to 16-bit using `_mm_unpacklo_epi8`
3. Applies color coefficients with `_mm_mullo_epi16`
4. Sums channel contributions and applies appropriate rounding
5. Packs results back to 8-bit with saturation using `_mm_packus_epi16`

## Building the Project

```bash
# Clone the repository
git clone https://github.com/yourusername/simd-image-processing.git
cd simd-image-processing

# Build with CMake
mkdir build
cd build
cmake ..
cmake --build .

# Run the application
./image_processor
```

## Performance

The SIMD implementation provides significant speedup compared to the scalar version:
- Processing 4 pixels in parallel with SSE instructions
- Minimizing branching in inner loops
- Leveraging the CPU's vector processing units
- Reducing memory access overhead

## Future Improvements

- Add AVX2/AVX-512 support for processing 8/16 pixels at once
- Implement more image filters (blur, sharpen, edge detection)
- Add cross-platform support for CPU feature detection
- Improve memory management with aligned allocations

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [stb_image](https://github.com/nothings/stb) for image loading/saving