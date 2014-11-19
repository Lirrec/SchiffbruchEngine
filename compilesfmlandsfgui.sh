#!/bin/bash

sfmldir=/d/dev/libs/sfml2_git
sfmllibs="window system graphics audio network"

sfguidir=/d/dev/libs/SFGUI
sbedir=/d/dev/libs/SchiffbruchEngine
boostdir=/d/dev/libs/boost_1_57_0
glmdir=/d/dev/libs/glm_0_9_5_4
clean=false
static=true

dlldir=/d/dev/lib/

if [ $# -ge 1 ]; then
	if [[ $1 != "false" && $1 != "true" ]]; then
		echo " - Parameter 1 ( build static libs ) should be either 'true' or 'false'"
	else
		static=$1
	fi
fi;

if [ $# -ge 2 ]; then
	if [[ $2 != "false" && $2 != "true" ]]; then
		echo " - Parameter 2 ( clean build directories ) should be either 'true' or 'false'"
	else
		clean=$2
	fi
fi;

if [ $static = "true" ]; then
	shared=false
else
	shared=true
fi;

function summary()
{
	echo
	echo " --------- SUMMARY -------- "
	echo " - Build static libaries: $static"
	echo " -"
	echo " - sfml dir:  $sfmldir"
	echo " - sfgui dir: $sfguidir"
	echo " - sbe dir:   $sbedir"
	echo " - boost dir: $boostdir"
	echo " -"
	echo " - Clean old builds: $clean"
	echo " -"
	[ $shared = "true" ] && echo " - DLLs will be stored in: $dlldir"
	echo " -------------------------- "
	echo

}

function collectdlls()
{
	echo
	echo "---- Copying dlls to $dlldir ----"
	for lib in $sfmllibs; do
		echo "- $sfmldir/build/sfml-$lib-2.dll"
		cp $sfmldir/build/lib/sfml-$lib-2.dll $dlldir
	done
	
	echo "- $sfmldir/extlibs/bin/x86/libsndfile-1.dll"
	cp $sfmldir/extlibs/bin/x86/libsndfile-1.dll $dlldir
	echo "- $sfmldir/extlibs/bin/x86/openal32.dll"
	cp $sfmldir/extlibs/bin/x86/openal32.dll $dlldir
	
	echo "- $sfguidir/build/sfgui.dll"
	cp $sfguidir/build/sfgui.dll $dlldir
	
	echo "- $sbedir/build/libsbe.dll"
	cp $sbedir/build/libsbe.dll $dlldir
}

function cleanbuild()
{
	if [ ! -d build ]; then
		mkdir build
	fi;

	cd build
	if [ $clean = "true" ]; then 
		rm -r *;
	fi;
}

function createsymlink()
{
	rmdir $1
	cmd /c "mklink /J $1 $2"
}

function createsfmlsymlinks()
{
	echo 
	echo "-- Symlinking SFML --"
	echo
	cd $sfmldir
	createsymlink lib "extlibs\\libs-mingw\\x86\\"
	createsymlink lib64 "build\\lib"
	echo
}

function createsfguisymlinks()
{
	echo 
	echo "-- Symlinking SFGUI --"
	echo
	cd $sfguidir
	createsymlink lib build
	echo
	echo
}

function buildsfml()
{
	cd $sfmldir
	echo 
	echo "-- BUILDING SFML --"
	echo 
	cleanbuild
	cmake -G"MSYS Makefiles" -DBUILD_SHARED_LIBS=$shared ..
	echo "---------------------------"
	make -j8
	createsfmlsymlinks

	echo
	echo "-- BUILT SFML --"
}

function buildsfgui()
{
	cd $sfguidir
	cleanbuild
	echo 
	echo "-- BUILDING SFGUI --"
	echo
	cmake --debug-trycompile -G"MSYS Makefiles" -DSFML_STATIC_LIBRARIES=$static -DCMAKE_MODULE_PATH="$sfmldir/cmake/Modules/" -DSFML_ROOT="$sfmldir" -DSFGUI_BUILD_EXAMPLES=false -DSFGUI_BUILD_SHARED_LIBS=$shared -DGLEW_INCLUDE_PATH=$sfmldir/extlibs/headers -DGLEW_LIBRARY=$sfmldir/extlibs/libs-mingw/x86/libglew.a ..
	echo "---------------------------"
	make -j8
	#sfgui doesn't seem to name the static .a correctly
	#[ $static = "true" ] && cp sfgui.a sfgui-s.a
	createsfguisymlinks

	echo
	echo "-- BUILT SFGUI --"
}

function buildsbe()
{
	cd $sbedir
	echo 
	echo "-- BUILDING SBE --"
	echo 
	cleanbuild
	cmake --debug-trycompile -G"MSYS Makefiles" -DGLM_ROOT_DIR="$glmdir" -DBOOST_ROOT="$boostdir" -DSFGUI_ROOT="$sfguidir" -DSFML_ROOT="$sfmldir" -DBUILD_SHARED_LIBS=$shared -DSFGUI_STATIC_LIBRARIES=$static -DSFML_STATIC_LIBRARIES=$static ..
	echo "---------------------------"
	make -j8

	echo
	echo "-- BUILT SBE --"
}

summary
buildsfml
buildsfgui
buildsbe
[ $shared = "true" ] && collectdlls

summary
