# A texture array data, used for displaying and merging textures
require 'cairo'
require 'devil'

# A texture, as presented here, consists from name and associated cairo data
class Texture
    private
    
    # DevIL image
    attr_accessor :image

    
    public
    # Array  texture rectangle data
    attr_accessor :textureRectangle
    # String name a name of texture
    attr_accessor :name
    
    # Initializes texture with some name
    # * String name of textures
    def initialize(name)
        @name = name
    end
    
    # Tries to load some texture from file
    # * return Boolean true on success, false otherwise
    def load()
        res = true
        @image = Devil.load_image(@name)
        if (@image.nil?())
            res = false
        end
        
        return res
    end
    
    # Returns an image of texture
    # * return DevIL::Image an image of texture
    def getImage()
        if (@image.nil?())
            raise 'Attempt to work with invalid texture'
        end
        return @image
    end
    
    
    # Size of texture returns
    # * return Array size of texture
    def size()
    
    end
end


# An array of textures
class TextureArray < Array

    # Pushes unique texture
    # * param texture texture
    # * return unique identifier of texture
    def pushUnique(name,texture)
    
    end
    # Defines whether texture array contains some data
    # * param String name
    # * return Boolean whether contains, or not
    def containsTexture(name)
    
    end
    
    # Returns a texture by it's name
    # * param String name
    # * return Texture
    def getTexture(name)
    
    end
end