//---------------------------------------------------------------------------


#pragma hdrstop

#include "IOFun.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//--------------------------------------------------------------------------------
bool __fastcall SaveFont( TStream* stream, TFont* Font)
{
    int int_size( sizeof(int) );
    int site[2], size[2];
    String cls_name = "TFont";

    site[0] = stream->Position;
    size[0] = stream->Size;
    try
    {
        stream->Write( &(size[0]), int_size );
        SaveString(stream, cls_name);
        cls_name = Font->Name;
        SaveString(stream, cls_name);

        TFontCharset charset = Font->Charset;
        stream->Write(&charset, sizeof(TFontCharset));

        TColor color(Font->Color);
        stream->Write(&color, sizeof(TColor));

        int height( Font->Height );
        stream->Write(&height,int_size);

        int sz(Font->Size);
        stream->Write(&sz, int_size);

        TFontPitch pitch( Font->Pitch );
        stream->Write(&pitch, sizeof(TFontPitch));

        int pixlperinch ( Font->PixelsPerInch);
        stream->Write(&pixlperinch, int_size);

        TFontStyles style(Font->Style);
        stream->Write(&style, sizeof(TFontStyle) );

        site[1] = stream->Position;
        size[1] = site[1] - site[0];
        stream->Position = site[0];
        stream->Write( &(size[1]), int_size );
        stream->Position = site[1];
        return( true );
    } catch(...)
    {
        stream->Position = site[0];
        stream->Size = size[0];
        return ( false );
    }
}
//---------------------------------------------------------------------------
bool __fastcall LoadFont( TStream* stream, TFont* Font)
{
    int int_size( sizeof(int) );
    int site[2], size[2],length,i,j;
    String cls_name = "TFont";

    site[0] = stream->Position;
    size[0] = stream->Size;
    try
    {
        stream->Read( &(size[1]), int_size );
        LoadString(stream, cls_name);
        if( cls_name != "TFont" ) throw(-1);

        LoadString(stream, cls_name);
        Font->Name = cls_name;

        TFontCharset charset;
        stream->Read(&charset, sizeof(TFontCharset));
        Font->Charset = charset;

        TColor color;
        stream->Read(&color, sizeof(TColor));
        Font->Color = color;

        int height ;
        stream->Read(&height,int_size);
        Font->Height = height;

        int sz;
        stream->Read(&sz, int_size);
        Font->Size = sz;

        TFontPitch pitch;
        stream->Read(&pitch, sizeof(TFontPitch));
        Font->Pitch = pitch;

        int pixlperinch;
        stream->Read(&pixlperinch, int_size);
        Font->PixelsPerInch = pixlperinch;

        TFontStyles style;
        stream->Read(&style, sizeof(TFontStyle) );
        Font->Style = style;
        stream->Position = site[0] + size[1];
        return( true );
    } catch(...)
    {
        stream->Position = site[0];
        stream->Size = size[0];
        return ( false );
    }
}

//---------------------------------------------------------------------------
bool __fastcall SavePen( TStream* stream, TPen* pen)
{
    int int_size( sizeof(int) );
    int site[2], size[2];
    String cls_name = "TPen";

    site[0] = stream->Position;
    size[0] = stream->Size;
    try
    {
        stream->Write( &(size[0]), int_size );
        SaveString(stream, cls_name);

        TColor color(pen->Color);
        stream->Write(&color, sizeof(TColor));

        TPenMode mode( pen->Mode );
        stream->Write(&mode, sizeof(TPenMode));

        TPenStyle style( pen->Style );
        stream->Write(&style, sizeof(TPenStyle));

        int width( pen->Width );
        stream->Write( &width, int_size);

        site[1] = stream->Position;
        size[1] = site[1] - site[0];
        stream->Position = site[0];
        stream->Write( &(size[1]), int_size );
        stream->Position = site[1];
        return( true );
    } catch(...)
    {
        stream->Position = site[0];
        stream->Size = size[0];
        return ( false );
    }
}
//---------------------------------------------------------------------------
bool __fastcall LoadPen( TStream* stream, TPen* pen)
{
    int int_size( sizeof(int) );
    int site[2], size[2];
    String cls_name;

    site[0] = stream->Position;
    size[0] = stream->Size;
    try
    {
        stream->Read( &(size[1]), int_size );
        LoadString(stream, cls_name);
        if( cls_name != "TPen" ) throw( -1 );

        TColor color;
        stream->Read(&color, sizeof(TColor));
        pen->Color = color;

        TPenMode mode ;
        stream->Read(&mode, sizeof(TPenMode));
        pen->Mode = mode;

        TPenStyle style;
        stream->Read(&style, sizeof(TPenStyle));
        pen->Style = style;

        int width;
        stream->Read( &width, int_size);
        pen->Width = width;
        stream->Position = site[0] + size[1];
        return( true );
    } catch(...)
    {
        stream->Position = site[0];
        return ( false );
    }
}

//---------------------------------------------------------------------------
bool __fastcall SaveString(TStream* stream, String& str)
{
    int length = str.Length();
    try{
        stream->Write(&length, sizeof(int) );
        if(length!=0) stream->Write(str.c_str(), length );
        return(true);
    }catch(...){return(false);};
}
//---------------------------------------------------------------------------
bool __fastcall LoadString(TStream* stream, String& str)
{
    int length;
    try{
        stream->Read(&length, sizeof(int) );
        if(length!=0){
            str.SetLength( length );
            stream->Read(str.c_str(), length );
        }
        return(true);
    }catch(...){
        return(false);
    }
}
//---------------------------------------------------------------------------
bool __fastcall SaveStrings(TStream* stream, String* string, int count)
{
    int int_size( sizeof(int) );
    int site[2], size[2],length,i,j;
    String cls_name = "AnsiString";
    site[0] = stream->Position;
    size[0] = stream->Size;
    try
    {
        stream->Write( &(size[0]), int_size );
        length = cls_name.Length();
        stream->Write(&length, int_size);
        stream->Write(cls_name.c_str(), length );
        stream->Write(&count, int_size);
        for(i=0; i < count; i++ )
        {
            length = string[i].Length();
            stream->Write( &length, int_size );
            stream->Write( string[i].c_str(), length );
        }
        site[1] = stream->Position;
        size[1] = site[1] - site[0];
        stream->Position = site[0];
        stream->Write( &(size[1]), int_size );
        stream->Position = site[1];
        return( true );
    } catch(...)
    {
        stream->Position = site[0];
        stream->Size = size[0];
        return ( false );
    }
}
//---------------------------------------------------------------------------
int __fastcall LoadStrings(TStream* stream, String* string, int count)
{
    int int_size( sizeof(int) );
    int site[2], size[2],length,store_count, read_count, i;
    String cls_name = "AnsiString";

    site[0] = stream->Position;
    size[0] = stream->Size;
    stream->Read( &(size[1] ), int_size );

    try
    {
        stream->Read(&length, int_size);
        cls_name.SetLength( length );
        stream->Read(cls_name.c_str(), length );
        if( cls_name != "AnsiString" ) throw(-1);

        stream->Read(&store_count, int_size);
        read_count = min ( store_count, count );
        for(i=0; i < read_count; i++ )
        {
            stream->Read( &length, sizeof(int) );
            string[i].SetLength( length );
            stream->Read( string[i].c_str(), length );
        }

        if( read_count != store_count ) stream->Position = site[0] + size[1];
        return( read_count );

    } catch(...)
    {
        stream->Position = site[0];
        stream->Size = size[0];
        return ( 0 );
    }
}
//---------------------------------------------------------------------------
bool __fastcall SaveStrings(TStream* stream, vector<String>& vec)
{
    int int_size( sizeof(int) );
    int site[2], size[2],count;
    WORD length;
    AnsiString type;

    type = AnsiString( typeid(String).name() );
    site[0] = stream->Position;
    size[0] = stream->Size;
    try
    {
        stream->Write( &(size[0]), sizeof(int) );
        length = type.Length();
        stream->Write( &length, int_size);
        stream->Write( type.c_str(), length );

        count = vec.size();
        stream->Write( &count, int_size);

        for(int i=0; i < count; i++){
            length = (WORD)vec[i].Length();
            stream->Write( &length,sizeof(WORD));
            if(length!=0) stream->Write( vec[i].c_str(), length );
        }
        site[1] = stream->Position;
        size[1] = site[1] - site[0];
        stream->Position = site[0];
        stream->Write( &(size[1]), sizeof(int) );
        stream->Position = site[1];
        return( true );
    }
    catch(...)
    {
        stream->Position = site[0];
        stream->Size = size[0];
        return ( false );
    }
}

//---------------------------------------------------------------------------
bool __fastcall LoadStrings(TStream* stream, vector<String>& vec)
{
    int int_size( sizeof(int) );
    int site[2], size[2],store_count, i;
    WORD length;
    String cls_name, type, buf;
    type = AnsiString( typeid(String).name() );

    site[0] = stream->Position;
    size[0] = stream->Size;
    stream->Read( &(size[1] ), int_size );

    try
    {
        stream->Read(&length, int_size);
        cls_name.SetLength( length );
        stream->Read(cls_name.c_str(), length );
        if( cls_name != type ) throw(-1);

        stream->Read(&store_count, int_size);
        for(i=0; i < store_count; i++ ){
            stream->Read(&length, sizeof(WORD));
            if(length!=0){
                buf.SetLength( length );
                stream->Read( buf.c_str(), length);
            }else buf="";
            vec.push_back( buf );
        }
        site[1] = stream->Position;
        if( (site[1] - site[0]) != size[1] ) stream->Position = site[0] + size[1];
        return( store_count );

    } catch(...){
        stream->Position = site[0];
        stream->Size = size[0];
        return ( 0 );
    }
}
//------------------------------------------------------------------------------
void __fastcall SaveRect(TStream* stream, TRect& rect)
{
    int i;
    i = rect.Left;
    stream->Write(&i,sizeof(int));
    i = rect.Top;
    stream->Write(&i,sizeof(int));
    i = rect.Right;
    stream->Write(&i,sizeof(int));
    i = rect.Bottom;
    stream->Write(&i,sizeof(int));
}
//------------------------------------------------------------------------------
void __fastcall LoadRect(TStream* stream, TRect& rect)
{
    int i;
    stream->Read(&i,sizeof(int));
    rect.Left =i;
    stream->Read(&i,sizeof(int));
    rect.Top = i;
    stream->Read(&i,sizeof(int));
    rect.Right = i;
    stream->Read(&i,sizeof(int));
    rect.Bottom = i;
}
//------------------------------------------------------------------------------
bool __fastcall NextObject(TStream* stream)
{
    int site[2], size[2];

    site[0] = stream->Position;
    size[0] = stream->Size;
    if( site[0] == size[0] ) return(false);

    stream->Read(&(size[1]), sizeof(int));
    stream->Position = site[0] + size[1];

    return(true);
}
//------------------------------------------------------------------------------
String __fastcall CurrentObjectName(TStream* stream)
{
    int site[2], size[2];
    String class_name("");

    site[0] = stream->Position;
    size[0] = stream->Size;
    if( site[0] == size[0] ) return("");

    stream->Read(&(size[1]), sizeof(int));
    if( size[1] == sizeof(int) ) return("");
    LoadString(stream, class_name);
    stream->Position = site[0];
    return(class_name);
}

