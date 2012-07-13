# Contains unit-tests for texture array module

load    'texturearray.rb'
require 'test/unit'
require 'FileUtils'

class TextureArrayTest < Test::Unit::TestCase
       
    def setup()
        @obj = TextureArray.new
        @tex = Texture.new("test_imgs/lena.jpg")
        @tex.load
        @tex2 = Texture.new("test_imgs/kde.png")
        @tex3 = Texture.new("test_imgs/gnu.png")
        @tex4 = Texture.new("test_imgs/tux.png")
    end
    
    
    
    def testPushUnique_containsTextureEmptyNameTexture()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(!@obj.containsTexture(''), 'Contains empty name texture')
    end
    
    def testPushUnique_containsTextureNonExistingTexture()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(!@obj.containsTexture('txtr'), 'Contains non-existing texture')
    end
    
    def testPushUnique_containsTextureSuccess()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(@obj.containsTexture('test_imgs/tux.png'), 'Contains must be successful')
    end

    def testPushUnique_getTextureEmptyNameTexture()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(!@obj.getTexture(''), 'GetTexture empty name texture')
    end
    
    def testPushUnique_getTextureSuccess()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(@obj.getTexture('test_imgs/gnu.png').eql?(@tex3), 'GetTexture corrupts texture')
    end
    
    def testPushUnique_getTextureNonExistingTexture()
        @obj.clear
        @obj.pushUnique(@tex)
        @obj.pushUnique(@tex2)
        @obj.pushUnique(@tex3)
        @obj.pushUnique(@tex4)
        assert(!@obj.getTexture('non-existing'), 'GetTexture returns non-existing texture')
    end
    
end


