
// 地图类型
var mapType = {
    localGoogleSatellite  : 0,
    remoteGoogleSatellite : 1,
    localGoogleStreet     : 2,
    remoteGoogleStreet    : 3
}

// 全局变量
var g_arg = {
    obj     : null,
    map     : null,
    street  : null,
    type    : mapType.localGoogleSatellite,
    center  : [28.1128547,112.8668242],
    zoom    : 17,
    minZoom : 3,
    maxZoom : 24,
    allLayer: [],
    nodes   : [],
    rsi     : [],
    group   : null
}

var g_car   = null;

var g_arrow_icon  = L.icon({ iconUrl:'style/image/arrow.png',iconSize: [15, 15]});
var g_arrow2_icon = L.icon({ iconUrl:'style/image/arrow2.png',iconSize: [15, 15]});
var g_car_icon    = L.icon({ iconUrl:'style/image/car.png',iconSize: [25, 48],iconAnchor:[12.5,48] });

var g_color       = ["#FF0000","#00FF00","#FFFF00"];
var g_color_index = 0;


// 初始化地图
function mapInit(){
    var url = "map/s/{z}/{x}/{y}.jpg";
//    g_arg.type = mapType.remoteGoogleStreet;
    if(g_arg.type == mapType.localGoogleSatellite){
        url = "map/s/{z}/{x}/{y}.jpg";
    }else if(g_arg.type == mapType.localGoogleStreet){
        url = "map/m/{z}/{x}/{y}.jpg";
    }else if(g_arg.type == mapType.remoteGoogleSatellite){
        url = "http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}";
    }else if(g_arg.type == mapType.remoteGoogleStreet){
        url = "http://{s}.google.com/vt/lyrs=m&x={x}&y={y}&z={z}";
    }
    
    if(g_arg.map == null){
        g_arg.map = L.map('googleMap',{zoomAnimation:true});
        g_arg.map.setView(g_arg.center,g_arg.zoom);
        if(g_arg.obj != null){
            g_arg.map.on("move",function(event){
                var b = g_arg.map.getBounds();
                var zoom = g_arg.map.getZoom();
                g_arg.obj.getBounds(b.getSouth(),b.getNorth(),b.getWest(),b.getEast(),zoom,g_arg.type);
            })
        }
    }
    
    if(g_arg.map.hasLayer(g_arg.street))g_arg.map.removeLayer(g_arg.street);
    g_arg.street = L.tileLayer(url,{ minZoom: g_arg.minZoom,maxZoom: g_arg.maxZoom,subdomains:['mt0','mt1','mt2','mt3']});
    g_arg.street.addTo(g_arg.map);
	if(g_arg.group == null){
		g_arg.group = L.markerClusterGroup();
		g_arg.map.addLayer(g_arg.group);
	}
}

// 在地图中心添加marker
function addMarkerAtCenter(){
    var center = g_arg.map.getCenter();
    addMarker(center.lng,center.lat);
}

// 添加marker
function addMarker(lng,lat){
    lat = lat.toFixed(7);
    lng = lng.toFixed(7);
    var marker = L.marker([lat,lng]).addTo(g_arg.map);
	marker.dragging.enable();
    marker.bindPopup("lng : "+lng+"</br>lat : "+lat);
    marker.on('dragend',function(event){
        var pt = marker.getLatLng();
        var lat_new = pt.lat.toFixed(7);
        var lng_new = pt.lng.toFixed(7);
        marker.setPopupContent("lng : "+lng_new+"</br>lat : "+lat_new);
	});
    g_arg.map.panTo([lat,lng],{"animate":true,"duration":1});
    saveLayer(marker);
}

// 更新车的位置
function updateCar(lng,lat,heading){
    if(g_car == null){
        g_car = L.marker([lat,lng],{icon:g_car_icon,rotationAngle:heading}).addTo(g_arg.map);
        g_car.bindPopup("lng : "+lng.toFixed(7)+"</br>lat : "+lat.toFixed(7)+"</br>heading : "+heading.toFixed(2));
    }else{
        g_car.setLatLng([lat,lng]);
        g_car.setRotationAngle(heading);
        g_car.setPopupContent("lng : "+lng.toFixed(7)+"</br>lat : "+lat.toFixed(7)+"</br>heading : "+heading.toFixed(2));
    }
    g_arg.map.panTo([lat,lng],{"animate":true,"duration":0.5});
}

// 添加车的位置
function addCar(lng,lat,heading){
    var marker = L.marker([lat,lng],{icon:g_car_icon,rotationAngle:heading}).addTo(g_arg.map);
    marker.bindPopup("lng : "+lng.toFixed(7)+"</br>lat : "+lat.toFixed(7)+"</br>heading : "+heading.toFixed(2));
    saveLayer(marker);
}

// layout 存到全局变量
function saveLayer(single_layer){
    g_arg.allLayer.push(single_layer);
}

// 清除所有
function clearAll(){
    for(var i=0;i<g_arg.allLayer.length;i++){
        g_arg.map.removeLayer(g_arg.allLayer[i])
    }
    g_arg.group.clearLayers();
    g_arg.nodes = [];
    g_arg.rsi   = [];    
}

// 
function checkMapNodeExist(id,region){
    var flag = false;
    for(var i=0;i<g_arg.nodes.length;i++){
        if(g_arg.nodes[i].id != id)continue;
        if(g_arg.nodes[i].region != region)continue;
        flag = true;
    }
    if(!flag){
        var tmp = {};
        tmp.id = id;
        tmp.region = region;
        g_arg.nodes.push(tmp);
    }
    
    return flag;
}

// 
function checkRsiExist(rsi_type,alert_type,lng,lat){
    var flag = false;
    for(var i=0;i<g_arg.rsi.length;i++){
        var rsi = g_arg.rsi[i];
        if(rsi.rsi_type != rsi_type)continue;
        if(rsi.alert_type != alert_type)continue;
        if(rsi.lng != lng)continue;
        if(rsi.lat != lat)continue;
        flag = true;
    }
    if(!flag){
        var tmp = {};
        tmp.rsi_type    = rsi_type;
        tmp.alert_type  = alert_type;
        tmp.lng         = lng;
        tmp.lat         = lat;
        g_arg.rsi.push(tmp);
    }
    return flag;
}


// -----------------------------------map json 解析--------------------------------------------------

// 添加车道的折线
function addLaneLine(latlngs,str,line_color){
    var len = latlngs.length ,angle = 0;
    if(len == 0 )return;
    for(var i=0;i<len;i++){
        if(i<len-1)angle = Math.atan2(latlngs[i+1][1]-latlngs[i][1],latlngs[i+1][0]-latlngs[i][0])*180/Math.PI;
        addNodePoint(latlngs[i],angle);
    }
    if(len>1){
        var polyline = L.polyline(latlngs, {color:line_color,weight:5}).addTo(g_arg.map);
        polyline.bindPopup(str);
        saveLayer(polyline);
    }
    
}


// 添加车道上点的图标
function addNodePoint(latlng,angle)
{
    var marker = L.marker([latlng[0],latlng[1]],{icon:g_arrow_icon,rotationAngle:angle,rotationOrigin:'center',}).addTo(g_arg.map);
    var tmp_angle = angle;
    if(tmp_angle <0)tmp_angle += 360.0;
    tmp_angle = tmp_angle.toFixed(2);
    marker.bindPopup("lng : "+latlng[1]+"</br>lat : "+latlng[0] + "</br>angle :"+tmp_angle);
    saveLayer(marker);
}


// 添加node中心点图标
function addCenterPoint(lat,lng,str){
    var marker = L.marker([lat,lng]).addTo(g_arg.map);
    marker.bindPopup(str);
    saveLayer(marker);
}

// 解析node中的links
function linksParse(links)
{
    for(var i=0;i<links.length;i++){
        var lanes = links[i].lanes;
         for(var j=0;j<lanes.length;j++){
            var points = lanes[j].points;
            var latlngs = [],latlngs_offset = [];
             for(var k=0;k<points.length;k++){
                var p = points[k];
                var plng = (p.lng*1e-7).toFixed(7);
                var plat = (p.lat*1e-7).toFixed(7);
                latlngs.push([plat,plng]);
            }
            var str = "laneID : "+lanes[j].laneID;
            addLaneLine(latlngs,str,"#FF0000");
        }
    }
}

// map
function addMapJson(data)
{
    var nodes = data.nodes;
    for(var i=0;i<nodes.length;i++){
        var node = nodes[i];
        var lng = (node.refPos.lng*1e-7).toFixed(7);
        var lat = (node.refPos.lat*1e-7).toFixed(7);
        var node_id = node.id.id;
        var node_region = node.id.region;
        if("undefined" == typeof node_region)node_region="--";
        if( checkMapNodeExist(node_id,node_region) )continue;                       
        var inlinks = node.links;
        var center = [lat,lng];
        var str = "id : "+node_id+"</br>region : "+node_region+"</br>lng : "+lng+"</br>lat : "+lat;
        addCenterPoint(lat,lng,str);
        g_arg.map.panTo(center,{animate:true,duration:0.5});
        linksParse(inlinks);
    }
}

// rsi
function addRsiJson(data)
{
    var lng = data.refPos.lng.toFixed(7);
    var lat = data.refPos.lat.toFixed(7);
    var center = [lat,lng];
    g_arg.map.panTo(center,{animate:true,duration:0.5});
    addCenterPoint(lat,lng,"");
    var latlngs = [];
    for(var k=0;k< data.alertPath.length;k++){
        var p = data.alertPath[k];
        var plng = (p.lng*1e-7).toFixed(7);
        var plat = (p.lat*1e-7).toFixed(7);
        latlngs.push([plat,plng]);
    }
    addLaneLine(latlngs,"","#FF0000");
}

// 解析json文件
function mapParse(filename){
	$.getJSON(filename,function(data){
        var type = "map";
        if(data.type)type = data.type;
        if(type == "map")addMapJson(data);
        if(type == "rsi")addRsiJson(data);
	}).error(function() {alert("解析json失败 , "+filename);});
}





// --------------------------采点保存的json相关--------------------

// 画线
function addPathLine(latlngs,line_color){
	var len = latlngs.length;
    if(len > 1){
        var polyline = L.polyline(latlngs, {color:line_color,weight:2}).addTo(g_arg.map);
        saveLayer(polyline);
    }
}

// 画点
function addPathPoint(latlngs)
{
	var len = latlngs.length ,i,angle = 0;
    if(len == 0 )return;
    for(var i=0;i<len;i++){
		// 过滤
//		if(i%3 !=0 )continue;
        if(i<len-1)angle = Math.atan2(latlngs[i+1][1]-latlngs[i][1],latlngs[i+1][0]-latlngs[i][0])*180/Math.PI;
        if(angle <0)angle += 360.0;
        var tmp_angle = angle.toFixed(2);
		var marker = L.marker([latlngs[i][0],latlngs[i][1]],{icon:g_arrow2_icon,rotationAngle:angle,rotationOrigin:'center',});
		marker.bindPopup("{\"lng\":"+latlngs[i][1]*1e7+",\"lat\":"+latlngs[i][0]*1e7+"}</br>angle:"+tmp_angle);
		g_arg.group.addLayer(marker);
    }
}

// 采点保存的json解析
function parsePath(filename){
	$.getJSON(filename,function(data){
		var points = data.points;
		var len = points.length , k;
		var latlngs = [];
		for(var k=0;k<len;k++){
			var p = points[k];
            if(p.length < 2)continue;
            var lng = p[0];
            var lat = p[1];
            latlngs.push([lat,lng]);
		}
        addPathPoint(latlngs);
        addPathLine(latlngs,g_color[g_color_index++%g_color.length]);
    }).error(function() {alert("解析文件失败 : "+filename);});
}


// ------------------------------- asn json -------------------------

// ----------- map --------------
function parseAsnMapLanes(lanes){
	for(var i=0;i<lanes.length;i++){
        var points = lanes[i].points;
        var latlngs = [];
        for(var k=0;k<points.length;k++){
            var p = points[k];
            var plng = p.lng.toFixed(7);
            var plat = p.lat.toFixed(7);
            latlngs.push([plat,plng]);
        }
        var str  = "laneID : "+lanes[i].lane_id + "</br>width : " + lanes[i].lane_width + "</br>maneuvers : " + lanes[i].maneuvers;
            str += "</br>phase_left : " + lanes[i].phase_left + "</br>phase_straight : " + lanes[i].phase_straight + "</br>phase_right : " + lanes[i].phase_right; 
        addLaneLine(latlngs,str,"#FF0000");
    }
}


function parseAsnMap(nodes){
	for(var i=0;i<nodes.length;i++){
		var node = nodes[i];
        var lng = node.lng.toFixed(7);
        var lat = node.lat.toFixed(7);
        var node_id = node.id;
        var node_region = node.region;
        if( checkMapNodeExist(node_id,node_region) )continue;                
        var center = [lat,lng];
        var str = "id : "+node_id+"</br>region : "+node_region+"</br>lng : "+lng+"</br>lat : "+lat;
        addCenterPoint(lat,lng,str);
//        g_arg.map.panTo(center,{animate:true,duration:0.5});
        parseAsnMapLanes(node.lanes);
	}
}

// ----------- rsi--------------

function parseAsnRefpaths(paths){
   for(var i=0;i<paths.length;i++){
        var points = paths[i].activePath;
        var latlngs = [];
        for(var k=0;k<points.length;k++){
            var p = points[k];
            var plng = p.lng.toFixed(7);
            var plat = p.lat.toFixed(7);
            latlngs.push([plat,plng]);
        }
        var str  = "radius : "+paths[i].pathRadius;
        addLaneLine(latlngs,str,"#FF0000");
    }
}

function parseAsnRtes(rtes){
    for(var i=0;i<rtes.length;i++){
		var rte = rtes[i];
        var lng = rte.lng.toFixed(7);
        var lat = rte.lat.toFixed(7);
        var des = rte.description;
        var type = rte.eventType;
        if( checkRsiExist("rte",type,lng,lat) )continue;                
        var center = [lat,lng];
        var str = "eventType : "+type+"</br>description : "+des+"</br>lng : "+lng+"</br>lat : "+lat;
        addCenterPoint(lat,lng,str);
//        g_arg.map.panTo(center,{animate:true,duration:0.5});
        parseAsnRefpaths(rte.referencePaths);
	}
}

function parseAsnRtss(rtss){
    for(var i=0;i<rtss.length;i++){
		var rts = rtss[i];
        var lng = rts.lng.toFixed(7);
        var lat = rts.lat.toFixed(7);
        var des = rts.description;
        var type = rts.signType;
        if( checkRsiExist("rts",type,lng,lat) )continue;                
        var center = [lat,lng];
        var str = "signType : "+type+"</br>description : "+des+"</br>lng : "+lng+"</br>lat : "+lat;
        addCenterPoint(lat,lng,str);
//        g_arg.map.panTo(center,{animate:true,duration:0.5});
        parseAsnRefpaths(rts.referencePaths);
	}
}


function parseAsnJson(json){
	if(json.type == "map_asn"){
		parseAsnMap(json.nodes);
	}else if(json.type == "rsi_asn"){
        parseAsnRtes(json.rtes);
        parseAsnRtss(json.rtss);
    }
}


