##
# :title: configwriter.rb
# A source file for writing an output XML writing
require 'rexml/document'
require 'rexml/element'
require 'json'

##
# :category: Public classes
# A class, that writes an output config to file.
# An output config is created from input config by filling some extra +outputTextureName+,+size+ if needed, 
# and +textureRectange+ fields, after it will be written to XML file
class XMLConfigWriter

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
	# [withindex]			_TrueClass_ or _FalseClass_ whether we should write element's index
    # [return]              _TrueClass_ or _FalseClass_ Boolean  whether writing was successfull
    def write(config, filename, outputTextureName, withindex)
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
                el = self.writeElement(entry, root, withindex)
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
	# [withindex] _TrueClass_ or _FalseClass_ whether we should write element's index	
    # [return] _REXML::Element_ resulting element of entry to be appended to root element.
    def writeElement(tmp, root, withindex)
        if (tmp.canOutput() == false)
            raise 'Attempted to write invalid config'
        end
        result = REXML::Element.new(tmp.name)
        if (tmp.index != nil && withindex)
            result.add_attribute( "index", tmp.index.to_s )
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

##
# :category: Public classes
# A class, that writes an output config to file.
# An output config is created from input config by filling some extra +outputTextureName+,+size+ if needed, 
# and +textureRectange+ fields, after it will be written to XML file
class JSONConfigWriter

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
	# [withindex]			_TrueClass_ or _FalseClass_ whether we should write element's index	
    # [return]              _TrueClass_ or _FalseClass_ Boolean  whether writing was successfull
    def write(config, filename, outputTextureName, withindex)
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
            root = {};
			root['resource'] = config.textureResourceName
			
            #Writes all elements
            entries = config.getConfigArray()
			
			resourcetexture = ""
			first = true
			entries.each{
                |entry|
				if first == true || entry.outputTextureName == resourcetexture
					first = false
					resourcetexture = entry.outputTextureName
				else
					@errors = @errors << "Texture atlas file should consist of one texture"
				end
			}
            
			root['file'] = resourcetexture
			root['atlas'] = []
            
            entries.each{
                |entry|
                el = self.writeElement(entry, root, withindex)
                root['atlas'] = root['atlas'].push(el);
            }
            
            # Writes a file
			file.write(JSON.pretty_generate(root))
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
    # [root]   _Array_ root element of document
	# [withindex] _TrueClass_ or _FalseClass_ whether we should write element's index		
    # [return] _Hash_ resulting element of entry to be appended to root element.
    def writeElement(tmp, root, withindex)
        if (tmp.canOutput() == false)
            raise 'Attempted to write invalid config'
        end
        result = {}
        if (tmp.index != nil && withindex)
            result["index"]  = tmp.index.to_s
        end
		result["name"] = tmp.name
        ##
		# Erased: decrease of file size due to
		# terminology change: texture atlas
		# should contain only one texture
		# result["texture"] = tmp.outputTextureName
        result["size"] = tmp.array_to_string(tmp.size)
        result["texrect"] = tmp.array_to_string(tmp.textureRectangle)
        if (tmp.transparent != nil)
           result["transparent"] = tmp.array_to_string(tmp.transparent)
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