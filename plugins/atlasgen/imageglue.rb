# A class, which merges a textures in specified order, changing rect and filling output texture
require 'cairo'

# Merges a textures into one
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