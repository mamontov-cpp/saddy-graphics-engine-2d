##
# :title: configwriter.rb
# A source file for writing an output XML writing
require 'rexml/document'
require 'rexml/element'

##
# :category: Public classes
# A class, that writes an output config to file.
# An output config is created from input config by filling some extra +outputTextureName+,+size+ if needed, 
# and +textureRectange+ fields, after it will be written to XML file
class ConfigWriter

    ##
    # :category: Public interface
    # Initializes a config with no errors
    def initialize()
        @errors = []
    end
    ##
    # :category: Public interface
    # Writes a config as XML document into file, creating an output file
    # [config]              _SpriteConfig_ config to be written
    # [filename]            _String_       name of output filename
    # [outputTextureName]   _String_       filename of output texture, used to produce output config.
    # [return]              _TrueClass_ or _FalseClass_ Boolean  whether writing was successfull
    def write(config, filename, outputTextureName)
        # Prepare config for output
        config.prepareForOutput(outputTextureName)
        #Try open file
        begin
            file=File.new(filename,"w")
        rescue Errno::ENOENT 
            file=nil
        rescue Errno::EACCES 
            file=nil
        rescue Errno::EINVAL 
            file=nil
        end
        
        # If file opening was succesfull
        if (file.nil?() == false)
            # Create a document and root elements 
            doc=REXML::Document.new
            root=REXML::Element.new("sprites")
            doc.add( root )
            
            #Writes all elements
            entries = config.getConfigArray()
            entries.each{
                |entry|
                el = self.writeElement(entry, root)
                root.add_element(el)
            }
            
            # Writes a file
            formatter=REXML::Formatters::Pretty.new
            formatter.write(doc,file)
            file.close()       
            # Successfull reading
            return true
        else
            # Otherwise write error
            @errors = [ "Can't save config to a file \"" + filename + "\""]
        end
        # If we get error, definiitely an error
        return false
    end
	
	##
    # :category: Public interface
    # Writes an element into XML element. Does not appends self into a root element.
	# [tmp]    _ConfigEntry_    a config entry for interface.
    # [root]   _REXML::Element_ root element of document
    # [return] _REXML::Element_ resulting element of entry to be appended to root element.
    def writeElement(tmp, root)
        if (tmp.canOutput() == false)
            raise 'Attempted to write invalid config'
        end
        result = REXML::Element.new(@name)
        if (tmp.index != nil)
            result.add_attribute( "index", @index.to_s )
        end
        result.add_attribute( "texture", tmp.outputTextureName )
        result.add_attribute( "size", tmp.array_to_string(tmp.size) )
        result.add_attribute( "texrect", tmp.array_to_string(tmp.textureRectangle) )
        if (tmp.transparent != nil)
            result.add_attribute( "transparent", tmp.array_to_string(tmp.transparent) )
        end
        return result
    end
    ##
    # :category: Public interface
    # Returns an array for errors
    # [return]  _Array_ of _String_.  A error list.  
    def getErrors()
        return @errors
    end
end
