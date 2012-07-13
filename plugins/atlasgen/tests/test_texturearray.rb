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
    

    def testPushUnique_pushUniqueEmptyName()
        @obj.clear
        prevSize = @obj.size
        @obj.pushUnique('',@tex)
        afterSize = @obj.size
        assert(prevSize==afterSize, 'Inserting texture with empty name')
    end
    
    def testPushUnique_pushUniqueNilTexture()
        @obj.clear
        prevSize = @obj.size
        @obj.pushUnique('name',nil)
        afterSize = @obj.size
        assert(prevSize==afterSize, 'Inserting nil texture')
    end
    
    def testPushUnique_containsTextureEmptyNameTexture()
        @obj.clear
        @obj.pushUnique('tex',@tex)
        @obj.pushUnique('tex2',@tex2)
        @obj.pushUnique('tex3',@tex3)
        @obj.pushUnique('tex4',@tex4)
        assert(!@obj.containsTexture(''), 'Contains empty name texture')
    end
    
    def testPushUnique_containsTextureNonExistingTexture()
        @obj.clear
        @obj.pushUnique('tex',@tex)
        @obj.pushUnique('tex2',@tex2)
        @obj.pushUnique('tex3',@tex3)
        @obj.pushUnique('tex4',@tex4)
        assert(!@obj.containsTexture('txtr'), 'Contains non-existing texture')
    end
    
    def testPushUnique_containsTextureSuccess()
        @obj.clear
        @obj.pushUnique('tex',@tex)
        @obj.pushUnique('tex2',@tex2)
        @obj.pushUnique('tex3',@tex3)
        @obj.pushUnique('tex4',@tex4)
        assert(@obj.containsTexture('tex2'), 'Contains must be successful')
    end

    def testPushUnique_getTextureEmptyNameTexture()
        @obj.clear
        @obj.pushUnique('tex',@tex)
        @obj.pushUnique('tex2',@tex2)
        @obj.pushUnique('tex3',@tex3)
        @obj.pushUnique('tex4',@tex4)
        assert(!@obj.getTexture(''), 'GetTexture empty name texture')
    end
    
    def testPushUnique_getTextureSuccess()
        @obj.clear
        @obj.pushUnique('tex',@tex)
        @obj.pushUnique('tex2',@tex2)
        @obj.pushUnique('tex3',@tex3)
        @obj.pushUnique('tex4',@tex4)
        assert(@obj.getTexture('tex3').eql?(@tex3), 'GetTexture corrupts texture')
    end
    
    def testPushUnique_getTextureNonExistingTexture()
        @obj.clear
        @obj.pushUnique('tex',@tex)
        @obj.pushUnique('tex2',@tex2)
        @obj.pushUnique('tex3',@tex3)
        @obj.pushUnique('tex4',@tex4)
        assert(!@obj.getTexture('non-existing'), 'GetTexture returns non-existing texture')
    end
    
end


