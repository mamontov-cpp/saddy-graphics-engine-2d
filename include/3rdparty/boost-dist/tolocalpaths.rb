# TODO: Here should be a script for converting boost paths to local

def getFiles(dir)
  Dir[ File.join(dir, '**', '*') ].reject { |p| File.directory? p }
end

getFiles('boost').each{ |x|
	includecount = x.split('/').length - 1
	prepend = "../" * includecount
	text = File.read(x)
	if (x != "boost/config/select_compiler_config.hpp")
		if (x != "boost/config/select_stdlib_config.hpp")
			if (x != "boost/config/select_platform_config.hpp")
				if (x == "boost/mpl/aux_/include_preprocessed.hpp")
					text = text.gsub(/#[ ]*include[ ]*BOOST_PP_STRINGIZE\((boost\/[^)]+)\)/, '#include BOOST_PP_STRINGIZE(' + prepend + '\1)')
					text = text.gsub(/#[ ]*define[ ]*AUX778076_INCLUDE_STRING BOOST_PP_STRINGIZE\((boost\/[^)]+)\)/, '#define AUX778076_INCLUDE_STRING BOOST_PP_STRINGIZE(' + prepend + '\1)')
				end
				text = text.gsub(/#[ ]*include[ ]*<(boost\/[^>]+)>/, '#include "' + prepend + '\1"')
				text = text.gsub(/#[ ]*include[ ]*\"(boost\/[^"]+)\"/, '#include "' + prepend + '\1"')
				text = text.gsub(/#[ ]*define ([A-Z_]+) <(boost\/[^>]+)>/, '#define \1 "' + prepend + '\2"')
			else
				text = text.gsub(/#[ ]*define[ ]*BOOST_PLATFORM_CONFIG[ ]*\"(boost\/[^"]+)\"/, '#define BOOST_PLATFORM_CONFIG "../\1"')
				text = text.gsub(/#[ ]*include[ ]*<(boost\/[^>]+)>/, '#include "../\1"')
				text = text.gsub(/#[ ]*include[ ]*\"(boost\/[^"]+)\"/, '#include "../\1"')			
			end
		else
			text = text.gsub(/#[ ]*define[ ]*BOOST_STDLIB_CONFIG[ ]*\"(boost\/[^"]+)\"/, '#define BOOST_STDLIB_CONFIG "../\1"')
			text = text.gsub(/#[ ]*include[ ]*<(boost\/[^>]+)>/, '#include "../\1"')
			text = text.gsub(/#[ ]*include[ ]*\"(boost\/[^"]+)\"/, '#include "../\1"')
		end
	else
		text = text.gsub(/#[ ]*define[ ]*BOOST_COMPILER_CONFIG[ ]*\"(boost\/[^"]+)\"/, '#define BOOST_COMPILER_CONFIG "../\1"')
		text = text.gsub(/#[ ]*include[ ]*<(boost\/[^>]+)>/, '#include "../\1"')
		text = text.gsub(/#[ ]*include[ ]*\"(boost\/[^"]+)\"/, '#include "../\1"')
	end
	#print text
	File.write(x, text)
};