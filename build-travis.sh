set -e
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd plugins/freetype
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd plugins/dukpp-03
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd plugins/irrklang
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd plugins/dukpp-03-irrklang
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd examples/game
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd examples/multithreading
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd examples/stress-physics
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/animations
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/db
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/dukpp-03
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/freetype
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/hfsm
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/input
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/irrklang
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/dukpp-03-irrklang
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/p2d
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/pipeline
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/resource
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/sad
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tests/layouts
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..