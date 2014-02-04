#!/bin/bash

sfmldir=/d/libs/sfml2_git
sfmllibs="window system graphics audio network"

sfguidir=/d/libs/SFGUI
sbedir=/d/libs/SchiffbruchEngine
boostdir=/d/libs/boost_1_55_0
clean=true
static=false

dlldir=/d/lib/

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
	rm $1
	cmd /c "mklink $1 $2\\$1"
}

function createsfmlsymlinks()
{
	echo 
	echo "-- Symlinking SFML --"
	echo

	cd $sfmldir
	
	if [ $static = "true" ]; then
		rmdir lib
		cmd /c 'mklink /D lib extlibs\\libs-mingw\\x86\\ '
		createsymlink libsfml-main.a build\\lib
		
		for lib in $sfmllibs; do
			createsymlink libsfml-$lib-s.a build\\lib
		done
	else
		rmdir lib
		cmd /c 'mklink /D lib extlibs\\libs-mingw\\x86\\ '
		createsymlink libsfml-main.a build\\lib
		
		for lib in $sfmllibs; do
			createsymlink libsfml-$lib.a build\\lib
			createsymlink sfml-$lib-2.dll build\\lib
		done
	fi
	
	echo
}

function createsfguisymlinks()
{
	echo 
	echo "-- Symlinking SFGUI --"
	echo
	
	cd $sfguidir
	if [ $static = "true" ]; then
		rmdir lib
		cmd /c 'mklink /D lib build '
		echo
	else
		rmdir lib
		cmd /c 'mklink /D lib build '
	fi
	
	echo
}

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

cd $sfguidir
cleanbuild
echo 
echo "-- BUILDING SFGUI --"
echo
cmake -G"MSYS Makefiles" -DSFML_STATIC_LIBRARIES=$static -DCMAKE_MODULE_PATH="$sfmldir/cmake/Modules/" -DSFML_ROOT="$sfmldir" -DSFGUI_BUILD_EXAMPLES=false -DSFGUI_BUILD_SHARED_LIBS=$shared ..
echo "---------------------------"
make -j8
#sfgui doesn't seem to name the static .a correctly
[ $static = "true" ] && cp sfgui.a sfgui-s.a
createsfguisymlinks

echo
echo "-- BUILT SFGUI --"


cd $sbedir
echo 
echo "-- BUILDING SBE --"
echo 
cleanbuild
cmake -G"MSYS Makefiles" -DBOOST_ROOT="$boostdir" -DSFGUI_ROOT="$sfguidir" -DSFML_ROOT="$sfmldir" -DBUILD_SHARED_LIBS=$shared -DSFGUI_STATIC_LIBRARIES=$static -DSFML_STATIC_LIBRARIES=$static ..
echo "---------------------------"
make -j8

echo
echo "-- BUILT SBE --"

[ $shared = "true" ] && collectdlls

summary
