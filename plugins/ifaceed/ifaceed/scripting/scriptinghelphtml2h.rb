content =  File.open("scriptinghelp.html", 'rb') { |f| f.read }
data = content.split("\r\n");
data = data.map{ |x|  ("\"" + x + "\"") }.join("\r\n")
data = "QString scripting_help = " + data + ";"
File.open("scriptinghelptext.h", 'wb') { |file| file.write(data) }