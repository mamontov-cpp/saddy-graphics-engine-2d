##
# :title: test_configwriter.rb
# Tests for ConfigWriter class
load    'configreader.rb'
load    'configwriter.rb'
require 'test/unit'
##
# :category: Tests
# Tests for ConfigWrtier class
class TestConfigWriter  < Test::Unit::TestCase

    ##
    # :category: Tests
    # One big test, which loads a source valid config, tries to replace working some other classes with Image*
    # prefix and tries to write it to output file. You need to check manually it's results, it is not
    # guaranteed, that they are correct
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