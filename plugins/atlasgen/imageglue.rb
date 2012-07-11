# A class, which merges a texture into output texture
require 'cairo'

# Creates a POT texture with specified size and copies specified texture entries 
# into it
class ImageGlue
    # Merges all images into one
    # * param images Array of Texture
    # * param GlueOrder array of GlueOrder
    # * param output output data
    # * param Config  config, which will be updated for writing
    # * return Array (surface, context)
    def glue(images, order, output, config)
    end
end