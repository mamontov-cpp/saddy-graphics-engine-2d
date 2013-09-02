##
# :title: imageglue.rb
# A class, that performs a texture rectangle computation, setting Texture::textureRectangle properties, according to gluing order
# and computes size of output texture
require_relative "imagegluingorder.rb"
##
# :category: Public classes
# A class, that performs a texture rectangle computation, setting Texture::textureRectangle properties, according to gluing order
# and computes size of output texture
class ImageArranger
    ##
    # :category: Implementation. DEPRECATED to use
    # A class, that holds current textures, which is glued by GlueOrder together
    class ImageBucket
        ## 
        # _Array_ of _Texture_.   Textures, glued together
        attr_accessor :images
        ## 
        # _Array_ of _Fixnum_.   Width and height of bucket as [w,h]
        attr_accessor :size   
        
        ##
        # :category: Public interface
        # Initializes bucket with one image or none. Used by arranger at algorithm initialization
        # [image] _Texture_ initialization texture, can be nil. In that case no initialization is performed 
        def initialize(image = nil)
            if (image.nil? == false)
                @images = [image]
                image.textureRectangle = [0,0, image.size[0], image.size[1]]
                @size = image.size()
            end
        end
        ##
        # :category: Implementation. DEPRECATED to use.
        # Shifts all images by the point, without modifying size. Used by merged to arrange one bucket against another. Point is Array[x,y]
        # [point] _Array_ of _Fixnum_ distance which would be added to coordinates of upper left corneer as [x,y]
        def shift(point)
            images.each{ |image|  
                image.textureRectangle[0] = image.textureRectangle[0] + point[0]
                image.textureRectangle[1] = image.textureRectangle[1] + point[1]
            }
        end
        ##
        # :category: Public interface
        # Merges two buckets into one in specified order
        # [bucket1] _ImageArranger::ImageBucket_ image bucket
        # [bucket2] _ImageArranger::ImageBucket_ image bucket
        # [order]   _GlueOrder_ order, in which they should be merged
        # [return]  _ImageArranger::ImageBucket_ resulting bucket
         def self.merge(bucket1, bucket2, order)
            
            bucketsize1 = bucket1.size
            bucketsize2 = bucket2.size
            # Our transformations must not affect bucket2 images
            result = ImageBucket.new()
            result.images = bucket1.images.clone
            result.images.concat(bucket2.images)
            
            if (order.mode == GlueMode::HORIZONTAL)
                # Handle horizontal merge
                result.size = [ (bucket1.size[0] + bucket2.size[0]), [bucket1.size[1] , bucket2.size[1]].max() ]
                bucket2.shift([bucket1.size[0],0])
            else
                # Handle vertical merge
                result.size = [ [bucket1.size[0] , bucket2.size[0]].max(), (bucket1.size[1] + bucket2.size[1])  ]
                bucket2.shift([0,bucket1.size[1]])
            end
            return result
        end
    end
    ##
    # :category: Implementation. DEPRECATED to use
    # Finds a nearesr bigger power of two to value 
    # [value] _Fixnum_ value to find nearest bigger power of two
    def self.nextPOT(value)
        if (value == 1)
            return 2
        end
        
        value = value - 1
        [1,2,4,8,16].each { |x|                 
            value = (value >> x) | value 
        }
        value = (value >> 32) | value if value.class == Bignum
        value = value + 1
        return value
    end
    
    public
    # :category: Public interface
    # Arranges an images in specified order.
    # Also changes images, setting their +textureRectangle+ parameters
    # [images]    _Array_ of _Texture_   images to be arranged by specified order
    # [order]     _Array_ of _GlueOrder_ order in which images should be arranged
    # [totalSize] _Array_ of _Fixnum_    Array[width,height] parameters of resulting images, which used to compute size of POT texture
    # [return]    _Fixnum_ width and height of resulting texture as one _Fixnum_ . Since it square POT texture, width and height are equal
    def arrange(images, order, totalSize)
        if (images.length>1)
            # Fill all buckets
            buckets = images.collect { |image| ImageArranger::ImageBucket.new(image) }
            # Foreach order in entry
            order.each{
                |entry|
                
                # Sort indexes descending
                indexes  = entry.images.clone
                indexes.sort!
                indexes.reverse!
                
                # Create a new bucket and append it  to end
                new_bucket = ImageBucket.merge(buckets[entry.images[0]],buckets[entry.images[1]],entry)
                indexes.each{ |index| buckets.delete_at(index) }
                buckets = buckets << new_bucket
            }
        elsif (images.length==1)
            # Since only one image, we assume, no glue orders is passed.
            images[0].textureRectangle = [0,0, images[0].size[0], images[0].size[1]]
        end
        # Compute and return size
        return ImageArranger.nextPOT(totalSize.max)
    end
end