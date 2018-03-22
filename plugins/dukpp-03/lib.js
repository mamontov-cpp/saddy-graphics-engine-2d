var sad = {};
sad.internal = {};
sad.util = {};

sad.Point2D = SadPoint2D;
sad.Point3D = SadPoint3D;
sad.Point2I = SadPoint2I;
sad.Point3I = SadPoint3I;


sad.internal.plus = function(a, b) { return a + b; };
sad.internal.minus = function(a, b) { return a - b; };
sad.internal.multiply = function(a, b) { return a * b; };
sad.internal.divide = function(a, b) { return a / b; };


sad.internal.copyObject = function(dest, src) {
	for(var key in src) {
		if (src.hasOwnProperty(key)) {
			dest[key] = src[key];
		}
	}
}

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

//	sad.Point2D bindings

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

//	sad.Point3D bindings

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

// sad.Point2I 

sad.Point2I.prototype.to2d = function() {
	return new sad.Point2D(this.x, this.y);
};

sad.Point2I.prototype.to3d = function() {
	return new sad.Point3D(this.x, this.y, 0);
};

sad.Point2I.prototype.to3i = function() {
	return new sad.Point3I(this.x, this.y, 0);
};


sad.Point2I.prototype.makeWithFn = function(fn, o) {
	return new sad.Point2I(fn(this.x, o.x), fn(this.y, o.y));
};

sad.internal.makeMathFns(sad.Point2I.prototype);

sad.Point2I.prototype.toString = function() {
	return "sad::Point2I(" + this.x + ',' + this.y	+ ')';
};

// sad.Point3I

sad.Point3I.prototype.to2d = function() {
	return new sad.Point2D(this.x, this.y);
};

sad.Point3I.prototype.to2i = function() {
	return new sad.Point2I(this.x, this.y);
};

sad.Point3I.prototype.to3d = function() {
	return new sad.Point3D(this.x, this.y, this.z);
};


sad.Point3I.prototype.makeWithFn = function(fn, o) {
	return new sad.Point3I(fn(this.x, o.x), fn(this.y, o.y), fn(this.z, o.z));
};

sad.internal.makeMathFns(sad.Point3I.prototype);

sad.Point3I.prototype.toString = function() {
	return "sad::Point3I(" + this.x + ',' + this.y + ',' + this.z + ')';
};

// sad.p2d.Vector bindings

sad.p2d = {};
sad.p2d.Vector = SadP2DVector;


sad.p2d.Vector.prototype.to3d = function() {
	return new sad.Point3D(this.x, this.y, 0);
};

sad.p2d.Vector.prototype.to3i = function() {
	return new sad.Point3I(this.x, this.y, 0);
};

sad.p2d.Vector.prototype.to2i = function() {
	return new sad.Point2I(this.x, this.y);
};

sad.p2d.Vector.prototype.makeWithFn = function(fn, o) {
	return new sad.Point2D(fn(this.x, o.x), fn(this.y, o.y));
};

sad.internal.makeMathFns(sad.p2d.Vector.prototype);

sad.p2d.Vector.prototype.toString = function() {
	return "sad::p2d::Vector(" + this.x + ',' + this.y + ')';
};

sad.p2d.modulo = SadP2DModulo;
sad.p2d.basis = SadP2DBasis;
sad.p2d.unit = SadP2DUnit;
sad.p2d.scalar = SadP2DScalar;
sad.p2d.ortho = SadP2DOrtho;
sad.p2d.OVI_DEG_90 = 0;
sad.p2d.OVI_DEG_270 = 1;

// sad.Size2D

sad.Size2D = SadSize2D;
sad.Size2D.prototype.to2i = function() {
	return new sad.Size2I(this.width, this.height);
};
sad.Size2D.prototype.toString = function() {
	return "sad::Size2D(" + this.width + ',' + this.height + ')';
};

// sad.Size2I

sad.Size2I = SadSize2I;
sad.Size2I.prototype.to2d = function() {
	return new sad.Size2D(this.width, this.height);
};
sad.Size2I.prototype.toString = function() {
	return "sad::Size2I(" + this.width + ',' + this.height + ')';
};

// sad.Rect2D

sad.Rect2D = SadRect2D;

sad.Rect2D.prototype.to2i = function() {
	return new sad.Rect2I(this.p0().to2i(), this.p1().to2i(), this.p2().to2i(), this.p3().to2i());
};
sad.Rect2D.prototype.toString = function() {
	var result = [];
	for(var i = 0; i < 4; i++)
	{
		result.push(sad.Point2D.prototype.toString.call(this['p' + i]()));
	}
	return "sad::Rect2D(" +	 result.join(', ') + ')';
};

// sad.Rect2I

sad.Rect2I = SadRect2I;

sad.Rect2I.prototype.to2d = function() {
	return new sad.Rect2D(this.p0().to2d(), this.p1().to2d(), this.p2().to2d(), this.p3().to2d());
};
sad.Rect2I.prototype.toString = function() {
	var result = [];
	for(var i = 0; i < 4; i++)
	{
		result.push(sad.Point2I.prototype.toString.call(this['p' + i]()));
	}
	return "sad::Rect2I(" +	 result.join(', ') + ')';
};

// sad.RectPoint3D

sad.RectPoint3D = SadRectPoint3D;
sad.Rect3D = SadRectPoint3D;

sad.RectPoint3D.prototype.toString = function() {
	var result = [];
	for(var i = 0; i < 4; i++)
	{
		result.push(sad.Point3D.prototype.toString.call(this['p' + i]()));
	}
	return "sad::Rect<sad::Point3D>(" +	 result.join(', ') + ')';
};

// console object

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
			var	 i = 0; 
			var	 result = [];
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
				var hasPrototypeToString = false;
				if (typeof o.prototype != "undefined")
				{
					hasPrototypeToString = (o.prototype.hasOwnProperty('toString') && o.prototype.toString != Object.toString);
				}
				if (hasPrototypeToString)
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

// sad.util and sad.fs
sad.fs = {};

sad.fs.pathDelimiter = SadUtilPathDelimiter;
sad.fs.canonicalizePath = SadUtilCanonicalizePath;
sad.fs.isAbsolutePath = SadUtilIsAbsolutePath;
sad.fs.concatPaths = SadUtilConcatPaths;
sad.fs.folder = SadUtilFolder;
sad.fs.fileExists = SadUtilFileExists;
sad.internal.copyObject(sad.util, sad.fs);

// sad.spit and sad.slurp

sad.spit = SadSpit;
sad.slurp = SadSlurp;

sad.spitJSON = function(a, obj) {
	return sad.spit(a, JSON.stringify(obj, null, 2));
};

sad.slurpJSON = function(o) {
	var	 r = sad.slurp(o);
	if (r !== null) {
		return JSON.parse(r);
	}
	return null;
};


// context

context.evalFromFile = function(string) {
	var s = sad.slurp(string);
	if (s == null) {
		throw new Error("Cannot read file \"" + string + "\"");
	}
	return eval(s);
};

context.eval = function(string) {
	return eval(string);
};

sad.Context = SadContext;

sad.Context.prototype.eval = function(string) {
	return SadContextEval(this, string);
};

sad.Context.prototype.evalFromFile = function(string) {
	return SadContextEvalFromFile(this, string);
};

// sad.Renderer bindings

sad.Renderer = SadRenderer;

sad.Renderer.ref = SadRendererRef;
sad.Renderer.makeFPSInterpolationDefault = function() {	 SadInternalMakeFPSInterpolationDefault(this); }
sad.Renderer.makeFPSInterpolationObjectDependent = function() {	 SadInternalMakeFPSInterpolationObjectDependent(this); }

// sad.Settings bindings

sad.Settings = SadSettings;

// sad.Timer bindings

sad.Timer = SadTimer;

sad.Timer.prototype.elapsedInMs = function() {
	this.stop();
	return this.elapsed();
};

// sad.Thread bindings

sad.Thread = SadThread;

// sad.Color bindings

sad.Color = SadColor;

sad.internal.uCharToInt = function(a) {
	if (a.length == 0) {
		return 0;
	}
	return a[0].charCodeAt(0);
}

sad.Color.prototype.toString = function() {
	var o =	 [ sad.internal.uCharToInt(this.r),	 sad.internal.uCharToInt(this.g), sad.internal.uCharToInt(this.b) ]; 
	return "sad::Color(" + o.join(', ')	 + ")";
}

sad.AColor = SadAColor;

sad.AColor.prototype.toString = function() {
	var o =	 [ sad.internal.uCharToInt(this.r),	 sad.internal.uCharToInt(this.g), sad.internal.uCharToInt(this.b), sad.internal.uCharToInt(this.a) ]; 
	return "sad::AColor(" + o.join(', ')  + ")";
}

// sad.log.Log bindings

sad.log = {};
sad.log.Log = SadLog;
sad.log.Log.prototype.ref = SadLogRef;
sad.log.Log.prototype.addTarget = function(a) { SadInternalLogAddTarget(this, a); }
sad.log.Log.prototype.removeTarget = function(a) { SadInternalLogRemoveTarget(this, a); }

sad.log.Priority = {
	"FATAL" : 6, 
	"CRITICAL" : 5, 
	"WARNING" : 4,
	"MESSAGE" : 3, 
	"DEBUG" : 2, 
	"USER" : 1, 
	"SADDY_INTERNAL" : 0  
};

sad.log.ConsoleTarget = SadLogConsoleTarget;
sad.log.FileTarget = SadLogFileTarget;

sad.sleep = SadSleep;
sad.Mutex = SadMutex;
sad.Semaphore = SadSemaphore;

sad.MouseCursor = SadMouseCursor;


sad.resource = {};
sad.resource.Tree = SadResourceTree;

sad.resource.Tree.prototype.ref = function() {
	return sad.Renderer.ref().tree("");
}

sad.db = {};
sad.db.Database = SadDBDatabase;
sad.db.Table = SadDBTable;

sad.db.custom = {};
sad.db.custom.Object = SadDbCustomObject; 

sad.db.custom.Object.prototype.getProperty = function(name) {
	return SadDBCustomObjectGetProperty(this, name);
};

sad.db.custom.Object.prototype.setProperty = function(name, value) {
	return SadDBCustomObjectSetProperty(this, name, value);
};

sad.db.custom.Object.prototype.ownPropertyNames = function() {
	return SadDBCustomObjectOwnPropertyNames(this);
};

sad.db.custom.Object.prototype.propertyNames = function() {
	return SadDBCustomObjectPropertyNames(this);
};


sad.input = {};

sad.input.QuitEvent = SadInputQuitEvent;
sad.input.MouseLeaveEvent = SadInputMouseLeaveEvent;
sad.input.ActivateEvent = SadInputActivateEvent;
sad.input.DeactivateEvent = SadInputDeactivateEvent;
sad.input.KeyPressEvent = SadInputKeyPressEvent;
sad.input.KeyReleaseEvent = SadInputKeyReleaseEvent;
sad.input.MouseMoveEvent = SadInputMouseMoveEvent;
sad.input.MouseEnterEvent = SadInputMouseEnterEvent;
sad.input.MousePressEvent = SadInputMousePressEvent;
sad.input.MouseReleaseEvent = SadInputMouseReleaseEvent;
sad.input.MouseDoubleClickEvent = SadInputMouseDoubleClickEvent;
sad.input.MouseWheelEvent = SadInputMouseWheelEvent;
sad.input.ResizeEvent = SadInputResizeEvent;

sad.KeyboardKey = {
	"Numeric0": 48,
	"Numeric1": 49,
	"Numeric2": 50,
	"Numeric3": 51,
	"Numeric4": 52,
	"Numeric5": 53,
	"Numeric6": 54,
	"Numeric7": 55,
	"Numeric8": 56,
	"Numeric9": 57,
	"A": 65,
	"B": 66,
	"C": 67,
	"D": 68,
	"E": 69,
	"F": 70,
	"G": 71,
	"H": 72,
	"I": 73,
	"J": 74,
	"K": 75,
	"L": 76,
	"M": 77,
	"N": 78,
	"O": 79,
	"P": 80,
	"Q": 81,
	"R": 82,
	"S": 83,
	"T": 84,
	"U": 85,
	"V": 86,
	"W": 87,
	"X": 88,
	"Y": 89,
	"Z": 90,  
	"Minus": 45, 
	"Equal": 61,
	"BackSlash": 92, 
	"Tilde": 126, 
	"OpeningSquareBracket": 91,
	"ClosingSquareBracket": 93,
	"Semicolon": 59,
	"Comma": 44,
	"Period": 46,
	"Slash": 47,
	"Apostrophe": 39,
	"Esc": 256,	 
	"F1" : 257,	 
	"F2" : 258,	 
	"F3" : 259,	 
	"F4" : 260,	 
	"F5" : 261,	 
	"F6" : 262,	 
	"F7" : 263,	 
	"F8" : 264,	 
	"F9" : 265,	 
	"F10": 266,	 
	"F11": 267,	 
	"F12": 268,	 
	"Pause":  269,	
	"Insert": 270,	
	"Delete": 271,	
	"Backspace": 272,  
	"Enter": 273,  
	"Space": 274,  
	"PrintScreen": 275,	 
	"Home": 276,  
	"End": 277,	 
	"PageUp": 278,	
	"PageDown": 279,  
	"Tab": 280,	 
	"NumLock": 281,	 
	"ScrollLock": 282,	
	"CapsLock": 283,  
	"WindowsKey": 284,	
	"AppKey": 285,	
	"KeyLeft": 286,	 
	"KeyRight": 287,  
	"KeyUp": 288,  
	"KeyDown": 289,	 
	"LeftShift": 290,  
	"RightShift": 291,	
	"LeftControl": 292,	 
	"RightControl": 293,  
	"LeftAlt": 294,	 
	"RightAlt": 295,  
	"KeyNone": 296	
};

sad.MouseButton = {
	"MouseLeft"	  : -16535,
	"MouseMiddle" : -16534,
	"MouseRight"  : -16533,
	"MouseNone"	  : 0
};

sad.SpecialKey = {
	"HoldsControl" :  -16538,  
	"HoldsAlt"	   :  -16537,  
	"HoldsShift"   :  -16536  
};

sad.input.EventType = {
	"ET_Quit" : 0,
	"ET_Activate" : 1,
	"ET_Deactivate" : 2,
	"ET_MouseEnter" : 3,
	"ET_MouseLeave" : 4,
	"ET_KeyPress" : 5,
	"ET_KeyRelease" : 6,
	"ET_MouseMove" : 7,
	"ET_MousePress" : 8,
	"ET_MouseRelease" : 9,
	"ET_MouseDoubleClick" : 10,
	"ET_MouseWheel" : 11,
	"ET_Resize" : 12
};

sad.input.JSControls = SadInputJSControls;

sad.input.JSControls.prototype.onQuit = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_Quit, ctx, fn);
};

sad.input.JSControls.prototype.onActivate = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_Quit, ctx, fn);
};

sad.input.JSControls.prototype.onDeactivate = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_Deactivate, ctx, fn);
};

sad.input.JSControls.prototype.onMouseEnter = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_MouseEnter, ctx, fn);
};

sad.input.JSControls.prototype.onEnter = sad.input.JSControls.prototype.onMouseEnter ;

sad.input.JSControls.prototype.onMouseLeave = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_MouseLeave, ctx, fn);
};

sad.input.JSControls.prototype.onLeave = sad.input.JSControls.prototype.onMouseLeave ;

sad.input.JSControls.prototype.onKeyPress = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_KeyPress, ctx, fn);
};

sad.input.JSControls.prototype.onKeyRelease = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_KeyRelease, ctx, fn);
};

sad.input.JSControls.prototype.onMouseMove = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_MouseMove, ctx, fn);
};

sad.input.JSControls.prototype.onMove  = sad.input.JSControls.prototype.onMouseMove;

sad.input.JSControls.prototype.onMousePress = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_MousePress, ctx, fn);
};

sad.input.JSControls.prototype.onClick = sad.input.JSControls.prototype.onMousePress;

sad.input.JSControls.prototype.onMouseRelease = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_MouseRelease, ctx, fn);
};

sad.input.JSControls.prototype.onRelease = sad.input.JSControls.prototype.onMouseRelease;

sad.input.JSControls.prototype.onMouseDoubleClick = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_MouseDoubleClick, ctx, fn);
};

sad.input.JSControls.prototype.onDblClick = sad.input.JSControls.prototype.onMouseDoubleClick;

sad.input.JSControls.prototype.onMouseWheel = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_MouseWheel, ctx, fn);
};

sad.input.JSControls.prototype.onWheel = sad.input.JSControls.prototype.onMouseWheel;

sad.input.JSControls.prototype.onResize = function(ctx, fn) {
	return this.onEvent(sad.input.EventType.ET_Resize, ctx, fn);
};

sad.isValidKeyboardKey = SadIsValidKeyboardKey;
sad.keyToString = SadIsValidKeyboardKey;
sad.defaultKeyIfNotValid = SadDefaultKeyIfNotValid;


sad.pipeline = function(r) {
	return {
		"r" : r,
		"beforeScene" : function() {
			return {
				"r" : this.r,
				"eachFrame": function(step, ctx, fn) { SadPipelineBeforeSceneEachFrame(this.r, step, ctx, fn) },
				"eachMS": function(step, interval, ctx, fn) { SadPipelineBeforeSceneEachMS(this.r, step, interval, ctx, fn) },
				"oneShot": function(step, ctx, fn) { SadPipelineBeforeSceneOneShot(this.r, step, ctx, fn) },
				"delayed": function(step, interval, ctx, fn) { SadPipelineBeforeSceneDelayed(this.r, step, interval, ctx, fn) }
			};
		},
		"afterScene" : function() {
			return {
				"r" : this.r,
				"eachFrame": function(step, ctx, fn) { SadPipelineAfterSceneEachFrame(this.r, step, ctx, fn) },
				"eachMS": function(step, interval, ctx, fn) { SadPipelineAfterSceneEachMS(this.r, step, interval, ctx, fn) },
				"oneShot": function(step, ctx, fn) { SadPipelineAfterSceneOneShot(this.r, step, ctx, fn) },
				"delayed": function(step, interval, ctx, fn) { SadPipelineAfterSceneDelayed(this.r, step, interval, ctx, fn) }
			};
		},
		"beforeEvent" : function(ev) {
			return {
				"r" : this.r,
				"ev" : ev, 
				"eachFrame": function(step, ctx, fn) { SadPipelineBeforeEventEachFrame(this.r, this.ev, step, ctx, fn) },
				"eachMS": function(step, interval, ctx, fn) { SadPipelineBeforeEventEachMS(this.r, this.ev, step, interval, ctx, fn) },
				"oneShot": function(step, ctx, fn) { SadPipelineBeforeEventOneShot(this.r, this.ev, step, ctx, fn) },
				"delayed": function(step, interval, ctx, fn) { SadPipelineBeforeEventDelayed(this.r, this.ev, step, interval, ctx, fn) }
			};
		},
		"afterEvent" : function(ev) {
			return {
				"r" : this.r,
				"ev" : ev, 
				"eachFrame": function(step, ctx, fn) { SadPipelineAfterEventEachFrame(this.r, this.ev, step, ctx, fn) },
				"eachMS": function(step, interval, ctx, fn) { SadPipelineAfterEventEachMS(this.r, this.ev, step, interval, ctx, fn) },
				"oneShot": function(step, ctx, fn) { SadPipelineAfterEventOneShot(this.r, this.ev, step, ctx, fn) },
				"delayed": function(step, interval, ctx, fn) { SadPipelineAfterEventDelayed(this.r, this.ev, step, interval, ctx, fn) }
			};
		}
	};
};

sad.Camera = SadCamera;

sad.Camera.prototype.move = function(o) {
	if (typeof o['z'] != "undefined") {
		this.move3d(o);
	} else {
		this.move2d(o);
	}
}

sad.OrthographicCamera = SadOrthographicCamera;

sad.OrthographicCamera.prototype.move = function(o) {
	if (typeof o['z'] != "undefined") {
		this.move3d(o);
	} else {
		this.move2d(o);
	}
}

sad.Scene = SadScene;
sad.Sprite3D = SadSprite3D;

sad.Label = SadLabel;

sad.Label.OverflowStrategy = {
	"LOS_VISIBLE" : 0,
	"LOS_HIDDEN" : 1,
	"LOS_ELLIPSIS" : 2
};

sad.Label.BreakText = {
	"LBT_NORMAL" : 0,
	"LBT_BREAK_WORD" : 1
};

sad.Label.TextEllipsisPosition = {
	"LTEP_BEGIN" : 0,
	"LTEP_MIDDLE" : 1,
	"LTEP_END" : 2
};

sad.Label.prototype.setFont = function(font, r, tree) {
	return SadLabelSetFont(this, font, r, tree);
};

sad.Sprite2D = SadSprite2D;
sad.Sprite2D.Options = SadSprite2DOptions;


sad.p2d.BoundType = {
	"BT_LEFT" : 0,
	"BT_RIGHT" : 1,
	"BT_DOWN" : 2,
	"BT_UP" : 3
};

sad.p2d.Bound = function(type, position) {
	if (typeof type == "undefined") {
		type = sad.p2d.BoundType.BT_LEFT;
	}
	if (typeof position == "undefined") {
		position = 0;
	}
	if ((type < 0) || (type > 3)) {
		throw new TypeError("Argument 0 is not a sad::p2d::BoundType");
	}
	this.type = type;
	this.position = position;
	this.clone =  function() {
		return new sad.p2d.Bound(this.type, this.position);
	};
	this.center = function() {
		return new sad.Point2D(0,0);
	};
	this.rotate = function(angle) {
		
	};
	this.move = function(v) {
		
	};
	this.dump = function() {
		var b =	 ["Left", "Right", "Bottom", "Top"];
		return b[this.type] + " bound at " + this.position;
	};
};

sad.p2d.isPoint2D = function(o) {
	if (typeof o == "object") {
		if (o != null) {
			if (typeof o["x"] == "number" && typeof o["y"] == "number") {
				return true;
			}
		}
	}
	return false;
};

sad.p2d.isRect2D = function(o) {
	try {
		return SadP2DRect2D(o);
	} catch(ex) {
		
	}
	return false;
};

sad.p2d.Matrix2x2 = {};

sad.p2d.Matrix2x2.counterclockwise = function(angle) {
	var m = [[0,0], [0], [0]];
	var cos = Math.cos(angle);
	var sin = Math.sin(angle);
	m[0][0] = cos; m[1][1] = cos;
	m[0][1] = (-1) * sin; m[1][0] = sin;
	return m;
};

sad.p2d.Matrix2x2.multiply = function(m, p) {
	var x = p.x * m[0][0] 
		  + p.y * m[1][0];
	var y = p.x * m[0][1] 
		  + p.y * m[1][1];
	return new sad.Point2D(x, y);
};

sad.p2d.Line = function(cx1, cy1, cx2, cy2) {
	var isconstructorcall = true;
	this.clone = function() {
		return new sad.p2d.Line([this.m_c[0].clone(), this.m_c[1].clone()]);
	};
	this.setCutter = function(x1,y1,x2,y2) {
		var cerror = true;
		if (typeof x2 == "undefined" && typeof y2 =="undefined") {
			if (typeof x1 == "undefined" && typeof y1 == "undefined") {
				this.m_c = [new sad.Point2D(0, 0), new sad.Point2D(0, 0)];
				cerror = false;
			} else {
				if (typeof x1 != "undefined" && typeof y1 == "undefined") {
					var error = true;
					cerror = false;
					if (x1 instanceof Array) {
						if (x1.length == 2) {
							if (sad.p2d.isPoint2D(x1[0]) && sad.p2d.isPoint2D(x1[1])) {
									error = false;
									this.m_c = x1;
							}
						}
					} 
					if (error) {
						throw new TypeError("Argument 0 is not a sad::p2d::Cutter2D instance");
					}
				} else {
					if (typeof x1 != "undefined"  && typeof y1 != "undefined") {
						cerror = false;
						if (sad.p2d.isPoint2D(x1) && sad.p2d.isPoint2D(y1)) { 
							this.m_c = [x1, y1];
						} else {
							throw new TypeError("Arguments 0 and 1 must have type sad::Point2D");
						}
					}
				}
			}
		} else {
			if (typeof x1  == "number" && typeof y1	 == "number" && typeof x2  == "number" && typeof y2	 == "number") {
				cerror = false;
				this.m_c = [ new sad.Point2D(x1, y1), new sad.Point2D(x2, y2)];
			}
		}
		if (cerror) {
			if (isconstructorcall) {
				throw new TypeError("Could not match call arguments for sad.p2d.Line constructor");
			} else {
				throw new TypeError("Could not match call arguments for sad.p2d.Line.setCutter");
			}
		}
	};
	this.cutter = function() {
		return this.m_c;
	};
	this.center = function() {
		return new sad.Point2D((this.m_c[0].x + this.m_c[1].x) / 2.0, (this.m_c[0].y + this.m_c[1].y) / 2.0);
	};
	this.move = function(d) {
		if (sad.p2d.isPoint2D(d)) {
			this.m_c[0] = new sad.Point2D(this.m_c[0].x + d.x, this.m_c[0].y + d.y);
			this.m_c[1] = new sad.Point2D(this.m_c[1].x + d.x, this.m_c[1].y + d.y);
		} else {
			throw new TypeError("Could not match argument 1 with type sad::Point2D");
		}
	};
	this.rotate = function(angle) {
		if (typeof angle == "number") {
			var center = this.center();
			var centerdp1 = this.m_c[0].minus(center);
			var centerdp2 = this.m_c[1].minus(center);
			var m = sad.p2d.Matrix2x2.counterclockwise(angle);
			var x1 = sad.p2d.Matrix2x2.multiply(m, centerdp1).plus(center);
			var x2 = sad.p2d.Matrix2x2.multiply(m, centerdp2).plus(center);
			this.m_c = [x1, x2];
		} else {
			throw new TypeError("Could not match argument 1 with type double");
		}
	}
	this.p1 = function() {
		return this.m_c[0];
	};
	this.p2 = function() {
		return this.m_c[1];
	};
	this.setP1 = function(d) {
		if (sad.p2d.isPoint2D(d)) {
			this.m_c[0] = d;
		} else {
			throw new TypeError("Could not match argument 1 with type sad::Point2D");
		}
	};
	this.setP2 = function(d) {
		if (sad.p2d.isPoint2D(d)) {
			this.m_c[1] = d;
		} else {
			throw new TypeError("Could not match argument 1 with type sad::Point2D");
		}
	};
	this.dump = function() {
		return "Line at (" + this.m_c[0].x + ", " + this.m_c[0].y + ") - (" + this.m_c[1].x + ", " + this.m_c[1].y + ")";
	}
	this.resizeBy = function(v) {
		if (sad.p2d.isPoint2D(v)) {
			var n = this.p2().minus(this.p1());
			n = sad.p2d.unit(n); 
			var length = sad.p2d.scalar(n, v);
			var vec = new sad.Point2D(n.x * length, n.y * length);
			this.m_c[1] = this.m_c[1].plus(vec);
			this.m_c[0] = this.m_c[0].minus(vec);
		} else {
			throw new TypeError("Could not match argument 1 with type sad::Point2D");
		}
	};
	
	
	this.setCutter(cx1, cy1, cx2, cy2);
	isconstructorcall = false;
};

sad.p2d.Rectangle = function(o) {
	this.clone = function() {
		return new sad.p2d.Rectangle(this.m_rect.clone());
	};
	this.setRect = function(o) {
		if (sad.p2d.isRect2D(o)) {
			this.m_rect = o;
		} else {
			throw new TypeError("Argument 0 is not a sad::Rect2D");
		}
	};
	this.point = function(i) {
		if (typeof i == "number") {
			if (i > -1 && i < 4) {
				return  this.m_rect["p" + i]();
			} else {
				throw new TypeError("Please, specify value in [0..3] range")
			}
		} else {
			throw new TypeError("Argument 0 is not a number");
		}
	};
	this.points = function() {
		return [this.m_rect.p0(), this.m_rect.p1(), this.m_rect.p2(), this.m_rect.p3()];
	};
	this.rect = function() {
		return this.m_rect;
	};
	this.center = function() {
		var p1 = this.m_rect.p0();
		var p2 = this.m_rect.p2();
		return new sad.Point2D((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
	};
	this.rotate = function(angle) {
		if (typeof angle == "number") {
			this.m_rect = sad.rotate(this.m_rect, angle);
		} else {
			throw new TypeError("Argument 0 is not a number");
		} 
	};
	this.moveBy = function(d) {
		if (sad.p2d.isPoint2D(d)) {
			this.m_rect = sad.moveBy(d, this.m_rect);
		} else {
			throw new TypeError("Argument 0 is not a sad::Point2D");
		}
	};
	this.dump = function(o) {
		var pts = this.points();
		return "Rectangle:\n[{0}, {1} - {2}, {3}]\n[{4}, {5} - {6}, {7}]\n"
				.replace("{0}", pts[0].x)
				.replace("{1}", pts[0].y)
				.replace("{2}", pts[1].x)
				.replace("{3}", pts[1].y)
				.replace("{4}", pts[3].x)
				.replace("{5}", pts[3].y)
				.replace("{6}", pts[2].x)
				.replace("{7}", pts[2].y);
	};
	this.resizeBy = function(v) {
		var sum = function(p, cx, cy) {
			return p.plus( new sad.Point2D(v.x * cx, v.y * cy) );
		};
		var pts = this.points();
		if (sad.p2d.isPoint2D(v)) {
			pts[0] = sum(pts[0], -1, -1);
			pts[1] = sum(pts[1], 1, -1);
			pts[2] = sum(pts[2], 1, 1);
			pts[3] = sum(pts[3], -1, 1);
			this.setRect(new sad.Rect2D(pts[0], pts[1], pts[2], pts[3]));
		} else {
			throw new TypeError("Argument 0 is not a sad::Point2D");
		}
	};
	if (typeof o == "undefined") {
		o = new sad.Rect2D();
	}
	this.setRect(o);
};


sad.p2d.Circle = function(center, radius) {
	if (typeof center == "undefined") {
		center = new sad.Point2D(0, 0);
	};
	if (typeof radius == "undefined") {
		radius = 0;
	};
	this.clone = function() {
		return new sad.p2d.Circle(this.m_center.clone(), this.m_radius);
	};
	this.center = function() {
		return this.m_center;
	};
	this.centerRef = this.center;
	this.radius = function() {
		return this.m_radius;
	};
	this.setCenter =  function(p) {
		if (sad.p2d.isPoint2D(p)) {
			this.m_center = p;
		} else {
			throw new TypeError("Argument 0 is not a sad::Point2D");
		}
	};
	this.setRadius = function(radius) {
		if (typeof radius == "number") {
			this.m_radius = radius;
		} else {
			throw new TypeError("Argument 0 is not a number");
		} 
	};
	this.rotate = function(angle) {
		if (typeof angle != "number") {
			throw new TypeError("Argument 0 is not a number");
		} 
	};
	this.moveBy = function(d) {
		if (sad.p2d.isPoint2D(d)) {
			this.m_center = new sad.Point2D(this.m_center.x + d.x, this.m_center.y + d.y);
		} else {
			throw new TypeError("Argument 0 is not a sad::Point2D");
		}
	};
	this.move = this.moveBy;
	this.dump = function(o) {
		return "Circle with center (" + this.m_center.x +"," +  + this.m_center.y + ") and radius " + this.m_radius;
	};
	this.resizeBy = function(v) {
		if (sad.p2d.isPoint2D(v)) {
			var modulo = sad.p2d.modulo(v);
			if (v.x > 0 || v.y > 0) {
				this.m_radius += modulo;
			} else {
				this.m_radius -= modulo;
			}
		} else {
			throw new TypeError("Argument 0 is not a sad::Point2D");
		}
	};
	this.setCenter(center);
	this.setRadius(radius);
};

SadLayoutsSerializableCellToString = function() {
	var o = {
		"Width" : "#Width#",
		"Height" : "#Height#",
		"RowSpan" : this.RowSpan,
		"ColSpan" : this.ColSpan,
		"VAlign" : "#VAlign#",
		"HAlign" : "#HAlign#",
		"Children" : this.Children,
		"StackingType" : "#StackingType#",
		"PaddingTop" : this.PaddingTop,
		"PaddingBottom" : this.PaddingBottom,
		"PaddingLeft" : this.PaddingLeft,
		"PaddingRight" : this.PaddingRight,
		"AssignedArea" : "#ASSIGNEDAREA#",
		"Row" : this.Row,
		"Col" : this.Col
	};
	var str = JSON.stringify(o, null, 2);
	var valigns = ["sad.layouts.LVA_Top", "sad.layouts.LVA_Middle", "sad.layouts.LVA_Bottom"];
	var haligns = ["sad.layouts.LHA_Left", "sad.layouts.LHA_Middle", "sad.layouts.LHA_Right"];
	var stackingtype = ["sad.layouts.LST_Horizontal", "sad.layouts.LST_Vertical", "sad.layouts.LST_NoStacking"];
	var result = str.replace("\"#Width#\"", sad.layouts.LengthValue.prototype.toString.call(this.Width))
					.replace("\"#Height#\"", sad.layouts.LengthValue.prototype.toString.call(this.Height))
					.replace("\"#VAlign#\"",valigns[this.VAlign])
					.replace("\"#HAlign#\"",haligns[this.HAlign])
					.replace("\"#StackingType#\"",stackingtype[this.StackingType])
					.replace("\"#ASSIGNEDAREA#\"", sad.Rect2D.prototype.toString.call(this.AssignedArea));
	return "sad::layouts::SerializableCell(" + result + ")";
};

sad.animations = {};
sad.animations.easing = {};

sad.animations.easing.ATTT_Linear = 0;
sad.animations.easing.ATTT_InSine = 1;
sad.animations.easing.ATTT_OutSine = 2;
sad.animations.easing.ATTT_InOutSine = 3;
sad.animations.easing.ATTT_InQuad = 4;
sad.animations.easing.ATTT_OutQuad = 5;
sad.animations.easing.ATTT_InOutQuad = 6;
sad.animations.easing.ATTT_InCubic = 7;
sad.animations.easing.ATTT_OutCubic = 8;
sad.animations.easing.ATTT_InOutCubic = 9;
sad.animations.easing.ATTT_InQuart = 10;
sad.animations.easing.ATTT_OutQuart = 11;
sad.animations.easing.ATTT_InOutQuart = 12;
sad.animations.easing.ATTT_InQuint = 13;
sad.animations.easing.ATTT_OutQuint = 14;
sad.animations.easing.ATTT_InOutQuint = 15;
sad.animations.easing.ATTT_InExpo = 16;
sad.animations.easing.ATTT_OutExpo = 17;
sad.animations.easing.ATTT_InOutExpo = 18;
sad.animations.easing.ATTT_InCirc = 19;
sad.animations.easing.ATTT_OutCirc = 20;
sad.animations.easing.ATTT_InOutCirc = 21;
sad.animations.easing.ATTT_InElastic = 22;
sad.animations.easing.ATTT_OutElastic = 23;
sad.animations.easing.ATTT_InOutElastic = 24;
sad.animations.easing.ATTT_InBack = 25;
sad.animations.easing.ATTT_OutBack = 26;
sad.animations.easing.ATTT_InOutBack = 27;
sad.animations.easing.ATTT_InBounce = 28;
sad.animations.easing.ATTT_OutBounce = 29;
sad.animations.easing.ATTT_InOutBounce = 30

sad.animations.easing.Types = {};
sad.animations.easing.Types.ATTT_Linear = 0;
sad.animations.easing.Types.ATTT_InSine = 1;
sad.animations.easing.Types.ATTT_OutSine = 2;
sad.animations.easing.Types.ATTT_InOutSine = 3;
sad.animations.easing.Types.ATTT_InQuad = 4;
sad.animations.easing.Types.ATTT_OutQuad = 5;
sad.animations.easing.Types.ATTT_InOutQuad = 6;
sad.animations.easing.Types.ATTT_InCubic = 7;
sad.animations.easing.Types.ATTT_OutCubic = 8;
sad.animations.easing.Types.ATTT_InOutCubic = 9;
sad.animations.easing.Types.ATTT_InQuart = 10;
sad.animations.easing.Types.ATTT_OutQuart = 11;
sad.animations.easing.Types.ATTT_InOutQuart = 12;
sad.animations.easing.Types.ATTT_InQuint = 13;
sad.animations.easing.Types.ATTT_OutQuint = 14;
sad.animations.easing.Types.ATTT_InOutQuint = 15;
sad.animations.easing.Types.ATTT_InExpo = 16;
sad.animations.easing.Types.ATTT_OutExpo = 17;
sad.animations.easing.Types.ATTT_InOutExpo = 18;
sad.animations.easing.Types.ATTT_InCirc = 19;
sad.animations.easing.Types.ATTT_OutCirc = 20;
sad.animations.easing.Types.ATTT_InOutCirc = 21;
sad.animations.easing.Types.ATTT_InElastic = 22;
sad.animations.easing.Types.ATTT_OutElastic = 23;
sad.animations.easing.Types.ATTT_InOutElastic = 24;
sad.animations.easing.Types.ATTT_InBack = 25;
sad.animations.easing.Types.ATTT_OutBack = 26;
sad.animations.easing.Types.ATTT_InOutBack = 27;
sad.animations.easing.Types.ATTT_InBounce = 28;
sad.animations.easing.Types.ATTT_OutBounce = 29;
sad.animations.easing.Types.ATTT_InOutBounce = 30