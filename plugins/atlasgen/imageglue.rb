##
# :title: imageglue.rb
# A class, which merges a source images into by copying them
require 'devil'

##
# :category: Public classes
# Creates a POT texture with specified size and copies specified texture entries 
# into it, returning some new +DevIL::image+ to be written
class ImageGlue

    ##
    # :category: Implementation. DEPRECATED for use.
	# Copies a source texture into a rectangle described in textureRectangle property (Array of [x,y,w,h]) of srcTexture
	# [srcTexture]  _Texture_        a source texture, which should be copied
	# [destTexture] _DevIL::Image_   destination texture, where it should be copied.
	def copy(srcTexture, destTexture)
        rect = srcTexture.textureRectangle
        x = rect[0]
        y = rect[1]
		IL.Disable(1590); # Disable IL_BLIT_BLEND - disable blending alpha-channel when blitting
        destTexture.blit(srcTexture.getImage, x, y)
        return destTexture
	end
    ##
    # :category: Public interface
    # Creates a POT texture and blits all textures to it
    # [widthheight] _Fixnum_            width and height of texture (width==height, so we need only one parameter) 
    # [images]     _Array_ of _Texture_ images array of Texture object, which should be blitted 
    # [return]     _DevIL::Image_       resulting image
    def glue(widthheight, images)
        res = Devil.create_image(widthheight, widthheight, {:color => [255, 255, 255, 0]})
        # Handle implementation exception
        if (res.nil?())
            raise 'Image not created'
        end
        # Copy images to destination
        images.each do |tex|
            res = self.copy(tex, res)
        end
        return res
    end
end