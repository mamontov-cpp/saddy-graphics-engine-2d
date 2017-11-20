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

//  sad.Point2D bindings

sad.Point2D.prototype.to3d = function() {
    return new sad.Point3D(this.x(), this.y(), 0);
};

sad.Point2D.prototype.to3i = function() {
    return new sad.Point3I(this.x(), this.y(), 0);
};

sad.Point2D.prototype.to2i = function() {
    return new sad.Point2I(this.x(), this.y());
};

sad.Point2D.prototype.mid = function(o) {
    return new sad.Point2D((this.x() + o.x()) / 2, (this.y() + o.y()) / 2);
};


sad.Point2D.prototype.makeWithFn = function(fn, o) {
    return new sad.Point2D(fn(this.x(), o.x()), fn(this.y(), o.y()));
};

sad.internal.makeMathFns(sad.Point2D.prototype);

sad.Point2D.prototype.toString = function() {
    return "sad::Point2D(" + this.x() + ',' + this.y() + ')';
};


// sad.Point3D bindings

sad.Point3D.prototype.to2d = function() {
    return new sad.Point2D(this.x(), this.y());
};

sad.Point3D.prototype.to2i = function() {
    return new sad.Point2I(this.x(), this.y());
};

sad.Point3D.prototype.to3I = function() {
    return new sad.Point3I(this.x(), this.y(), this.z());
};


sad.Point3D.prototype.makeWithFn = function(fn, o) {
    return new sad.Point3D(fn(this.x(), o.x()), fn(this.y(), o.y()), fn(this.z(), o.z()));
};

sad.Point3D.prototype.mid = function(o) {
    return new sad.Point3D((this.x() + o.x()) / 2, (this.y() + o.y()) / 2,  (this.z() + o.z()) / 2);
};

sad.internal.makeMathFns(sad.Point3D.prototype);

sad.Point3D.prototype.toString = function() {
    return "sad::Point3D(" + this.x() + ',' + this.y() + ',' + this.z() + ')';
};


// sad.Point2I 

sad.Point2I.prototype.to2d = function() {
    return new sad.Point2D(this.x(), this.y());
};

sad.Point2I.prototype.to3d = function() {
    return new sad.Point3D(this.x(), this.y(), 0);
};

sad.Point2I.prototype.to3i = function() {
    return new sad.Point3I(this.x(), this.y(), 0);
};


sad.Point2I.prototype.makeWithFn = function(fn, o) {
    return new sad.Point2I(fn(this.x(), o.x()), fn(this.y(), o.y()));
};

sad.internal.makeMathFns(sad.Point2I.prototype);

sad.Point2I.prototype.toString = function() {
    return "sad::Point2I(" + this.x() + ',' + this.y() + ')';
};

// sad.Point3I


sad.Point3I.prototype.to2d = function() {
    return new sad.Point2D(this.x(), this.y());
};

sad.Point3I.prototype.to2i = function() {
    return new sad.Point2I(this.x(), this.y());
};

sad.Point3I.prototype.to3d = function() {
    return new sad.Point3D(this.x(), this.y(), this.z());
};


sad.Point3I.prototype.makeWithFn = function(fn, o) {
    return new sad.Point3I(fn(this.x(), o.x()), fn(this.y(), o.y()), fn(this.z(), o.z()));
};

sad.internal.makeMathFns(sad.Point3I.prototype);

sad.Point3I.prototype.toString = function() {
    return "sad::Point3I(" + this.x() + ',' + this.y() + ',' + this.z() + ')';
};

// sad.Size2D

sad.Size2D = SadSize2D;
sad.Size2D.prototype.to2i = function() {
    return new sad.Size2I(this.Width, this.Height);
};
sad.Size2D.prototype.getWidth = function() {
    return this.Width;
}
sad.Size2D.prototype.setWidth = function(a) {
    this.Width = a;
}
sad.Size2D.prototype.getHeight = function() {
    return this.Height;
}
sad.Size2D.prototype.setHeight = function(a) {
    this.Height = a;
}
sad.Size2D.prototype.toString = function() {
    return "sad::Size2D(" + this.Width + ',' + this.Height + ')';
};

// sad.Size2I

sad.Size2I = SadSize2I;
sad.Size2I.prototype.to2d = function() {
    return new sad.Size2D(this.Width, this.Height);
};
sad.Size2I.prototype.getWidth = function() {
    return this.Width;
}
sad.Size2I.prototype.setWidth = function(a) {
    this.Width = a;
}
sad.Size2I.prototype.getHeight = function() {
    return this.Height;
}
sad.Size2I.prototype.setHeight = function(a) {
    this.Height = a;
}
sad.Size2I.prototype.toString = function() {
    return "sad::Size2I(" + this.Width + ',' + this.Height + ')';
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
    return "sad::Rect2D(" +  result.join(', ') + ')';
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
    return "sad::Rect2I(" +  result.join(', ') + ')';
};

sad.internal.uCharToInt = function(a) {
    if (a.length == 0) {
        return 0;
    }
    return a[0].charCodeAt(0);
}

sad.Color = SadColor;

sad.Color.prototype.toString = function() {
    var o =  [ sad.internal.uCharToInt(this.r),  sad.internal.uCharToInt(this.g), sad.internal.uCharToInt(this.b) ]; 
    return "sad::Color(" + o.join(', ')  + ")";
}

sad.AColor = SadAColor;

sad.AColor.prototype.toString = function() {
    var o =  [ sad.internal.uCharToInt(this.r),  sad.internal.uCharToInt(this.g), sad.internal.uCharToInt(this.b), sad.internal.uCharToInt(this.a) ]; 
    return "sad::AColor(" + o.join(', ')  + ")";
}
