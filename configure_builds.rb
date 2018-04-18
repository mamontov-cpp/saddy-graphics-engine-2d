@qtIsEnabled = true;

def replace_and_put(file_name, out_file_name, regex, entry)
  text = File.read(file_name)
  new_contents = text.gsub(regex, entry)
  File.open(out_file_name, "w") {|file| file.puts new_contents }
end

ARGV.each do |item|
  if (item == "-qt")
    @qtIsEnabled = true
  end
  if (item == "-no-qt")
    @qtIsEnabled = false
  end
end

textLinuxGCC = File.read("build-all-linux-gcc.sh.template")
if (!@qtIsEnabled)
  textLinuxGCC = textLinuxGCC.gsub("cd tools/atlasgen\nqmake CONFIG+=$QtConfig atlasgen.pro\nmake\ncd ../..\n", "")
  textLinuxGCC = textLinuxGCC.gsub("cd tools/exporter\nqmake CONFIG+=$QtConfig exporter.pro\nmake\ncd ../..\n", "")
  textLinuxGCC = textLinuxGCC.gsub("cd tools/ifaceed\nqmake CONFIG+=$QtConfig ifaceed.pro\nmake\ncd ../..\n", "")
  replaceString = "cd tools/isqt580\nqmake CONFIG+=$QtConfig isqt580.pro\nmake\ncd ../..\neval $CheckQtVerTool\n"
  replaceString += "if [ $? -eq 1 ] \nthen\n"
  replaceString += "	cd plugins/qt-widget\n	qmake CONFIG+=$QtConfig qt-widget.pro\n	make\n	cd ../..\n	\n"
  replaceString += "	cd examples/example-game-in-qt\n	qmake CONFIG+=$QtConfig example-game-in-qt.pro\n	make\n	cd ../..\n"
  replaceString += "fi\n"
  textLinuxGCC = textLinuxGCC.gsub(replaceString, "")
end
File.open("build-all-linux-gcc.sh", "w") {|file| file.puts textLinuxGCC }

textWinMinGW = File.read("build-all-mingw.bat.template")
if (!@qtIsEnabled)
  textWinMinGW = textWinMinGW.gsub("cd tools/atlasgen\nqmake CONFIG+=%QtConfig% atlasgen.pro || goto :error\nmingw32-make || goto :error\ncd ../..\n", "")
  textWinMinGW = textWinMinGW.gsub("cd tools/exporter\nqmake CONFIG+=%QtConfig% exporter.pro || goto :error\nmingw32-make || goto :error\ncd ../..\n", "")
  textWinMinGW = textWinMinGW.gsub("cd tools/ifaceed\nqmake CONFIG+=%QtConfig% ifaceed.pro\nmingw32-make\ncd ../..\n", "")
  textWinMinGW = textWinMinGW.gsub("cd tools/isqt580\nqmake CONFIG+=%QtConfig% isqt580.pro || goto :error\nmingw32-make || goto :error\ncd ../..\n%CHECKQTVERTOOL%\n", "")
  textWinMinGW = textWinMinGW.gsub("if errorlevel 1 (\n", "")
  textWinMinGW = textWinMinGW.gsub("  cd plugins/qt-widget\n  qmake CONFIG+=%QtConfig% qt-widget.pro || goto :error\n  mingw32-make || goto :error\n  cd ../..\n", "")
  textWinMinGW = textWinMinGW.gsub("  cd examples/example-game-in-qt\n  qmake CONFIG+=%QtConfig% example-game-in-qt.pro || goto :error\n  mingw32-make || goto :error\n  cd ../..\n)\n", "")
end
File.open("build-all-mingw.bat", "w") {|file| file.puts textWinMinGW }

textWinMSVC = File.read("build-all-msvc.bat.template")

if (!@qtIsEnabled)
  textWinMSVC = textWinMSVC.gsub("msbuild tools/atlasgen/atlasgen.vcxproj /m /verbosity:minimal /p:Configuration=\"%1\" /p:Platform=\"%2\"  /p:BuildProjectReferences=false || goto :error\n", "")
  textWinMSVC = textWinMSVC.gsub("msbuild tools/exporter/exporter.vcxproj /m /verbosity:minimal /p:Configuration=\"%1\" /p:Platform=\"%2\"  /p:BuildProjectReferences=false || goto :error\n", "")
  textWinMSVC = textWinMSVC.gsub("msbuild tools/ifaceed/ifaceed/ifaceed.vcxproj /m /verbosity:minimal /p:Configuration=\"%1\" /p:Platform=\"%2\"  /p:BuildProjectReferences=false || goto :error\n", "")
  textWinMSVC = textWinMSVC.gsub("msbuild tools/isqt580/isqt580.vcxproj  /m /verbosity:minimal /p:Configuration=\"%1\" /p:Platform=\"%2\"  /p:BuildProjectReferences=false || goto :error\n", "")
  textWinMSVC = textWinMSVC.gsub("%CHECKQTVERTOOL%\n", "")
  replaceString = "if errorlevel 1 (\n"
  replaceString += "  msbuild plugins/qt-widget/qt-widget.vcxproj  /m /verbosity:minimal /p:Configuration=\"%1\" /p:Platform=\"%2\"  /p:BuildProjectReferences=false || goto :error\n"
  replaceString += "  msbuild examples/example-game-in-qt/example-game-in-qt.vcxproj  /m /verbosity:minimal /p:Configuration=\"%1\" /p:Platform=\"%2\"  /p:BuildProjectReferences=false  || goto :error\n"
  replaceString += ")\n"
  textWinMSVC = textWinMSVC.gsub(replaceString, "")
end
File.open("build-all-msvc.bat", "w") {|file| file.puts textWinMSVC }
