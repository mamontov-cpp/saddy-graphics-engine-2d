var sad = {};
sad.internal = {};
sad.util = {};

sad.Point2D = SadPoint2D;
sad.Point3D = SadPoint3D;
//sad.Point2I = SadPoint2I;
//sad.Point3I = SadPoint3I;


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
    return new sad.Point3D((this.x() + o.x()) / 2, (this.y() + o.y()) / 2, , (this.z() + o.z()) / 2);
};

sad.internal.makeMathFns(sad.Point3D.prototype);

sad.Point3D.prototype.toString = function() {
	return "sad::Point3D(" + this.x() + ',' + this.y() + ',' + this.z() + ')';
};