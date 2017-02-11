content =  File.open("lib.js", 'rb') { |f| f.read }
data = content.split("\r\n");
data = data.map{ |x|  ("\"" + x.gsub(/\\/, "\\\\\\").gsub('"', '\\"') + "\\n\"") }.join("\r\n")
data = "#include<string>\r\nextern const std::string __context_eval_info;\r\nconst std::string __context_eval_info = std::string(" + data + ");"
File.open("lib.js.cpp", 'wb') { |file| file.write(data) }