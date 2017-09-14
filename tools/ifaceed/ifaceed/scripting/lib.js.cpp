#include<string>
extern const std::string __context_eval_info;
const std::string __context_eval_info = std::string("internal.dump = function(o) {\n"
"    var type = typeof o;\n"
"    if (type == \"undefined\") { return \"undefined\"};\n"
"    if ((type == \"boolean\") || (type == \"number\") || (type == \"string\") || (type == \"symbol\") || (type == \"function\")) { return o.toString();}\n"
"    if (type == \"object\") {\n"
"        if (o === null) {\n"
"            return \"null\";\n"
"        }\n"
"        if (o instanceof Array) {\n"
"            var  i = 0; \n"
"            var  result = [];\n"
"            for(var i = 0; i < o.length; i++)\n"
"            {\n"
"                if (i in o)\n"
"                {\n"
"                    result.push(internal.dump(o[i]));\n"
"                }\n"
"            }\n"
"            return \"[\" + result.join(\", \") + \"]\";\n"
"        } else {\n"
"            if (internal.isNativeObject(o)) {\n"
"                if (o.hasOwnProperty('toString')) {\n"
"                    return o.toString();\n"
"                } \n"
"                var hasPrototypeToString = false;\n"
"                if (typeof o.prototype != \"undefined\") {\n"
"                    hasPrototypeToString = (o.prototype.hasOwnProperty('toString') && o.prototype.toString != Object.toString);\n"
"                }\n"
"                if (hasPrototypeToString) {\n"
"                    return o.toString();\n"
"                }\n"
"                return internal.dumpNativeObject(o);\n"
"            }\n"
"            else\n"
"            {\n"
"                if (o.hasOwnProperty('toString')) {\n"
"                    return o.toString();\n"
"                } else {\n"
"                    var asString = o.toString();\n"
"                    if (asString != \"[object Object]\") {\n"
"                        return asString;\n"
"                    } else {\n"
"                        var result = [];\n"
"                        for(var key in o) { result.push(\"\\\"\" + key + \"\\\"\" + ' : ' + internal.dump(o[key]));}\n"
"                    }\n"
"                }\n"
"                return \"{\" + result.join(\", \") + \"}\";\n"
"            }\n"
"        }\n"
"    }\n"
"    return \"\";\n"
"};\n"
"\n"
"internal.log = function() {\n"
"    for (var i = 0; i < arguments.length; i++) {\n"
"        internal.outputString(internal.dump(arguments[i]));\n"
"    }\n"
"};\n");