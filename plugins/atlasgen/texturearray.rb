# A texture array data, used for displaying and merging textures
require 'cairo'

# A texture, as presented here, consists from name and associated cairo data
class Texture
    private
    
    # Cairo::Surface a cairo surface
    attr_accessor :surface
    # Cairo::Context a cairo context
    attr_accessor :context
    
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
    def getSurface()
        if (@surface.nil?())
            rise 'Attempt to work with invalid texture'
        end
        return @surface
    end
    
    # Returns a context
    # * return Cairo::Surface a cairo surface
    def getContext()
        if (@context.nil?())
            rise 'Attempt to work with invalid texture'
        end
        return @context
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