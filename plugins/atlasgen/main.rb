


puts "Saddy texture atlas generator.\n"
if $0 == __FILE__
    if ARGV.length < 1
        puts "Error: no input file specified. Please specify valid XML config.\n"
    else
        puts ARGV[0]
    end
end