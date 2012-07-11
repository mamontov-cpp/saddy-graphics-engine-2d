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
    #  Does nothing
    def teardown()
    end
end