set -e
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd plugins/freetype
(cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 && make) || (exit 1)
cd ../..
cd tools/ifaceed
cd ../..