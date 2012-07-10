# Tests an image arranger part of work
# Tests a mageArranger
load    'imagearranger.rb'
require 'test/unit'
# Tests a ImageArranger
class TestImageArranger < Test::Unit::TestCase

    def setup() 
    
    end
    
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
    
    # Does really nothing
    def teardown()
        
    end
    
end