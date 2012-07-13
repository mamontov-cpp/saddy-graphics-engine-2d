# A reader, that used to load some config is placed here.
# It loads config from XML and also some textures into cairo contexts

require 'devil'
require 'rexml/document'
require 'rexml/element'

load 'spriteconfig.rb'

# A reader, that reads some config from file and reads it to fully functional config
# Also reads some textures. Each time, config is being read, it kept in there, and old config must
# be saved
class ConfigReader
    private
    
    # Boolean. This field stores, whether some one successfull reading was performed
    attr_writer :computed
    # SpriteConfig.  Inner loaded config
    attr_writer :config
    # Array.   Array of strings
    attr_writer :errors
    # String.  Name of output config 
    attr_writer :outputConfig
    # String.  Name of ouput texture
    attr_writer :outputTexture
    
   public
    
    # Boolean. This field stores, whether some one successfull reading was performed
    attr_reader :computed
    
    # The default reader is created, as broken and every try to read from it leads to exception
    def initialize()
        @computed = false
        @config = nil
        @errors = Array.new()
        @outputConfig = nil
        @outputTexture = nil
    end
    
    # Reads a document from parent ReXML::Element
    # * param ReXML::Element root element
    # * return SpriteConfig resulting sprite config
    def readDocument(root)
        if (root.attributes['texture'] == nil)
            @errors = @errors <<  "Output texture name is not defined"
        else
            @outputTexture = root.attributes['texture']
        end
        
        if (root.attributes['config'] == nil)
            @errors = @errors <<  "Output config is not defined"
        else
            @outputConfig = root.attributes['config']
        end
        result = SpriteConfig.new()
        root.elements.each{
                |element|
                tmp = ConfigEntry.new()
                errors = tmp.read(element,result)
                @errors = @errors + errors
                if (result.hasEntry(tmp.name, tmp.index))
                    @errors = @errors << ("Entry with name " + tmp.getFullName() + " already exists")
                else
                    result.pushEntry(tmp)
                end
        }
        if (@errors.length == 0)
            @computed = true
            return result
        else
            return nil
        end
    end
    # Reads a config from file. Returns config or null on failure
    #  * param String filename
    #  * return SpriteConfig object 
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
    
    # Returns, whether last time reading of config was successfull
    # * return Boolean whether reading was successfull or not.
    def hasSucessfullReading?() 
        return @computed
    end
    
    # Returns read config
    # * return Config read config
    def getConfig()
        if (@computed == false)
            raise 'Error: reading was not successfull'
        end
        return @config
    end
    
    # Returns an output config name
    # * return Sting output config name
    def getOutputConfigName()
        if (@computed == false)
            raise 'Error: reading was not successfull'
        end
        return @outputConfig
    end
    
    # Returns an output texture name
    # * return Sting output texture name
    def getOutputTextureName()
        if (@computed == false)
            raise 'Error: reading was not successfull'
        end
        return @outputTexture
    end
    
    # Returns an array of errors
    # * return Array error array
    def getErrors() 
        return @errors
    end
end