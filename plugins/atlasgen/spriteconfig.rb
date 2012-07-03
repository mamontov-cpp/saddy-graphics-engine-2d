# An input config as described in input file
require 'cairo'

load 'texturearray.rb'

# An entry for config array
class ConfigEntry

    public 
    
    # int Index of array, can be null if tag is omitted 
    attr_accessor :index
    # Array size of sprite can be null at input, but must be set with value, when output time is reached
    attr_accessor :size
    # String string texture name . Can't be null
    attr_accessor :inputTextureName
    # String name of config entry
    attr_accessor :name
    # Array list of, can be transparent entry. Set to null if omitted
    attr_accessor :transparent
    
    # String Output texture name, must be set when output
    attr_accessor :outputTextureName
    # Array  texture rectangle data
    attr_accessor :textureRectangle
    
    # Inits as non-valid node
    def initialize()
        @index = Integer.nil
        @size = Array.nil
        @inputTextureName = String.nil
        @name = String.nil
        @transparent = Array.nil
        @outputTextureName = Array.nil
        @textureRectangle = Array.nil
    end
    
    # Determines, whether reading of entry was successfull and it's valid
    # * return Boolean true if valid
    def isValid()
        if ((@inputTextureName.nil?()) || (@name.nil?()))
            return false
        end
        return true
    end
    
    # Determines, whether we can output some entry to an xml node
    # * return Boolean true if valid
    def canOutput()
        if ((@outputTextureName.nil?()) || (@textureRectangle.nil?()) || (@size.nil?()))
            return false
        end
        return true
    end
    
    # Reads an element to entry
    # * param element XMLElement
    def read(element)
        raise 'Not implemented'
    end
    
    # Writes an element to config
    # * return XMLElement
    def write()
        if (!canOutput())
            raise 'Attempted to write invalid config'
        end
    end
end


# An input config, as described in file, that user inputs to program
# Also contains input textures.
class SpriteConfig

    private
    
    # TextureArray array of textures, that will be merged
    attr_writer  :textureArray
    # Array array of config entries, that must be fixed and replaced in output config
    attr_writer  :configArray
     # Array array of errors
    attr_writer :errors
    
    public
    
    
    # Inits new empty config
    def initialize()
        @textureArray = TextureArray.new()
        @configArray = Array.new()
    end
    
    # Returns a texture array
    # * return TextureArray. An array of textures entry
    def getTextures()
        return @textureArray
    end
    # Returns a config array
    # * return Array. A config entries array
    def getConfigArray()
        return @configArray
    end
    # Sets a texture array
    # * param textureArray TextureArray. An array of textures entry
    def setTextures(textureArray)
        @textureArray = textureArray
    end
    # Sets a config array
    # * param configArray Array. A config entries array
    def setConfigArray(configArray)
        @configArray = configArray
    end
    # Tried to load a texture, if absent in base, otherwise return true. If fails, return false. Adds a new error, if has some
    # * param filename name of texture
    # * return true on success, otherwise false.
    def queryLoadTexture(filename)
    end
    
    # Returns a list of errors
    # * return Array list of errors
    def getErrors()
    end
end