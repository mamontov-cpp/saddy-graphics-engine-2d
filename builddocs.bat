cd examples/multithreading
doxygen
cd ../..
cd examples/game
doxygen
cd ../..
rdoc --force-output --all --output=docs/atlasgen/ "--title=Atlas Generator" --main=plugins/atlasgen/main.rb plugins/atlasgen/main.rb plugins/atlasgen/configreader.rb plugins/atlasgen/configwriter.rb plugins/atlasgen/imageglue.rb plugins/atlasgen/imagegluingorder.rb  plugins/atlasgen/imagewriter.rb  plugins/atlasgen/spriteconfig.rb plugins/atlasgen/texturearray.rb plugins/atlasgen/imagearranger.rb