##
# :title: texturearray.rb
# Describes an array of textures with handling of unique names
require 'devil'

##
# :category: Public classes
# A source image texture with name of source file stored and rectangle in output texture
class Texture
    private
    
    ##
    # _DevIL::Image_. a source image surface, which can be copied
    attr_accessor :image

    
    public
    ##
    # _Array_ of _Fixnum_.  Position of texture in output data as [x,y,width,height]
    attr_accessor :textureRectangle
    ##
    # _String_. Name of source file for texture
    attr_accessor :name
    ##
    # :category: Public interface
    # Initializes texture with some soure filename
    # [name] _String_ source file name of image
    def initialize(name)
        @name = name
    end
    ##
    # :category: Public interface
    # Tries to load some texture from file specified by +Texture::name+
    # [return] _TrueClass_ or _FalseClass_ true on success, false otherwise
    def load()
        res = true
        # Sometimes, library rise some strange errors, so we capture them.
        begin       
            @image = Devil.load_image(@name)
            if (@image.nil?())
                res = false
            end
        rescue RuntimeError
            res = false
        end
        return res
    end
    ##
    # :category: Public interface
    # Returns a source image from texture
    # [return] _DevIL::Image_ an image of texture
    def getImage()
        if (@image.nil?())
            raise 'Attempt to work with invalid texture'
        end
        return @image
    end
    
    ##
    # :category: Public interface
    # Returns size of texture
    # [return] _Array_ of _FixNum_ size of texture as [width,height]
    def size()
        return [ image.width(), image.height() ]
    end
end


##
# :category: Public classes
# A an array of textures, which handles unique names for textures
class TextureArray < Array
    ##
    # :category: Public interface
    # Pushes unique image with specified name
    # [texture] _Texture_ source image
    def pushUnique(texture)
        if (texture.nil?)
            rise 'Invalid texture'
        end
        if (self.containsTexture(texture.name) == false)
            self.concat( [texture] )
        end
    end
    ##
    # :category: Public interface
    # Detects whether texture array contains some texture with specified image file name
    # [name]   _String_                     source image filename
    # [return] _TrueClass_ or _FalseClass_  true. if found, otherwise null.
    def containsTexture(name)
        return  ( (self.index{ |tex| tex.name==name } ) != nil )
    end
    ##
    # :category: Public interface
    # Returns a texture object by it's name
    # [name]   _String_     source image filename
    # [return] _Texture_    source image or nilm if can't be found
    def getTexture(name)
        arr = self.reject{ |tex| tex.name!=name }
        if (arr.length ==0 )
            return nil
        else
            return arr[0]
        end
    end
end