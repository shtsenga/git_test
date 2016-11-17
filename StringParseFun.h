//---------------------------------------------------------------------------
#include <math.h>
#include <stack.h>
#include <typeinfo.h>
#include <vector.h>
#include <vcl.h>
#ifndef StringParseFunH
#define StringParseFunH
//---------------------------------------------------------------------------
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal, vector<TPoint>& pSep);
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal);
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal, vector<String>& Sep);
bool __fastcall ParseString(String S, AnsiString Sym, TStringList* List);
bool __fastcall ParseString(String S, AnsiString Sym, vector<String>& List);
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal, vector<char>& Sep);
bool __fastcall ParseString(String S, String Sym, vector<String>& Val, vector<char>& Sep);
bool __fastcall ParseColon(String Begin, String End, String Step, vector<String>& List);
bool __fastcall ParseColon(String& S);
bool __fastcall ParseColon(String S, vector<String>& List);
bool __fastcall ParseRepeat(String& S);
bool __fastcall ParseSequence(String& str, vector<String>& list);
bool __fastcall ParseSequence(String& Str, bool RemoveSpace);
bool __fastcall ParseIntStr(String str, vector<int>& seq);
bool __fastcall ParseDoubleStr(String str, vector<double>& seq);
bool __fastcall Parse2DList(String& str, vector<String>& Xstr, vector<String>& Ystr );
// Parse integer , double list
bool __fastcall ParseListStr(String s, vector<int>& seq);
void __fastcall ParseTabRetSep(const String& S,vector< vector<String> >& Lst);
void __fastcall ParseSep(const String& S, char Sep, vector<String>& Lst);
template<class T1>  String __fastcall Vec2Str(vector<T1>& v,int precision=0,int digits=0);
//Template function
// Give t1, v1, t2, v2... list string and time_interval
// the program will generate v1, v1,v1....v2,v2.... list
//----------------------------------------------------------------------------
template<class T1> bool SequenceList(String str, double space, double shift, vector<T1>& buffer)
{
    vector<String> str_list(0);
    vector<double> t_mark(0);
    vector<T1> v_lst(0);
    int t,index, t_next;
    T1 current_val;

    //Parse string to list
    ParseString(str, ",", str_list);
    if( (str_list.size() % 2)>0 ) return( false );
    if( str_list.size()==0) return(false);
    try{
        //gather edge time, and transition value
        for(unsigned i =0; i < str_list.size()/2; i++){
            t_mark.push_back((str_list[2*i].ToDouble()+shift)/space);
            if( str_list[2*i+1].UpperCase().Pos("X") > 0 ){
                v_lst.push_back((T1)(str_list[2*i+1].ToInt()));
            } else{
                v_lst.push_back((T1)(str_list[2*i+1].ToDouble()));
            }
        }
        //Save to buffer
        buffer.clear();
        t_next = t_mark[0];
        current_val = v_lst[0];
        t=0;    index=0;
        for(;;){
            if( t >= t_next ){
                index++;
                if(index == (int)t_mark.size()){
                    buffer.push_back(v_lst[index-1]);
                     return(true);
                }
                t_next = t_mark[index];
                current_val  = v_lst[index-1];
                buffer.push_back(current_val);
                t++;
            }else{
                buffer.push_back(current_val);
                t++;
            }
        }
    } catch(...){
        buffer.clear();
        return( false);
    }
};
//----------------------------------------------------------------------------
template<class T1>  String __fastcall Vec2Str(vector<T1>& v,int precision,int digits)
{
    String l("");
    String tn=typeid(T1).name();
    if((tn=="int")||(tn=="short")||(tn=="char")||(tn=="long")||(tn=="TColor")){
        for(uint i=0;i<v.size();i++)l+=IntToStr((int)v[i])+",";
    }else if((tn=="unsigned int")||(tn=="unsigned short")||(tn=="unsigned char")||
             (tn=="unsigned long")){
        for(uint i=0;i<v.size();i++)l+=IntToStr((unsigned int)v[i])+",";
    }else if((tn=="__int64")){
        for(uint i=0;i<v.size();i++)l+=IntToStr((__int64)v[i])+",";
    }else if((tn=="double")||(tn=="float")||(tn=="TDateTime")||(tn=="long double")){
        if(precision==0){
            for(uint i=0;i<v.size();i++)l+=FloatToStr(v[i])+",";
        }else{
            for(uint i=0;i<v.size();i++)l+=FloatToStrF(v[i],ffGeneral,precision,digits)+",";
        }
    }
    return(l);
}

#endif
