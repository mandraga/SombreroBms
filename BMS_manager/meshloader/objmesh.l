
%{
#include <stdio.h>
#include "parse_codes.h"
#if 0
#define DEBUG_FLEX(m) printf(m)
#else
#define DEBUG_FLEX(m)
#endif
int           num_line;
%}

%x snotused
%x sobjectname
%x sgroupname
%x ssmooth
%x sface
%%
[\n]                     DEBUG_FLEX("EOL\n"); num_line++; return (EOL);
[ ]
#.*                      DEBUG_FLEX("COMMENT\n"); return (COMMENT);
v                        DEBUG_FLEX("VERTEX "); return (VERTEX);
vt                       DEBUG_FLEX("TEXTUR_COORD "); return (TEXTURE_COORD);
vn                       DEBUG_FLEX("NORMAL "); return (NORMAL);
vp                       DEBUG_FLEX("FREEFORM "); BEGIN(snotused); return (FREEFORM);
f                        DEBUG_FLEX("FACE "); BEGIN (sface); return (FACE);
mtllib                   DEBUG_FLEX("MATERIAL_FILE "); BEGIN(snotused); return (MATERIAL_FILE);
usemtl                   DEBUG_FLEX("MATERIAL_NAME "); BEGIN(snotused); return (MATERIAL_NAME);
o                        DEBUG_FLEX(" OBJECT_NAME "); BEGIN(sobjectname); return (OBJECT_NAME);
g                        DEBUG_FLEX(" OBJECT_GROUP_NAME "); BEGIN(sgroupname); return (OBJECT_GROUP_NAME);
s                        DEBUG_FLEX(" SMOOTH_SHADING "); BEGIN(ssmooth); return (SMOOTH_SHADING);
"+"
"-"                      DEBUG_FLEX(" SIGN "); return (SIGN);
[0-9]+\.[0-9]+           DEBUG_FLEX(" FLOAT "); return (FLOAT);
<snotused>
{
.*
[\n]                     DEBUG_FLEX(" EOL\n");  num_line++; BEGIN(INITIAL); return (EOL);
}
<sface>
{
[ ]                      return (SPACE);
[0-9]+                   DEBUG_FLEX(" INT "); return (FACE_INDEX);
"/"                      DEBUG_FLEX(" / "); return (FACE_POINT_SEPARATOR);
[\n]                     DEBUG_FLEX(" EOL\n");  num_line++; BEGIN(INITIAL); return (EOL);
}
<sobjectname>
{
[ ]
[[:alnum:]]+             DEBUG_FLEX(" OBJECT_NAME");  return (OBJECT_NAME);
[\n]                     DEBUG_FLEX(" EOL\n");  num_line++; BEGIN(INITIAL); return (EOL);
}
<sgroupname>
{
[ ]
[[:alnum:]]+             DEBUG_FLEX(" GROUP_NAME");  return (GROUP_NAME);
[\n]                     DEBUG_FLEX(" EOL\n");  num_line++; BEGIN(INITIAL); return (EOL);
}
<ssmooth>
{
[ ]
off                      DEBUG_FLEX(" SMOOTH_OFF"); return (SMOOTH_OFF);
[0-9]+                   DEBUG_FLEX(" SMOOTH_GROUP"); return (SMOOTH_GROUP);
[\n]                     DEBUG_FLEX(" EOL\n");  num_line++; BEGIN(INITIAL); return (EOL);
}
%%