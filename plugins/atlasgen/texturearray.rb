# A texture array data, used for displaying and merging textures
require 'cairo'

# A texture, as presented here, consists from name and associated cairo data
class Texture
    private
    
    # DevIL image
    attr_accessor :image

    
    public
    
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
    end
    
    # Returns a surface
    # * return Cairo::Surface a cairo surface
    def getImage()
        if (@image.nil?())
            rise 'Attempt to work with invalid texture'
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
    def pushUnique(texture)
    
    end
    # Defines whether texture array contains some data
    # * param String name
    # * return Boolean whether contains, or not
    def containsTexture(name)
    
    end
end