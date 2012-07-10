# Contains unit-tests for image gluing order module parts
# A unit-test for this module is placed here
load    'imagegluingorder.rb'
require 'test/unit'


# Returns a test material objects to provide some data
class TestSupply
    def self.get()
        return self.toEntries([ [50,40], [30,30], [20,30], [50,40], [50,40], [30,30], [20,30] , [50,40] ])
    end
    
    def self.toEntries(sizes)
        return sizes.collect{ |size| GlueEntry.new(size[0],size[1]); };
    end
    
    def self.printOrders(orders)
        result = orders.length.to_s() + ": "
        orders.each{  |order| result = result + TestSupply.printOrder(order) + ";"}
        return result
    end
    
    def self.printOrder(order)
        return "{ image0: " + order.images[0].to_s() + "; image1: "+ order.images[1].to_s() + "; mode:" + order.mode.to_s() + "}";
    end
    
    def self.printImages(entries)
      result = ""
      entries.each{ |entry| result=result + TestSupply.printImage(entry) + ";"}
      return result
    end
    
    def self.printImage(entry)
        return "[" + entry.size()[0].to_s() + "," + entry.size()[1].to_s() + "]"
    end
end


class TestGlueEntry < Test::Unit::TestCase

    attr_accessor :entries
    
    #  Inits entry array with test entries
    def setup()
        @entries = TestSupply.get()
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
    
    # Test a horizontal merge, when images are equal
    def testMerge_horizontalEqual()
        entry = GlueEntry.merge(@entries, GlueOrder.new(0,0,GlueMode::HORIZONTAL))
        size = entry.size()
        assert( (size[0]-100).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-40).abs() < 0.1 , size[1].to_s() )
    end
    
    # Tests a vertical merge, when upper image is bigger then lower
    def testMerge_verticalUD()
        entry = GlueEntry.merge(@entries, GlueOrder.new(4,5,GlueMode::VERTICAL) )
        size = entry.size()
        assert( (size[0]-50).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-70).abs() < 0.1 , size[1].to_s() )
    end
     # Tests a vertical merge, when lower image is bigger then upper
    def testMerge_verticalDU()
        entry = GlueEntry.merge(@entries, GlueOrder.new(6,7,GlueMode::VERTICAL) )
        size = entry.size()
        assert( (size[0]-50).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-70).abs() < 0.1 , size[1].to_s() )
    end
    # Tests a vertical merge, when images are equal
    def testMerge_verticalEqual()
        entry = GlueEntry.merge(@entries, GlueOrder.new(0,0,GlueMode::VERTICAL) )
        size = entry.size()
        assert( (size[0]-50).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-80).abs() < 0.1 , size[1].to_s() )
    end
    
    # Does really nothing
    def teardown()

    end
end

# Test of Min Area metric
class MinAreaMetricTest < Test::Unit::TestCase
    
    #  Inits entry array with test entries and object with valid object 
    def setup()
        @entries = TestSupply.get()
        @obj = MinAreaMetric.new()
    end
    
    # Test a metric computation on horizontal merge, when left image is bigger then right
    def testGetMetric_horizontalRL()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,1,GlueMode::HORIZONTAL))
        assert( (metric-3200).abs() < 0.1 , metric.to_s() )
    end
    # Test a metric computation on horizontal merge, when right image is bigger then left
    def testGetMetric_horizontalLR()
        metric = @obj.getMetric(@entries, GlueOrder.new(2,3,GlueMode::HORIZONTAL))
        assert( (metric-2800).abs() < 0.1 , metric.to_s() )
    end
    # Test a metric computation on horizontal merge, when images are equal
    def testGetMetric_horizontalEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::HORIZONTAL))
        assert( (metric-4000).abs() < 0.1 , metric.to_s() )
    end
    
    # Test a metric computation on vertical merge, when upper image is bigger then lower
    def testGetMetric_verticallUD() 
        metric = @obj.getMetric(@entries, GlueOrder.new(4,5,GlueMode::VERTICAL))
        assert( (metric-3500).abs() < 0.1 , metric.to_s() )
    end
    
    # Test a metric computation on vertical merge, when lower image is bigger then upper
    def testGetMetric_verticallDU()
        metric = @obj.getMetric(@entries, GlueOrder.new(6,7,GlueMode::VERTICAL))
        assert( (metric-3500).abs() < 0.1 , metric.to_s() )
    end
    
    # Test a metric computation on vertical merge, when images are equal
    def testGetMetric_verticallEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::VERTICAL))
        assert( (metric-4000).abs() < 0.1 , metric.to_s() )
    end
    
    # Test first iteration of method
    def testFindMinMetric1()
        orders = @obj.findMinMetricOrder(@entries)
        len = orders.length.to_s()
   
        assert(orders.length == 2, TestSupply.printOrders(orders))
        assert(orders[0].images[0] == 2)
        assert(orders[0].images[1] == 6)
        assert(orders[0].mode == GlueMode::HORIZONTAL)
        assert(orders[1].images[0] == 2)
        assert(orders[1].images[1] == 6)
        assert(orders[1].mode == GlueMode::VERTICAL)
        
    end

    # Tests second iteration of method
    def testFindMinMetric2()
        orders = @obj.findMinMetricOrder(@entries)
        
        entries = @entries.clone()
        entries.delete_at(6)
        entries.delete_at(2)
        entries = entries <<  GlueEntry.merge(@entries, orders[0])
        orders =  @obj.findMinMetricOrder(entries)
        
        assert(orders.length == 2, TestSupply.printOrders(orders))
        assert(orders[0].images[0] == 1)
        assert(orders[0].images[1] == 4)
        assert(orders[0].mode == GlueMode::HORIZONTAL)
        assert(orders[1].images[0] == 1)
        assert(orders[1].images[1] == 4)
        assert(orders[1].mode == GlueMode::VERTICAL)
    end
	
	# Tests finding order of merging images
	def testfindOrder()
		entries = @entries.clone()
		[7,6,5,4,3].each{ |x| entries.delete_at(x) }
		result = @obj.findOrder(entries)
		assert( (result[0][1][0]-50).abs() < 0.1 , result[0][1][0].to_s() )
		assert( (result[0][1][1]-70).abs() < 0.1 , result[0][1][1].to_s() )
		
		# TODO: Uncomment in case of error
		#result.each {
		#	|resultentry|
			
		#	print "<" + resultentry[1][0].to_s() + " "  + resultentry[1][1].to_s() + ">: " + TestSupply.printOrders(resultentry[0]) + "\n"
		
		#}
		
	end
    # Does really nothing
    def teardown()

    end
    
end
# Test of Min Diff metric
class MinDiffMetricTest < Test::Unit::TestCase
    
    #  Inits entry array with test entries and object with valid object 
    def setup()
        @entries = TestSupply.get()
        @obj = MinDiffMetric.new()
    end
    
    # Test a metric computation on horizontal merge, when left image is bigger then right
    def testGetMetric_horizontalRL()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,1,GlueMode::HORIZONTAL))
        assert( (metric-300).abs() < 0.1 , metric.to_s() )
    end
    # Test a metric computation on horizontal merge, when right image is bigger then left
    def testGetMetric_horizontalLR()
        metric = @obj.getMetric(@entries, GlueOrder.new(2,3,GlueMode::HORIZONTAL))
        assert( (metric-200).abs() < 0.1 , metric.to_s() )
    end
    # Test a metric computation on horizontal merge, when images are equal
    def testGetMetric_horizontalEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::HORIZONTAL))
        assert( (metric-0).abs() < 0.1 , metric.to_s() )
    end
    
    # Test a metric computation on vertical merge, when upper image is bigger then lower
    def testGetMetric_verticallUD() 
        metric = @obj.getMetric(@entries, GlueOrder.new(4,5,GlueMode::VERTICAL))
        assert( (metric-600).abs() < 0.1 , metric.to_s() )
    end
    
    # Test a metric computation on vertical merge, when lower image is bigger then upper
    def testGetMetric_verticallDU()
        metric = @obj.getMetric(@entries, GlueOrder.new(6,7,GlueMode::VERTICAL))
        assert( (metric-900).abs() < 0.1 , metric.to_s() )
    end
    
    # Test a metric computation on vertical merge, when images are equal
    def testGetMetric_verticallEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::VERTICAL))
        assert( (metric-0).abs() < 0.1 , metric.to_s() )
    end
    
    # Does really nothing
    def teardown()

    end
    
     # Test first iteration of method
    def testFindMinMetric1()
        entries = @entries.clone()
        [7,6,5,4,3].each{ |i| entries.delete_at(i) }
        orders = @obj.findMinMetricOrder(entries)
        #len = orders.length.to_s()
   
        assert(orders.length == 1, TestSupply.printOrders(orders))
        assert(orders[0].images[0] == 1)
        assert(orders[0].images[1] == 2)
        assert(orders[0].mode == GlueMode::HORIZONTAL)
       
    end
    # Test second iteration of method
    def testFindMinMetric2()
        entries = @entries.clone()
        entries = entries <<  GlueEntry.merge(entries, GlueOrder.new(0,1, GlueMode::HORIZONTAL))
        [7,6,5,4,3,2,1].each{ |i| entries.delete_at(i) }
        orders =  @obj.findMinMetricOrder(entries)
        assert(orders.length == 1, TestSupply.printOrders(orders))
        assert(orders[0].images[0] == 0)
        assert(orders[0].images[1] == 1)
        assert(orders[0].mode == GlueMode::HORIZONTAL, TestSupply.printImages(entries))
    end
end