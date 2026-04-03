# Image Processing

This repository contains a set of practical labs focused on fundamental concepts in image processing. The practices explore how images can be manipulated, reconstructed, and enhanced using different techniques such as frequency filtering, image alignment, and restoration algorithms. The goal is to understand how images are processed both in the spatial and frequency domains, and how these methods can be applied to real problems.

===========================================================================

## Practicum 1: Hybrid Images
===========================================================================

The main topics are:

- Image filtering (low-pass and high-pass)
- Image fusion
- Spatial and frequency domain processing

In this practicum, hybrid images are created by combining two images with different frequency components. One image is filtered to keep low frequencies (visible from far away), while the other keeps high frequencies (visible up close).  

The first part focuses on building hybrid images in the spatial domain using Gaussian filters and convolution. The second part applies the same concept in the frequency domain using the Fourier Transform. 

===========================================================================

## Practicum 2: Russian Empire Images
===========================================================================

The main topics are:

- RGB image reconstruction
- Image alignment using cross-correlation
- Multi-scale (pyramidal) processing

This practicum reconstructs color images from historical photographs composed of three separate channels (R, G, B). The main task is to split the channels, align them correctly, and combine them into a final RGB image.  

Alignment is first implemented using block matching and cross-correlation, and later improved using a pyramidal approach to make the process faster and more efficient. Image enhancement techniques such as contrast and color adjustment are also applied. 

===========================================================================

## Practicum 3: Inpainting
===========================================================================

The main topics are:

- Image restoration
- Patch-based methods
- Similarity search (SSD)

This practicum focuses on restoring damaged images using an inpainting algorithm. Given an image with missing regions (mask), the goal is to fill those areas using information from the surrounding pixels.  

The method searches for similar patches in the image using SSD (Sum of Squared Differences) and fills the missing regions progressively using an "onion ring" strategy. Also, more advanced approaches such as gradient-based filling can be implemented for better results. 