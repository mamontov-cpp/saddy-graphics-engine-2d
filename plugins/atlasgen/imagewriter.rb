##
# :title: imageglue.rb
# A class, which writes a output texture atlas into output file
require 'devil'

##
# :category: Public classes
# A class, which writes an source _DevIL::Image_ into output file
class ImageWriter

    ##
    # :category: Public interface
    # Writes an image into output file
    # [texture]  _DevIL::Image_               resulting image
    # [filename] _String_                     name of output file
    # [return]   _TrueClass_ or _FalseClass_  true on success
    def write(texture, filename)
        # We treat invalid texture as success
		if (texture.nil?)
			return true
		end
        # Handle invalid value paths, throwing exceptions
        begin
            # Check, whether destination exists and write file if so
            # otherwise return false
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