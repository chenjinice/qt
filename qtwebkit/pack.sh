#! /bin/bash

qt_dir=/home/duser/Qt5.5.0/5.5/gcc
qt_plugin_dir=${qt_dir}/plugins
qt_lib_dir=${qt_dir}/lib

app=mapTool
lib_dir=$PWD/lib
conf=$PWD/qt.conf
launch=start.sh

if [ ! -d ${lib_dir} ]; then
    mkdir ${lib_dir}
fi

#将字符串重定向到dependList
lib_list=$( ldd ${app} | awk '{if (match($3,"/")){ print $3}}' )
 

#拷贝依赖
cp ${lib_list} ${lib_dir}
cp ${qt_lib_dir}/libQt5XcbQpa.so.5 	${lib_dir}
cp ${qt_lib_dir}/libQt5DBus.so.5  	${lib_dir}
cp ${qt_lib_dir}/libQt5Svg.so.5 	${lib_dir}
cp -r ${qt_plugin_dir}  ./

echo "[Paths]" > ${conf}
echo "Prefix = ./" >> ${conf}
echo "Plugins = ./plugins" >> ${conf}
echo "#Imports = ./qml" >> ${conf}
echo "#Qml2Imports = ./qml" >> ${conf}

echo -e "#! /bin/bash  \n" > ${launch}
echo 'export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH' >> ${launch}
echo "./${app}" >> ${launch}
chmod +x ${launch}




# ?????  :  执行完删除一个库文件libc.so.6（不删会出问题）
#将Qt5.5.0/5.5/gcc目录下plugins和qml两个文件夹全部复制过来
#编写qt.conf文件
#[Paths]
#Prefix = ./
#Plugins = plugins
#Imports = qml
#Qml2Imports = qml


#启动文件 start.sh
# export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
# ./mapTool




