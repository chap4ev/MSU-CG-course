## Basic implementation:
move ./tmp/GP_ONE.cpp to ./Source/ to run

Debug:

Total time = 0.779934 seconds

Release:

scene_0 : PASSED : 0.014437 seconds \
scene_1 : PASSED : 0.014468 seconds \
scene_2 : PASSED : 0.040457 seconds \
scene_3 : PASSED : 0.10804 seconds \
scene_4 : PASSED : 0.028381 seconds \
Total time = 0.205783 seconds

## SIMD implementation:

./Source/GP_ONE.cpp

Debug:

Total time = 0.499722 seconds

Release:

scene_0 : PASSED : 0.003738 seconds \
scene_1 : PASSED : 0.004362 seconds \
scene_2 : PASSED : 0.007752 seconds \
scene_3 : PASSED : 0.019385 seconds \
scene_4 : PASSED : 0.00624 seconds \
Total time = 0.041477 seconds

up to 5.5x performance boost!

testing on \
intel core i7 6700 \
32 gb 2133hz

mkdir build \
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release \
cmake --build build