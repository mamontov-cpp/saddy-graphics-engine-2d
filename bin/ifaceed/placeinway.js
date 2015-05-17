var wayid = "way";
var scenename = "primary";
var schemaname = "placeable_item2";
var templateobjectname = "card_";
var instanceobjectname = "card_on_way_";
var groupname = "allcards";
var totalobjects = 70;

var treeitemname = E.resourceSchema(schemaname).TreeItemName;
var schemaitemrectangle = E.resourceOptions(treeitemname).Rectangle; 

var waytime = E.ways.attr(wayid, "totaltime");
var pointcount = E.ways.length(wayid);
var points = [];

var i = 0, pnt1, pnt2;
if (pointcount <= 1)
      throw "A way should be at leas two points";

for(var i = 1; i < pointcount; i++)
{
    pnt1 = E.ways.point(wayid, i - 1);
    pnt2 = E.ways.point(wayid, i);

    points.push([
            p2d(pnt1.x, pnt1.y),
            p2d(pnt2.x, pnt2.y)
   ]);
}

if (E.ways.attr(wayid, "closed"))
{
    pnt1 = E.ways.point(wayid, pointcount - 1);
    pnt2 = E.ways.point(wayid, 0);
     
   points.push([
            p2d(pnt1.x, pnt1.y),
            p2d(pnt2.x, pnt2.y)
   ]);
}
var timeslice = waytime / totalobjects - 1;
var sideslice = waytime / points.length;
var mytime =  0;
var myside  = 0;
var myrelativetime, mmrt;
var mypoint = 0;
var myrect = 0;
var object = 0;
var instances = [], instance = 0;
for(i = 0; i < totalobjects; i++)
{
    mytime = timeslice * i;
    myside = parseInt(mytime / sideslice);
    myrelativetime = (mytime - myside * sideslice) / sideslice;
    mmrt = 1 - myrelativetime;
    mypoint = (points[myside][0].mul(myrelativetime))
                      .sum(points[myside][1].mul( mmrt));
    myrect = schemaitemrectangle.movedToPoint(mypoint);
    object = E.scenenodes.addCustomObject({
         "schema" :  schemaname,
         "fontsize" : 16,
         "scene" : scenename,
         "text" : "Ignored",
         "area" : myrect,
         "name" : templateobjectname + i
   });
   instance = E.animations.instances.addWayInstance({
       "name" : instanceobjectname + i,
       "way" : wayid,
       "object": object,
       "starttime" : mytime
  });
  instances.push(instance);
}

var allgroup = E.animations.groups.add({
   "name" : groupname,
   "looped" : true
});

for(i = 0; i < instances.length; i++ )
{
   E.animations.groups.addInstance(allgroup, instances[i]);
}
