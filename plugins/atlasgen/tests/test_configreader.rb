# Contains unit-tests for configuration reading module
# A unit-test for this module is placed here
load    'configreader.rb'
require 'test/unit'


class TestTexture < Test::Unit::TestCase

    def testSize()
        tex = Texture.new("test_imgs/lena.jpg")
        tex.load()
        assert(tex.size()[0] == 256, tex.size()[0].to_s())
        assert(tex.size()[1] == 256, tex.size()[1].to_s())
    end
end

class TestConfigReader < Test::Unit::TestCase

    #  Inits entry array with test entries
    def setup()
        @obj = ConfigReader.new()
    end
    
    # Tests non-existent file
    def testFileNoExists()
        filename = "<>sia)"
        assert( @obj.read(filename) == nil )
        assert (@obj.getErrors()[0] == "Cannot open file: " + filename)
        
        filename = "/wiaaa"
        assert( @obj.read(filename) == nil )
        assert (@obj.getErrors()[0] == "Cannot open file: " + filename)
    end
    # Test invalid file parts
    def testInvalidFails()
        filename = "test_xml/fail1.xml"
        assert( @obj.read(filename) == nil )
        assert (@obj.getErrors()[0] == "Cannot read file: " + filename)
        
        filename = "test_xml/fail2.xml"
        assert( @obj.read(filename) == nil )
        assert (@obj.getErrors()[0] == "Cannot read file: " + filename)
        
        filename = "test_xml/fail3.xml"
        assert( @obj.read(filename) == nil )
        assert (@obj.getErrors()[0] == "Cannot read file: " + filename)
        
    end
    # Test non full description
    def testNonFull()
        filename = "test_xml/no_texture_and_config.xml"
        assert( @obj.read(filename) == nil )
        assert( @obj.getErrors().length == 2 )
        
        filename = "test_xml/no_texture.xml"
        assert( @obj.read(filename) == nil )
        assert( @obj.getErrors().length == 1 )
        
        filename = "test_xml/no_config.xml"
        assert( @obj.read(filename) == nil )
        assert( @obj.getErrors().length == 1 )
    end
    # Tests empty  valid config
    def testEmptyValidConfig()
        filename = "test_xml/emptyconfig.xml"
        assert( @obj.read(filename) != nil )
        assert( @obj.getErrors().length == 0 )
        assert( @obj.getOutputConfigName() == "a.xml",@obj.getOutputConfigName() )
        assert( @obj.getOutputTextureName() == "a.png" )
    end
    # Tests config with absent texture
    def testFirstInvalidConfig()
        filename = "test_xml/tcfg1.xml"
        assert( @obj.read(filename) == nil , @obj.getErrors().join("\n"))
        assert( @obj.getErrors().length == 1 )
        
    end
    # Tests some valid config loading
    def testValidConfig()
        filename = "test_xml/valid.xml"
        config = @obj.read(filename)
        assert( config != nil , @obj.getErrors().join("\n"))
        assert( @obj.getErrors().length == 0 )
        assert( @obj.getOutputConfigName() == "a.xml",@obj.getOutputConfigName() )
        assert( @obj.getOutputTextureName() == "a.png" )
        textures = config.getTextures()
        entries = config. getConfigArray()
        assert( textures.length == 1 )
        assert( entries.length == 1 )
        assert( textures[0].name == 'test_imgs\kde.png' )
        assert( entries[0].inputTextureName == 'test_imgs\kde.png' )
        assert( entries[0].size == [64,64] )
        assert( entries[0].transparent == [0,102,404] )
        assert( entries[0].name == 'itest' )
        assert( entries[0].index == 500 )
    end
    
    #  Does nothing
    def teardown()
    end
end