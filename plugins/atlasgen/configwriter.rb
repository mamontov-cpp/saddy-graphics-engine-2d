# Writes a new config into file
require 'rexml/document'
require 'rexml/element'

# Writes a new config into file
class ConfigWriter

    def initialize()
        @errors = []
    end
    # Writes an config into file
    # * param config   Config output config data
    # * param filename String name of output file
    # * param outputTextureName String output texture name
    # * return Boolean  whether writing was successfull
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
        
        if (file.nil?() == false)
            # Create a document and root elements 
            doc=REXML::Document.new
            root=REXML::Element.new("sprites")
            doc.add( root )
            
            #Writes all elements
            entries = config.getConfigArray()
            entries.each{
                |entry|
                el = entry.write(root)
                root.add_element(el)
            }
            
            #Writes a file
            formatter=REXML::Formatters::Pretty.new
            formatter.write(doc,file)
            file.close()       
            return true
        else
            @errors = [ "Can't save config to a file \"" + filename + "\""]
        end
        return false
    end
    
    def getErrors()
        return @errors
    end
end
