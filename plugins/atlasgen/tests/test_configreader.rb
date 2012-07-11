# Contains unit-tests for configuration reading module
# A unit-test for this module is placed here
load    'configreader.rb'
require 'test/unit'

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
    # Tests empty  valid config
    def testFirstInvalidConfig()
        filename = "test_xml/tcfg1.xml"
        assert( @obj.read(filename) != nil , @obj.getErrors().join("\n"))
        assert( @obj.getErrors().length == 1 )
        
    end
    #  Does nothing
    def teardown()
    end
end