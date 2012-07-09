# Contains unit-tests for image gluing order module parts
# A unit-test for this module is placed here
load    'imagegluingorder.rb'
require 'test/unit'



class TestGlueEntry < Test::Unit::TestCase

    #  Inits entry array with test entries
    def setup()
        sizes =  [ [50,40], [30,30], [20,30], [50,40], [50,40], [30,30], [20,30] , [50,40] ];
        @entries = sizes.collect{ |size| GlueEntry.new(size[0],size[1]); };
    end
    
    # Test a horizontal merge, when first image is bigger
    def testMerge_horizontalLR()
        entry = GlueEntry.merge(@entries, GlueOrder.new(0,1,GlueMode::HORIZONTAL) )
        size = entry.size()
        assert( (size[0]-80).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-40).abs() < 0.1 , size[1].to_s() )
    end
    
    # Test a horizontal merge, when first image is smaller
    def testMerge_horizontalRL()
        entry = GlueEntry.merge(@entries, GlueOrder.new(2,3,GlueMode::HORIZONTAL))
        size = entry.size()
        assert( (size[0]-70).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-40).abs() < 0.1 , size[1].to_s() )
    end
    
    # Test a horizontal merge, when images are Equal
    def testMerge_horizontalEqual()
        entry = GlueEntry.merge(@entries, GlueOrder.new(0,0,GlueMode::HORIZONTAL))
        size = entry.size()
        assert( (size[0]-100).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-40).abs() < 0.1 , size[1].to_s() )
    end
    
    # Does really nothing
    def teardown()

    end
end