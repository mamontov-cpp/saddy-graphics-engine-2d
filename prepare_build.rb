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

replace_in_file("tools/isqt580/isqt580.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("tools/isqt580/isqt580.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
replace_in_file("tools/isqt580/isqt580.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")

replace_in_file("plugins/qt-widget/qt-widget.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("plugins/qt-widget/qt-widget.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
replace_in_file("plugins/qt-widget/qt-widget.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")

replace_in_file("examples/example-game-in-qt/example-game-in-qt.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("examples/example-game-in-qt/example-game-in-qt.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
replace_in_file("examples/example-game-in-qt/example-game-in-qt.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")
