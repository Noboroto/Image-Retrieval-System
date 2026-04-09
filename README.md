# Image Searching System

Content-based image retrieval (CBIR) system using OpenCV. Extracts visual features (histogram and SIFT) from image datasets, then searches for similar images by comparing feature vectors.

## Prerequisites

- **CMake** >= 3.10
- **C++20** compiler (MSVC 2022 recommended)
- **OpenCV** 4.x installed at `C:/opencv` (or update `OpenCV_DIR` in `Source/CMakeLists.txt`)

## Project Structure

```
Source/
  CMakeLists.txt          # Root CMake configuration
  main.cpp                # Entry point
  Libs/
    CMakeLists.txt         # Shared library (DLib) configuration
    includes/
      CmdFactory.hpp       # Command factory (singleton, CLI router)
      Command.hpp          # Command abstraction
      Histogram.hpp        # Histogram feature extraction
      SIFT.hpp             # SIFT feature extraction
      SaveBinaryCV.hpp     # Binary serialization for cv::Mat/KeyPoints
      lib_export.h         # DLL export macros
    src/
      CmdFactory.cpp
      Command.cpp
      Histogram.cpp
      SIFT.cpp
      SaveBinaryCV.cpp
Dataset/
  CD/
    training_images/       # Training image set (99 images)
    TestImages/            # Query image set (10 images)
```

## Build

```bash
cd Source
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The executable and DLL are placed in `Source/executable/` after build.

## Usage

All commands use `--input_path` and `--output_path` as named parameters.

### Show Help

```bash
21127469.exe --help
```

### Extract Features

Extract histogram and/or SIFT features from an image folder:

```bash
# Extract all features (histogram + SIFT)
21127469.exe --extract --input_path=Dataset/CD/training_images --output_path=output

# Extract histogram only
21127469.exe --extract --input_path=Dataset/CD/training_images --output_path=output --mode=histogram

# Extract SIFT only
21127469.exe --extract --input_path=Dataset/CD/training_images --output_path=output --mode=sift
```

Output files:
- `output_histograms.bin` / `output_histograms.yaml` - Histogram features + metadata
- `output_SIFT.bin` / `output_SIFT.yaml` - SIFT descriptors + metadata

### Search by Histogram

Find similar images by comparing histogram features:

```bash
# 1. Extract training set histograms
21127469.exe --extract --input_path=Dataset/CD/training_images --output_path=db --mode=histogram

# 2. Extract query image histogram (put single image in a folder)
21127469.exe --extract --input_path=query_folder --output_path=query --mode=histogram

# 3. Search
21127469.exe --find_by_hist --input_path=db_histograms.bin --histogram=query_histograms.bin --metadata_hist=db_histograms.yaml
```

### Visualize Features (PCA)

Visualize feature vectors projected to 2D via PCA:

```bash
21127469.exe --visualize --input_path=output_histograms.bin
```

### K-Means Clustering

Cluster feature vectors using K-Means:

```bash
21127469.exe --kmean --input_path=output_SIFT.bin --clusters=256
```

## Feature Extraction Methods

| Method | Description | Output |
|--------|-------------|--------|
| **Histogram** | Grayscale intensity histogram (256 bins), min-max normalized | Nx256 matrix |
| **SIFT** | Scale-Invariant Feature Transform descriptors | Mx128 matrix |

## License

See `Source/License.txt`.
