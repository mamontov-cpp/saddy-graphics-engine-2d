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
    # * return Array of GlueOrder data
    def find(images)
    end
end