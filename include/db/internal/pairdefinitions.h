#pragma once
// Forward declaration for triplets
namespace sad
{

template<typename T1, typename T2>
class Pair;

template<typename T1, typename T2, typename T3>
class Triplet;

template<typename T1, typename T2, typename T3, typename T4>
class Quadruplet;

namespace db
{

template<>
template<typename T1, typename T2>
class TypeName<sad::Pair<T1, T2> >
{
public:

    enum { SFINAE_BASE_CHECK = false };
	
    static const bool IsSadObject = false;
	
    static inline  void init()
    {

    }
    static inline bool isSadObject()
    {
        return IsSadObject;
    }
    static inline const sad::String& name()
    {
		sad::db::TypeName<T1>::init();
		sad::db::TypeName<T2>::init();		
        return  sad::db::internal::fetchTypeNameForTemplate2(
			"sad::Pair", 
			sad::db::TypeName<T1>::name(), 
			sad::db::TypeName<T2>::name()
		);
    }
    static inline const sad::String& baseName()
    {
        return name();
    }
    enum ObjectCastValueHelper
    {
        CAN_BE_CASTED_TO_OBJECT  = false,
        POINTER_STARS_COUNT = 0
    };
};	


template<>
template<typename T1, typename T2, typename T3>
class TypeName<sad::Triplet<T1, T2, T3> >
{
public:

    enum { SFINAE_BASE_CHECK = false };
	
    static const bool IsSadObject = false;
	
    static inline  void init()
    {

    }
    static inline bool isSadObject()
    {
        return IsSadObject;
    }
    static inline const sad::String& name()
    {
		sad::db::TypeName<T1>::init();
		sad::db::TypeName<T2>::init();		
		sad::db::TypeName<T3>::init();		
        return  sad::db::internal::fetchTypeNameForTemplate3(
			"sad::Triplet", 
			sad::db::TypeName<T1>::name(), 
			sad::db::TypeName<T2>::name(),
			sad::db::TypeName<T3>::name()
		);
    }
    static inline const sad::String& baseName()
    {
        return name();
    }
    enum ObjectCastValueHelper
    {
        CAN_BE_CASTED_TO_OBJECT  = false,
        POINTER_STARS_COUNT = 0
    };
};	


template<>
template<typename T1, typename T2, typename T3, typename T4>
class TypeName<sad::Quadruplet<T1, T2, T3, T4> >
{
public:

    enum { SFINAE_BASE_CHECK = false };
	
    static const bool IsSadObject = false;
	
    static inline  void init()
    {

    }
    static inline bool isSadObject()
    {
        return IsSadObject;
    }
    static inline const sad::String& name()
    {
		sad::db::TypeName<T1>::init();
		sad::db::TypeName<T2>::init();		
		sad::db::TypeName<T3>::init();	
		sad::db::TypeName<T4>::init();		
        return  sad::db::internal::fetchTypeNameForTemplate4(
			"sad::Quadruplet", 
			sad::db::TypeName<T1>::name(), 
			sad::db::TypeName<T2>::name(),
			sad::db::TypeName<T3>::name(),
			sad::db::TypeName<T4>::name()			
		);
    }
    static inline const sad::String& baseName()
    {
        return name();
    }
    enum ObjectCastValueHelper
    {
        CAN_BE_CASTED_TO_OBJECT  = false,
        POINTER_STARS_COUNT = 0
    };
};	

}

}
