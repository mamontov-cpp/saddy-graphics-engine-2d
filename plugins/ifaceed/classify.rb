require 'pp'

parts = "";
predefined = {
	"excluded" => ["centralWidget", "lblEditorState", "txtEditorState"]
};

excludedTypes = [
	"QLabel",
	"QWidget",
	"QVBoxLayout",
	"QHBoxLayout",
	"QGridLayout",
	"QGroupBox"
];

typesToIncludes = {
	"gui::resourcetreewidget::ResourceTreeWidget" => "../resourcetreewidget/resourcetreewidget.h",
	"gui::anglewidget::AngleWidget" => "../anglewidget/anglewidget.h",
	"gui::colorpicker::ColorPicker" => "../gui/colorpicker/colorpicker.h",
	"gui::colorview::ColorView" => "../gui/colorview/colorview.h",
	"gui::fontsizewidget::FontSizeWidget" => "../gui/fontsizewidget/fontsizewidget.h",
	"gui::rectwidget::RectWidget" => "../gui/rectwidget/rectwidget.h",
	"gui::textedit::TextEdit"  => "../gui/textedit/textedit.h",
	"gui::tuplewidget::DoubleTupleWidget" => "../gui/tuplewidget/doubletuplewidget.h",
	"gui::tuplewidget::Int64TupleWidget" => "../gui/tuplewidget/int64tuplewidget.h",
	"gui::codeedit::CodeEdit" => "../gui/textedit/codeedit.h"
}

alreadyDiscovered = {
	"Scene" => "Scene",
	"SceneNode" => "SceneNode",
	"Sprite" => "Sprite",
	"Label" => "Label",
	"Animation" => "Animation",
	"CustomObject" => "CustomObject",
	"Dialogue" => "Dialogue",
	"Phrase" => "Phrase",
	"AnimationsInstance" => "AnimationInstance",
	"AnimationInstance" => "AnimationInstance",
	"AnimationsGroup" => "AnimationsGroup",
	"Way" => "Way"
};

@classification = {};

if File.exist?("ui_classification.rb")
	load("ui_classification.rb")
end

text = File.read("ifaceed/GeneratedFiles/ui_mainpanel.h")
matches = /public\:([^v]|v[^o]|vo[^i]|voi[^d])+/m.match(text)
matches = matches
		  .to_s
		  .gsub("public:", "")
		  .split("\n")
		  .map{ |x| x.strip.gsub("\n", "").gsub("\r", "").gsub(";", "").gsub(/(^[ \t]+)|([ \t]+$)/, "").gsub(" *", " ").split(" ") }
		  .reject{ |x| x.length == 0}
	
def foundInClassification(t, hash)	
	result = false
	hash.each_value{
		|value|
		
		value.each{
			|avalue|
			
			result = result || avalue[1] == t
		}
	}
	return result
end


def findInMap(t, hash)	
	result = nil
	hash.each_pair{
		|key, value|
		
		if value.include?(t)
			result = key
		end
	}
	return result
end

groups = alreadyDiscovered.keys | @classification.keys

def returnGroupIfAlreadyDiscovered(t, groups)
	result = []
	groups.each_key{ |x|
		if t.include?(x)
			if (result.length > 0)
				if (result[0].length < x.length)
					result[0] = x
				end
			else
				result.push(x)
			end
		end
	}
	if (result.length == 0)
		return nil
	else
		return groups[result[0]]
	end
end

def pollGroup(match, groups)
	print "What groups " + match[0] + "*" + " " + match[1] + " belongs to?\n";
	groups.each_index{
		|x|
		print x.to_s + ") " + groups[x] + "\n"
	}
	print (groups.length).to_s + ") new group\n"
	val = gets.to_i
	if ((val < 0) || (val >= groups.length))
		print "Enter new group: \n"
		val = gets.gsub("\r", "").gsub("\n", "").strip
		groups.push(val)
		return val
	else
		return groups[val]
	end
end

def printNamespaces(tree)
	result = ""
	tree.each_pair{
		|k, v|
		if (v.length == 0)
			result << "class " + k + ";\n\n"
		else
			result << "namespace " + k + "\n{\n\n"
			result << printNamespaces(v)
			result << "}\n\n"
		end
	}
	return result
end

# Classify matches by groups
mlen = matches.length - 1;
for i in 0..mlen do
	if (foundInClassification(matches[i][1], @classification) == false)
		if (excludedTypes.include?(matches[i][0]) == false)
			pos = findInMap(matches[i][1], predefined)
			if (pos.nil?)
				pos = returnGroupIfAlreadyDiscovered(matches[i][1], alreadyDiscovered)
				if (pos.nil?)
					pos = pollGroup(matches[i], groups)
				end
			end
			if (pos.nil? == false)
				if (@classification.key?(pos) == false)
					@classification[pos] = []
				end
				@classification[pos].push(matches[i])
			end
		else
			if (@classification.key?("excluded") == false)
				@classification["excluded"] = []
			end
			@classification["excluded"].push(matches[i])
		end
	end
end


# Write classification to file
File.write("ui_classification.rb", "@classification = " + @classification.pretty_inspect)


# uiblocks.h generation
uiblocksHeaderFileName = "uiblocks.h"
uiblocksHeaderFileContent = "/* \\file uiblocks.h
	
	Contains data about all UIBlocks, used in the program
*/	

namespace gui 
{

namespace uiblocks
{\n\n
"

@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	
	if key != "excluded"
		uiblocksHeaderFileContent += "class UI" + key + "Block;\n";
	end
}

uiblocksHeaderFileContent += "\n\n
/*! A list of all UIBlocks of editor
 */
class UIBlocks
{
public:
	/*! Constructs new UIBlocks
	 */
	UIBlocks();
	/*! Inits with widgets of uiblocks
		\\param[in] w widgets
	 */
	void init(QWidget* w);
	/*! Destroys object
	 */
	~UIBlocks();
"

@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	
	if key != "excluded"
		uiblocksHeaderFileContent += "" + 
		"\t/*! Returns UI group for " + key + " group of widgets\n\t    \\return UI group for " + key + " objects\n\t*/\n" + 
		"\tinline gui::uiblocks::UI" + key + "Block* UI" + key + "Block()\n\t{\n\t\treturn m_ui_" + lowerkey + "_block;\n\t}\n";
	end
}

uiblocksHeaderFileContent += "protected:\n"
@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	
	if key != "excluded"
		uiblocksHeaderFileContent += "" + 
		"\t/*! An UI group for " + key + " group of widgets\n" + "\t*/\n" + 
		"\tgui::uiblocks::UI" + key + "Block* m_ui_" + lowerkey + "_block;\n";
	end
}

uiblocksHeaderFileContent += "};

}

}
"

# uiblocks.cpp code generation

uiblocksSourceFileContent = "#include \"uiblocks.h\"\n"
@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	
	if key != "excluded"
		uiblocksSourceFileContent += "#include \"ui" + lowerkey + "block.h\"\n";
	end
}

uiblocksSourceFileContent += "\n\n"

uiblocksSourceFileContent += "gui::uiblocks::UIBlocks::UIBlocks() : " 

data = [];
@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	
	if key != "excluded"
		data.push("m_ui_" + lowerkey + "_block(new gui::iblocks::UI" + key + "Block())");
	end
}

uiblocksSourceFileContent += data.join(",\n") + "\n"
uiblocksSourceFileContent += "{\n\n}\n\n"

uiblocksSourceFileContent += "void gui::uiblocks::UIBlocks::init(QWidget* w)\n{\n"
@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	
	if key != "excluded"
		uiblocksSourceFileContent += "\tm_ui_" + lowerkey + "_block->init(w);\n";
	end
}
uiblocksSourceFileContent += "}\n"

uiblocksSourceFileContent += "\nvoid gui::uiblocks::UIBlocks::~UIBlocks()\n{\n"
@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	
	if key != "excluded"
		uiblocksSourceFileContent += "\tdelete m_ui_" + lowerkey + "_block;\n";
	end
}
uiblocksSourceFileContent += "}\n"


if (File.exist?("ifaceed/gui/uiblocks/") == false || File.directory?("ifaceed/gui/uiblocks/") == false)
	Dir.mkdir("ifaceed/gui/uiblocks/")
end

File.write("ifaceed/gui/uiblocks/uiblocks.h", uiblocksHeaderFileContent)
File.write("ifaceed/gui/uiblocks/uiblocks.cpp", uiblocksSourceFileContent)

# Write headers
@classification.each_pair{
	|key, value|
	lowerkey = key.downcase
	if key != "excluded"
		different_headers = []
		basic_headers = []
		value.each{
			|x|
			if (typesToIncludes.key?(x[0]))
				if (different_headers.include?(x[0]) == false)
					different_headers.push(x[0])
				end
			else
				if (basic_headers.include?(x[0]) == false)
					basic_headers.push(x[0])
				end
			end
		}
		
		headerFileContent = "/* \\file ui" + lowerkey + "block.h\n\n"  +		
							"\tContains definition of UI group for " + key + " group of widgets\n" +
							" */\n\n"
		if (different_headers.length > 0)
			tree = {}
			different_headers.each{
				|header|
				current = tree
				header.split("::").each{
					|header_part|
					if (current.key?(header_part) == false)
						current[header_part] = Hash.new
					end
					current = current[header_part]
				}
			}
			headerFileContent += printNamespaces(tree)
			headerFileContent += "\n"
		end
		
		basic_headers.each{
			|basic_header|
			headerFileContent += "class " +  basic_header + ";\n"
		}
		
		headerFileContent += "\n\n"
		headerFileContent += "namespace gui\n{\n\n"
		headerFileContent += "namespace uiblocks\n{\n\n"
		
		headerFileContent += "class UI" + key + "Block\n{\n\n"
		
		headerFileContent += "};\n\n"	
		headerFileContent += "}\n\n"	
		headerFileContent += "}\n"
		
		
		headerSourceContent = "#include \"ui" + lowerkey + "block.h\"\n"
		
		File.write("ifaceed/gui/uiblocks/ui" + lowerkey + "block.h", headerFileContent)
		File.write("ifaceed/gui/uiblocks/ui" + lowerkey + "block.cpp", headerSourceContent)
	end
}
