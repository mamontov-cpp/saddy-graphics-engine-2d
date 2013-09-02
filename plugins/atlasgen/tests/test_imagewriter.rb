##
# :title: test_imagewriter.rb
# Tests for ImageWriter class
load    'imagewriter.rb'
require 'test/unit'
require 'FileUtils'
##
# :category: Tests
# Tests for ImageWriter class
class TestImageWriter < Test::Unit::TestCase
    ##
    # :category: Test Utilities
    # A texture loading and object creation performed here  performed here      
    def setup()
        @obj = ImageWriter.new
        @tex = Devil.load_image("test_imgs/lena.jpg")
    end
    ##
    # :category: Tests
    # Tests a ImageWriter::write on a case, when destination does not exists
    def testWrite_dir_doesnt_exists()
        fn = '../?12P1><\/lena_saved.jpg'
        res = @obj.write(@tex, fn)
        assert(!res,"Saved into non-valid directory")
    end
    ##
    # :category: Tests
    # Tests a ImageWriter::write on a case, when passed path is invalid
    def testWrite_inval()
        fn = '../?12P1><<,.>)'
        res = @obj.write(@tex, fn)
        assert(!res,"Saved into non-valid file")
    end
    ##
    # :category: Tests
    # Tests a ImageWriter::write on a valid path. The result must be verified manually.
	def testWriteSuccess()
		fn = 'test_imgs/lena_copy.jpg';
		res = @obj.write(@tex,'test_imgs/lena_copy.png')
		assert(res,'Failed to copy');
	end
end


