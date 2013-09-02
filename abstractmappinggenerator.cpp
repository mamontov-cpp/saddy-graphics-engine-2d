/*! This file is used to generate some mappings for data methods for
    properties
 */
#include <stdio.h>

int main(int argc, char ** argv)
{
   //Generate void mappings
   printf("#pragma once\n\n");
   
   
   //Generate void mappings
   for (int args=0;args<17;args++)
   {
    printf("template<typename _Class");
    for (int i=0;i<args;i++)
    {
     printf(", typename T%d",i);
    }
    printf(">\n");
    printf("class VoidMappedMethod%d: public MappedMethod<_Class>\n{\n",args);
    printf("private:\n");
    //Callback
    printf("        void (_Class::*m_callback)(");
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("T%d a%d",i,i);
    }
    printf(");\n");
    printf("public:\n");
    printf("       inline VoidMappedMethod%d(  void (_Class::*callback)(",args);
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("T%d a%d",i,i);
    }
    printf(")  ) { m_callback=callback; }\n");
    printf("       virtual void call(const sad::VariantVector & v,ActionContext * c)\n");
    printf("       {    \n");
    printf("             if (v.count()!=%d) throw new serializable::InvalidParams(%d,v.count(),c);\n",args,args);
    printf("             (this->realObject()->*m_callback)(");
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("v[%d].template get<T%d>(c)",i,i);
    }
    printf(");\n");
    printf("       }    \n");
    printf("};\n\n");
    
    printf("template< typename _Class ");
    for (int i=0;i<args;i++)
    {
     printf(", typename T%d",i);
    }
    printf(">\nAbstractProperty * createVoidMethodBinding( void (_Class::*callback)(");
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("T%d a%d",i,i);
    }
    printf(")  )\n{\n");
    printf(" return new VoidMappedMethod%d<_Class",args);
    for (int i=0;i<args;i++)
    {
     printf(",T%d",i);
    }
    printf(">(callback);\n");
    printf("}\n");
    
    
   }

   //Generate non-void mappings
   //Generate void mappings
   for (int args=0;args<17;args++)
   {
    printf("template<typename _ReturnType,typename _Class");
    for (int i=0;i<args;i++)
    {
     printf(", typename T%d",i);
    }
    printf(">\n");
    printf("class NonVoidMappedMethod%d: public ReturnMappedMethod<_Class,_ReturnType>\n{\n",args);
    printf("private:\n");
    //Callback
    printf("        _ReturnType (_Class::*m_callback)(");
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("T%d a%d",i,i);
    }
    printf(");\n");
    printf("public:\n");
    printf("       inline NonVoidMappedMethod%d(  _ReturnType (_Class::*callback)(",args);
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("T%d a%d",i,i);
    }
    printf(")  ) { m_callback=callback; }\n");
    printf("       virtual void call(const sad::VariantVector & v,ActionContext * c)\n");
    printf("       {    \n");
    printf("             if (v.count()!=%d) throw new serializable::InvalidParams(%d,v.count(),c);\n",args,args);
    printf("             this->m_return_data = (this->realObject()->*m_callback)(");
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("v[%d].template get<T%d>(c)",i,i);
    }
    printf(");\n");
    printf("       }    \n");
    printf("};\n\n");
    
    printf("template<typename _ReturnType, typename _Class ");
    for (int i=0;i<args;i++)
    {
     printf(", typename T%d",i);
    }
    printf(">\nAbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(");
    for (int i=0;i<args;i++)
    {
     if (i!=0)
        printf(", ");
     printf("T%d a%d",i,i);
    }
    printf(")  )\n{\n");
    printf(" return new NonVoidMappedMethod%d<_ReturnType,_Class",args);
    for (int i=0;i<args;i++)
    {
     printf(",T%d",i);
    }
    printf(">(callback);\n");
    printf("}\n");
    
    
   } 
   return 0; 
}