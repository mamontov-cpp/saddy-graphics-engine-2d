# Tests an image arranger part of work
# Tests a mageArranger
load    'imagearranger.rb'
require 'test/unit'
# Tests a ImageArranger
class TestImageArranger < Test::Unit::TestCase
    @mockEntry
    
    def setup() 
        @mockEntry = Struct.new(:size, :textureRectangle)
    end
    
    # Tests power of two computation in arrange
    def testPOT()
        obj = ImageArranger.new()
        size = obj.arrange([],[],[0,0])
        assert(size == 0)
        size = obj.arrange([],[],[1,1])
        assert(size == 2)
        size = obj.arrange([],[],[2,2])
        assert(size == 2)
        size = obj.arrange([],[],[16,0])
        assert(size == 16)
        size = obj.arrange([],[],[100,0])
        assert(size == 128)
        size = obj.arrange([],[],[257,0])
        assert(size == 512)
    end
    # Tests one image arrangement
    def testOneImage()
        obj = ImageArranger.new()
        size = [120,140]
        image = @mockEntry.new(size,[])
        rsize = obj.arrange([image],[],size)
        assert(rsize == 256, rsize.to_s())
        assert(image.textureRectangle[0] == 0)
        assert(image.textureRectangle[1] == 0)
        assert(image.textureRectangle[2] == 120)
        assert(image.textureRectangle[3] == 140)
        
        size = [220,257]
        image = @mockEntry.new(size,[])
        rsize = obj.arrange([image],[],size)
        assert(rsize == 512, rsize.to_s())
        assert(image.textureRectangle[0] == 0)
        assert(image.textureRectangle[1] == 0)
        assert(image.textureRectangle[2] == 220)
        assert(image.textureRectangle[3] == 257)
        
    end
    # Does really nothing
    def teardown()
        
    end
    
end