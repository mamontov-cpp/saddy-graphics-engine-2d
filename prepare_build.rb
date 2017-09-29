#!/usr/bin/ruby -w


def replace_in_file(file_name, regex, entry)
  text = File.read(file_name)
  new_contents = text.gsub(regex, entry)
  File.open(file_name, "w") {|file| file.puts new_contents }
end

replace_in_file("tools/atlasgen/atlasgen.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("tools/atlasgen/atlasgen.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")

replace_in_file("tools/exporter/exporter.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("tools/exporter/exporter.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")

replace_in_file("tools/ifaceed/ifaceed/ifaceed.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("tools/ifaceed/ifaceed/ifaceed.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
if ENV["PLATFORM"] != 'x64'
  replace_in_file("tools/ifaceed/ifaceed/ifaceed.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")
end

replace_in_file("tools/isqt580/isqt580.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("tools/isqt580/isqt580.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
if ENV["PLATFORM"] != 'x64'
  replace_in_file("tools/isqt580/isqt580.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")
end

replace_in_file("plugins/qt-widget/qt-widget.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("plugins/qt-widget/qt-widget.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
if ENV["PLATFORM"] != 'x64'
  replace_in_file("plugins/qt-widget/qt-widget.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")
end

replace_in_file("examples/example-game-in-qt/example-game-in-qt.vcxproj.user", /<QTDIR>[^<]+<\/QTDIR>/, "<QTDIR>$(QTDIR)</QTDIR>")
replace_in_file("examples/example-game-in-qt/example-game-in-qt.vcxproj", /<AdditionalIncludeDirectories>/, "<AdditionalIncludeDirectories>$(QTDIR)\\include\\QtANGLE;")
if ENV["PLATFORM"] != 'x64'
  replace_in_file("examples/example-game-in-qt/example-game-in-qt.vcxproj", /<AdditionalDependencies>/, "<AdditionalDependencies>WindowsApp.lib;")
end

# Comment pipeline test
replace_in_file("tests/dukpp-03/pipeline.cpp", /__pipeline_test/, "/*__pipeline_test*/")
replace_in_file("tests/dukpp-03/main.cpp", /\#define[ ]+TEST_INTERACTIVE/, "//#define TEST_INTERACTIVE")
