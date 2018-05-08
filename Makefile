system = linux

ifeq (macos, $(system))
libhello.jnilib: Hello.cpp
	g++ -I/System/Library/Frameworks/JavaVM.framework/Headers \
	-dynamiclib Hello.cpp -o libhello.jnilib
endif

ifeq (linux, $(system))
libhello.so: Hello.cpp
	g++ -shared -fPIC -I/usr/lib/jvm/java-8-openjdk-amd64/include \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux \
	 -o libhello.so Hello.cpp
endif
