#include <jni.h>
#include <string>
#include <unistd.h>
#include <android/log.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>

#define TAG "TAGG"
std::string c_package_name = "::";
std::string str_package_name = "?d";
const char* c_package_name_base = "WTI5dExtTnlZV1owYzIxaGJpNW5idz09";
//Class Path
const char* ironClassPath= "com/ironsource/mediationsdk/IronSource";
const char* bannerClassPath= "com/google/ads/adsirc/Baller";
const char* ironHelperClassPath= "com/ironsource/mediationsdk/integration/IntegrationHelper";
const char* mainActivityPath = "com/mojang/minecraftpe/MainActivity";

//Class
static jclass bannerActivityClass = nullptr;
static jclass ironsourceClass = nullptr;
static jclass mainActivityClass = nullptr;

static JavaVM* javaVM = nullptr;
static JNIEnv* myEnv = nullptr;

static jobject rlBannerLayout;

//TODO Init Ironsource Ads Info
const char* app_key = "TVRaaE9HSTJNMlUx";//"16a8b63e5";
jstring j_app_key = nullptr;
jboolean is_active = true;
const char* pos = "c";


void iInitView(JNIEnv *pEnv, jobject pJobject);
void doCheck(JNIEnv *pEnv);

jstring decodeBase64ToString(JNIEnv *pEnv, jstring pJstring);


void checkGetPackageName(){
    pid_t pid = getpid();
    char path[64] = { 0 };
    sprintf(path, "/proc/%d/cmdline", pid);
    FILE *cmdline = fopen(path, "r");
    if (cmdline) {
        char application_id[64] = { 0 };
        fread(application_id, sizeof(application_id), 1, cmdline);
        c_package_name = application_id;
        fclose(cmdline);
    }
}
void doCheck(JNIEnv *env) {
    checkGetPackageName();

    jstring j_decode = env->NewStringUTF(c_package_name_base);
    const char *outDecodeText = (*env).GetStringUTFChars(decodeBase64ToString(env,
                                                                              decodeBase64ToString(env,j_decode)), NULL);
    str_package_name = outDecodeText;
    if(str_package_name.compare(c_package_name) == 0){
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "equal");
    }else{
        //__android_log_print(ANDROID_LOG_DEBUG, TAG, "not equal");
        exit(0);
    }
}

jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    javaVM = vm;
    JNIEnv* env;
    myEnv = env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }
    doCheck(env);

    bannerActivityClass = env->FindClass(bannerClassPath);
    jmethodID constructor = env->GetMethodID(bannerActivityClass,"<init>", "()V");
    jobject newObjectMethod = env->NewObject(bannerActivityClass, constructor);

    return  JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_google_ads_adsirc_Baller_onSetVolumeSystem(JNIEnv *env, jobject thiz,
                                                           jobject main_activity) {
    // TODO: implement onSetVolumeSystem()

    iInitView(env, main_activity);
}

void iInitView(JNIEnv *pEnv, jobject mMainActivity) {
    myEnv = pEnv;

    bannerActivityClass = pEnv->FindClass(bannerClassPath);
    jmethodID constructor = pEnv->GetMethodID(bannerActivityClass,"<init>", "(Lcom/mojang/minecraftpe/MainActivity;)V");
    jobject mainActivity = mMainActivity;
//    jobject newObjectMethod = pEnv->NewObject(bannerActivityClass, constructor, mainActivity);

    //TODO BEGIN Decode
//    jmethodID jDecodeMethodID = (*pEnv).GetStaticMethodID(bannerActivityClass,"decodeString", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring jTextString = pEnv->NewStringUTF(app_key);
//    jstring j_1 = static_cast<jstring>((*pEnv).CallStaticObjectMethod(bannerActivityClass, jDecodeMethodID, jTextString));
//    j_app_key = static_cast<jstring>((*pEnv).CallStaticObjectMethod(bannerActivityClass, jDecodeMethodID, j_1));
    j_app_key = decodeBase64ToString(pEnv,decodeBase64ToString(pEnv, jTextString));
    //TODO END Decode

    if(is_active) {
        /*
        //Init Ironsource
        ironsourceClass = pEnv->FindClass(ironClassPath);//Get Ironsource Class
        //Init Ironsource
        jmethodID initIronMethod = pEnv->GetStaticMethodID(ironsourceClass, "init",
                                                           "(Landroid/content/Context;Ljava/lang/String;)V");
        (*pEnv).CallStaticVoidMethod(ironsourceClass, initIronMethod, mainActivity, j_app_key);
        //Validate Integration
        jclass helperClass = pEnv->FindClass(ironHelperClassPath);
        jmethodID validateMethod = pEnv->GetStaticMethodID(helperClass, "validateIntegration",
                                                           "(Landroid/content/Context;)V");
        (*pEnv).CallStaticVoidMethod(helperClass, validateMethod, mMainActivity);

        //Init BannerLayout
        // Find the ISBannerSize class
        jclass isBannerSizeClass = pEnv->FindClass("com/ironsource/mediationsdk/ISBannerSize");
        // Get the BANNER field from ISBannerSize
        jfieldID bannerFieldID = pEnv->GetStaticFieldID(isBannerSizeClass, "BANNER",
                                                        "Lcom/ironsource/mediationsdk/ISBannerSize;");
        jobject bannerSize = pEnv->GetStaticObjectField(isBannerSizeClass, bannerFieldID);

        // Get the createBanner method ID (the method signature is key here)
        jmethodID createBannerMethodID = pEnv->GetStaticMethodID(ironsourceClass, "createBanner",
                                                                 "(Landroid/app/Activity;Lcom/ironsource/mediationsdk/ISBannerSize;)Lcom/ironsource/mediationsdk/IronSourceBannerLayout;");

        // Assuming 'instance' is your activity instance (mActivity)
        jobject banner = pEnv->CallStaticObjectMethod(ironsourceClass, createBannerMethodID,
                                                      mMainActivity, bannerSize);

        //Add Listener
        jmethodID getListenMethod = pEnv->GetStaticMethodID(bannerActivityClass, "addListenter",
                                                            "(Lcom/ironsource/mediationsdk/IronSourceBannerLayout;)V");
        (*pEnv).CallStaticVoidMethod(bannerActivityClass, getListenMethod, banner);

        //Load Banner
        jmethodID loadBannerMethod = pEnv->GetStaticMethodID(ironsourceClass, "loadBanner",
                                                             "(Lcom/ironsource/mediationsdk/IronSourceBannerLayout;)V");
        (*pEnv).CallStaticVoidMethod(ironsourceClass, loadBannerMethod, banner);

        // Step 1: Find the RelativeLayout class and its constructor
        jclass relativeLayoutClass = pEnv->FindClass("android/widget/RelativeLayout");
        jmethodID relativeLayoutConstructor = pEnv->GetMethodID(relativeLayoutClass, "<init>",
                                                                "(Landroid/content/Context;)V");
        // Step 2: Create a new RelativeLayout object
        rlBannerLayout = pEnv->NewObject(relativeLayoutClass, relativeLayoutConstructor,
                                         mMainActivity);
        // Step 3: Find the RelativeLayout$LayoutParams class and its constructor
        jclass layoutParamsClass = pEnv->FindClass("android/widget/RelativeLayout$LayoutParams");
        jmethodID layoutParamsConstructor = pEnv->GetMethodID(layoutParamsClass, "<init>", "(II)V");
        // Step 4: Create a new RelativeLayout.LayoutParams object with WRAP_CONTENT for both width and height
        jint WRAP_CONTENT = -2; // RelativeLayout.LayoutParams.WRAP_CONTENT is -2 in Java
        jobject layoutParams = pEnv->NewObject(layoutParamsClass, layoutParamsConstructor,
                                               WRAP_CONTENT, WRAP_CONTENT);
        // Step 5: Find the addRule method in RelativeLayout.LayoutParams and add the CENTER_IN_PARENT rule
        jmethodID addRuleMethod = pEnv->GetMethodID(layoutParamsClass, "addRule", "(II)V");
        jint CENTER_IN_PARENT = 13; // RelativeLayout.CENTER_IN_PARENT is 13 in Java
        jint TRUE = -1; // RelativeLayout.TRUE is -1 in Java
        pEnv->CallVoidMethod(layoutParams, addRuleMethod, CENTER_IN_PARENT, TRUE);
        // Step 6: Set the layout parameters on the RelativeLayout
        jmethodID setLayoutParamsMethod = pEnv->GetMethodID(relativeLayoutClass, "setLayoutParams",
                                                            "(Landroid/view/ViewGroup$LayoutParams;)V");
        pEnv->CallVoidMethod(rlBannerLayout, setLayoutParamsMethod, layoutParams);
        // Step 7: Set the background color of the RelativeLayout to white
        jclass colorClass = pEnv->FindClass("android/graphics/Color");
        jfieldID whiteFieldID = pEnv->GetStaticFieldID(colorClass, "WHITE", "I");
        jint whiteColor = pEnv->GetStaticIntField(colorClass, whiteFieldID);
        jmethodID setBackgroundColorMethod = pEnv->GetMethodID(relativeLayoutClass,
                                                               "setBackgroundColor", "(I)V");
        pEnv->CallVoidMethod(rlBannerLayout, setBackgroundColorMethod, whiteColor);

        jmethodID setLayoutMethod = pEnv->GetStaticMethodID(bannerActivityClass, "onSetLayout",
                                                            "(Landroid/widget/RelativeLayout;)V");
        pEnv->CallStaticVoidMethod(bannerActivityClass, setLayoutMethod, rlBannerLayout);
        //Add to view
        jmethodID addBannerToView = pEnv->GetStaticMethodID(bannerActivityClass, "setParamss",
                                                            "(Lcom/ironsource/mediationsdk/IronSourceBannerLayout;Landroid/widget/RelativeLayout$LayoutParams;Lcom/mojang/minecraftpe/MainActivity;Landroid/widget/RelativeLayout;Ljava/lang/String;)V");
        jstring jPos = pEnv->NewStringUTF(pos);
        pEnv->CallStaticVoidMethod(bannerActivityClass, addBannerToView, banner, layoutParams,
                                   mainActivity, rlBannerLayout, jPos);
                                   */
    }
}

jstring decodeBase64ToString(JNIEnv *env, jstring base64String) {
    // Step 1: Find the Base64 class
    jclass base64Class = env->FindClass("android/util/Base64");

    // Step 2: Get the Base64 decode method ID
    jmethodID decodeMethodID = env->GetStaticMethodID(base64Class, "decode", "(Ljava/lang/String;I)[B");

    // Step 3: Define the Base64.DEFAULT flag (typically 0)
    jint defaultFlag = 0;

    // Step 4: Call the Base64.decode method
    jbyteArray decodedBytes = (jbyteArray)env->CallStaticObjectMethod(base64Class, decodeMethodID, base64String, defaultFlag);

    // Step 5: Get the length of the decoded byte array
    jsize length = env->GetArrayLength(decodedBytes);

    // Step 6: Convert jbyteArray to a native char array
    jbyte *nativeBytes = env->GetByteArrayElements(decodedBytes, NULL);

    // Step 7: Create a std::string from the char array
    std::string decodedString(reinterpret_cast<char*>(nativeBytes), length);

    // Step 8: Release the jbyteArray
    env->ReleaseByteArrayElements(decodedBytes, nativeBytes, JNI_ABORT);

    // Step 9: Convert std::string to jstring
    jstring resultString = env->NewStringUTF(decodedString.c_str());

    // Step 1: Convert jstring to const char*
    const char *nativeString = env->GetStringUTFChars(resultString, 0);
//    __android_log_print(ANDROID_LOG_ERROR, TAG, "Decode Result: %s", nativeString);
    //env->ReleaseStringUTFChars(javaString, nativeString);
    // Step 10: Return the jstring
    return resultString;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_ads_adsirc_Baller_onClickk(JNIEnv *env, jclass clazz) {
    // TODO: implement onClickk()
    // Step 1: Find the View class
    jclass viewClass = myEnv->FindClass("android/view/View");

    // Step 2: Get the GONE constant value from View
    jfieldID goneFieldID = myEnv->GetStaticFieldID(viewClass, "GONE", "I");
    jint goneValue = myEnv->GetStaticIntField(viewClass, goneFieldID);

    // Step 3: Find the setVisibility method ID
    jmethodID setVisibilityMethodID = myEnv->GetMethodID(viewClass, "setVisibility", "(I)V");

    // Step 4: Call setVisibility on rlBannerLayout with View.GONE
    if(rlBannerLayout != nullptr)
        myEnv->CallVoidMethod(rlBannerLayout, setVisibilityMethodID, goneValue);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_google_ads_adsirc_Baller_onClickDone(JNIEnv *env, jclass clazz) {
    // TODO: implement onClickDone()
    // Step 1: Find the View class
    jclass viewClass = env->FindClass("android/view/View");

    // Step 2: Get the VISIBLE constant value from View
    jfieldID visibleFieldID = env->GetStaticFieldID(viewClass, "VISIBLE", "I");
    jint visibleValue = env->GetStaticIntField(viewClass, visibleFieldID);

    // Step 3: Find the setVisibility method ID
    jmethodID setVisibilityMethodID = env->GetMethodID(viewClass, "setVisibility", "(I)V");

    // Step 4: Call setVisibility on rlBannerLayout with View.VISIBLE
    env->CallVoidMethod(rlBannerLayout, setVisibilityMethodID, visibleValue);
}