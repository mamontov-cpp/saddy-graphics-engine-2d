#!/usr/bin/ruby -w


def replace_in_file(file_name, regex, entry)
  text = File.read(file_name)
  new_contents = text.gsub(regex, entry)
  File.open(file_name, "w") {|file| file.puts new_contents }
end

# Patch tools/ifaceed/ifaceed/ifaceed.vcxproj.user
replace_in_file("tools/ifaceed/ifaceed/ifaceed.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("tools/ifaceed/ifaceed/ifaceed.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
replace_in_file("tools/ifaceed/ifaceed/ifaceed.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")
