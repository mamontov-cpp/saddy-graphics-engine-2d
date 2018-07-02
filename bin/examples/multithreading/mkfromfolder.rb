require 'json'
@outputFile = ARGV[0]
@folder = ARGV[1]

@result = {"config" => @outputFile + ".json", "texture" => @outputFile + ".png", "resource" => @outputFile, "sprites" => [] }
@outputFileInput = @outputFile + "_in.json"
@result["sprites"] = Dir.entries(@folder).select {|f| (!File.directory? f) && (f != "Thumbs.db")}.map{ |f| {"texture" => (@folder + "/" + f), "name" => (@outputFile + "_list/" + f.gsub(/\.([gGiIfF]|[pPnNgG]|[bBmMpP])/, ""))} }
File.open(@outputFileInput, 'w') { |file| file.write(JSON.pretty_generate(@result)) }
