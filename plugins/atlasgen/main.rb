## 
# :title: Atlas Generator
# A script for automatic generation of configs and texture atlases for saddy
# Suppose, you have an input xml file like
#          <sprites config="a.xml" texture="a.png" >
#          <a texture="aa.png" size="64;64" transparent="0;0;0" index="2" />
#          ...
#          </sprites>  
# You want to merge aa.png with other texture into a texture atlas (POT texture) 
# converting it to a saddy XML config format and write atlas into a.png and config - a.xml
# This is where this script comes in handy. Also you can omit size tag - it can be taken from size of source image file
# and transparent - to make texture part non-transparent. By default index is "0", so you could omit it too.
# The resulting config will be like 
#       <sprites>
#       <a texture="a.png" size="64;64" transparent="0;0;0" index="2"  texrect="0;0;w;h"/>
#       ...
#       </sprites>
# So, you can load new texture by loading it in saddy engine and parsing xml, resulting a texture atlas.
# 
# You can run the program using ruby main.rb "name of valid xml config"
# Required gems - devil (don't forget to include OpenIL libraries in Ruby's bin folder)
# json to support json
# Preferred version of ruby - 1.9.3
require 'optparse'
require_relative 'configreader'
require_relative 'imagegluingorder.rb'
require_relative 'imagearranger.rb'
require_relative 'imageglue.rb'
require_relative 'imagewriter.rb'
require_relative 'configwriter.rb'

##
# :category: Utility
# Prints source array of errors as string, adding a new line after them
# [arr] _Array_ of _Strings_ source array of strings to print on screen
def printErrors(arr)
    print arr.join("\n") + "\n"
end


if $0 == __FILE__
	options = {}
	options["format"] = "json"
	options["with-index"] = false
	optparse = OptionParser.new do|opts|
		# Set a banner, displayed at the top
		# of the help screen.
		opts.banner = "Saddy texture atlas generator.\nUsage: main.rb file1 [--format-xml|--format-json] [--with-index]\n"
 
		# Define the options, and what they do
		options["format"] = "json"		
		opts.on( '-json', '--format-json', 'Use JSON for input and output files (default)' ) do
			options["format"] = "json"
		end
		opts.on( '-xml', '--format-xml', 'Use XML for input and output files' ) do
			options["format"] = "xml"
		end
		
		options["with-index"] = false 
		opts.on( '-with-index', '--with-index', 'Scan and print index part in file definitions' ) do
			options["with-index"] = true
		end
  
		# This displays the help screen, all programs are
		# assumed to have this option.
		opts.on( '-h', '--help', 'Display this screen' ) do
			puts opts
			exit
		end
	end
    optparse.parse!
    # Handle argv error, when input file is not specified
    if ARGV.length < 1
        puts "Error: no input file specified. Please specify valid XML config.\n"
    else
        # Read a config
		reader = JSONConfigReader.new
		if (options['format'] == 'xml')
			reader = XMLConfigReader.new
		end
		
        config = reader.read(ARGV[0])
        # Proceed if  no errors
        if ((config != nil) && (reader.getErrors().length ==0))
            textures = config.getTextures()
            # If we should build atlas, build it
            if (textures.length!=0)
                # Find a gluing order
                gluingorder = ImageGluingOrder.new()
                glorder = gluingorder.find(textures)
                # Arrange images to it
                arranger = ImageArranger.new()
                widthheight = arranger.arrange(textures,glorder.order,glorder.size)
                # Glue images into one
                glue = ImageGlue.new()
                outputTexture = glue.glue(widthheight,textures)
                # Write an image to file
                imgwriter = ImageWriter.new()
                if (imgwriter.write(outputTexture,reader.getOutputTextureName) == false)
                    print "Can't write resulting texture to file " + reader.getOutputTextureName() + "\n" 
                end
            end
            # Write a new config
			writer = JSONConfigWriter.new
			if (options['format'] == 'xml')
				writer = XMLConfigWriter.new
			end		
            # Print errors, if writing failed
            if (writer.write(config, reader.getOutputConfigName(),reader.getOutputTextureName, options["with-index"]) == false)
                printErrors(writer.getErrors())
            end
        else
            printErrors(reader.getErrors())
        end
    end
end