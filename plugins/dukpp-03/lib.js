var sad = {};
sad.internal = {};
sad.Point2D = SadPoint2D;
sad.Point3D = SadPoint3D;

sad.internal.plus = function(a, b) { return a + b; };
sad.internal.minus = function(a, b) { return a - b; };
sad.internal.multiply = function(a, b) { return a * b; };
sad.internal.divide = function(a, b) { return a / b; };


sad.internal.makeMathFns = function(obj) {
	obj.plus = function(o) {
		return this.makeWithFn(sad.internal.plus, o);
	};

	obj.minus = function(o) {
		return this.makeWithFn(sad.internal.minus, o);
	};

	obj.multiply = function(o) {
		return this.makeWithFn(sad.internal.multiply, o);
	};

	obj.divide = function(o) {
		return this.makeWithFn(sad.internal.divide, o);
	};
};

//  sad.Point2D bindings

sad.Point2D.prototype.to3d = function() {
	return new sad.Point3D(this.x, this.y, 0);
};

sad.Point2D.prototype.to3i = function() {
	return new sad.Point3I(this.x, this.y, 0);
};

sad.Point2D.prototype.to2i = function() {
	return new sad.Point2I(this.x, this.y);
};

sad.Point2D.prototype.makeWithFn = function(fn, o) {
	return new sad.Point2D(fn(this.x, o.x), fn(this.y, o.y));
};

sad.internal.makeMathFns(sad.Point2D.prototype);

sad.Point2D.prototype.toString = function() {
	return "sad::Point2D(" + this.x + ',' + this.y + ')';
};

//  sad.Point3D bindings


sad.Point3D.prototype.to2d = function() {
	return new sad.Point2D(this.x, this.y);
};

sad.Point3D.prototype.to2i = function() {
	return new sad.Point2I(this.x, this.y);
};

sad.Point3D.prototype.to3I = function() {
	return new sad.Point3I(this.x, this.y, this.z);
};


sad.Point3D.prototype.makeWithFn = function(fn, o) {
	return new sad.Point3D(fn(this.x, o.x), fn(this.y, o.y), fn(this.z, o.z));
};

sad.internal.makeMathFns(sad.Point3D.prototype);

sad.Point3D.prototype.toString = function() {
	return "sad::Point3D(" + this.x + ',' + this.y + ',' + this.z + ')';
};

var console = {};
console.dump = function(o) {
	var type = typeof o;
	if (type == "undefined") { return "undefined"};
	if ((type == "boolean") || (type == "number") || (type == "string") || (type == "symbol") || (type == "function")) { return o.toString();}
	if (type == "object")
	{
		if (o === null)
		{
			return "null";
		}
		if (o instanceof Array)
		{
			var  i = 0; 
			var  result = [];
			for(var i = 0; i < o.length; i++)
			{
				if (i in o)
				{
					result.push(console.dump(o[i]));
				}
			}
			return "[" + result.join(", ") + "]";
		}
		else
		{
			if (SadInternalIsNativeObject(o))
			{
				if (o.hasOwnProperty('toString'))
				{
					return o.toString();
				}
				if (o.prototype.hasOwnProperty('toString') && o.prototype.toString != Object.toString)
				{
					return o.toString();
				}
				return SadInternalDumpNativeObject(o);
			}
			else
			{
				if (o.hasOwnProperty('toString'))
				{
					return o.toString();
				}
				else
				{
					var asString = o.toString();
					if (asString != "[object Object]")
					{
						return asString;
					}
					else
					{
						var result = [];
						for(var key in o) { result.push("\"" + key + "\"" + ' : ' + console.dump(o[key]));}
					}
				}
				return "{" + result.join(", ") + "}";
			}
		}
	}
	return "";
};

console.log = function(o)
{
	print(console.dump(o));
};
