##
# :title: spriteconfig.rb
# Describes an input and output config some data
require_relative 'texturearray.rb'


##
# :category: Public classes
# An entry of config, describing some sprite template
class ConfigEntry

    public 
    ##
    # _Fixnum_. Index of entry in sprite template group. Can be null (element with 0)
    attr_accessor :index
    ##
    # _Array_ of _Fixnum_ size of sprite on a screen as Array[width, height]. Can be nil in input config
    # But if nil in output time - must be set by +SpriteConfig::prepareForOutput+ to a size of texture
    attr_accessor :size
    ##
    # _String_ source image file name. Can't be nil.
    attr_accessor :inputTextureName
    ##
    # _String_ name of sprite template group, where it belongs.
    attr_accessor :name
    ##
    # _Array_ of _Fixnum_  Mask color for transparency as array [red,green,blue]. Can be nil if not specified
    attr_accessor :transparent
    
    ##
    # _String_ Output texture file name. Must be set by +SpriteConfig::prepareForOutput+ 
    attr_accessor :outputTextureName
    ##
    # _Array_ of _Fixnum_  a position of source image in an output texture as rectangle [x,y,width, height]
    attr_accessor :textureRectangle
    
    ##
    # :category: Public interface
    # Inits as non-valid node with nil properties
    def initialize()
        @index = nil
        @size = nil
        @inputTextureName = nil
        @name = nil
        @transparent = nil
        @outputTextureName = nil
        @textureRectangle = nil
    end
    ##
    # :category: Public interface
    # Determines, whether reading of entry was successfull and it's valid
    # [return]  _TrueClass_ or _FalseClass_ true, if valid
    def isValid()
        if ((@inputTextureName.nil?()) || (@name.nil?()))
            return false
        end
        return true
    end
    
    
    ##
    # :category: Public interface
    # Determines, whether we can output an entry to an XML document
    # [return]  _TrueClass_ or _FalseClass_ true, if can
    def canOutput()
        if ((@outputTextureName.nil?()) || (@textureRectangle.nil?()) || (@size.nil?()))
            return false
        end
        return true
    end
    
    ##
    # :category: Public interface
    # Returns an index of sprite template in sprite template group
    # [return]  _Fixnum_ index of sprite in a template group
    def index()
        if (@index.nil?())
            return 0
        end
        return @index
    end
    ##
    # :category: Public interface
    # Returns a full qualified name of sprite as name:index
    # [return]  _String_ full name
    def getFullName()
        return @name + ":" + index().to_s()
    end    
    ##
    # :category: Implementation. DEPRECATED to use
    # Converts an array of numbers to a semicolon-separated string for output an attributes like +ConfigEntry::size+,
    # +ConfigEntry::textureRectangle+ and +ConfigEntry::transparent+
    # [array] _Array_ of _Fixnum_ an input array
    # [return] _String_  resulting XML string
    def array_to_string(array)
        strings = array.collect{ |item| item.to_s() }
        return strings.join(";")
    end
    ##
    # :category: Public interface
    # Writes an element into XML element. Does not appends self into a root element.
    # [root]   _REXML::Element_ root element of document
    # [return] _REXML::Element_ resulting element of entry to be appended to root element.
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


##
# :category: Public classes
# An input and output config, which can be read and written
class SpriteConfig

    private
    ##
    # _TextureArray_.  Array of source images
    attr_writer  :textureArray
    ##
    # _Array_ of _ConfigEntry_ an array of sprite templates
    attr_writer  :configArray
    ##
    # _Array_ of _String_  an array of errors.
    attr_writer :errors
    
    public
    ##
    # :category: Public interface
    # Checks, whether sprite template has been loaded already. Used to check for unique.
    # [name]   _String_  name of sprite template group
    # [index]  _Fixnum_  index of element in sprite template group
    # [return] _TrueClass_ or _FalseClass_ true, if already loaded
    def hasEntry(name, index)
        return (@configArray.index{ |entry|    (entry.name == name) && (entry.index() == index) } != nil)
    end
    ##
    #  :category: Public interface
    #  Adds a new sprite tempate to a list
    # [entry] _ConfigEntry_ source sprite template
    def pushEntry(entry)
        @configArray = @configArray << entry
    end
    ##
    # :category: Public interface
    # Inits new empty config.
    def initialize()
        @textureArray = TextureArray.new()
        @configArray = Array.new()
    end
    ##
    # :category: Public interface
    # Returns an array of source images
    # [return] _TextureArray_  An array of source images
    def getTextures()
        return @textureArray
    end
    ##
    # :category: Public interface
    # Returns an array of sprite templates
    # [return] _Array_  of _Texture_ An array of sprite templates
    def getConfigArray()
        return @configArray
    end
    ##
    # :category: Public interface
    # Sets an array of source images
    # [textureArray] _TextureArray_  A soure array
    def setTextures(textureArray)
        @textureArray = textureArray
    end
    ##
    # :category: Public interface
    # Sets an array of sprite templates
    # [configArray] _Array_  of _Texture_ An array of sprite templates
    def setConfigArray(configArray)
        @configArray = configArray
    end
    ##
    # :category: Public interface
    # Tried to load a texture, if absent in base, otherwise return true. 
    # If fails, return false. Adds a new error, if loading was failed.
    # [filename] _String_                    name of file with source texture
    # [return]   _TrueClass_ or _FalseClass_ true on success, otherwise false.
    def queryLoadTexture(filename)
        if (@textureArray.getTexture(filename)!=nil)
            return true
        end
        tex = Texture.new(filename)
        if (tex.load()==false)
            @errors = (@errors << ("Can't load texture with name: " + filename))
        else
            @textureArray.pushUnique(tex)
            return true
        end
        return false
    end
    ##
    # :category: Public interface
    # Prepares a config for output to XML documents
    # Copies a +textureRectangle+ from source +Texture+ data, +size+ if not explicitly specified 
    # and sets an +outputTextureName+ from passed string
    # [outputTexName] _String_ filename of output texture.
    def prepareForOutput(outputTexName)
        @configArray.each{
            |entry|
            # Get texture
            texture = @textureArray.getTexture(entry.inputTextureName)
            # Get rectangle
            entry.textureRectangle = texture.textureRectangle
            # Get size if need
            if (entry.size.nil?)
                entry.size = texture.size()
            end
            # Get output texture
            entry.outputTextureName = outputTexName
        }
    end
    ##
    # :category: Public interface
    # Returns an array of errors
    # [return] _Array_ of _String_ array of errors
    def getErrors()
        return @errors
    end
end