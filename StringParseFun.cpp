//---------------------------------------------------------------------------


#pragma hdrstop

#include "StringParseFun.h"
#include "StringFun.h"
#include "MyTypeDef.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//------------------------------------------------------------------------------
// Given a string and separactors sym
// find words, and separactor and generate words list and separactor list
//example str="1,23;34:23:23,45" sym=",;:"
// st = {(1,1), (3,2), (6,2), (9,2), (12,2), (15,2) }
// separactor = {(2,1), (5,1), (8,1), (11,1), (14,1)}
//--------------------------------------------------------------------------------------
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal, vector<TPoint>& pSep)
{
    vector<int> loc(0);
    unsigned i,j;
    pVal.clear();       pSep.clear();
    if(S=="") return(true);
    //locate all symbol site
    for(i=1; i <=(unsigned)S.Length(); ++i) if(IsCharContained(Sym, S[i])) loc.push_back(i);

    if(loc.size()==0){pVal.push_back(TPoint(1,S.Length())); return(true);};

    for(i=0;i<loc.size();++i){ // get separacter
        j=i+1;
        if(i!=(loc.size()-1))  while((j<loc.size())&&((loc[j]-loc[j-1])==1)) j++;
        pSep.push_back(TPoint(loc[i],loc[j-1]-loc[i]+1));
        i=j-1;
    }
    if(loc[0]>1) pVal.push_back(TPoint(1,loc[0]-1));

    for(i=1;i<loc.size();++i) // get separacter
        if((loc[i]-loc[i-1])>1)pVal.push_back(TPoint(loc[i-1]+1,loc[i]-loc[i-1]-1));

    j = *(loc.end()-1);

    if(j<(unsigned)S.Length())pVal.push_back(TPoint(j+1,S.Length()-j));

    return(true);
}
//--------------------------------------------------------------------------------------
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal)
{
    vector<TPoint> p(0);
    return(ParseString(S,Sym,pVal,p));
}
//--------------------------------------------------------------------------------------
// Given a string and separactors sym
// find words, and separactor and generate words list and separactor list
//example str="1,23;34:23:23,45" sym=",;:"
// st = {(1,1), (3,2), (6,2), (9,2), (12,2), (15,2) }
// separactor = {",",";", ":", ":", ":"}
//--------------------------------------------------------------------------------------
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal, vector<String>& Sep)
{
    vector<TPoint> sp;
    if(!ParseString(S,Sym,pVal,sp)) return(false);
    for(unsigned i=0;i<sp.size();++i) Sep.push_back(S.SubString(sp[i].x,sp[i].y));
    return(true);
}
//--------------------------------------------------------------------------------------
bool __fastcall ParseString(String S, AnsiString Sym, TStringList* List)
{
    vector<TPoint>lst(0),p(0);
    List->Clear();
    if(!ParseString(S, Sym, lst, p)) return(false);
    for(unsigned i=0; i < lst.size(); ++i)
        List->Add( S.SubString(lst[i].x, lst[i].y));
    return(true);
}

//--------------------------------------------------------------------------------------
bool __fastcall ParseString(String S, AnsiString Sym, vector<String>& List)
{
    vector<TPoint> lst(0),p(0);
    List.clear();
    if(!ParseString(S,Sym, lst, p))return(false);
    for(unsigned i=0; i < lst.size(); ++i) List.push_back(S.SubString(lst[i].x, lst[i].y) );
    return(true);
}
//--------------------------------------------------------------------------------------
// Given a string and separactors sym
// find words, and separactor and generate words list and separactor list
//example str="1,23;34:23:23,45" sym=",;:"
// st = {(1,1), (3,2), (6,2), (9,2), (12,2), (15,2) }
// separactor = {',',';', ':', ':', ':'}
//------------------------------------------------------------------------------
bool __fastcall ParseString(String S, String Sym, vector<TPoint>& pVal, vector<char>& Sep)
{
    int j;
    vector<int> loc(0);
    pVal.clear();
    Sep.clear();

    RemoveSpecialCharacter(S,' '); //remove space character
    if(S=="") return(true);

    //locate all symbol site
    for(int i=1; i <=S.Length(); ++i)
        if(IsCharContained(Sym, S[i])){
            loc.push_back(i);
            Sep.push_back(S[i]);
        };
    if(loc.size()==0){
        pVal.push_back(TPoint(1,S.Length()));
        return(true);
    };
    if(loc[0]>1) pVal.push_back(TPoint(1,loc[0]-1));
    for(uint i=1;i<loc.size();++i) // get separacter
        if((loc[i]-loc[i-1])>1)pVal.push_back(TPoint(loc[i-1]+1,loc[i]-loc[i-1]-1));
    j = *(loc.end()-1);
    if(j<S.Length())pVal.push_back(TPoint(j+1,S.Length()-j));
    return(true);
}
//------------------------------------------------------------------------------
bool __fastcall ParseString(String S, String Sym, vector<String>& Val, vector<char>& Sep)
{
    vector<TPoint> p(0);
    Val.clear();
    if(!ParseString(S,Sym,p,Sep))return(false);
    for(unsigned i=0;i<p.size();++i)Val.push_back(S.SubString(p[i].x,p[i].y));
    return(true);
}
//--------------------------------------------------------------------------------------
// decode A:B:C to A , A+B , ... A+(C-1)*B string vector
//--------------------------------------------------------------------------------------
bool __fastcall ParseColon(String Begin, String End, String Step, vector<String>& List)
{
    try{
        List.clear();
        if(IsIntString(Begin)&&IsIntString(End)&&IsIntString(Step)){
        // integer mode ( include hex mode)
            bool HexMode;
            int to, sp, val;
            HexMode = (Begin.Pos("0x")+Begin.Pos("0X"))>0;
            val = Begin.ToInt(); to = End.ToInt(); sp = Step.ToInt();
            if(to >= val){
                if(HexMode) while(val<=to){List.push_back(IntToHex(val,8)); val += sp;}
                else while(val<=to){List.push_back(IntToStr(val)); val += sp;}
            }else{
                sp = -sp;
                if(HexMode) while(val>=to){List.push_back(IntToHex(val,8)); val += sp;}
                else while(val>=to){List.push_back(IntToStr(val)); val += sp;}
            }
        }else{// double mode
            double fr, to, sp,val;
            val = Begin.ToDouble(); to = End.ToDouble(); sp = Step.ToDouble();
            if(to>=val){
                while(val<=to){List.push_back(FloatToStr(val)); val += sp;}
            }else{
                sp= -sp;
                while(val>=to){List.push_back(FloatToStr(val)); val += sp;}
            }
        }
        return(true);
    }catch(...){
        List.clear();
        return(false);
    };
}
//------------------------------------------------------------------------------
// Replace A:B:C string by A,A+B,A+2B,..,A+(C-1)*B
//------------------------------------------------------------------------------
bool __fastcall ParseColon(String& S)
{
    vector<String> str;
    if(!ParseString(S,":",str)) return(false);
    if(str.size()==0)return(false);
    else if(str.size()==1) return(true);
    ParseColon(str[0],str[1],(str.size()>2)?str[2]:String("1"), str);
    if(str.size()==0) return(false);
    S = str[0];
    for(unsigned i=1; i<str.size();++i) S=S+","+str[i];
    return(true);
}
//------------------------------------------------------------------------------
bool __fastcall ParseColon(String S, vector<String>& List)
{
    if(!ParseString(S,":",List)) return(false);
    if(List.size()==0)return(false);
    else if(List.size()==1) return(true);
    ParseColon(List[0],List[1],(List.size()>2)?List[2]:String("1"), List);
    return(List.size()>0);
}

// given A/B --> A,A,A...,A repeat A with B times
// success return true
//--------------------------------------------------------------------------------------
bool __fastcall ParseRepeat(String& S)
{
    vector<String> l;
    int cnt;
    try {
        if((!ParseString(S,"/",l))||(l.size()!=2)) return(false);
        S = l[0]; cnt = l[1].ToInt();
        for(int i=1; i < cnt; ++i) S = S + "," + l[0];
        return( true);
    }catch (... ){return( false );}
}
//--------------------------------------------------------------------------------------
//Generate a sequence by the following rules
//  A,B     separate two value
//  A/B     create A,A,....etc repeat B times
//  A:B:C   create A, A+B, A+2B, A+3B...A+(C-1)*B sequence
//  (A,B)/C    create A,B,A,B...etc with A,B repeat C times
//--------------------------------------------------------------------------------------
bool __fastcall ParseSequence(String& Str, bool RemoveSpace)
{
    int i,bra,ket, repnum,pos;
    String s,pre,post;
    if(!RemoveSpace){
        RemoveSpecialCharacter(Str,' ');
        RemoveSpace = true;
    }
    try{
        for(;;){
            if((pos=Str.Pos("/"))>0){//parse repeat number
                i=pos;
                while((++i)<=Str.Length()){
                    if(i==Str.Length()){
                        repnum = Str.SubString(pos+1,Str.Length()).ToInt();
                        post="";
                    }else if(Str[i]==','){
                        repnum = Str.SubString(pos+1,i-pos-1).ToInt();
                        post = Str.SubString(i,Str.Length());
                        break;
                    }
                }
                //get repeat item
                if(Str[pos-1]==')'){
                    ket=pos-1;
                    if(!RetrieveBraket(Str,'(',')',bra,ket,s))return(false);
                    if(!ParseSequence(s,RemoveSpace))return(false);
                    pre = Str.SubString(1,bra-1);
                }else{
                    i= pos;
                    while((--i)>0){
                        if(i==1){
                            s = Str.SubString(1,pos-1);
                            pre="";
                        }else if(Str[i]==','){
                            s = Str.SubString(i+1,pos-i-1);
                            pre = Str.SubString(1,i);
                            break;
                        }
                    }
                }
                Str = pre;
                for(i=0;i<repnum;++i) Str = Str + s + ((i!=(repnum-1))?",":"");
                Str = Str + post;
            }else if((pos=Str.Pos("("))>0){
                bra = pos; ket=0;
                if(!RetrieveBraket(Str,'(',')',bra,ket,s))return(false);
                if(!ParseSequence(s,RemoveSpace))return(false);
                Str = Str.SubString(1,bra-1)+s+Str.SubString(ket+1,Str.Length());
            }else if((pos=Str.Pos(":"))>0){
                double base,stp;
                int count;
                //retrive base value
                i=pos;
                while((--i)>0){
                    if(Str[i]==','){
                        base = Str.SubString(i+1,pos-i-1).ToDouble();
                        pre = (i!=1)?Str.SubString(1,i):(String)"";
                        break;
                    }else if(i==1){
                        base= Str.SubString(1,pos-1).ToDouble();
                        pre="";
                    }
                }
                s= Str.SubString(pos+1,Str.Length());
                i=0;  stp=1.0;  post="";
                while((++i)<=s.Length()){
                    if(s[i]==':'){
                        stp = s.SubString(1,i-1).ToDouble();
                        if((pos=s.Pos(","))<1){
                            count= s.SubString(i+1,s.Length()).ToInt();
                        }else{
                            count = s.SubString(i+1,pos-i-1).ToInt();
                            if(pos!=s.Length())post = s.SubString(pos,s.Length());
                        }
                        break;
                    }else if(s[i]==','){
                        count = s.SubString(1,i-1).ToInt();
                        if(i!=s.Length()) post = s.SubString(i,s.Length());
                        break;
                    }else if(i==s.Length()){
                        count = s.ToInt();
                    }
                }
                Str = pre;
                for(i=0;i<count;++i, base += stp)
                    Str = Str + FloatToStr(base) +((i!=(count-1))?",":"");
                Str = Str + post;
            }else{
                return(true);
            }
        }
    }catch(...){
        return(false);
    }
}
//Generate a sequence by the following rules
//  A,B     separate two value
//  A/B     create A,A,....etc repeat B times
//  A:B:C   create A, A+B, A+2B, A+3B...A+(C-1)*B sequence
//  (A,B)/C    create A,B,A,B...etc with A,B repeat C times
bool __fastcall ParseSequence(String& str, vector<String>& list)
{
    bool state(false);
    if(ParseSequence(str,false)){
        list.clear();
        ParseString(str,",",list);
        state=true;
    }
    return(state);
}
//--------------------------------------------------------------------------------------
//Generate a integer sequence by the following rules
//  A,B     separate two value
//  A/B     create A,A,....etc repeat B times
//  A:B:C   create A, A+B, A+2B, A+3B...A+(C-1)*B sequence
//  (A,B)/C    create A,B,A,B...etc with A,B repeat C times
//--------------------------------------------------------------------------------------
bool __fastcall ParseIntStr(String str, vector<int>& seq)
{
    vector<String> lst;
    try{
        seq.clear();
        ParseSequence(str, lst);
        for(unsigned i=0; i<lst.size(); ++i)
            seq.push_back(lst[i].ToInt());
        return( true );
    } catch ( ... ){
        return( false );
    }
}
//--------------------------------------------------------------------------------------
//Generate a double sequence by the following rules
//  A,B     separate two value
//  A/B     create A,A,....etc repeat B times
//  A:B:C   create A, A+B, A+2B, A+3B...A+(C-1)*B sequence
//  (A,B)/C    create A,B,A,B...etc with A,B repeat C times
//--------------------------------------------------------------------------------------
bool __fastcall ParseDoubleStr(String str, vector<double>& seq)
{
    vector<String> lst;
    try{
        if(str=="") return(false);
        seq.clear();
        ParseSequence(str, lst);
        for(unsigned i=0; i<lst.size(); ++i)
            seq.push_back(lst[i].ToDouble());
        return( true );
    } catch ( ... ){
        return( false );
    }
}
//--------------------------------------------------------------------------------------
//decode a integer sequence
// Given a string s, this code generate a sequence following the rules
//  A,B generate A, B integers
//  A-B generate a list of integers as A, A+1, A+2, ... B
// example s = "1,2-5,3,4"  seq = {1,2,3,4,5,3,4}
//--------------------------------------------------------------------------------------
bool __fastcall ParseListStr(String s, vector<int>& seq)
{
    vector<String>l(0);
    vector<char> sep(0);
    int val1, val2,index(0);
    ReplaceCharacter(s,';',',');
    try
    {
        seq.clear();
        if(!ParseString(s,",-",l,sep)) return(false);
        if(l.size()==0) return(true);
        for(unsigned i=0; i <sep.size(); ++i)
        {
            if(sep[i]==',') seq.push_back(l[index++].ToInt());
            else{
                val1 = l[index++].ToInt();
                val2 = l[index++].ToInt();
                if(val2>val1)   while(val1!=(val2+1))seq.push_back(val1++);
                else            while(val1!=(val2-1))seq.push_back(val1--);
                ++i;
            }
        }
        if(index<(signed)l.size())seq.push_back(l[index].ToInt());
        return(true);
    }catch(...){ seq.clear();return(false);};
}

// Replace A:B:C string by A,A+B,A+2B,..,A+(C-1)*B
//--------------------------------------------------------------------------------------
//Generate a sequence by the following rules
//  A,B         means a XY pair (A,B)
//  A1,B1,A2,B2 create (A1,B1), (A2,B2) pair
//  A,B/C       create (A,B),(A,B)...C pairs
//  A,B:C:D     create (A,B),(A,B+C), (A,B+2C)..(A,B+(D-1)*C) pairs
//  (...)/C     create pairs inside braket C times
//--------------------------------------------------------------------------------------
bool __fastcall Parse2DList(String& str, vector<String>& Xstr, vector<String>& Ystr )
{
    int cnt,sf,j;
    unsigned i;
    vector<TPoint> lst, sym;
    vector<String> val, opt, buf;
    String temp, temp2, s1,s2;
    try
    {
        ReplaceCharacter(str,';',',');
        ParseString(str, ":,()/", lst, sym);
        if(sym.size()==0) {return(false); }
        sf = sym[0].x > lst[0].x ? 0:-1;
        temp="";

        //create value and operator vectors
        opt.clear();
        for(i=0;i< sym.size(); ++i) opt.push_back( str.SubString(sym[i].x, sym[i].y));
        val.clear();
        for(i=0;i< lst.size(); ++i) val.push_back( str.SubString(lst[i].x, lst[i].y));

        for(i=0;i < opt.size(); ++i)
        {
            if(opt[i].Pos("(") > 0 )
            {
                for(j=1; j <= opt[i].Length(); j++)
                {
                    if(opt[i][j]=='(')
                    {
                        buf.push_back(temp);
                        temp="";
                    }
                }
            } else if(opt[i]==",")
            {
                if(((i+1)<opt.size())&&(opt[i+1]=="/") )      //decode A,B/C type
                {
                    s1= val[i+sf] + "," + val[i+sf+1];
                    cnt = val[i+2+sf].ToInt();
                    for(j=0;j<cnt;j++) temp = temp + "," + s1;
                    i = i + ((((i+2)<opt.size()) && ContainChar(opt[i+2],"()"))? 1:2);
                }else if(((i+1)<opt.size())&&(opt[i+1]==":")) //decode A,B:C:D type
                {
                    s1 = val[i+sf]+",";
                    cnt = val[i+sf+3].ToInt();
                    s2 = val[i+sf+1]+val[i+sf+2];
                    if(ContainChar(s2,".eE"))//double type
                    {
                        double d1, d2;
                        d1 = val[i+sf+1].ToDouble();
                        d2 = val[i+sf+2].ToDouble();
                        for(j=0; j<cnt; j++)
                        {
                            s2 = FloatToStrF(d1+((double)j) * d2, ffGeneral,5,5);
                            temp += "," + s1 + s2;
                        }
                    }else   // integer type
                    {
                        int it1, it2;
                        bool hex;
                        it1 = val[i+sf+1].ToInt();
                        it2 = val[i+sf+2].ToInt();
                        hex = ContainChar(s2,"xX")? true : false;
                        for(j=0; j<cnt; j++)
                        {
                            if( hex ) s2 = "0x" + IntToHex(it1 + j*it2,4);
                            else  s2 = IntToStr( it1 + j*it2 );
                            temp += "," + s1 + s2;
                        }
                    }
                    i = i + ((((i+3)<opt.size()) && ContainChar(opt[i+3],"()"))? 2:3);
                }else  // decode A,B standard pair
                {
                    temp = temp + "," + val[i+sf] + "," + val[i+sf+1];
                    i = i+ ((((i+1)<opt.size())&& ContainChar(opt[i+1],"()"))?0:1);
                }
            }else if(opt[i].Pos(")") > 0 )
            {
                for(j=1; j <=opt[i].Length(); j++)
                {
                    if( opt[i][j]=='/' ) //decode (...)/ operator
                    {
                        s1 = (temp[1]==','?"":",") + temp;
                        cnt = val[i+sf+1].ToInt();
                        for(j=1;j<cnt;j++) s1 = s1+ temp;
                        temp = buf[buf.size()-1]+s1;
                        buf.pop_back();
                        i = i + ((((i+1)<opt.size()) && ContainChar(opt[i+1],"()"))?0:1);
                        break;
                    } else if( opt[i][j]==')')
                    {
                        temp = (temp[1]==','?"":",")+ temp;
                        temp = buf[buf.size()-1] + temp;
                        buf.pop_back();
                    }
                }
            }
        }
        ParseString(temp, ",", lst, sym);
        if( (lst.size() % 2)==1 ) return( false );
        cnt = lst.size()/2;
        Xstr.clear();
        Ystr.clear();
        for(j=0;j <cnt; j++)
        {
            Xstr.push_back(temp.SubString(lst[2*j].x, lst[2*j].y));
            Ystr.push_back(temp.SubString(lst[2*j+1].x, lst[2*j+1].y));
        }
        return(true);

    }catch(...)
    {
        Xstr.clear();
        Ystr.clear();
        return( false );
    }
}
//----------------------------------------------------------------------------
// Parse a string containing tab, return char
void __fastcall ParseTabRetSep(const String& S,vector< vector<String> >& Lst)
{
    int p(1);
    vector<String> sl;
    String s;
    Lst.clear();
    for(int i=1;i<=S.Length();++i){
        if(S[i]=='\t'){
            sl.push_back(S.SubString(p,i-p));
            p=i+1;
        }else if(S[i]=='\n'){
            sl.push_back(S.SubString(p,i-p));
            Lst.push_back(sl);
            sl.clear();
            p=i+1;
        }else if(i==S.Length()){
            sl.push_back(S.SubString(p,i-p));
            Lst.push_back(sl);
        }
    }
}
//----------------------------------------------------------------------------
void __fastcall ParseSep(const String& S, char Sep, vector<String>& Lst)
{
    int p(1),i;
    Lst.clear();
    for(int i=1;i<=S.Length();++i){
        if(S[i]==Sep){
            Lst.push_back(S.SubString(p,i-p));
            p=i+1;
        }
    }
    if(p<=S.Length()) Lst.push_back(S.SubString(p,S.Length()));
}
