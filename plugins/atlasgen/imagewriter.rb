# Writes an image into file
require 'devil'

# Writes an image into file
class ImageWriter

    # Writes an image into file
    # * param texture DevIL image
    # * param filename String name of output file
    # * return Boolean  whether writing was successfull
    def write(texture, filename)
		if (texture.nil?)
			return true
		end
        begin
            dirname = File.dirname(filename)
            if File.directory?(dirname)
                    res = true
                    texture.save(filename)
            else
                res = false
            end
        rescue RuntimeError
            res = false
        end
        return res
    end
end