content =  File.open("lib.js", 'rb') { |f| f.read }
data = content.split("\r\n");
data = data.map{ |x|  ("\"" + x.gsub(/\\/, "\\\\\\").gsub('"', '\\"') + "\\n\"") }.join("\r\n")

content2 =  File.open("types.js", 'rb') { |f| f.read }
data2 = content2.split("\r\n");
data2 = data2.map{ |x|  ("\"" + x.gsub(/\\/, "\\\\\\").gsub('"', '\\"') + "\\n\"") }.join("\r\n")

data = "#include<string>\r\nextern const std::string __context_eval_info;\r\nextern const std::string __context_types_info;\r\nconst std::string __context_eval_info = std::string(" + data + ");\r\nconst std::string __context_types_info = std::string(" + data2 + ");\r\n"
File.open("lib.js.cpp", 'wb') { |file| file.write(data) }