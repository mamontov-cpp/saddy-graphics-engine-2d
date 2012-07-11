# A class, which merges a texture into output texture
require 'devil'

# Creates a POT texture with specified size and copies specified texture entries 
# into it, returning some result
class ImageGlue

	# Copies a source texture into a rectangle described in textureRectangle property (Array of [x,y,w,h]) of destTexture
	# * param Texture      srcTexture  a source texture entry (see texturearray.rb) for description
	# * param DevIL::Image destTexture destination texture, where it should be copied.
	def copy(srcTexture, destTexture)
        rect = srcTexture.textureRectangle
        x = rect[0]
        y = rect[1]
        destTexture.blit(srcTexture.getImage, x, y)
	end
    # Creates a POT texture and blits all textures to it
    # * param Fixnum widthheight width and height of texture (width==height, so we need only one parameter) 
    # * param Array images array of Texture object, which should be blitted 
    # * return DevIL::Image resulting image
    def glue(widthheight, images)
        
    end
end