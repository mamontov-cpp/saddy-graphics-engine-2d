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
    # Tests some horizontal merge
    def testImageBucketMergeHorizontal()
        sizes = [ [30,50], [30,40] ]
        images = sizes.collect{ |size| @mockEntry.new(size,[]) }
        buckets = images.collect{ |image| ImageArranger::ImageBucket.new(image) }
        result = ImageArranger::ImageBucket.merge(buckets[0], buckets[1], GlueOrder.new(0,1, GlueMode::HORIZONTAL))
        assert( result.images.length == 2)
        assert( result.size[0] == 60, result.size[0].to_s())
        assert( result.size[1] == 50, result.size[0].to_s())
        assert(images[0].textureRectangle[0] == 0)
        assert(images[0].textureRectangle[1] == 0)
        assert(images[0].textureRectangle[2] == 30)
        assert(images[0].textureRectangle[3] == 50)
        assert(images[1].textureRectangle[0] == 30)
        assert(images[1].textureRectangle[1] == 0)
        assert(images[1].textureRectangle[2] == 30)
        assert(images[1].textureRectangle[3] == 40)
        
        sizes = [ [30,40], [30,50] ]
        images = sizes.collect{ |size| @mockEntry.new(size,[]) }
        buckets = images.collect{ |image| ImageArranger::ImageBucket.new(image) }
        result = ImageArranger::ImageBucket.merge(buckets[0], buckets[1], GlueOrder.new(0,1, GlueMode::HORIZONTAL))
        assert( result.images.length == 2)
        assert( result.size[0] == 60, result.size[0].to_s())
        assert( result.size[1] == 50, result.size[0].to_s())
        assert(images[0].textureRectangle[0] == 0)
        assert(images[0].textureRectangle[1] == 0)
        assert(images[0].textureRectangle[2] == 30)
        assert(images[0].textureRectangle[3] == 40)
        assert(images[1].textureRectangle[0] == 30)
        assert(images[1].textureRectangle[1] == 0)
        assert(images[1].textureRectangle[2] == 30)
        assert(images[1].textureRectangle[3] == 50)
    end
    
    
    # Tests some vertical merging
    def testImageBucketMergeVertical()
        sizes = [ [30,50], [30,40] ]
        images = sizes.collect{ |size| @mockEntry.new(size,[]) }
        buckets = images.collect{ |image| ImageArranger::ImageBucket.new(image) }
        result = ImageArranger::ImageBucket.merge(buckets[0], buckets[1], GlueOrder.new(0,1, GlueMode::VERTICAL))
        assert( result.images.length == 2)
        assert( result.size[0] == 30, result.size[0].to_s())
        assert( result.size[1] == 90, result.size[0].to_s())
        assert(images[0].textureRectangle[0] == 0)
        assert(images[0].textureRectangle[1] == 0)
        assert(images[0].textureRectangle[2] == 30)
        assert(images[0].textureRectangle[3] == 50)
        assert(images[1].textureRectangle[0] == 0)
        assert(images[1].textureRectangle[1] == 50)
        assert(images[1].textureRectangle[2] == 30)
        assert(images[1].textureRectangle[3] == 40)
        
        sizes = [ [30,40], [30,50] ]
        images = sizes.collect{ |size| @mockEntry.new(size,[]) }
        buckets = images.collect{ |image| ImageArranger::ImageBucket.new(image) }
        result = ImageArranger::ImageBucket.merge(buckets[0], buckets[1], GlueOrder.new(0,1, GlueMode::VERTICAL))
        assert( result.images.length == 2)
        assert( result.size[0] == 30, result.size[0].to_s())
        assert( result.size[1] == 90, result.size[0].to_s())
        assert(images[0].textureRectangle[0] == 0)
        assert(images[0].textureRectangle[1] == 0)
        assert(images[0].textureRectangle[2] == 30)
        assert(images[0].textureRectangle[3] == 40)
        assert(images[1].textureRectangle[0] == 0)
        assert(images[1].textureRectangle[1] == 40)
        assert(images[1].textureRectangle[2] == 30)
        assert(images[1].textureRectangle[3] == 50)
    end
    
     # Tests some horizontal merge
    def testArrangeHorizontal()
        sizes = [ [30,50], [30,40], [30,40] ]
        images = sizes.collect{ |size| @mockEntry.new(size,[]) }
        buckets = images.collect{ |image| ImageArranger::ImageBucket.new(image) }
        ImageArranger.new.arrange(images, [GlueOrder.new(1,2, GlueMode::HORIZONTAL), GlueOrder.new(0,1, GlueMode::HORIZONTAL)], [0,0])
        assert(images[0].textureRectangle[0] == 0,images[0].textureRectangle[0].to_s())
        assert(images[0].textureRectangle[1] == 0,images[0].textureRectangle[1].to_s())
        assert(images[0].textureRectangle[2] == 30)
        assert(images[0].textureRectangle[3] == 50)
        assert(images[1].textureRectangle[0] == 30)
        assert(images[1].textureRectangle[1] == 0)
        assert(images[1].textureRectangle[2] == 30)
        assert(images[1].textureRectangle[3] == 40)
        assert(images[2].textureRectangle[0] == 60, images[2].textureRectangle[0].to_s())
        assert(images[2].textureRectangle[1] == 0)
        assert(images[2].textureRectangle[2] == 30)
        assert(images[2].textureRectangle[3] == 40)
    end
    
    # Tests some vertical merge
    def testArrangeVertixal()
        sizes = [ [30,50], [30,40], [30,40] ]
        images = sizes.collect{ |size| @mockEntry.new(size,[]) }
        buckets = images.collect{ |image| ImageArranger::ImageBucket.new(image) }
        ImageArranger.new.arrange(images, [GlueOrder.new(1,2, GlueMode::VERTICAL), GlueOrder.new(0,1, GlueMode::VERTICAL)], [0,0])
        assert(images[0].textureRectangle[0] == 0,images[0].textureRectangle[0].to_s())
        assert(images[0].textureRectangle[1] == 0,images[0].textureRectangle[1].to_s())
        assert(images[0].textureRectangle[2] == 30)
        assert(images[0].textureRectangle[3] == 50)
        assert(images[1].textureRectangle[0] == 0)
        assert(images[1].textureRectangle[1] == 50)
        assert(images[1].textureRectangle[2] == 30)
        assert(images[1].textureRectangle[3] == 40)
        assert(images[2].textureRectangle[0] == 0, images[2].textureRectangle[0].to_s())
        assert(images[2].textureRectangle[1] == 90)
        assert(images[2].textureRectangle[2] == 30)
        assert(images[2].textureRectangle[3] == 40)
    end
    # Does really nothing
    def teardown()
        
    end
    
end