# Contains unit-tests for configuration writing module
# A unit-test for this module is placed here
load    'configreader.rb'
load    'configwriter.rb'
require 'test/unit'

class TestConfigWriter  < Test::Unit::TestCase

    def testPassthrough()
        reader = ConfigReader.new()
        writer = ConfigWriter.new()
        config = reader.read('test_xml/passthrough.xml')
        assert( config!=nil, reader.getErrors().join("\n") )
        config.getTextures().each{ |tex| tex.textureRectangle = [120,120,120,120] }
        result = writer.write(config, reader.getOutputConfigName(), reader.getOutputTextureName())
        assert( result == true, writer.getErrors().join("\n") ) 
    end
end