#
#
#
#

require_relative 'configreader'
require_relative 'imagegluingorder.rb'
require_relative 'imagearranger.rb'
require_relative 'imageglue.rb'
require_relative 'imagewriter.rb'
require_relative 'configwriter.rb'

# Prints array
# * param Array arr array of strings
def printErrors(arr)
    print arr.join('\n') + '\n'
end

puts "Saddy texture atlas generator.\n"
if $0 == __FILE__
    if ARGV.length < 1
        puts "Error: no input file specified. Please specify valid XML config.\n"
    else
        # Read a config
        reader = ConfigReader.new
        config = reader.read(ARGV[0])
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
            writer = ConfigWriter.new
            if (writer.write(config, reader.getOutputConfigName(),reader.getOutputTextureName) == false)
                printErrors(writer.getErrors())
            end
        else
            printErrors(reader.getErrors())
        end
    end
end