# A class, which merges a texture into output texture
require 'devil'

# Creates a POT texture with specified size and copies specified texture entries 
# into it, returning some result
class ImageGlue

	# Copies a source texture into a rectangle described in textureRectangle property (Array of [x,y,w,h]) of destTexture
	# * param Texture      srcTexture  a source texture entry (see texturearray.rb) for description
	# * param DevIL::Image destTexture destination texture, where it should be copied.
	def copy(srcTexture, destTexture)
	end
    # Merges all images into one
    # * param images Array of Texture
    # * param GlueOrder array of GlueOrder
    # * param output output data
    # * param Config  config, which will be updated for writing
    # * return Array (surface, context)
    def glue(images, order, output, config)
    end
end