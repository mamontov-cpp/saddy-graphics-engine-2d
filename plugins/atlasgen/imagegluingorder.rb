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
# A class, which can find an order to merge an array of images into one
class ImageGluingOrder
    # Finds an order on specified array
    # * param images Array of Texture image
    # * return Array of GlueOrder data
    def find(images)
    end
end