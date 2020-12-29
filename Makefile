#system = linux
system = macos

ifeq (macos, $(system))

headersPath = /Library/Java/JavaVirtualMachines/jdk1.8.0_271.jdk/Contents/Home
libhello.jnilib: Hello.cpp
	g++ -I$(headersPath)/include -I$(headersPath)/include/darwin \
	-dynamiclib Hello.cpp -o libhello.jnilib

endif

ifeq (linux, $(system))

libhello.so: Hello.cpp
	g++ -shared -fPIC -I/usr/lib/jvm/java-8-openjdk-amd64/include \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux \
	-o libhello.so Hello.cpp

endif

