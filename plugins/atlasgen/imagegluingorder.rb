# Describes, how two images, should be merged
module GlueMode
    HORIZONTAL = 1
    VERTICAL = 2
end

# Describes what images should be merhed
class GlueOrder
    # Array of image index
    attr_accessor :images
    # GlueMode  how should images be merged
    attr_accessor :mode
    
    # Declares a merges with indexes
    # * int image1 index of first image
    # * int image2 index of second image
    # * int mode   mode numeric
    # return int part
    def initialize(image1, image2,  mode)
        @images = [image1, image2]
        @mode = mode
    end
end
# Behaves as infinitely big number if not initialized
class MaybeNumber
    # Inits as infinitely big metric
    attr_accessor :number
    
    def initialize()
       @number = nil
    end
    
    def >(number)
        if (self.valid?() == false)
            return true
        else
            return (@number > number)
        end
    end
    
    def ==(number)
        if (self.valid?())
            return (@number-number).abs() < 0.1
        end
        return false
    end
    
    def valid?()
        return (@number.nil? == false)
    end
end

# A task for gluing all data
class GlueTask
    # Array of Glue Entry to do
    attr_accessor :entries
    # Array of Glue Order of current order
    attr_accessor :orders
    
    # Creates a new task with specified entries and orders
    def initialize(entries, orders)
        @entries = entries
        @orders = orders
    end
    # Applies a new order, creating a new task
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
class GlueMetric

    # Computes a glue metric result as maximum metric
    # * Array of GlueEntry part to merge
    # * order what should be merged
    # * index what metric should be used (0 for width, 1 to height)
    def self.maxMerge(entries, order, index)
        return [ entries[order.images[0]].size()[index] , entries[order.images[1]].size()[index] ].max
    end
    
    # Computes a glue metric result as maximum metric
    # * Array of GlueEntry part to merge
    # * order what should be merged
    # * index what metric should be used (0 for width, 1 to height)
    def self.sumMerge(entries, order, index)
        return entries[order.images[0]].size()[index] + entries[order.images[1]].size()[index] 
    end
    
    # Computes a metric for describing what is maximum and what is not
    # * Array of GlueEntry part to merge
    # * order what should be merged
    def getMetric(entries, order)
        raise 'Not implemented!'
    end
    
    # Finds an order, where the metric is minimal
    # * param entries array of GlueEntry to merge
    # * return Array of new orders
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
    
    # Finds an order on all entries
    # * param entries Array of all entries to be merged
    # * return Array[ Array[orders, Array[width, height]] ] . Can return nil if no entries supplied. Also can be returned [ [ nil, [w,h]]] if one image
    def findOrder(entries)
        if (entries.length==0)
            return nil
        end
        if (entries.length==1)
            return [[nil,[entries[0].size[0],entries[0].size[1]]]]
        end
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
    
    # Scans for minimum order in array of GlueMetric::findOrder results (results parameter) and finds min, with given Array[MaybeNumber, result]
    def self.findMinimumOrder(start, results )
        min = start[0]
        resultorder =  start[1]
        results.each{
            |result|
            
            maxValue = result[1].max()
            if (min>maxValue)
                resultorder = result
                min.number = maxValue
            end
        }
        return [min, resultorder]
    end
end
# A metric, that minimizes a total square between two merged images
class MinAreaMetric < GlueMetric
     # Computes a metric for describing what is maximum and what is not
    # * Array of GlueEntry part to merge
    # * order what should be merged
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
# A metric, that minimizes a total diff between images
class MinDiffMetric < GlueMetric
     # Computes a metric for describing what is maximum and what is not
    # * Array of GlueEntry part to merge
    # * order what should be merged
    def getMetric(entries, order)
        w = 0 
        h = 0
        entry1 = entries[order.images[0]]
        entry2 = entries[order.images[1]]
        size1 = [entry1.size()[0],entry1.size()[1]]
        size2 = [entry2.size()[0],entry2.size()[1]]
        if (order.mode == GlueMode::HORIZONTAL)
            if (size1[1] > size2[1])
                w = size2[0]
                h = size1[1] - size2[1]
            else
                w = size1[0]
                h = size2[1] - size1[1]
            end
        else
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
# Represents an entry, used two glue two entry
class GlueEntry

    # Initialises an entry from texture
    # * texture texture entry
    def intialize(texture)
        @size = texture.size()
    end
    # Initialises entry as width and height
    def initialize(w,h)
        @size = Array[w,h]
    end
    
    def clone()
        return GlueEntry.new(@size[0], @size[1])
    end
    
    # Metges a two entries, from order
    # * param Array entries input entries data
    # * param GlueOrder order   order, in which they must be merged
    # * return GlueEntry merged glue entry
    def self.merge(entries, order)
        w = 0 
        h = 0
        if (order.mode == GlueMode::HORIZONTAL)
            w = GlueMetric.sumMerge(entries, order, 0)
            h = GlueMetric.maxMerge(entries, order, 1) 
        else
           w = GlueMetric.maxMerge(entries, order, 0)   
           h = GlueMetric.sumMerge(entries, order, 1) 
        end
        return GlueEntry.new(w, h)
    end
    
    # Returns a size of two entries, glued together
    # * Array[width, height]
    def size()
        return @size
    end
    
end
# A class, which can find an order to merge an array of images into one
class ImageGluingOrder
    # Finds an order on specified array
    # * param images Array of Texture image
    # * return Struct(order => Array of order, size => Array)
    def find(images)
        if (images.length == 0)
            return [ [], [0,0] ]
        end
        if (images.length == 1)
            return [ [], [images[0].size()[0],images[0].size()[1]] ]
        end
       
        
        minAreaResults = MinAreaMetric.new().findOrder(images)
        minDiffResults = MinDiffMetric.new().findOrder(images)
        
        # Get minimum object
        
        min = [MaybeNumber.new() , nil]
        min = GlueMetric.findMinimumOrder(min,minAreaResults)
        min = GlueMetric.findMinimumOrder(min,minDiffResults)
        
        glueOrderResult = Struct.new("GlueOrderResult",:order, :size)
        result = glueOrderResult.new(min[1][0], min[1][1])
        
        return result
    end
end