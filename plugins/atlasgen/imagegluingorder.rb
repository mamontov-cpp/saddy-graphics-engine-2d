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
end

class GlueMetrics

    # Computes a glue metric result as maximum metric
    # * Array of GlueEntry part to merge
    # * order what should be merged
    # * index what metric should be used (0 for width, 1 to height)
    def maxMerge(entries, order, index)
        return [ entries[order.images[0]].size()[index] , entries[order.images[1]].size()[index] ].max
    end
    
    # Computes a glue metric result as maximum metric
    # * Array of GlueEntry part to merge
    # * order what should be merged
    # * index what metric should be used (0 for width, 1 to height)
    def sumMerge(entries, order, index)
        return entries[order.images[0]].size()[index] + entries[order.images[1]].size()[index] 
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
    def merge(entries, order)
        w = 0 
        h =0
        if (order.mode == GlueMode::HORIZONTAL)
            w = GlueMetrics.sumMerge(entries, order, 0)
            h = GlueMetrics.maxMerge(entries, order, 1) 
        else
           w = GlueMetrics.maxMerge(entries, order, 0)   
           h = GlueMetrics.sumMerge(entries, order, 1) 
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