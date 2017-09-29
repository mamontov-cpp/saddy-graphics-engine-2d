cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd plugins/freetype
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd plugins/dukpp-03
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd plugins/irrklang
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd plugins/dukpp-03-irrklang
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd examples/game
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd examples/multithreading
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd examples/stress-physics
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/animations
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/db
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/dukpp-03
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/freetype
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/hfsm
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/input
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/irrklang
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/dukpp-03-irrklang
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/p2d
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/pipeline
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/sad
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..
cd tests/layouts
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make
cd ../..