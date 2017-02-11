require 'mustache'

def readTemplate(name)
  return File.open(name, "rb") { |f| f.read }
end

def writeTemplate(name, text)
	File.open(name, "wb") { |f|  f.write(text) }
end

def render(inname, outname, variants)
	templatedata = { "variants" => variants }
	templatetext  = readTemplate(inname)
	writeTemplate(outname, Mustache.render(templatetext, templatedata))
end

variants = [];
(0..16).each do |arglimit|
  args = []
  (0..arglimit-1).each do |argnumber|
    argnumberentry = { "number" => argnumber, "numberp1" => argnumber + 1, "numberp2" => argnumber + 2 }; 
    if (argnumber == arglimit - 1)
	  argnumberentry["last"] = true
	else  
	  argnumberentry["not_last"] = true
    end
    args.push(argnumberentry)
  end
  variants.push({"not_has_args" => args.count == 0, "has_args" => args.count != 0, "args" => args, "argscount" => args.count, "argscountp1" => args.count + 1})
end

if (ARGV.empty?)
	render("ptrconstructor.h.t", "ptrconstructor.h", variants)
else
	render(ARGV[0] + ".t", ARGV[0], variants)
end