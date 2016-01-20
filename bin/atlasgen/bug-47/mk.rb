file = File.open("list.txt", "rb")
contents = file.read
files = contents.split("\r\n")

file = File.open("bug-47.json", "wt")
file.write("{
	\"config\" : \"bug-47/bug-47-atlas.json\",
	\"texture\" : \"bug-47/bug-47-atlas.png\",
	\"resource\": \"bug-47\",
	\"sprites\" : [\n")
	
entries = files.map{ |x|
"        {
			\"name\" : \"" + x + "\",
			\"texture\" : \"bug-47/lorc_originals_png/" + x + "\"
		}" 
}

file.write(entries.join(",\n"))
	
file.write("	
	]
}
")