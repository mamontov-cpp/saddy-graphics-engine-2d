#include "dukpp-03/getvalue.h"
#include "object.h"
#include "classmetadatacontainer.h"
#include "p2d/body.h"

#include <iostream>

::dukpp03::Maybe<double> dukpp03::internal::tryGetDoubleProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
)
{
    ::dukpp03::Maybe<double> result;
    if (duk_has_prop_string(ctx->context(), pos, property_name))
    {
        duk_get_prop_string(ctx->context(), pos, property_name);
        result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}

::dukpp03::Maybe<int>  dukpp03::internal::tryGetIntProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
)
{
    ::dukpp03::Maybe<int> result;
    if (duk_has_prop_string(ctx->context(), pos, property_name))
    {
        duk_get_prop_string(ctx->context(), pos, property_name);
        result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}


::dukpp03::Maybe<unsigned char>  dukpp03::internal::tryGetUnsignedCharProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
)
{
    ::dukpp03::Maybe<unsigned char> result;
    if (duk_has_prop_string(ctx->context(), pos, property_name))
    {
        duk_get_prop_string(ctx->context(), pos, property_name);
        result = ::dukpp03::GetValue<unsigned char, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}



::dukpp03::Maybe<sad::Point2D>  dukpp03::internal::tryGetPoint2DProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
)
{
    ::dukpp03::Maybe<sad::Point2D> result;
    if (duk_has_prop_string(ctx->context(), pos, property_name))
    {
        duk_get_prop_string(ctx->context(), pos, property_name);
        result = ::dukpp03::GetValue<sad::Point2D, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}


dukpp03::Maybe<const char*> dukpp03::GetValue<const char*, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos
)
{
    dukpp03::Maybe<const char*> result;
    if (duk_is_string(ctx->context(), pos))
    {
        result.setValue(duk_to_string(ctx->context(), pos));
    }
    return result;
}

dukpp03::Maybe<sad::String> dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos
)
{
    dukpp03::Maybe<sad::String> result;
    if (duk_is_string(ctx->context(), pos))
    {
        result.setValue(duk_to_string(ctx->context(), pos));
    }
    return result;
}

dukpp03::Maybe<sad::db::Object*> dukpp03::GetValue<sad::db::Object*,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::db::Object*> result;
    if (duk_is_object(ctx->context(), pos))
    {
        duk_get_prop_string(ctx->context(), pos, DUKPP03_VARIANT_PROPERTY_SIGNATURE);
        if (duk_is_pointer(ctx->context(), -1))
        {
            void* ptr = duk_to_pointer(ctx->context(), -1);
            sad::db::Variant * v = reinterpret_cast<sad::db::Variant *>(ptr);
            if (v->pointerStarsCount() == 1)
            {
                if (v->baseName() == "sad::db::Object") 
                {
                    result.setValue(v->get<sad::db::Object*>(true).value());
                } 
                else 
                {
                    if (sad::ClassMetaDataContainer::ref()->contains(v->baseName()))
                    {
                        bool created = false;
                        if (sad::ClassMetaDataContainer::ref()->get(v->baseName(), created)->canBeCastedTo("sad::Object"))
                        {
                            sad::Object** object = reinterpret_cast<sad::Object**>(v->data());
                            result.setValue(static_cast<sad::db::Object*>(*object));
                        }
                    }
                    
                    if (result.exists() == false) 
                    {
                        // A patch for sad::Scene and other classes, derived directly from sad::db::Object
                        if (v->isSadObject() && v->pointerStarsCount() == 1) 
                        {
                            sad::db::Object** object = reinterpret_cast<sad::db::Object**>(v->data());
                            result.setValue(*object);
                        }
                    }
                }
            }
        }
        duk_pop(ctx->context());
    }
    return result;
}


::dukpp03::Maybe<sad::Object*>  dukpp03::GetValue<sad::Object*,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Object*> result;
    if (duk_is_object(ctx->context(), pos))
    {
        duk_get_prop_string(ctx->context(), pos, DUKPP03_VARIANT_PROPERTY_SIGNATURE);
        if (duk_is_pointer(ctx->context(), -1))
        {
            void* ptr = duk_to_pointer(ctx->context(), -1);
            sad::db::Variant * v = reinterpret_cast<sad::db::Variant *>(ptr);
            if (v->pointerStarsCount() == 1)
            {
                if (v->baseName() == "sad::Object") 
                {
                    result.setValue(v->get<sad::Object*>(true).value());
                } 
                else 
                {
                    if (v->baseName() == "sad::db::Object")
                    {
                        sad::db::Object* o = v->get<sad::db::Object*>(true).value();
                        sad::String real_name = o->serializableName();
                        bool created = false;
                        if (sad::ClassMetaDataContainer::ref()->contains(real_name))
                        {
                            if (sad::ClassMetaDataContainer::ref()->get(real_name, created)->canBeCastedTo("sad::Object"))
                            {
                                sad::Object** object = reinterpret_cast<sad::Object**>(v->data());
                                result.setValue(*object);
                            }
                        }
                    }
                    else
                    {
                        bool created = false;
                        if (sad::ClassMetaDataContainer::ref()->get(v->baseName(), created)->canBeCastedTo("sad::Object"))
                        {
                            sad::Object** object = reinterpret_cast<sad::Object**>(v->data());
                            result.setValue(*object);
                        }
                        else
                        {
                            if (v->isSadObject() && v->pointerStarsCount() == 1) 
                            {
                                sad::Object** object = reinterpret_cast<sad::Object**>(v->data());
                                result.setValue(*object);
                            }
                        }
                    }
                }
            }
        }
        duk_pop(ctx->context());
    }
    return result;
}


::dukpp03::Maybe<sad::db::Variant> dukpp03::GetValue<sad::db::Variant,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::db::Variant> result;
    if (duk_is_string(ctx->context(), pos))
    {
        result.setValue(sad::String(duk_to_string(ctx->context(), pos)));
    }
    if (duk_is_boolean(ctx->context(), pos))
    {
        result.setValue(duk_to_boolean(ctx->context(), pos) > 0);
    }
    if (duk_is_number(ctx->context(), pos))
    {
        result.setValue(duk_to_number(ctx->context(), pos));
    }
    if (duk_is_object(ctx->context(), pos))
    {
        duk_get_prop_string(ctx->context(), pos, DUKPP03_VARIANT_PROPERTY_SIGNATURE);
        if (duk_is_pointer(ctx->context(), -1))
        {
            void* ptr = duk_to_pointer(ctx->context(), -1);
            sad::db::Variant * v = reinterpret_cast<sad::db::Variant *>(ptr);
            if (v)
            {
                result.setValue(*v);
            }
        }
        duk_pop(ctx->context());
    }
    return result;
}


::dukpp03::Maybe<sad::Point2D>  dukpp03::GetValue<sad::Point2D,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Point2D> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::Point2D, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::Point2D*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::Point2D*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<double> maybex = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "x");
                ::dukpp03::Maybe<double> maybey = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "y");
                if (maybex.exists() && maybey.exists())
                {
                    result.setValue(sad::Point2D(maybex.value(), maybey.value()));
                }
            }
        }
    }
    return result;
}

::dukpp03::Maybe<sad::p2d::Bound*> dukpp03::GetValue<sad::p2d::Bound*, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::p2d::Bound*> result;
    if (duk_is_object(ctx->context(), pos))
    {
        ::dukpp03::Maybe<int> maybetype = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "type");
        ::dukpp03::Maybe<double> maybeposition = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "position");
        if (maybetype.exists() && maybeposition.exists())
        {
            if (maybetype.value() >= 0 && maybetype.value() <= 3)
            {
                sad::p2d::Bound* b = new sad::p2d::Bound();
                b->setType(static_cast<sad::p2d::BoundType>(maybetype.value()));
                b->setPosition(maybeposition.value());
                result.setValue(b);
            }
        }
    }
    return result;
}


::dukpp03::Maybe<sad::p2d::Line*> dukpp03::GetValue<sad::p2d::Line*, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::p2d::Line*> result;
    if (duk_is_object(ctx->context(), pos))
    {
        ::dukpp03::Maybe<sad::Pair<sad::Point2D,  sad::Point2D> > part;
        if (duk_has_prop_string(ctx->context(), pos, "m_c"))
        {
            duk_get_prop_string(ctx->context(), pos, "m_c");
            part = ::dukpp03::GetValue<sad::Pair<sad::Point2D,  sad::Point2D>, sad::dukpp03::BasicContext>::perform(ctx, -1);
            duk_pop(ctx->context());
        }
        if (part.exists())
        {
            sad::p2d::Line* b = new sad::p2d::Line();
            b->setCutter(part.value().p1(), part.value().p2());
            result.setValue(b);
        }
    }
    return result;
}


::dukpp03::Maybe<sad::p2d::Rectangle*> dukpp03::GetValue<sad::p2d::Rectangle*, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::p2d::Rectangle*> result;
    if (duk_is_object(ctx->context(), pos))
    {
        ::dukpp03::Maybe<sad::Rect2D> part;
        if (duk_has_prop_string(ctx->context(), pos, "m_rect"))
        {
            duk_get_prop_string(ctx->context(), pos, "m_rect");
            part = ::dukpp03::GetValue<sad::Rect2D, sad::dukpp03::BasicContext>::perform(ctx, -1);
            duk_pop(ctx->context());
        }
        if (part.exists())
        {
            sad::p2d::Rectangle* b = new sad::p2d::Rectangle();
            b->setRect(part.value());
            result.setValue(b);
        }
    }
    return result;
}



::dukpp03::Maybe<sad::p2d::Circle*> dukpp03::GetValue<sad::p2d::Circle*, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::p2d::Circle*> result;
    if (duk_is_object(ctx->context(), pos))
    {
        ::dukpp03::Maybe<sad::Point2D> maybe_center = dukpp03::internal::tryGetPoint2DProperty(ctx, pos, "m_center");
        ::dukpp03::Maybe<double> maybe_radius = dukpp03::internal::tryGetDoubleProperty(ctx, pos, "m_radius");
        if (maybe_center.exists() && maybe_radius.exists())
        {
            sad::p2d::Circle* b = new sad::p2d::Circle();
            b->setCenter(maybe_center.value());
            b->setRadius(maybe_radius.value());
            result.setValue(b);
        }
    }
    return result;
}

::dukpp03::Maybe<sad::p2d::CollisionShape*> dukpp03::GetValue<sad::p2d::CollisionShape*, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::p2d::CollisionShape*> result;

    ::dukpp03::Maybe<sad::p2d::Bound*> maybe_bound = dukpp03::GetValue<sad::p2d::Bound*, sad::dukpp03::BasicContext>::perform(ctx, pos);
    if (maybe_bound.exists())
    {
        result.setValue(maybe_bound.value());
    }
    // Try to get line
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::p2d::Line*> maybe_line = dukpp03::GetValue<sad::p2d::Line*, sad::dukpp03::BasicContext>::perform(ctx, pos);
        if (maybe_line.exists())
        {
            result.setValue(maybe_line.value());
        }
    }
    
    // Try to get rectangle
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::p2d::Rectangle*> maybe_rectangle = dukpp03::GetValue<sad::p2d::Rectangle*, sad::dukpp03::BasicContext>::perform(ctx, pos);
        if (maybe_rectangle.exists())
        {
            result.setValue(maybe_rectangle.value());
        }
    }

    // Try to get circle
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::p2d::Circle*> maybe_circle = dukpp03::GetValue<sad::p2d::Circle*, sad::dukpp03::BasicContext>::perform(ctx, pos);
        if (maybe_circle.exists())
        {
            result.setValue(maybe_circle.value());
        }
    }

    return result;
}

::dukpp03::Maybe<sad::p2d::BasicCollisionEvent> dukpp03::GetValue<sad::p2d::BasicCollisionEvent, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::p2d::BasicCollisionEvent> result;
    ::dukpp03::Maybe<sad::Triplet<sad::p2d::Body*, sad::p2d::Body*, double> > maybe_triplet = dukpp03::GetValue<sad::Triplet<sad::p2d::Body*, sad::p2d::Body*, double>, sad::dukpp03::BasicContext>::perform(ctx, pos);
    if (maybe_triplet.exists())
    {
        result.setValue(sad::p2d::BasicCollisionEvent(maybe_triplet.value().p1(), maybe_triplet.value().p2(), maybe_triplet.value().p3()));
    }
    return result;
}


::dukpp03::Maybe<sad::Point3D>  dukpp03::GetValue<sad::Point3D,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Point3D> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::Point3D, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::Point3D*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::Point3D*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<double> maybex = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "x");
                ::dukpp03::Maybe<double> maybey = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "y");
                ::dukpp03::Maybe<double> maybez = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "z");
                if (maybex.exists() && maybey.exists())
                {
                    if (maybez.exists())
                    {
                        result.setValue(sad::Point3D(maybex.value(), maybey.value(), maybez.value()));                        
                    }
                    else
                    {
                        result.setValue(sad::Point3D(maybex.value(), maybey.value(), 0));
                    }
                }
            }
        }
    }
    return result;
}



::dukpp03::Maybe<sad::Point2I>  dukpp03::GetValue<sad::Point2I,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Point2I> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::Point2I, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::Point2I*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::Point2I*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<int> maybex = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "x");
                ::dukpp03::Maybe<int> maybey = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "y");
                if (maybex.exists() && maybey.exists())
                {
                    result.setValue(sad::Point2I(maybex.value(), maybey.value()));
                }
            }
        }
    }
    return result;
}



::dukpp03::Maybe<sad::Point3I>  dukpp03::GetValue<sad::Point3I,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Point3I> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::Point3I, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::Point3I*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::Point3I*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<int> maybex = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "x");
                ::dukpp03::Maybe<int> maybey = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "y");
                ::dukpp03::Maybe<int> maybez = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "z");
                if (maybex.exists() && maybey.exists())
                {
                    if (maybez.exists())
                    {
                        result.setValue(sad::Point3I(maybex.value(), maybey.value(), maybez.value()));                        
                    }
                    else
                    {
                        result.setValue(sad::Point3I(maybex.value(), maybey.value(), 0));
                    }
                }
            }
        }
    }
    return result;
}



::dukpp03::Maybe<sad::Size2D>  dukpp03::GetValue<sad::Size2D,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Size2D> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::Size2D, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::Size2D*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::Size2D*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<double> maybewidth = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "width");
                ::dukpp03::Maybe<double> maybeheight = ::dukpp03::internal::tryGetDoubleProperty(ctx, pos, "height");
                if (maybewidth.exists() && maybeheight.exists())
                {
                    result.setValue(sad::Size2D(maybewidth.value(), maybeheight.value()));
                }
            }
        }
    }
    return result;
}


::dukpp03::Maybe<sad::Size2I>  dukpp03::GetValue<sad::Size2I,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Size2I> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::Size2I, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::Size2I*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::Size2I*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<int> maybewidth = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "width");
                ::dukpp03::Maybe<int> maybeheight = ::dukpp03::internal::tryGetIntProperty(ctx, pos, "height");
                if (maybewidth.exists() && maybeheight.exists())
                {
                    result.setValue(sad::Size2I(maybewidth.value(), maybeheight.value()));
                }
            }
        }
    }
    return result;
}


::dukpp03::Maybe<sad::Color>  dukpp03::GetValue<sad::Color,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::Color> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::Color, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::Color*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::Color*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<unsigned char> mayber = ::dukpp03::internal::tryGetUnsignedCharProperty(ctx, pos, "r");
                ::dukpp03::Maybe<unsigned char> maybeg = ::dukpp03::internal::tryGetUnsignedCharProperty(ctx, pos, "g");
                ::dukpp03::Maybe<unsigned char> maybeb = ::dukpp03::internal::tryGetUnsignedCharProperty(ctx, pos, "b");
                if (mayber.exists() && maybeg.exists() && maybeb.exists())
                {
                    result.setValue(sad::Color(mayber.value(), maybeg.value(), maybeb.value()));                    
                }
            }
        }
    }
    return result;
}


::dukpp03::Maybe<sad::AColor>  dukpp03::GetValue<sad::AColor,  sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::AColor> result;
    ::dukpp03::internal::TryGetValueFromObject<sad::AColor, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
    if (result.exists() == false)
    {
        ::dukpp03::Maybe<sad::AColor*> result2;
        ::dukpp03::internal::TryGetValueFromObject<sad::AColor*, sad::dukpp03::BasicContext>::perform(ctx, pos, result2);
        if (result2.exists())
        {
            result.setReference(result2.value());
        }
        else
        {
            if (duk_is_object(ctx->context(), pos))
            {
                ::dukpp03::Maybe<unsigned char> mayber = ::dukpp03::internal::tryGetUnsignedCharProperty(ctx, pos, "r");
                ::dukpp03::Maybe<unsigned char> maybeg = ::dukpp03::internal::tryGetUnsignedCharProperty(ctx, pos, "g");
                ::dukpp03::Maybe<unsigned char> maybeb = ::dukpp03::internal::tryGetUnsignedCharProperty(ctx, pos, "b");
                ::dukpp03::Maybe<unsigned char> maybea = ::dukpp03::internal::tryGetUnsignedCharProperty(ctx, pos, "a");
                if (mayber.exists() && maybeg.exists() && maybeb.exists())
                {
                    if (maybea.exists())
                    {
                        result.setValue(sad::AColor(mayber.value(), maybeg.value(), maybeb.value(), maybea.value()));
                    }
                    else
                    {
                        result.setValue(sad::AColor(mayber.value(), maybeg.value(), maybeb.value()));                    
                    }
                }
            }
        }
    }
    return result;
}

::dukpp03::Maybe<sad::layouts::HorizontalAlignment> dukpp03::GetValue<sad::layouts::HorizontalAlignment, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::layouts::HorizontalAlignment> result;
    ::dukpp03::Maybe<unsigned int> v = dukpp03::GetValue<unsigned int, sad::dukpp03::BasicContext>::perform(ctx, pos);
    if (v.exists())
    {
        if (v.value() <= static_cast<int>(sad::layouts::HorizontalAlignment::LHA_Right))
        {
            result.setValue(static_cast<sad::layouts::HorizontalAlignment>(v.value()));
        }
    }
    return result;
}

::dukpp03::Maybe<sad::layouts::VerticalAlignment> dukpp03::GetValue<sad::layouts::VerticalAlignment, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::layouts::VerticalAlignment> result;
    ::dukpp03::Maybe<unsigned int> v = dukpp03::GetValue<unsigned int, sad::dukpp03::BasicContext>::perform(ctx, pos);
    if (v.exists())
    {
        if (v.value() <= static_cast<int>(sad::layouts::VerticalAlignment::LVA_Bottom))
        {
            result.setValue(static_cast<sad::layouts::VerticalAlignment>(v.value()));
        }
    }
    return result;
}

::dukpp03::Maybe<sad::layouts::StackingType> dukpp03::GetValue<sad::layouts::StackingType, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::layouts::StackingType> result;
    ::dukpp03::Maybe<unsigned int> v = dukpp03::GetValue<unsigned int, sad::dukpp03::BasicContext>::perform(ctx, pos);
    if (v.exists())
    {
        if (v.value() <= static_cast<int>(sad::layouts::StackingType::LST_NoStacking))
        {
            result.setValue(static_cast<sad::layouts::StackingType>(v.value()));
        }
    }
    return result;
}


::dukpp03::Maybe<sad::layouts::Unit> dukpp03::GetValue<sad::layouts::Unit, sad::dukpp03::BasicContext>::perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
)
{
    ::dukpp03::Maybe<sad::layouts::Unit> result;
    ::dukpp03::Maybe<unsigned int> v = dukpp03::GetValue<unsigned int, sad::dukpp03::BasicContext>::perform(ctx, pos);
    if (v.exists())
    {
        if (v.value() <= static_cast<int>(sad::layouts::Unit::LU_Percents))
        {
            result.setValue(static_cast<sad::layouts::Unit>(v.value()));
        }
    }
    return result;
}
