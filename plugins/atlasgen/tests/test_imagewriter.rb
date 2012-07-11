# Contains unit-tests for image writer module

load    'imagewriter.rb'
require 'test/unit'
require 'FileUtils'

class ImageWriterTest < Test::Unit::TestCase
       
    def setup()
        @obj = ImageWriter.new
        @tex = Devil.load_image("test_imgs/lena.jpg")
    end
    
    def testWrite_dir_doesnt_exists()
        fn = '../?12P1><\/lena_saved.jpg'
        res = @obj.write(@tex, fn)
        assert(!res,"invalid dir!")
    end

	def testWriteSuccess()
		fn = 'test_imgs/lena_copy.jpg';
		res = @obj.write(@tex,'test_imgs/lena_copy.jpg')
		assert(res,'Failed to copy');
	end
end


