##
# :title: imagegluingorder.rb
# Describes a metrics and algorithm to find a method, which
# merges all images into one with minimized width and height

##
# :category: Public classes
# Describes, how two images, should be merged - horizontally or vertically
module GlueMode
    ##
    # Images should be merged horizontally
    HORIZONTAL = 1
    ##
    # Images should be merged vertically
    VERTICAL = 2
end

##
# :category: Public classes
# Describes what images should be merged, and how they must be merged
class GlueOrder
    ##
    # _Array_ of _Fixnum_. Indexes of images in source +TextureArray+  or +Array+ of  _Fixnum_ to be merged
    attr_accessor :images
    # _Fixnum_ with values from _GlueMode_. A mode, describing how images should be merged.  
    attr_accessor :mode
    
    ##
    # :category: Public interface
    # Creates a new order, with specified parameters
    # [image1]  _Fixnum_  index of first image in source +TextureArray+  or +Array+ of  _Fixnum_ to be merged
    # [image2]  _Fixnum_  index of source image in source +TextureArray+  or +Array+ of  _Fixnum_ to be merged
    # [mode]    _Fixnum_ with values from _GlueMode_ A mode, describing how images should be merged.  
    def initialize(image1, image2,  mode)
        @images = [image1, image2]
        @mode = mode
    end
end
##
# :category: Public classes
# Behaves as infinitely big number if +number+ field is not initialized by number.
# Can be useful in finding minimum computation
class MaybeNumber
    ##
    #  _Fixnum_. Current value of number
    attr_accessor :number
    
    ##
    #  :category: Public interface
    # Initializes as infinitely big number
    def initialize()
       @number = nil
    end
    ##
    #  :category: Public interface
    # Returns true, if bigger than number
    # [number] _Fixnum_ numbet to compare with
    def >(number)
        if (self.valid?() == false)
            return true
        else
            return (@number > number)
        end
    end
    ##
    #  :category: Public interface
    # Returns true, if equal to number
    # [number] _Fixnum_ numbet to compare with
    def ==(number)
        if (self.valid?())
            return (@number-number).abs() < 0.1
        end
        return false
    end
    
    ##
    #  :category: Implementation. Deprecated to use
    # Returns true, if not infinitely big
    # [return] _TrueClass_ or _FalseClass_ true, if not infinitely big
    def valid?()
        return (@number.nil? == false)
    end
end

##
# :category: Implementation. DEPRECATED to use
# A task, which is used by metrics to find recursively an order. 
# A metric must merge all +entries+ into one, extending +orders+ array
class GlueTask
    ##
    # :category: Implementation. DEPRECATED to use
    # _Array_ of _GlueEntry_. entries to be merged into one
    attr_accessor :entries
    ##
    # :category: Implementation. DEPRECATED to use
    # _Array_ of _GlueOrder_. resulting order, which current entries are created by
    attr_accessor :orders
    ##
    # :category: Implementation. DEPRECATED to use
    # Creates a new task with specified entries and orders
    # [entries] _Array_ of _GlueEntry_ entries to be merged into one
    # [orders]  _Array_ of _GlueOrder_ resulting order, which current entries are created by
    def initialize(entries, orders)
        @entries = entries
        @orders = orders
    end
    ## 
    # :category: Implementation. DEPRECATED to use
    # Applies a new order, merging some specified in order images into one and adding it to an end
    # of resulting task and adding order to a part
    # [order]  _GlueOrder_  order to be applied
    # [return] _GlueTask_ resulting task
    def applyOrder(order)
        entries = @entries.clone()
        orders = @orders.clone()
        entries = entries << GlueEntry.merge(entries, order)
        entries.delete_at(order.images[1])
        entries.delete_at(order.images[0])
        orders = orders << order
        return GlueTask.new(entries, orders)
    end
end
##
# :category: Implementation. DEPRECATED to use
# An abstract, metric, that finds best order to merge an images into one
class GlueMetric

    ##
    # :category: Implementation. DEPRECATED to use
    # Computes a maximum between size dimensions specified by +index+ between images, specified by +order+ 
    # on an entries of +entries+
    # [entries] _Array_ of _GlueEntry_ source array of entry
    # [order]   _GlueOrder_ source order to be merged
    # [index]   _Fixnum_    0 - for width, 1 - for height
    # [return]  _Fixnum_    computation result
    def self.maxMerge(entries, order, index)
        return [ entries[order.images[0]].size()[index] , entries[order.images[1]].size()[index] ].max
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Computes a sum between size dimensions specified by +index+ between images, specified by +order+ 
    # on an entries of +entries+
    # [entries] _Array_ of _GlueEntry_ source array of entry
    # [order]   _GlueOrder_ source order to be merged
    # [index]   _Fixnum_    0 - for width, 1 - for height
    # [return]  _Fixnum_    computation result
    def self.sumMerge(entries, order, index)
        return entries[order.images[0]].size()[index] + entries[order.images[1]].size()[index] 
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Computes a metric for getting best order. If metric value is smaller - the order is better
    # [entries] _Array_ of _GlueEntry_  entries to be merged
    # [order]   _GlueOrder_ test order to be merged
    # [return]  _Fixnum_ metric value
    def getMetric(entries, order)
        raise 'Not implemented!'
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Finds all possible orders, where metric is minimum
    # [entries] _Array_ of _GlueEntry_  source entrues to be merged
    # [return]  _Array_ of _GlueOrder_  all possible orders, where metric is minimum
    def findMinMetricOrder(entries)
        possible_orders = []
        min = MaybeNumber.new()
        # Search all combination
        entries.each_index{
            |i1|
            entries.each_index {
                |i2|
                if (i2>i1)
                    # Check all possible combination of two entries
                    combinations = [ GlueOrder.new(i1,i2, GlueMode::HORIZONTAL), GlueOrder.new(i1,i2, GlueMode::VERTICAL) ]; 
                    combinations.each{
                        |combination|
                        
                        # Scans metric
                        metric = self.getMetric(entries, combination)
                        if (min>metric == true)
                            min.number = metric
                            possible_orders = [ combination ] 
                        else
                            if (min == metric)
                                possible_orders = (possible_orders << combination)
                            end
                        end
                    }
                end
            }
        }
        return possible_orders
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Finds an all possible orders with minimal metric
    # Can return nil if no entries supplied. Also can be returned [ [ [], [w,h]]] if one image.
    # [entries] _Array_ of _GlueEntry_  all possible entries to be merged
    # [return]  _Array_ of _Array_ of Array of _GlueOrder_ and _Array_ of _Fixnum_ [ Array[orders, Array[width, height]] ] . 
    def findOrder(entries)
        # If no entries is specified nothing is found
        if (entries.length==0)
            return nil
        end
        # If only one entry - nothing can be found, so no entries is used
        if (entries.length==1)
            return [[[],[entries[0].size[0],entries[0].size[1]] ]]
        end
        # Function returns only finished tasks
        finishedtasks = []
        tasks = [ GlueTask.new(entries, []) ]
        i = 0
        while (i!= tasks.length)
            task = tasks[0]
            tasks.delete_at(0)
            # If task is finished
            if (task.entries.length == 1)
                finishedtasks = finishedtasks << ( [ task.orders, [task.entries[0].size()[0],task.entries[0].size()[1]] ] )
            else
                # If not finished add new task, with min orders
                orders = self.findMinMetricOrder(task.entries)
                orders.each{
                    |order|
                    tasks = tasks << task.applyOrder(order)
                }
            end
        end
        return finishedtasks
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Scans for minimum order in array of GlueMetric::findOrder results (results parameter) 
    # and finds metric width minimum from maximum from width and height, with given Array[MaybeNumber, result]
    # [start]   _Array_ of _MaybeNumber_ and _Array_ of _GlueOrder_ result of computing previous minimum texture
    # [results] _Array_ of _Array_ of _GlueOrder_ and _Array_ of _Fixnum_  an array of results as [ Array of GlueOrder, [width,height]]
    def self.findMinimumOrder(start, results )
        min = start[0]
        resultorder =  start[1]
        results.each{
            |result|
            
            # Extract minimum
            maxValue = result[1].max()
            if (min>maxValue)
                resultorder = result
                min.number = maxValue
            end
        }
        return [min, resultorder]
    end
end
##
# :category: Implementation. DEPRECATED to use
# A type of metric, that minimizes area of resulting texture
class MinAreaMetric < GlueMetric
    ##
    # :category: Implementation. DEPRECATED to use
    # Computes a metric as an area of result texture, obtained from merging data
    # [entries] _Array_ of _GlueEntry_  entries to be merged
    # [order]   _GlueOrder_ test order to be merged
    # [return]  _Fixnum_ metric value
    def getMetric(entries, order)
        w = 0 
        h = 0
        if (order.mode == GlueMode::HORIZONTAL)
            w = GlueMetric.sumMerge(entries, order, 0)
            h = GlueMetric.maxMerge(entries, order, 1) 
        else
            w = GlueMetric.maxMerge(entries, order, 0)   
            h = GlueMetric.sumMerge(entries, order, 1) 
        end
        return w*h
    end
end
##
# :category: Implementation. DEPRECATED to use
# A type of metric, that minimizes differences between height and width
class MinDiffMetric < GlueMetric
    ##
    # :category: Implementation. DEPRECATED to use
    # Computes a metric for describing as a max(h1,h1)*(w2-w1), where h and w various parameters 
    # [entries] _Array_ of _GlueEntry_  entries to be merged
    # [order]   _GlueOrder_ test order to be merged
    # [return]  _Fixnum_ metric value
    def getMetric(entries, order)
        w = 0 
        h = 0
        # Get entries
        entry1 = entries[order.images[0]]
        entry2 = entries[order.images[1]]
        # Get sizes
        size1 = [entry1.size()[0],entry1.size()[1]]
        size2 = [entry2.size()[0],entry2.size()[1]]
        # Handle horizontal merge
        if (order.mode == GlueMode::HORIZONTAL)
            if (size1[1] > size2[1])
                w = size2[0]
                h = size1[1] - size2[1]
            else
                w = size1[0]
                h = size2[1] - size1[1]
            end
        else
            # Handle vertical merge
            if (size1[0] > size2[0])
                w = size1[0] - size2[0]
                h = size2[1] 
            else
                w = size2[0] - size1[0]
                h = size1[1] 
            end
        end
        return w*h
    end
end
##
# :category: Implementation. DEPRECATED to use
# An entry, which holds a texture size and used in algorithm instead of texture 
# in computing sizes
class GlueEntry

    ##
    # :category: Implementation. DEPRECATED to use
    # Initialises an entry from texture
    # [texture] _Texture_ texture, where size can be taken from
    def intialize(texture)
        @size = texture.size()
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Initialises entry by width and height
    # [w] _Fixnum_ width of entry
    # [h] _Fixnum_ height of entry
    def initialize(w,h)
        @size = Array[w,h]
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Performs a deep clone of entry
    # [return] _GlueEntry_ new entry 
    def clone()
        return GlueEntry.new(@size[0], @size[1])
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Merges a two entries into one, using specified order order
    # [entries] _Array_ of _GlueEntry_ input entries array, textures from it is taken by indexes from order
    # [order]   _GlueOrder_            order, in which they must be merged
    # [return]  _GlueEntry_            new entry, with two images merged
    def self.merge(entries, order)
        w = 0 
        h = 0
        # Handle a horizontal merge
        if (order.mode == GlueMode::HORIZONTAL)
            w = GlueMetric.sumMerge(entries, order, 0)
            h = GlueMetric.maxMerge(entries, order, 1) 
        else
           #Handle vertical merge
           w = GlueMetric.maxMerge(entries, order, 0)   
           h = GlueMetric.sumMerge(entries, order, 1) 
        end
        return GlueEntry.new(w, h)
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Returns a size of entry
    # [return] _Array_ of _Fixnum_ [width, height] of resulting image
    def size()
        return @size
    end
    
end
##
# :category: Public classes
# A class, which can find an order. Merging images on this order, must lead to a texture with minimum width and height
class ImageGluingOrder
    ##
    # :category: Public interface
    # Finds an order on specified array of images.
    # Merging by this order, must lead to image with minimal width and height.
    # [images] _Array_ of _Texture_ resulting image
    # [return] _Struct_(+order+ => _Array_ of _GlueOrder_, size => _Array_ of _Fixnum_) Resulting order and size. Size is Array [width, height]
    def find(images)
        # If no images specified, return empty order and texture with size [0,0]
        if (images.length == 0)
            return [ [], [0,0] ]
        end
        # If one images, order is empty, but size is taken from it
        if (images.length == 1)
            return [ [], [images[0].size()[0],images[0].size()[1]] ]
        end
       
        # Create a metric
        minAreaResults = MinAreaMetric.new().findOrder(images)
        minDiffResults = MinDiffMetric.new().findOrder(images)
        
        # Get minimum order, from two metrics
        # The first parameter of array is current minimum value (infinite)
        # The second is current GlueOrder array (nil, because nothing found)
        min = [MaybeNumber.new() , nil]
        min = GlueMetric.findMinimumOrder(min,minAreaResults)
        min = GlueMetric.findMinimumOrder(min,minDiffResults)
        # Create a resulting order
        glueOrderResult = Struct.new("GlueOrderResult",:order, :size)
        result = glueOrderResult.new(min[1][0], min[1][1])
        return result
    end
end