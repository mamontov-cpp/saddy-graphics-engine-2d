void sad::dukpp03::Context::exposeCameraRotation()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::CameraRotation>("SadCameraRotation");
    c->addAccessor("min_angle", sad::dukpp03::bind_method::from(&sad::CameraRotation::minAngle), sad::dukpp03::bind_method::from(&sad::CameraRotation::setMinAngle));
    c->addAccessor("max_angle", sad::dukpp03::bind_method::from(&sad::CameraRotation::maxAngle), sad::dukpp03::bind_method::from(&sad::CameraRotation::setMaxAngle));
    c->addAccessor("pivot", sad::dukpp03::bind_method::from(&sad::CameraRotation::pivot), sad::dukpp03::bind_method::from(&sad::CameraRotation::setPivot));

    c->addMethod("basicSchema", sad::dukpp03::bind_method::from(&sad::CameraRotation::basicSchema)); .
    c->addMethod("schema", sad::dukpp03::bind_method::from(&sad::CameraRotation::schema)); 
    c->addMethod("loadFromValue", sad::dukpp03::bind_method::from(&sad::CameraRotation::loadFromValue)); 
    c->addMethod("stateCommand", sad::dukpp03::bind_method::from(&sad::CameraRotation::stateCommand)); 
    c->addMethod("setState", sad::dukpp03::bind_method::from(&sad::CameraRotation::setState)); 
    c->addMethod("applicableTo", sad::dukpp03::bind_method::from(&sad::CameraRotation::applicableTo)); 

    c->setPrototypeFunction("sad.CameraRotation");

    this->addClassBinding("sad::CameraRotation", c);
}