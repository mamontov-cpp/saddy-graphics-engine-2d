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
        @index = nil
        @size = nil
        @inputTextureName = nil
        @name = nil
        @transparent = nil
        @outputTextureName = nil
        @textureRectangle = nil
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
    
    
    def index()
        if (@index.nil?())
            return 0
        end
        return @index
    end
    
    def getFullName()
        return @name + ":" + index().to_s()
    end
    # Reads an element to entry
    # * param element XMLElement
    # * param textures SpriteConfig, where could be entries added
    # * returns array of errors, empty if nothing found
    def read(element,config)
       errors = []
       # Parse name and index
       @name = element.name
       if (element.attributes['index'] != nil)
            @index = element.attributes['index'].to_i
       end
       # Parse texture
       if (element.attributes['texture'] != nil)
            @inputTextureName = element.attributes['texture'] 
            if (config.getTextures().containsTexture(@inputTextureName) == false)
                texture = Texture.new(@inputTextureName)
                # If texture is loaded successfully
                if (texture.load())
                    config.getTextures().pushUnique(@inputTextureName, texture)
                else
                    errors = errors << ("Can't load texture with name " + @inputTextureName)
                end
            end
       else
            errors = errors << ("At element with name " + self.getFullName() + " texture is not specified" )
       end
       # Parse size
       if (element.attributes['size'] != nil)
            list = element.attributes['size'].split(';')
            if (list.length!=2)
                errors = errors << ("At element with name " + self.getFullName() + " size must be defined as \"width;height\", but defined strangely")
            else
                @size = [ list[0].to_i(), list[1].to_i() ]
            end
       end
       # Parse transparency
       if (element.attributes['transparent'] != nil)
            list = element.attributes['transparent'].split(';')
            if (list.length!=3)
                errors = errors << ("At element with name " + self.getFullName() + " transparency mask color must be defined as \"r;g;b\", but defined strangely")
            else
                @transparent = [ list[0].to_i(), list[1].to_i(), list[2].to_i() ]
            end
       end
       return errors
    end
    # Converts integer array to a valid XML config string
    def array_to_string(array)
        strings = array.collect{ |item| item.to_s() }
        return strings.join(";")
    end
    # Writes an element to config
    # * param root root element
    # * return XMLElement
    def write(root)
        if (self.canOutput() == false)
            raise 'Attempted to write invalid config'
        end
        result = REXML::Element.new(@name)
        if (@index != nil)
            result.add_attribute( "index", @index.to_s )
        end
        result.add_attribute( "texture", @outputTextureName )
        result.add_attribute( "size", self.array_to_string(@size) )
        result.add_attribute( "texrect", self.array_to_string(@textureRectangle) )
        if (@transparent != nil)
            result.add_attribute( "transparent", self.array_to_string(@transparent) )
        end
        return result
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
    
    def hasEntry(name, index)
        return (@configArray.index{ |entry|    (entry.name == name) && (entry.index == index) } != nil)
    end
    
    def pushEntry(entry)
        @configArray = @configArray << entry
    end
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
        if (@textureArray.getTexture(filename)!=nil)
            return true
        end
        tex = texture.new()
        if (tex.load()==false)
            @errors = (@errors << ("Can't load texture with name: " + filename))
        else
            @textureArray.pushUnique(filename, tex)
            return true
        end
        return false
    end
    # Copies a texture rectangle from part data, size if not set and sets an output texture name from string
    def prepareForOutput(outputTexName)
        @configArray.each{
            |entry|
            texture = @textureArray.getTexture(entry.inputTextureName)
            entry.textureRectangle = texture.textureRectangle
            if (entry.size.nil?)
                entry.size = texture.size()
            end
            entry.outputTextureName = outputTexName
        }
    end
    # Returns a list of errors
    # * return Array list of errors
    def getErrors()
        return @errors
    end
end