# An input config as described in input file
require 'cairo'

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
    
    # Array array of errors
    attr_reader :errors
    
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
end