#include <iostream>
#include "Hello.h"

/**
 *  ----- sduty notes -----
 *
 * field and method IDs:
 * A field or method ID remains valid until the virtual machine unloads
 * the class or interface that defines the corresponding field or
 * method.
 *
 * Caching field and method IDs:
 * If you cache these IDs at the point of use you must make sure that
 * the defining class will not be   unloaded and reloaded  as long as
 * the native code still relies on the value of cached ID.
 * On the other hand, if caching is done in the static initializer of
 * the defining class, the cached IDs will automatically be recalculated
 * when the class is unloaded and later reloaded (because you must run
 * native code through java, so java class will be loaded firstly).
 *
 * reference:
 * Each reference contains(NOT IS) a direct pointer to the underlying
 * object. The pointer to the object is never directly used by native
 * code. From the native code's point of view, references are opaque.
 * a reference may be like this:
 * struct reference {
 * 	pointer ---> object(a real struct of the Java type)
 * }
 * jobject, jclass... is a pointer type of the reference.
 *
 * Most jni functions create local references, and VM will free them
 * automatically. But jfieldID and jmethodID is NOT local reference,
 * so you can cache it for performance.
 *
 * WeakGlobalRef:
 * If you fail to call DeleteWeakGlobalRef the JVM will still be able
 * to garbage collect the underlying object, but will not be able to
 * reclaim the memory consumed by the weak global reference itself.
 *
 * PopLocalFrame:
 * PoplocalFrame converts its argment to a new local reference in the
 * previous frame before popping the topmost frame(top of a stack).
 */

/**
 * registered a native method for java.
 */
void JNICALL javaCall_rgs(JNIEnv *env, jobject thiz) {
	std::cout << "java call rgs executed\n";
}

// register a native method
void rgsJavaCall_rgs(JNIEnv *env) {
	JNINativeMethod nm;
	char cstr[] = "javaCall_rgs";
	nm.name = cstr;
	char cstr1[] = "()V";
	nm.signature = cstr1;
	nm.fnPtr = (void*)javaCall_rgs;

	jclass javaCls;
	javaCls = env->FindClass("Hello");
	
	env->RegisterNatives(javaCls, &nm, 1);
}

/**
 * When System.loadLibrary loads a native library, the virtual machine
 * searches for the method.
 * typical use: caching javaVM pointer, class references, field and
 * 		method IDs.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	std::cout << "lib is attached to jvm.\n";
	JNIEnv *env;
	vm->GetEnv((void**)&env, JNI_VERSION_1_6);
	rgsJavaCall_rgs(env);
	return JNI_VERSION_1_6;
}

/**
 * In summary, you should be careful when writing JNI_OnUnload
 * handlers. Avoid complex locking operations that may introduce
 * deadlocks. Keep in mind that classes have been unloaded when
 * the JNI_OnUnload handler is invoked.
 */
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
	std::cout << "lib is detached from jvm.\n";
}

void callJava(JNIEnv *env) {
	jclass jcls = env->FindClass("Hello");
	if (jcls == NULL) {
		std::cout << "jcls is NULL\n";
		return;
	}
	// there are no separators between multiple argument types.
	jmethodID cid = env->GetMethodID(jcls, "<init>", "(ZIJ)V");
	if (cid == NULL) {
		std::cout << "cid is NULL\n";
		return;
	}
	jobject hello = env->NewObject(jcls, cid, JNI_TRUE, 7, 7);
	if (hello == NULL) {
		std::cout << "hello is NULL\n";
		return;
	}
	jmethodID printInfo = env->GetMethodID(jcls, "printInfo", "()V");
	env->CallVoidMethod(hello, printInfo);
}

JNIEXPORT void JNICALL Java_Hello_sayHello(JNIEnv *env, jobject jobj) {
	std::cout << "hello from cpp!\n";
	std::cout << jobj << "\n";

	callJava(env);
	
	std::cout << "len of 陈志菲: " << strlen("陈志菲") << "\n"; // 9
	std::cout << "ajdjlj" "alfdjlajsdl\n"; // 相邻字符串会自动连接为一个。
}

JNIEXPORT void JNICALL Java_Hello_staticFn(JNIEnv *env, jclass jcls) {
	std::cout << "static native fn!\n";
}

/**
 * Do not forget to check the return value of GetStringUTFChars.
 * Because the Java virtual machine implementation needs to allocate
 * memory to hold the UTF-8 string, there is a chance that memory
 * allocation will fail.
 *
 * NOTE: the UTF-8 string obtained through GetStringUTFChars will need
 * to be freed, call ReleaseStringUTFChars.
 *
 * arg *isCopy in GetStringUTFChars: 
 *  JNI_FLASE: the returned string is a direct pointer to the characters
 *             in the original java.lang.String instance.
 *  JNI_TRUE: the returned string is a copy of characters in the original
 *            java.lang.String instance.
 *  NULL: a copy of the characters in the java.lang.String instance or a
 *        direct pointer to the original.
 *
 * NOTE: the ReleaseStringUTFChars call is necessary whether the *isCopy
 *       has be set to JNI_TRUE or JNI_FALSE. ReleaseStringUTFChars will
 *       free the copy or unpin the instance.
 */
JNIEXPORT void JNICALL Java_Hello_passStr2cpp
  (JNIEnv *env, jobject jobj, jstring jstr) {
	// the number of bytes (not including the trailing 0) deeded to
	// represent a string in the utf-8 fromat.
	std::cout << "len utf: " << env->GetStringUTFLength(jstr) << "\n";
	// the number of unicode characters in the string.
	std::cout << "len uni: " << env->GetStringLength(jstr) << "\n";
	const char *cstr;
	cstr = env->GetStringUTFChars(jstr, NULL);
	std::cout << cstr << ".\n";
	env->ReleaseStringUTFChars(jstr, cstr);
}

JNIEXPORT jstring JNICALL Java_Hello_getStr(JNIEnv *env, jobject jobj) {
	char buf[128];
	scanf("%s", buf);
	jstring str = env->NewStringUTF(buf);
	return str;
}

JNIEXPORT jint JNICALL Java_Hello_sumArr
  (JNIEnv *env, jobject jobj, jintArray jiarr) {
	jsize len = env->GetArrayLength(jiarr);
	jint sum = 0;
	jint buf[len];
	env->GetIntArrayRegion(jiarr, 0, 3, buf);
	for (jint i = 0; i < len; i++) {
		sum += buf[i];
	}
	return sum;
}

JNIEXPORT void JNICALL Java_Hello_setArr
  (JNIEnv *env, jobject jobj, jintArray jiarr) {
	jsize len = env->GetArrayLength(jiarr);
	jint willArr[] = {7, 7, 7};
	env->SetIntArrayRegion(jiarr, 0, len, willArr);
}

/**
 * Each JNI reference consumes a certain amount of memory by itself,
 * in addition to the memory taken by the referred object.
 * Passing NULL to DeleteLocalRef is a no-op that is an appropriate
 * action.
 */
JNIEXPORT jobjectArray JNICALL Java_Hello_getStrArr
  (JNIEnv *env, jobject jobj, jint jlen) {
	jobjectArray result;
	jclass strCls = env->FindClass("Ljava/lang/String;");
	if (strCls == NULL) {
		std::cout << "strCls is NULL\n";
		return NULL;
	}
	result = env->NewObjectArray(jlen, strCls, NULL);
	if (result == NULL) {
		std::cout << "result is NULL\n";
		return NULL;
	}
	for (int i = 0; i < jlen; i++) {
		char const *cstr = "chenzhifei";
		jstring str = env->NewStringUTF(cstr);
		env->SetObjectArrayElement(result, i, str);
		// the ref will be deal automatically when this method
		// is returned, but threre is a ref acumulation.
		env->DeleteLocalRef(str);
	}
	// JVM will deal the ref, becuase it be returned to JVM directly.
	return result;
}

JNIEXPORT jobjectArray JNICALL Java_Hello_getInt2DArr
  (JNIEnv *env, jobject jobj, jint len1, jint len2) {
	jobjectArray result;
	jclass intArrCls = env->FindClass("[I");
	if (intArrCls == NULL) {
		std::cout << "intArrCls is NULL\n";
		return NULL;
	}
	result = env->NewObjectArray(len1, intArrCls, NULL);
	if (result == NULL) {
		std::cout << "result is NULL\n";
		return NULL;
	}
	for (int i = 0; i < len1; i++) {
		jint tmp[128]; // make sure it is large enough.
		jint len2 = i+1;
		jintArray iarr = env->NewIntArray(len2);
		if (iarr == NULL) {
			std::cout << "iarr is NULL\n";
			return NULL;
		}
		for (int j = 0; j < len2; j++) {
			tmp[j] = 7;
		}
		env->SetIntArrayRegion(iarr, 0, len2, tmp);
		env->SetObjectArrayElement(result, i, iarr);
		env->DeleteLocalRef(iarr);
	}
	return result;
}

jobject globalObjRef;

JNIEXPORT void JNICALL Java_Hello_register2native
  (JNIEnv *env, jobject thiz, jobject jobj) {
	globalObjRef = env->NewGlobalRef(jobj);
}

JNIEXPORT jboolean JNICALL Java_Hello_checkIsSameObj
  (JNIEnv *env, jobject thiz, jobject jobj) {
	// determine if refers to the same underlying obj.
	return env->IsSameObject(jobj, globalObjRef);
}

/**
 * In particular, you should be aware of the upper bound of the
 * number of local references your program can create at any point
 * during its execution, even though these local references will
 * eventually be freed automatically by the VM.
 *
 * Creating a large number of local references in a single native
 * method invocation may result in an overflow of the internal JNI
 * local reference table.
 */
JNIEXPORT void JNICALL Java_Hello_testLocalRefUpperBound
  (JNIEnv *env, jobject jobj) {
	jint upperBound = 100; // 1000*1000*10 is still ok.
	jintArray iarr;
	for (jint i = 0; i < upperBound; i++) {
		iarr = env->NewIntArray(1);
	}
}







