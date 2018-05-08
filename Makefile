libhello.jnilib: Hello.cpp
	g++ -I/System/Library/Frameworks/JavaVM.framework/Headers \
	-dynamiclib Hello.cpp -o libhello.jnilib