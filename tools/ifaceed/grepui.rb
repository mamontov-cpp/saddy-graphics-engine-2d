# An extraction script to see the dependencies from UI

def getFiles(dir)
  Dir[ File.join(dir, '**', '*') ].reject { |p| File.directory? p }
end

getFiles('ifaceed').each{ |x|
	text = File.read(x)
	matches = text.scan(/UI\(\)\-\>[a-zA-Z0-9_]+/)
	if (matches.length != 0)
		print x + ":\n";
		matches.each {
			|match|
			print "\t" + match + "\n"
		}
	end
};