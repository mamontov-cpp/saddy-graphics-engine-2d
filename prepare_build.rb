#!/usr/bin/ruby -w

# Patch tools/ifaceed/ifaceed/ifaceed.vcxproj.user

file_name = "tools/ifaceed/ifaceed/ifaceed.vcxproj.user"
text = File.read(file_name)
new_contents = text.gsub(/<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
File.open(file_name, "w") {|file| file.puts new_contents }