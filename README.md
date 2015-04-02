# Valhalla
Valhalla contains a suite of vector algorithms benchmarked across various parallel computing libraries.

## Benchmarked Libraries

- [ArrayFire](https://github.com/arrayfire/arrayfire)
- [BoostCompute](https://github.com/kylelutz/compute)
- [HSA-Bolt](https://github.com/HSA-Libraries/Bolt)
- [Thrust](https://github.com/thrust/thrust)

## Build Requirements

- A CUDA or OpenCL capable device.
- Linux or OSX with a C++11 compiler. Windows currently not tested.
- Prebuilt and installed versions of each library.

### Notes
- `FindBolt.cmake` depends on `BOLT_ROOT` variable to be set. Set `BOLT_ROOT` to the *installed* location of the Bolt library
