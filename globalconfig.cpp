#include "globalconfig.h"

GlobalConfig::GlobalConfig(QObject *parent) :
    QObject(parent)
{
    time_format = "hh:mm AP";
    //Sets Global Date Format
    date_format = "yyyy-M-d";
    //Sets USA States List
    usa_states = QString("IN,AL,AK,AZ,AR,CA,CO,CT,DE,FL,GA,HI,ID,IL,IA,KS,KY,LA,ME,MD,MA,MI,MN,MS,MO,MT,NE,NV,NH,NJ,NM,NY,NC,ND,OH,OK,OR,PA,RI,SC,SD,TN,TX,UT,VT,VA,WA,WV,WI,WY").split(",");

}
