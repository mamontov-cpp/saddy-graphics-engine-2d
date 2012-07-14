##
# :title: test_texturearray.rb
# Tests for TextureArray class
load    'texturearray.rb'
require 'test/unit'
require 'FileUtils'
##
# :category: Tests
# Tests for TextureArray class
class TextureArrayTest < Test::Unit::TestCase
    
    ##
    # :category: Test Utilities
    # A data texture loading  performed here
    def setup()
        @obj = TextureArray.new
        @tex = Texture.new("test_imgs/lena.jpg")
        @tex.load
        @tex2 = Texture.new("test_imgs/kde.png")
        @tex3 = Texture.new("test_imgs/gnu.png")
        @tex4 = Texture.new("test_imgs/tux.png")
    end
    
    
    ##
    # :category: Tests
    # Tests a TextureArray::containsTexture  method on a case with empty name
    def testPushUnique_containsTextureEmptyNameTexture()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(!@obj.containsTexture(''), 'Contains empty name texture')
    end
    ##
    # :category: Tests
    # Tests a TextureArray::containsTexture method on a case when texture with specified name does not exists in array
    def testPushUnique_containsTextureNonExistingTexture()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(!@obj.containsTexture('txtr'), 'Contains non-existing texture')
    end
    ##
    # :category: Tests
    # Tests a TextureArray::containsTexture method on a case when we can get some texture
    def testPushUnique_containsTextureSuccess()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(@obj.containsTexture('test_imgs/tux.png'), 'Contains must be successful')
    end
    ##
    # :category: Tests
    # Tests a TextureArray::containsTexture method on a case when we can get some texture and checks it with existing texture
    def testPushUnique_getTextureSuccess()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(@obj.getTexture('test_imgs/gnu.png').eql?(@tex3), 'GetTexture corrupts texture')
    end
end


