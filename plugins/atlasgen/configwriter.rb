# Writes a new config into file
require 'cairo'
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
        
    end
    
    def getErrors()
        return @errors
    end
end
