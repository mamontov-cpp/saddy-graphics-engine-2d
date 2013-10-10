##
# :title: test_imageglueingorder.rb
# Tests for ImageGluingOrder class
load    'imagegluingorder.rb'
require 'test/unit'


##
# :category: Test Utilities
# Contains a various utility methods used in tests  
class TestSupply
    ##
    # :category: Test Utilities
    # Returns an array of sizes, used in tests
    # [return] _Array_ of _Array_ [w,h] result sizes
    def self.get()
        return self.toEntries([ [50,40], [30,30], [20,30], [50,40], [50,40], [30,30], [20,30] , [50,40] ])
    end
    ##
    # :category: Test Utilities
    # Converts a source array of sizes to a GlueEntry array
    # [sizes]  _Array_ of _Array_ [w,h] sizes array
    # [return] _Array_ of _GlueEntry_   an array of valid GlueEntry
    def self.toEntries(sizes)
        return sizes.collect{ |size| GlueEntry.new(size[0],size[1]); };
    end
    ##
    # :category: Debug Print Functions
    # Prints an array of orders 
    # [orders]  _Array_ of _GlueOrder_ a glue orders
    def self.printOrders(orders)
        result = orders.length.to_s() + ": "
        orders.each{  |order| result = result + TestSupply.printOrder(order) + ";"}
        return result
    end
    ##
    # :category: Debug Print Functions
    # Prints an image order
    # [orders]  _GlueOrder_ an order
    def self.printOrder(order)
        return "{ image0: " + order.images[0].to_s() + "; image1: "+ order.images[1].to_s() + "; mode:" + order.mode.to_s() + "}";
    end
    ##
    # :category: Debug Print Functions
    # Prints an array of orders 
    # [entries]  _Array_ of _Texture_ a textures array
    def self.printImages(entries)
      result = ""
      entries.each{ |entry| result=result + TestSupply.printImage(entry) + ";"}
      return result
    end
    ##
    # :category: Debug Print Functions
    # Prints an image
    # [entry]  _Texture_ A texture
    def self.printImage(entry)
        return "[" + entry.size()[0].to_s() + "," + entry.size()[1].to_s() + "]"
    end
end

##
# :category: Tests
# Tests for GlueEntry class
class TestGlueEntry < Test::Unit::TestCase
    ##
    # :category: Test Utilities
    # Inits a source entries
    def setup()
        @entries = TestSupply.get()
    end
    ##
    # :category: Tests
    # Tests a GlueEntry::merge on a case of horizontal merge, when left image is bigger.
    def testMerge_horizontalLR()
        entry = GlueEntry.merge(@entries, GlueOrder.new(0,1,GlueMode::HORIZONTAL) )
        size = entry.size()
        assert( (size[0]-80).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-40).abs() < 0.1 , size[1].to_s() )
    end
    ##
    # :category: Tests
    # Tests a GlueEntry::merge on a case of horizontal merge, when left image is smaller.
    def testMerge_horizontalRL()
        entry = GlueEntry.merge(@entries, GlueOrder.new(2,3,GlueMode::HORIZONTAL))
        size = entry.size()
        assert( (size[0]-70).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-40).abs() < 0.1 , size[1].to_s() )
    end
    ##
    # :category: Tests
    # Tests a GlueEntry::merge on a case of horizontal merge, when images are equal
    def testMerge_horizontalEqual()
        entry = GlueEntry.merge(@entries, GlueOrder.new(0,0,GlueMode::HORIZONTAL))
        size = entry.size()
        assert( (size[0]-100).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-40).abs() < 0.1 , size[1].to_s() )
    end
    ##
    # :category: Tests
    # Tests a GlueEntry::merge on a case of vertical merge, when upper image is bigger.
    def testMerge_verticalUD()
        entry = GlueEntry.merge(@entries, GlueOrder.new(4,5,GlueMode::VERTICAL) )
        size = entry.size()
        assert( (size[0]-50).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-70).abs() < 0.1 , size[1].to_s() )
    end
    ##
    # :category: Tests
    # Tests a GlueEntry::merge on a case of vertical merge, when lower image is bigger.
    def testMerge_verticalDU()
        entry = GlueEntry.merge(@entries, GlueOrder.new(6,7,GlueMode::VERTICAL) )
        size = entry.size()
        assert( (size[0]-50).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-70).abs() < 0.1 , size[1].to_s() )
    end
    ##
    # :category: Tests
    # Tests a GlueEntry::merge on a case of vertical merge, when images are equal
    def testMerge_verticalEqual()
        entry = GlueEntry.merge(@entries, GlueOrder.new(0,0,GlueMode::VERTICAL) )
        size = entry.size()
        assert( (size[0]-50).abs() < 0.1 , size[0].to_s() )
        assert( (size[1]-80).abs() < 0.1 , size[1].to_s() )
    end
    
    ##
    # :category: Test Utilities
    # Does nothing
    def teardown()

    end
end

##
# :category: Tests
# Tests for MinAreaMetric class
class MinAreaMetricTest < Test::Unit::TestCase
    
    ##
    # :category: Test Utilities
    # Inits a source size array os array[w,h] and MinAreaMetric object
    def setup()
        @entries = TestSupply.get()
        @obj = MinAreaMetric.new()
    end
    
    ##
    # :category: Tests
    # Tests a MinAreaMetric::getMetric on a case of horizontal merge, when left image is bigger.
    def testGetMetric_horizontalRL()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,1,GlueMode::HORIZONTAL))
        assert( (metric-3200).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Tests
    # Tests a MinAreaMetric::getMetric on a case of horizontal merge, when right image is bigger.
    def testGetMetric_horizontalLR()
        metric = @obj.getMetric(@entries, GlueOrder.new(2,3,GlueMode::HORIZONTAL))
        assert( (metric-2800).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Tests
    # Tests a MinAreaMetric::getMetric on a case of horizontal merge, when images are equal
    def testGetMetric_horizontalEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::HORIZONTAL))
        assert( (metric-4000).abs() < 0.1 , metric.to_s() )
    end
    
    ##
    # :category: Tests
    # Tests a MinAreaMetric::getMetric on a case of vetical merge, when upper image is bigger
    def testGetMetric_verticallUD() 
        metric = @obj.getMetric(@entries, GlueOrder.new(4,5,GlueMode::VERTICAL))
        assert( (metric-3500).abs() < 0.1 , metric.to_s() )
    end
    
    ##
    # :category: Tests
    # Tests a MinAreaMetric::getMetric on a case of vertical merge, when lower image is bigger
    def testGetMetric_verticallDU()
        metric = @obj.getMetric(@entries, GlueOrder.new(6,7,GlueMode::VERTICAL))
        assert( (metric-3500).abs() < 0.1 , metric.to_s() )
    end
    
    ##
    # :category: Tests
    # Tests a MinAreaMetric::getMetric on a case of vertical merge, when images are equal
    def testGetMetric_verticallEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::VERTICAL))
        assert( (metric-4000).abs() < 0.1 , metric.to_s() )
    end
    
    ##
    # :category: Tests
    # Test MinAreaMetric::findMinMetric order in common case
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

    ##
    # :category: Tests
    # Test MinAreaMetric::findMinMetric order in common case
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
	
	##
    # :category: Tests
    # Tests an MinAreaMetric, checking resulting size
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
    ##
    # :category: Test Utilities
    # Does nothing
    def teardown()

    end
    
end
##
# :category: Tests
# Tests for MinDiffMetric class
class MinDiffMetricTest < Test::Unit::TestCase
    
    ##
    # :category: Test Utilities
    # Inits a source size array os array[w,h] and MinDiffMetric object
    def setup()
        @entries = TestSupply.get()
        @obj = MinDiffMetric.new()
    end
    
    ##
    # :category: Tests
    # Tests a MinDiffMetric::getMetric on a case of horizontal merge, when left image is bigger.
    def testGetMetric_horizontalRL()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,1,GlueMode::HORIZONTAL))
        assert( (metric-300).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Tests
    # Tests a MinDiffMetric::getMetric on a case of horizontal merge, when right image is bigger.
    def testGetMetric_horizontalLR()
        metric = @obj.getMetric(@entries, GlueOrder.new(2,3,GlueMode::HORIZONTAL))
        assert( (metric-200).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Tests
    # Tests a MinDiffMetric::getMetric on a case of horizontal merge, when images are equal
    def testGetMetric_horizontalEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::HORIZONTAL))
        assert( (metric-0).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Tests
    # Tests a MinDiffMetric::getMetric on a case of vertical merge, when upper image is bigger.
    def testGetMetric_verticallUD() 
        metric = @obj.getMetric(@entries, GlueOrder.new(4,5,GlueMode::VERTICAL))
        assert( (metric-600).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Tests
    # Tests a MinDiffMetric::getMetric on a case of vertical merge, when lower image is bigger.
    def testGetMetric_verticallDU()
        metric = @obj.getMetric(@entries, GlueOrder.new(6,7,GlueMode::VERTICAL))
        assert( (metric-900).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Tests
    # Tests a MinDiffMetric::getMetric on a case of vertical merge, when images are equal
    def testGetMetric_verticallEqual()
        metric = @obj.getMetric(@entries, GlueOrder.new(0,0,GlueMode::VERTICAL))
        assert( (metric-0).abs() < 0.1 , metric.to_s() )
    end
    ##
    # :category: Test Utilities
    # Does nothing
    def teardown()

    end
    
    ##
    # :category: Tests
    # Test MinDiffMetric::findMinMetric order in common case
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
    ##
    # :category: Tests
    # Test MinDiffMetric::findMinMetric order in common case
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
	
	##
    # :category: Tests
    # Tests an MinDiffMetric, checking resulting size
	def testfindOrder()
		entries = @entries.clone()
		[7,6,5,4,3].each{ |x| entries.delete_at(x) }
		result = @obj.findOrder(entries)
		assert( (result[0][1][0]-50).abs() < 0.1 , result[0][1][0].to_s() )
		assert( (result[0][1][1]-70).abs() < 0.1 , result[0][1][1].to_s() )
		
		#TODO: Uncomment in case of error to see test results
		#result.each {
		#	|resultentry|
			
		#	print "<" + resultentry[1][0].to_s() + " "  + resultentry[1][1].to_s() + ">: " + TestSupply.printOrders(resultentry[0]) + "\n"
		
		#}
		
	end
end

##
# :category: Tests
# Tests for ImageGluingIOrder
class TestImageGluingOrder < Test::Unit::TestCase
    
    ##
    # :category: Test Utilities
    # Inits a source size array os array[w,h] and ImageGluingOrder object
    def setup()
        @entries = TestSupply.get()
        @obj = ImageGluingOrder.new()
    end
    
    ##
    # :category: Tests
    # Tests an ImageGluingOrder::find, checking resulting size
    def testFind()
        result = @obj.find(@entries)
        assert( (result.size()[0]-100).abs() < 0.1 , result.size()[0].to_s() )
		assert( (result.size()[1]-110).abs() < 0.1 , result.size()[1].to_s() )
        #TODO: Uncomment this in case of error
        #print "<" + result.size[0].to_s() + " "  + result.size[1].to_s() + ">: " + TestSupply.printOrders(result.order) + "\n"
    end
    
    
    ##
    # :category: Test Utilities
    # Does nothing
    def teardown()
        
    end
end
