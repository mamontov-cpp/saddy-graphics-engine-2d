# This class performs a texture rect computation and mutates input entry objects setting their coordinates, according to gluing order.
class ImageArranger


    def self.nextPOT(value)
        if (value == 1)
            return 2
        end
        
        value = value - 1
        [1,2,4,8,16].each { |x|                 
            value = (value >> x) | value 
        }
        value = (value >> 32) | val if self.class == Bignum
        value = value + 1
        return value
    end
    
    public
    # Arranges an images in specified order
    # Also mutates images, setting their texture rects
    # Second parameter is an array of GlueOrder objects, describing and order
    # Third parameter totalSize Array[width,height] parameters of resulting texture
    # Returns width and height of resulting texture as one integer (since it square POT texture it's all equal)
    def arrange(images, order, totalSize)
        if (images.length>1)
            
        elsif (images.length==1)
            images[0].textureRectangle = [0,0, images[0].size[0], images[0].size[1]]
        end
        return ImageArranger.nextPOT(totalSize.max)
    end
end