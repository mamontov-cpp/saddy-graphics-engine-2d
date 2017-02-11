#include "dukpp-03/getvalue.h"
#include "object.h"
#include "classmetadatacontainer.h"

::dukpp03::Maybe<double> dukpp03::internal::tryGetDoubleProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
)
{
    ::dukpp03::Maybe<double> result;
    if (duk_has_prop_string(ctx->context(), pos, propname))
    {
        duk_get_prop_string(ctx->context(), pos, propname);
        result = ::dukpp03::GetValue<double, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}

::dukpp03::Maybe<int>  dukpp03::internal::tryGetIntProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
)
{
    ::dukpp03::Maybe<int> result;
    if (duk_has_prop_string(ctx->context(), pos, propname))
    {
        duk_get_prop_string(ctx->context(), pos, propname);
        result = ::dukpp03::GetValue<int, sad::dukpp03::BasicContext>::perform(ctx, -1);
        duk_pop(ctx->context());
    }
    return result;
}


::dukpp03::Maybe<unsigned char>  dukpp03::internal::tryGetUnsignedCharProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
)
{
    ::dukpp03::Maybe<unsigned char> result;
    if (duk_has_prop_string(ctx->context(), pos, propname))
    {
        duk_get_prop_string(ctx->context(), pos, propname);
        result = ::dukpp03::GetValue<unsigned char, sad::dukpp03::BasicContext>::perform(ctx, -1);
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
                        if (sad::ClassMetaDataContainer::ref()->contains(v->baseName()))
                        {
                            bool created = false;
                            if (sad::ClassMetaDataContainer::ref()->get(v->baseName(), created)->canBeCastedTo("sad::Object"))
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
        result.setValue(static_cast<bool>(duk_to_string(ctx->context(), pos) != NULL));
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

