internal.dump = function(o) {
    var type = typeof o;
    if (type == "undefined") { return "undefined"};
    if ((type == "boolean") || (type == "number") || (type == "string") || (type == "symbol") || (type == "function")) { return o.toString();}
    if (type == "object") {
        if (o === null) {
            return "null";
        }
        if (o instanceof Array) {
            var  i = 0; 
            var  result = [];
            for(var i = 0; i < o.length; i++)
            {
                if (i in o)
                {
                    result.push(internal.dump(o[i]));
                }
            }
            return "[" + result.join(", ") + "]";
        } else {
            if (internal.isNativeObject(o)) {
                if (o.hasOwnProperty('toString')) {
                    return o.toString();
                } 
                var hasPrototypeToString = false;
                if (typeof o.prototype != "undefined") {
                    hasPrototypeToString = (o.prototype.hasOwnProperty('toString') && o.prototype.toString != Object.toString);
                }
                if (hasPrototypeToString) {
                    return o.toString();
                }
                return internal.dumpNativeObject(o);
            }
            else
            {
                if (o.hasOwnProperty('toString')) {
                    return o.toString();
                } else {
                    var asString = o.toString();
                    if (asString != "[object Object]") {
                        return asString;
                    } else {
                        var result = [];
                        for(var key in o) { result.push("\"" + key + "\"" + ' : ' + internal.dump(o[key]));}
                    }
                }
                return "{" + result.join(", ") + "}";
            }
        }
    }
    return "";
};

internal.log = function() {
    for (var i = 0; i < arguments.length; i++) {
        internal.outputString(internal.dump(arguments[i]));
    }
};
