**Copyright (C) 2015 [Wang Renxin](https://cn.linkedin.com/pub/wang-renxin/43/494/20). All rights reserved.**

**framepacker** is a freeware with texture bin packing algorithm. It's similar to texture tools such as TexturePacker, and far beyond, it's totally open source thus you can use, copy, modify, distribute it free.

## Algorithm

I used a bin packer algorithm refer to the [Binary Tree Bin Packing Algorithm](http://codeincomplete.com/posts/2011/5/7/bin_packing/).

## Code with framepacker

**framepacker** is implemented with C++ template, I used a little C++11 standard (Shared Pointer), so it requires a C++11 compatible compiler. The `framepacker::packer` template class in `framepacker.hpp` is where the pack algorithm is; I did a template specializations in `main.cpp` using another open source FreeImage. Guess it'd be a good start to customize your own packer on **framepacker**.

## Usage

I made **framepacker** a command line tool which accepts arguments as follow:

 * framepacker FILE_LIST [-o output_file] [OPTIONS] - Packs some images
 * FILE_LIST
  * := file*
 * OPTIONS
  * := -p n    : Padding, 1 as default
  * := -s mxn  : Expected texture size, eg. 256x512, may enlarge result
  * := -t      : Disable rotation
  * := -w      : Disable forcing texture to POT (Power of 2)
  * := -m      : Disable alpha trim

For example, we may write a command line as: "**framepacker** foo.png bar.png -o out", and it will generate a packed `out.png` image file and another `out.json` meta data file which includes packed frame information.

## Performance

**framepacker** is fast, it packs 200 textures in less than 0.15s on common desktop machines.
