echo File should not exist
atlasgen-debug -xml -with-index 1.xml
echo Should fail with parse error
atlasgen-debug -xml -with-index tests/atlasgen/xml/fail1.xml
atlasgen-debug -xml -with-index tests/atlasgen/xml/fail2.xml
atlasgen-debug -xml -with-index tests/atlasgen/xml/fail3.xml
echo Should fail - config is not specified
atlasgen-debug -xml -with-index tests/atlasgen/xml/no_config.xml
echo Should fail - texture is not specified
atlasgen-debug -xml -with-index tests/atlasgen/xml/no_texture.xml
echo Should fail - both texture and config are not specified
atlasgen-debug -xml -with-index tests/atlasgen/xml/no_texture_and_config.xml
echo Should fail - texture cannot be loaded
atlasgen-debug -xml -with-index tests/atlasgen/xml/tcfg1.xml
echo Should generate simple result
atlasgen-debug -xml -with-index tests/atlasgen/xml/valid.xml