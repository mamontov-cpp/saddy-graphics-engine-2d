##
# :title: configreader.rb
# A source file for reading an input config from file as
# XML document


require 'devil'
require 'rexml/document'
require 'rexml/element'

load 'spriteconfig.rb'

##
# :category: Public classes
# A class, that reads a config and source images and returns some _SpriteConfig_ objects or _nil_,
# if cannot read source file
class ConfigReader
    private
    ##
    # _TrueClass_ or _FalseClass_. True if one successfull reading was performed
    attr_writer :computed
    ##
    # _SpriteConfig_. A read config data (nil if no reading where performed or failed)
    attr_writer :config
    ## 
    # _Array_ of _String_.   Array of strings
    attr_writer :errors
    ##
    # _String_.  Name of file for output config 
    attr_writer :outputConfig
    ##
    # _String_.  Name of file, where ouput texture should be stored
    attr_writer :outputTexture
    
   public
    
    ##
    # _TrueClass_ or _FalseClass_. True if one successfull reading was performed
    attr_reader :computed
    
    ##
    # :category: Public interface
    # The default reader is broken, every attempt to get some config data like +getOutputConfigName()+ or +getOutputTextureName()+ leads to
    # exception
    # A +errors+ field is empty array. +computed+ is false. 
    def initialize()
        @computed = false
        @config = nil
        @errors = Array.new()
        @outputConfig = nil
        @outputTexture = nil
    end
    
    ##
    # :category: Implementation. DEPRECATED for use.
    # Reads a document from parent REXML::Element, filling a errors field on every error
    # Checks for duplicates and source image existence
    # [root]   _REXML::Element_ root element, where reading should be performed from
    # [return] _SpriteConfig_   a resulting sprite config. nil on error.
    def readDocument(root)
        # Get output texture name from attribute 'texture'
        if (root.attributes['texture'] == nil)
            @errors = @errors <<  "Output texture name is not defined"
        else
            @outputTexture = root.attributes['texture']
        end
        # Get output config name from attribute 'config'
        if (root.attributes['config'] == nil)
            @errors = @errors <<  "Output config is not defined"
        else
            @outputConfig = root.attributes['config']
        end
        # Try to read a config, scanning each subelement
        result = SpriteConfig.new()
        root.elements.each{
                |element|
                # Reads a new subelement and add to config
                # Element adds to texturearray source image, from +ConfigEntry::read+ by calling +TextureArray::pushUnique+
                tmp = ConfigEntry.new()
                errors = tmp.read(element,result)
                # Merges array of errors
                @errors = @errors + errors
                # Checks for unique items
                if (result.hasEntry(tmp.name, tmp.index))
                    @errors = @errors << ("Entry with name " + tmp.getFullName() + " already exists")
                else
                    result.pushEntry(tmp)
                end
        }
        # Check, whether errors were found
        if (@errors.length == 0)
            @computed = true
            return result
        else
            return nil
        end
    end
    ##
    # :category: Public interface
    # Reads a document from file, filling array of errors
    # Checks for duplicates and source image existence
    # [filename] _String_         name of file, where reading will be performed
    # [return]   _SpriteConfig_   a resulting sprite config. nil on error.
    def read(filename) 
        @computed = false
        @errors = []
        
        defaultMesg = [ "File error: Cannot open file: " + filename ]
        parseMesg = [ "Parsing error: Cannot read file: " + filename ]
        begin
            file = File.new(filename, "r")
            doc = REXML::Document.new file
            if (doc.root!=nil)
                return self.readDocument(doc.root)
            else
                 @errors = parseMesg
            end
        rescue Errno::ENOENT
            @errors = defaultMesg
        rescue Errno::EBADF
            @errors = defaultMesg
        rescue Errno::EINVAL
            @errors = defaultMesg
        rescue REXML::ParseException
            @errors = parseMesg
        end
        
        return nil
    end
    
    ##
    # :category: Public interface
    # Returns, whether last call of +read()+ was successfull
    # [return]  _TrueClass_ or _FalseClass_  whether reading was successfull
    def hasSucessfullReading?() 
        return @computed
    end
    
    ##
    # :category: Public interface
    # Returns last read config. Rises exception if reading was not successfull
    # [return]  _SpriteConfig_  last read config. 
    def getConfig()
        if (@computed == false)
            raise 'Error: reading was not successfull'
        end
        return @config
    end
    
    ##
    # :category: Public interface
    # Returns an output config filename
    # [return]  _String_  filename for output config.  
    def getOutputConfigName()
        if (@computed == false)
            raise 'Error: reading was not successfull'
        end
        return @outputConfig
    end
    
    ##
    # :category: Public interface
    # Returns an output texture filename
    # [return]  _String_  filename for output texture.  
    def getOutputTextureName()
        if (@computed == false)
            raise 'Error: reading was not successfull'
        end
        return @outputTexture
    end
    
    ##
    # :category: Public interface
    # Returns an array for errors
    # [return]  _Array_ of _String_  A error list.  
    def getErrors() 
        return @errors
    end
end