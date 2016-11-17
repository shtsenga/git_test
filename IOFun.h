//---------------------------------------------------------------------------
#include <vcl.h>
#include <CheckLst.hpp>
#include <Grids.hpp>
#include <vector.h>
#include <typeinfo.h>

#ifndef IOFunH
#define IOFunH
//---------------------------------------------------------------------------
bool __fastcall SaveFont( TStream* stream, TFont* Font);
bool __fastcall LoadFont( TStream* stream, TFont* Font);
bool __fastcall SavePen( TStream* stream, TPen* pen);
bool __fastcall LoadPen( TStream* stream, TPen* pen);
bool __fastcall SaveString(TStream* stream, String& str);
bool __fastcall LoadString(TStream* stream, String& str);
bool __fastcall SaveStrings(TStream* stream, String* string, int count);
int  __fastcall LoadStrings(TStream* stream, String* string, int count);
bool __fastcall SaveStrings(TStream* stream, vector<String>& vec);
bool __fastcall LoadStrings(TStream* stream, vector<String>& vec);
void __fastcall LoadRect(TStream* stream, TRect& rect);
void __fastcall SaveRect(TStream* stream, TRect& rect);
bool __fastcall NextObject(TStream* stream);
String __fastcall CurrentObjectName(TStream* stream);
template <typename T1> bool __fastcall SaveVector(TStream* stream, vector<T1>& vec);
template <typename T1> bool __fastcall LoadVector(TStream* stream, vector<T1>& vec);
//---------------------------------------------------------------------------
template <typename T1> bool __fastcall SaveVector(TStream* stream, vector<T1>& vec)
{
    int int_size( sizeof(int) );
    int site[2], size[2],length, count;
    AnsiString type;

    type = AnsiString( typeid( T1 ).name() );
    if( ( type.Pos("String") >0 ) || (type.Pos("*") > 0 )) return( false );
    site[0] = stream->Position;
    size[0] = stream->Size;
    try
    {
        stream->Write( &(size[0]), sizeof(int) );
        length = type.Length();
        stream->Write( &length, int_size);
        stream->Write( type.c_str(), length );

        length = sizeof(T1);
        count = vec.size();
        stream->Write( &count, int_size);
        if( count > 0 ){
            vector<T1>::iterator it;
            for(it=vec.begin() ; it < vec.end(); it++) stream->Write( it, length );
        }

        site[1] = stream->Position;
        size[1] = site[1] - site[0];
        stream->Position = site[0];
        stream->Write( &(size[1]), sizeof(int) );
        stream->Position = site[1];
        return( true );
    }catch(...){
        stream->Position = site[0];
        stream->Size = size[0];
        return ( false );
    }
}

//---------------------------------------------------------------------------
template <typename T1> bool __fastcall LoadVector(TStream* stream, vector<T1>& vec)
{
    int int_size( sizeof(int) );
    int site[2], size[2], length, store_count;
    String cls_name, type;
    type = AnsiString( typeid( T1 ).name() );
    if( ( type.Pos("String") >0 ) || (type.Pos("*") > 0 )) return( false );
    site[0] = stream->Position;
    size[0] = stream->Size;
    stream->Read( &(size[1] ), int_size );

    try{
        stream->Read(&length, int_size);
        cls_name.SetLength( length );
        stream->Read(cls_name.c_str(), length );
        if( cls_name != type ) throw(-1);
        stream->Read(&store_count, int_size);

        if( store_count > 0){
            //vec = vector<T1>( store_count );
            T1* val = new T1;
            length = sizeof(T1);
            for(int i=0; i < store_count; i++){
                stream->Read(val, length);
                vec.push_back(*val);
            }
            delete val;
        }
        stream->Position = site[0] + size[1];
        return( true );

    } catch(...){
        stream->Position = site[0];
        return ( false );
    }
}
//---------------------------------------------------------------------------------


#endif
